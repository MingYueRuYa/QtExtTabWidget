#ifndef QTEXTPICTURE_WIDGET_H
#define QTEXTPICTURE_WIDGET_H

#include <QUrl>
#include <QBuffer>
#include <QWidget>
#include <QFutureWatcher>

#include "DzDefine.h"

/*! \brief QtExtPictureWidget
    \author liushixiong (635672377@qq.com)
    \version 0.01
    \date 2021-12-27 16:18:05

    图片控件:
    1.支持图片缩放
    2.支持widget本身缩放
    3.支持图片和widget 九宫格位置设置
*/
class QtExtPictureWidget : public QWidget
{
    Q_OBJECT

public:
    enum ObjectPosition 
    {
        PosNone = 0,
        TopLeft = 1,
        TopMiddle,
        TopRight,

        RightMiddle,
        RightBottom,

        BottomMiddle,
        BottomLeft,

        LeftMiddle,
        HVCenter,
    };

    enum WidgetSizePolicy
    {
        WidgetSizeNone = 0,           // 无动作，让布局自动调整

        //size不可变
        FixSize,                // m_widgetSize_为准
        UseImageSize,

        // 可变，比例不变
        ScaleSize,              // 宽和高都变化，保持比例，以m_widgetSize_为参考
        ScaleWidgetWidth,       // 高度不变，缩放宽度，以m_widgetSize_为参考
        ScaleWidgetHeight,      // 宽度不变，缩放高度，以m_widgetSize_为参考
        // ScaleParentWidthKeepRatio,  // 以parent height为准，保持比例缩放width --- 未实现
        // ScaleParentHeightKeepRatio, // 以parent width为准，保持比例缩放height --- 未实现

        // 可变，比例随机
        ScaleParentSize,        // 以parent size为准
        Scale2ParentWidth,     // 高度不变，width保持和parent一致
        Scale2ParentHeight,    // 宽度不变，height保持和parent一致
    };

    enum ImageSizePolicy
    {
        // TODO:图片包裹widget特性暂时不考虑
        ImageSizeNone = 0,
        // 不可变
        FixImageSize,       // 图片大小固定，用户可设置以m_imageSize_为准
        SelfSize,           // 图片大小固定，使用图片大小

        // 可变：确定比例
        ScaleImageByRatio,      // 根据图片比例缩放，以宽度为准，比如image_width*ratio
        AdjustifyMaxImageSize,  // 图片自适应缩小，上限为图片自身大小
        AdjustifyMinImageSize,  // 图片自适应放大，下限为图片自身大小
        AdjustifyImageSize,     // 保持比例拉伸

        // 可变：不确定比例
        Stretch,                // 双向拉伸
        ScaleImageWidth,        // 高度不变，宽度伸缩
        ScaleImageHeight,       // 宽度不变，高度伸缩
    };

    DZ_PROPERTY_B(Qt::TransformationMode, transformationMode, setTransformationMode, Qt::SmoothTransformation, renderPixmap())
    DZ_PROPERTY_B(double, imageMinRatio, setImageMinRatio, 0.0, renderPixmap())
    DZ_PROPERTY_B_2_CALL(double, imageRatio, setImageRatio, 0.0, checkImageRatio(value), renderPixmap())
    DZ_PROPERTY_E(ImageSizePolicy, imageSizePolicy, setImageSizePolicy, ImageSizePolicy::ImageSizeNone, renderPixmap())
    DZ_PROPERTY_B(QSize, imageSize, setImageSize, QSize(0, 0), renderPixmap())
    DZ_PROPERTY_E(ObjectPosition, imagePosition, setImagePosition, ObjectPosition::HVCenter, update())
    DZ_PROPERTY_B(double, widgetSizeRatio, setWidgetSizeRatio, -1, updateWidgetSize())
    DZ_PROPERTY_E(ObjectPosition, widgetPosition, setWidgetPosition, ObjectPosition::HVCenter, updateWidgetPos())
    DZ_PROPERTY_E(WidgetSizePolicy, widgetSizePolicy, setWidgetSizePolicy, WidgetSizePolicy::WidgetSizeNone, updateWidgetSize())
    DZ_PROPERTY_B(QSize, widgetSize, setWidgetSize, QSize(200, 200), updateWidgetSize())
    DZ_PROPERTY_OC(QUrl, url, setUrl)


    // 对外提供属性接口，这样可以避免多次对image操作
    struct PictureWidgetProperty
    {
        PictureWidgetProperty() = default;
        PictureWidgetProperty(QUrl              imageUrl,
                              QSize             imageSize               = QSize(0, 0),
                              double            imageRatio              = 0.0,
                              double            imageMinRatio           = 0.5,
                              ObjectPosition    imagePosition           = ObjectPosition::HVCenter,
                              Qt::TransformationMode transformationMode = Qt::SmoothTransformation,
                              ImageSizePolicy   imageSizePolicy         = ImageSizePolicy::AdjustifyImageSize,
                              QSize             widgetSize              = QSize(200, 200),
                              ObjectPosition    widgetPosition          = ObjectPosition::HVCenter,
                              double            widgetSizeRatio         = 0.0,
                              WidgetSizePolicy  widgetSizePolicy        = WidgetSizePolicy::WidgetSizeNone)
                              :transformationMode_(transformationMode),
                               imageRatio_(imageRatio),
                               imageMinRatio_(imageMinRatio),
                               imageSize_(imageSize),
                               imageSizePolicy_(imageSizePolicy),
                               imagePosition_(imagePosition),
                               imageUrl_(imageUrl),
                               widgetSizeRatio_(widgetSizeRatio),
                               widgetPosition_(widgetPosition),
                               widgetSize_(widgetSize),
                               widgetSizePolicy_(widgetSizePolicy)
        {
        }

        // image property
        QUrl                    imageUrl_;
        QSize                   imageSize_;
        double                  imageRatio_;
        double                  imageMinRatio_;
        ObjectPosition          imagePosition_;
        ImageSizePolicy         imageSizePolicy_;
        Qt::TransformationMode  transformationMode_;

        // widget property
        QSize               widgetSize_;
        double              widgetSizeRatio_;
        ObjectPosition      widgetPosition_;
        WidgetSizePolicy    widgetSizePolicy_;
    };

public:
    explicit QtExtPictureWidget(QWidget* parent = nullptr);
    virtual ~QtExtPictureWidget();

public:
    void setImage(const QUrl& url);
    void setImage(const QString& path) { setImage(QUrl::fromUserInput(path)); }
    void setImage(const char* path) { setImage(QString::fromUtf8(path)); }

    void setDefaultPixmap(const QPixmap& pixmap)
    {
        m_defaultPixmap_ = pixmap;

        if (m_bUseDefaultImage)
        {
            renderPixmap();
            emit ShowImage(true);
        }
    }

    void setDefaultPixmap(const QString& path) { setDefaultPixmap(QPixmap(path)); }
    void clear();
    void updateProperty(const PictureWidgetProperty &pictureWidgetProperty);
    PictureWidgetProperty getProperty() const;
    void checkImageRatio(double &value) const;

protected:
    static QPixmap scaledPixmap(QPixmap pixmap, 
                                QSize size, 
                                QSize imageSize, 
                                double imageRatio, 
                                ImageSizePolicy imageSizePolicy, 
                                WidgetSizePolicy widgetSizePolicy,
                                Qt::TransformationMode transformationMode);
    virtual void resizeEvent(QResizeEvent *ev) override;
    virtual void paintEvent(QPaintEvent *paintEvent) override;

    void renderPixmap();
    void initUI();
    bool setImage(const QByteArray& bytes);

    void updateWidgetPos();
    void updateWidgetSize();
    QPoint calcImagePosition();
    void drawClipImage(QPainter &painter, const QPoint &imagePos);
    void updateSizeAndPixmap(const QSize &size);
    QPoint calcPosition(const QSize &childSize,
                        const QSize &parentSize,
                        ObjectPosition objPosition);
    bool checkProperty() const;

signals:
    void renderFinished();
    void ShowImage(bool bDefaultImage);

protected slots:
    void onParentSizeChanged(const QRect &rect);
    void onReadWatcherFinished();
    void onReadWatcherCanceled();

    void onScaleWatcherFinished();
    void onScaleWatcherCanceled();

protected:
    void _ReadWatcherFinishedHandler();

protected:
    bool m_bUseDefaultImage = true;
    QUrl m_url_;

    // watcher
    QFutureWatcher<QByteArray>* m_readWatcher_ = nullptr;
    QFutureWatcher<QPixmap>* m_scaleWatcher_ = nullptr;

    // pixmap
    QPixmap m_pixmap_;
    QPixmap m_scaled_pixmap_;
    QPixmap m_defaultPixmap_;

};

#endif // QTEXTPICTURE_WIDGET_H
