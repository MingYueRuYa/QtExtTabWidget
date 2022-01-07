#include "stdafx.h"
#include "QtExtWidget.h"
#include "QtExtPictureWidget.h"
#include "DzDownloadManager.h"

#include <QPainter>
#include <QStyleOption>
#include <QPixmapCache>
#include <QtConcurrent/QtConcurrent>

QtExtPictureWidget::QtExtPictureWidget(QWidget* parent)
    : QWidget(parent)
{
    initUI();
}

QtExtPictureWidget::~QtExtPictureWidget()
{
    if (m_readWatcher_)
    {
        m_readWatcher_->disconnect(this);
        m_readWatcher_->cancel();
    }
    if (m_scaleWatcher_)
    {
        m_scaleWatcher_->disconnect(this);
        m_scaleWatcher_->cancel();
    }
}

void QtExtPictureWidget::setImage(const QUrl& url)
{
    if (m_url_ == url)
    {
        return;
    }

    clear();

    m_url_ = url;
    auto pixmap = QPixmapCache::find(url.url());
    if (pixmap)
    {
        m_pixmap_ = *pixmap;

        m_bUseDefaultImage = false;
        emit ShowImage(false);
        renderPixmap();
    }
    else if (m_url_.isLocalFile())
    {
        if (m_pixmap_.load(url.toString(QUrl::UrlFormattingOption::RemoveScheme)))
        {
            QPixmapCache::insert(m_url_.url(), m_pixmap_);

            m_bUseDefaultImage = false;
            emit ShowImage(false);
            renderPixmap();
        }
    }
    else
    {
        if (!m_readWatcher_)
        {
            m_readWatcher_ = new QFutureWatcher<QByteArray>(this);
        }

        m_readWatcher_->disconnect(this);
        VERIFY(connect(m_readWatcher_, SIGNAL(finished()), this, SLOT(onReadWatcherFinished())));
        VERIFY(connect(m_readWatcher_, SIGNAL(canceled()), this, SLOT(onReadWatcherCanceled())));

        auto future = g_DownloadManager.request(url);
        m_readWatcher_->setFuture(future);
    }
}

void QtExtPictureWidget::initUI()
{
    if (QObject *obj_parent = this->parent())
    {
        int index = obj_parent->metaObject()->indexOfSignal(
                                QMetaObject::normalizedSignature("Resize(const QRect&)"));
        if (-1 != index)
        {
            VERIFY(connect(obj_parent, SIGNAL(Resize(const QRect&)), 
                           this, SLOT(onParentSizeChanged(const QRect&))));
        }
    }
}

bool QtExtPictureWidget::setImage(const QByteArray& bytes)
{
    bool bRet = false;

    m_pixmap_.loadFromData(bytes);
    if (!m_pixmap_.isNull())
    {
        bRet = true;
        QPixmapCache::insert(m_url_.url(), m_pixmap_);
        renderPixmap();
    }
    else
    {
        clear();
    }

    return bRet;
}

void QtExtPictureWidget::clear()
{
    if (!m_pixmap_.isNull())
    {
        m_pixmap_ = QPixmap();
    }

    if (!m_scaled_pixmap_.isNull())
    {
        m_scaled_pixmap_ = QPixmap();
    }

    if (m_readWatcher_)
    {
        m_readWatcher_->disconnect(this);
    }

    if (m_scaleWatcher_)
    {
        m_scaleWatcher_->disconnect(this);
    }

    m_url_.clear();
    update();
}

void QtExtPictureWidget::updateProperty(const PictureWidgetProperty &pictureWidgetProperty)
{
    m_transformationMode_   = pictureWidgetProperty.transformationMode_;
    m_imageRatio_           = pictureWidgetProperty.imageRatio_;
    m_imageMinRatio_        = pictureWidgetProperty.imageMinRatio_;
    m_imageSize_            = pictureWidgetProperty.imageSize_;
    m_imageSizePolicy_      = pictureWidgetProperty.imageSizePolicy_;
    m_imagePosition_        = pictureWidgetProperty.imagePosition_;

    m_widgetSizeRatio_  = pictureWidgetProperty.widgetSizeRatio_;
    m_widgetPosition_   = pictureWidgetProperty.widgetPosition_;
    m_widgetSize_       = pictureWidgetProperty.widgetSize_;
    m_widgetSizePolicy_ = pictureWidgetProperty.widgetSizePolicy_;

    m_url_ = pictureWidgetProperty.imageUrl_;
    updateWidgetSize();
}

QtExtPictureWidget::PictureWidgetProperty QtExtPictureWidget::getProperty() const
{
    PictureWidgetProperty pictureWidgetProperty;
    pictureWidgetProperty.transformationMode_   = m_transformationMode_;
    pictureWidgetProperty.imageMinRatio_        = m_imageMinRatio_;
    pictureWidgetProperty.imageRatio_           = m_imageRatio_;
    pictureWidgetProperty.imageSize_            = m_imageSize_;
    pictureWidgetProperty.imageSizePolicy_      = m_imageSizePolicy_;
    pictureWidgetProperty.imagePosition_        = m_imagePosition_;
    pictureWidgetProperty.imageUrl_             = m_url_;

    pictureWidgetProperty.widgetSizeRatio_  = m_widgetSizeRatio_;
    pictureWidgetProperty.widgetPosition_   = m_widgetPosition_;
    pictureWidgetProperty.widgetSize_       = m_widgetSize_;
    pictureWidgetProperty.widgetSizePolicy_ = m_widgetSizePolicy_;
    return pictureWidgetProperty;
}

void QtExtPictureWidget::checkImageRatio(double &value) const
{
    value = value < m_imageMinRatio_ ? m_imageMinRatio_ : value;
}

QUrl QtExtPictureWidget::url() const
{
    return m_url_;
}

void QtExtPictureWidget::setUrl(const QUrl& url)
{
    setImage(url);
}

QPixmap QtExtPictureWidget::scaledPixmap(QPixmap pixmap, 
                                         QSize widgetSize, 
                                         QSize imageSize, 
                                         double imageRatio, 
                                         ImageSizePolicy imageSizePolicy, 
                                         WidgetSizePolicy widgetSizePolicy, 
                                         Qt::TransformationMode transformationMode)
{

    if (pixmap.isNull() || widgetSize.isEmpty())
    {
        return QPixmap();
    }

    QSize image_size    = pixmap.size();
    int origin_width    = widgetSize.width();
    int origin_height   = widgetSize.height();
    int image_width     = image_size.width();
    int image_height    = image_size.height();

    auto scaledImageSize = [image_size, origin_width, origin_height](const int scaledWidth, 
                                                                     const QSize &limitSize)->QSize {
            double ratio        = image_size.width() / (1.0*image_size.height());
            double scale_width  = scaledWidth*0.95;
            double scale_height = scale_width / ratio;
            if (limitSize.width() > scale_width || limitSize.height() > scale_height)
            {
                return limitSize;
            }
            else
            {
                while ((scale_width > origin_width || scale_height > origin_height) && 
                        (scale_width > limitSize.width() && scale_height > limitSize.height()))
                {
                    scale_width = scale_width * 0.95;
                    scale_height = scale_width / ratio;
                }
            }

            return QSize(static_cast<int>(scale_width), static_cast<int>(scale_height));
    };

#define PIXMAP_SCALE(pixmap, size) { return pixmap.scaled(size, Qt::IgnoreAspectRatio, transformationMode); }

    switch (imageSizePolicy)
    {
    case ImageSizePolicy::AdjustifyImageSize:
        PIXMAP_SCALE(pixmap, scaledImageSize(origin_width, QSize(0, 0)));
        break;
    case ImageSizePolicy::ScaleImageByRatio:
    {
        if (imageRatio > 1e-6)
        {
            PIXMAP_SCALE(pixmap, QSize(image_width*imageRatio, image_height*imageRatio));
        }
        else
        {
            return QPixmap();
        }
    }
        break;
    case ImageSizePolicy::AdjustifyMaxImageSize:
    {
        if (image_width > origin_width || image_height > origin_height)
        {
            PIXMAP_SCALE(pixmap, scaledImageSize(image_width, QSize(0,0)));
        }
        else
        {
            return pixmap;
        }
    }
        break;
    case ImageSizePolicy::AdjustifyMinImageSize:
    {
        double ratio = image_size.width() / (1.0*image_size.height());
        if (origin_width/ratio > origin_height)
        {
            PIXMAP_SCALE(pixmap, scaledImageSize(origin_width, image_size));
        }
        else
        {
            if ((image_width > origin_width || image_height > origin_height))
            {
                PIXMAP_SCALE(pixmap, image_size);
            }
            else
            {
                PIXMAP_SCALE(pixmap, scaledImageSize(origin_width, image_size));
            }
        }
    }
        break;
    case ImageSizePolicy::ScaleImageWidth:
        PIXMAP_SCALE(pixmap, QSize(origin_width, image_height));
        break;
    case ImageSizePolicy::ScaleImageHeight:
        PIXMAP_SCALE(pixmap, QSize(image_width, origin_height));
        break;
    case ImageSizePolicy::Stretch:
        PIXMAP_SCALE(pixmap, QSize(origin_width, origin_height));
        break;
    case ImageSizePolicy::FixImageSize:
    {
        if (0 == imageSize.width() || 0 == imageSize.height())
        {
            return QPixmap();
        }
        else
        {
            PIXMAP_SCALE(pixmap, imageSize);
        }
        break;
    }
    case ImageSizePolicy::SelfSize:
    case ImageSizePolicy::ImageSizeNone:
        return pixmap;
    default:
        break;
    }

#undef PIXMAP_SCALE

    return pixmap;
}

void QtExtPictureWidget::resizeEvent(QResizeEvent *ev)
{
    updateWidgetSize();
    QWidget::resizeEvent(ev);
}

void QtExtPictureWidget::paintEvent(QPaintEvent *paintEvent)
{
    // 1.画图片
    QPainter painter(this);
    QPoint image_pos = calcImagePosition();

    // 如果返回的image_pos为负数，说明图片比widget大
    // 需要对图片进行裁剪
    if (image_pos.x() < 0 || image_pos.y() < 0)
    {
        drawClipImage(painter, image_pos);
    }
    else
    {
        painter.drawPixmap(image_pos, m_scaled_pixmap_, this->rect());
    }

// #ifdef _DEBUG
    painter.setPen(Qt::red);
    painter.drawRect(image_pos.x(), image_pos.y(), m_scaled_pixmap_.width(), m_scaled_pixmap_.height());
    painter.drawRect(0, 0, this->width()-1, this->height()-1);
// #endif // _DEBUG
}

void QtExtPictureWidget::renderPixmap()
{
    VERIFY_RETURN(checkProperty());

    QSize widget_size = this->size();
    QPixmap* pixmap = nullptr;
    if (!m_pixmap_.isNull())
    {
        pixmap = &m_pixmap_;
    }
    else if (!m_defaultPixmap_.isNull())
    {
        pixmap = &m_defaultPixmap_;
    }
    else
    {
        return;
    }

    if (m_transformationMode_ == Qt::SmoothTransformation)
    {
        if (!m_scaleWatcher_)
        {
            m_scaleWatcher_ = new QFutureWatcher<QPixmap>(this);
        }

        m_scaleWatcher_->cancel();
        m_scaleWatcher_->disconnect(this);
        VERIFY(connect(m_scaleWatcher_, SIGNAL(finished()), this, SLOT(onScaleWatcherFinished())));
        VERIFY(connect(m_scaleWatcher_, SIGNAL(canceled()), this, SLOT(onScaleWatcherCanceled())));

        auto pixmapNew = *pixmap;
        auto future = QtConcurrent::run([ = ]
        {
            return scaledPixmap(pixmapNew, this->size(), m_imageSize_, m_imageRatio_, 
                                m_imageSizePolicy_, m_widgetSizePolicy_, Qt::SmoothTransformation);
        });
        m_scaleWatcher_->setFuture(future);
    }
    else
    {
        m_scaled_pixmap_ = this->scaledPixmap(*pixmap, this->size(), m_imageSize_, m_imageRatio_, 
                                              m_imageSizePolicy_, m_widgetSizePolicy_, m_transformationMode_);
        update();
        emit renderFinished();
    }
}

void QtExtPictureWidget::updateWidgetSize()
{
    // 此函数不能再调用resize(), setGeometry()，否则会造成死循环
    VERIFY_RETURN(checkProperty());
    switch (m_widgetSizePolicy_)
    {
    case QtExtPictureWidget::WidgetSizeNone:
    {
        updateSizeAndPixmap(this->size());
        this->setMinimumSize(QSize(0, 0));
        this->setMaximumSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
    }
        break;
    case QtExtPictureWidget::FixSize:
        this->setFixedSize(m_widgetSize_);
        break;
    case QtExtPictureWidget::UseImageSize:
        if (!m_scaled_pixmap_.isNull())
        {
            updateSizeAndPixmap(m_scaled_pixmap_.size());
        }
        break;
    case QtExtPictureWidget::ScaleSize:
    {
        // 含有父窗口，在自动缩放时，宽和高均不能超过parent widget 的size
        if (QWidget *parent = this->parentWidget())
        {
            int parent_width    = parent->width();
            int parent_height   = parent->height();
            int now_width       = parent_width;
            int now_height      = now_width/m_widgetSizeRatio_;
            while (now_width > parent_width || now_height > parent_height)
            {
                now_width = now_width * 0.95;
                now_height = now_width / m_widgetSizeRatio_;
            }
            updateSizeAndPixmap(QSize(now_width, now_height));
        }
        else
        {
            QSize size = this->size();
            updateSizeAndPixmap(QSize(size.width(), size.width()/m_widgetSizeRatio_));
        }
    }
        break;
    case QtExtPictureWidget::ScaleWidgetHeight:
        updateSizeAndPixmap(QSize(m_widgetSize_.width(), m_widgetSize_.width()/m_widgetSizeRatio_));
        break;
    case QtExtPictureWidget::ScaleWidgetWidth:
        updateSizeAndPixmap(QSize(m_widgetSize_.height()*m_widgetSizeRatio_, m_widgetSize_.height()));
        break;
    case QtExtPictureWidget::ScaleParentSize:
    {
        if (QWidget *parent = this->parentWidget())
        {
            updateSizeAndPixmap(parent->size());
        }
    }
        break;
    case QtExtPictureWidget::Scale2ParentHeight:
    {
        if (QWidget *parent = this->parentWidget())
        {
            updateSizeAndPixmap(QSize(m_widgetSize_.width(), parent->height()));
        }
    }
        break;
    case QtExtPictureWidget::Scale2ParentWidth:
    {
        if (QWidget *parent = this->parentWidget())
        {
            updateSizeAndPixmap(QSize(parent->width(), m_widgetSize_.height()));
        }
    }
        break;
//    case QtExtPictureWidget::ScaleParentWidthKeepRatio:
//    case QtExtPictureWidget::ScaleParentHeightKeepRatio:
//        return;
    default:
        break;
    }
}

QPoint QtExtPictureWidget::calcImagePosition()
{
    QPoint image_pos = QPoint(0, 0);
    if (m_scaled_pixmap_.isNull())
    {
        return image_pos;
    }

    return calcPosition(m_scaled_pixmap_.size(), size(), m_imagePosition_);
}

void QtExtPictureWidget::drawClipImage(QPainter &painter, const QPoint &imagePos)
{
    QRect draw_rect     = { 0, 0, 0, 0 };
    QRect source_rect   = { 0, 0, 0, 0 };
    
    if (imagePos.x() < 0)
    {
        draw_rect.setX(0);
        draw_rect.setWidth(this->width());
        source_rect.setX(qAbs(imagePos.x()));
        source_rect.setWidth(m_scaled_pixmap_.width()+2*imagePos.x());
    }
    else
    {
        draw_rect.setX(imagePos.x());
        draw_rect.setWidth(m_scaled_pixmap_.width());
        source_rect.setX(0);
        source_rect.setWidth(m_scaled_pixmap_.width());
    }

    if (imagePos.y() < 0)
    {
        draw_rect.setY(0);
        draw_rect.setHeight(this->height());
        source_rect.setY(qAbs(imagePos.y()));
        source_rect.setHeight(m_scaled_pixmap_.height() + 2*imagePos.y());
    }
    else
    {
        draw_rect.setY(imagePos.y());
        draw_rect.setHeight(m_scaled_pixmap_.height());
        source_rect.setY(0);
        source_rect.setHeight(m_scaled_pixmap_.height());
    }

    painter.drawPixmap(draw_rect, m_scaled_pixmap_, source_rect);
}

void QtExtPictureWidget::updateSizeAndPixmap(const QSize &size)
{
    if (this->size() == size)
    {
        return;
    }
    m_widgetSize_ = size;
    this->setFixedSize(size);
    updateWidgetPos();
    renderPixmap();
}

QPoint QtExtPictureWidget::calcPosition(const QSize &childSize, 
                                        const QSize &parentSize, 
                                        ObjectPosition objPosition)
{
    int child_width     = childSize.width();
    int child_height    = childSize.height();
    int parent_width    = parentSize.width();
    int parent_height   = parentSize.height();

    std::map<ObjectPosition, QPoint> mapObjectPosSize = 
    {
        { ObjectPosition::TopLeft,        QPoint(0, 0) },
        { ObjectPosition::TopMiddle,      QPoint((parent_width-child_width)/2, 0) },
        { ObjectPosition::TopRight,       QPoint(parent_width-child_width, 0) },
        { ObjectPosition::RightMiddle,    QPoint(parent_width-child_width, (parent_height-child_height)/2) },
        { ObjectPosition::RightBottom,    QPoint(parent_width-child_width, parent_height-child_height) },
        { ObjectPosition::BottomMiddle,   QPoint((parent_width-child_width)/2, parent_height-child_height) },
        { ObjectPosition::BottomLeft,     QPoint(0, parent_height-child_height) },
        { ObjectPosition::LeftMiddle,     QPoint(0, (parent_height-child_height)/2) },
        { ObjectPosition::HVCenter,       QPoint((parent_width-child_width)/2, (parent_height-child_height)/2) },
    };
    
    auto ifind = mapObjectPosSize.find(objPosition);
    return ifind != mapObjectPosSize.end() ? ifind->second : this->pos();
}

bool QtExtPictureWidget::checkProperty() const
{
    if (WidgetSizePolicy::UseImageSize == m_widgetSizePolicy_ && 
        (m_imageSizePolicy_ >= AdjustifyMaxImageSize && m_imageSizePolicy_ <= ScaleImageHeight))
    {
        qWarning() << "widget size policy(UseImageSize) conflict with image size policy(AdjustifyMaxImageSize~ScaleImageHeight)";
        return false;
    }

    if (ScaleImageByRatio == m_imageSizePolicy_ && (m_imageRatio_ < 1e-6))
    {
        qWarning() << "image size ratio must be greater than 0.0";
        return false;
    }

    if (m_widgetSizeRatio_ < 1e-6 &&
        (m_widgetSizePolicy_ >= ScaleSize && m_widgetSizePolicy_ <= ScaleWidgetHeight))
    {
        qWarning() << "widget size ratio must be greater than 0.0";
        return false;
    }

    return true;
}

void QtExtPictureWidget::updateWidgetPos()
{
    if (QWidget *parent = this->parentWidget())
    {
        this->move(calcPosition(this->size(), parent->size(), m_widgetPosition_));
    }
}

void QtExtPictureWidget::onReadWatcherFinished()
{
    __try
    {
        _ReadWatcherFinishedHandler();
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        //qDebug() << "onReadWatcherFinished exception";
    }
}

void QtExtPictureWidget::_ReadWatcherFinishedHandler()
{
    QByteArray bytes = m_readWatcher_->result();
    if (!bytes.isEmpty())
    {
        bool bRet = setImage(bytes);
        if (bRet)
        {
            m_bUseDefaultImage = false;
            emit ShowImage(false);
        }
    }
    else
    {
        clear();
    }
}

void QtExtPictureWidget::onReadWatcherCanceled()
{
    qDebug() << "onReadWatcherCanceled";
}

void QtExtPictureWidget::onScaleWatcherFinished()
{
    m_scaled_pixmap_    = m_scaleWatcher_->result();
    m_imageSize_        = m_scaled_pixmap_.size();
    update();
    // 图片的大小反过来影响widget size
    if (WidgetSizePolicy::UseImageSize == m_widgetSizePolicy_)
    {
        updateWidgetSize();
    }
    emit renderFinished();
}

void QtExtPictureWidget::onScaleWatcherCanceled()
{
    qDebug() << "onScaleWatcherCanceled";
}

void QtExtPictureWidget::onParentSizeChanged(const QRect &rect)
{
    updateWidgetSize();
}
