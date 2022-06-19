#ifndef QT_EXT_BUTTON_H
#define QT_EXT_BUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QPixmap>

class QtExtButton : public QPushButton
{
    enum class ButtonState
    {
        Normal,
        Hover,
        Pressed,
        Disabled,
    };

    struct BTN_IMAGE
    {
        QPixmap normal_pixmap_;
        QPixmap hover_pixmap_;
        QPixmap pressed_pixmap_;
        QPixmap disabled_pixmap_;
    };

public:
    QtExtButton(QWidget *parent = nullptr);
    ~QtExtButton();
    // 0 normal, 1 hover, 2 pressed, 3 disabled
    void setImagePath(const QStringList &path);

protected:
    virtual void paintEvent(QPaintEvent *ev) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;

private:
    ButtonState state_ = ButtonState::Normal;
    BTN_IMAGE btn_image_; 
};

#endif // QT_EXT_BUTTON_H