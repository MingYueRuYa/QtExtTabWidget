#include "qt_ext_button.h"
#include <QPainter>

QtExtButton::QtExtButton(QWidget *parent)
    : QPushButton(parent)
{
}

QtExtButton::~QtExtButton()
{
}

void QtExtButton::setImagePath(const QStringList &path)
{
    if (path.size() < 4)
        return;
    btn_image_.normal_pixmap_.load(path[0]);
    btn_image_.hover_pixmap_.load(path[1]);
    btn_image_.pressed_pixmap_.load(path[2]);
    btn_image_.disabled_pixmap_.load(path[3]);
    update();
}

void QtExtButton::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    QPixmap *pixmap = nullptr;
    if (!isEnabled()) { // state_ == ButtonState::Disabled
        pixmap = &btn_image_.disabled_pixmap_;
    } else if (state_ == ButtonState::Normal) {
        pixmap = &btn_image_.normal_pixmap_;
    } else if (state_ == ButtonState::Hover) {
        pixmap = &btn_image_.hover_pixmap_;
    } else if (state_ == ButtonState::Pressed) {
        pixmap = &btn_image_.pressed_pixmap_;
    } else {
        return;
    }
    if (!pixmap->isNull())
        painter.drawPixmap(this->rect(), *pixmap);
}

void QtExtButton::enterEvent(QEvent *event)
{
    if (state_ == ButtonState::Hover)
        return;
    state_ = ButtonState::Hover;
    update();
    QPushButton::enterEvent(event);
}

void QtExtButton::leaveEvent(QEvent *event)
{
    if (state_ == ButtonState::Normal)
        return;
    state_ = ButtonState::Normal;
    update();
    QPushButton::leaveEvent(event);
}

void QtExtButton::mousePressEvent(QMouseEvent *event)
{
    if (state_ == ButtonState::Pressed)
        return;
    state_ = ButtonState::Pressed;
    update();
    QPushButton::mousePressEvent(event);
}

void QtExtButton::mouseReleaseEvent(QMouseEvent *event)
{
    if (state_ == ButtonState::Normal)
        return;
    state_ = ButtonState::Normal;
    update();
    QPushButton::mouseReleaseEvent(event);
}