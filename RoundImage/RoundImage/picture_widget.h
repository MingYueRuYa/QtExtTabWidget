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

    ͼƬ�ؼ�:
    1.֧��ͼƬ����
    2.֧��widget��������
    3.֧��ͼƬ��widget �Ź���λ������
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
        WidgetSizeNone = 0,           // �޶������ò����Զ�����

        //size���ɱ�
        FixSize,                // m_widgetSize_Ϊ׼
        UseImageSize,

        // �ɱ䣬��������
        ScaleSize,              // ��͸߶��仯�����ֱ�������m_widgetSize_Ϊ�ο�
        ScaleWidgetWidth,       // �߶Ȳ��䣬���ſ�ȣ���m_widgetSize_Ϊ�ο�
        ScaleWidgetHeight,      // ��Ȳ��䣬���Ÿ߶ȣ���m_widgetSize_Ϊ�ο�
        // ScaleParentWidthKeepRatio,  // ��parent heightΪ׼�����ֱ�������width --- δʵ��
        // ScaleParentHeightKeepRatio, // ��parent widthΪ׼�����ֱ�������height --- δʵ��

        // �ɱ䣬�������
        ScaleParentSize,        // ��parent sizeΪ׼
        Scale2ParentWidth,     // �߶Ȳ��䣬width���ֺ�parentһ��
        Scale2ParentHeight,    // ��Ȳ��䣬height���ֺ�parentһ��
    };

    enum ImageSizePolicy
    {
        // TODO:ͼƬ����widget������ʱ������
        ImageSizeNone = 0,
        // ���ɱ�
        FixImageSize,       // ͼƬ��С�̶����û���������m_imageSize_Ϊ׼
        SelfSize,           // ͼƬ��С�̶���ʹ��ͼƬ��С

        // �ɱ䣺ȷ������
        ScaleImageByRatio,      // ����ͼƬ�������ţ��Կ��Ϊ׼������image_width*ratio
        AdjustifyMaxImageSize,  // ͼƬ����Ӧ��С������ΪͼƬ�����С
        AdjustifyMinImageSize,  // ͼƬ����Ӧ�Ŵ�����ΪͼƬ�����С
        AdjustifyImageSize,     // ���ֱ�������

        // �ɱ䣺��ȷ������
        Stretch,                // ˫������
        ScaleImageWidth,        // �߶Ȳ��䣬�������
        ScaleImageHeight,       // ��Ȳ��䣬�߶�����
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


    // �����ṩ���Խӿڣ��������Ա����ζ�image����
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
