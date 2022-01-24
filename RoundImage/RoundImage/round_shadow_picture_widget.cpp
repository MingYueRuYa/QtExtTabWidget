#include "round_shadow_picture_widget.h"
#include "round_image_helper.h"

#include <QPainter>

RoundShadowPictureWidget::RoundShadowPictureWidget(const QPixmap& pixmap,
                                                   QWidget* parent)
    : pixmap_(pixmap), QWidget(parent) {
  setupUI();
  updateUI();
}

RoundShadowPictureWidget::~RoundShadowPictureWidget() {}

void RoundShadowPictureWidget::setupUI() {
  RoundImageHelper helper;
  round_shadow_pixmap_ = helper.RoundImage(pixmap_);
}

void RoundShadowPictureWidget::updateUI() {
  this->setFixedSize(pixmap_.size());
}

void RoundShadowPictureWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.drawPixmap(this->rect(), round_shadow_pixmap_);
}

void RoundShadowPictureWidget::mousePressEvent(QMouseEvent* event) {
  move_widget_helper_.mousePressEvent(event, pos());
}

void RoundShadowPictureWidget::mouseReleaseEvent(QMouseEvent* event) {
  move_widget_helper_.mouseReleaseEvent(event);
}

void RoundShadowPictureWidget::mouseMoveEvent(QMouseEvent* event) {
  if (move_widget_helper_.CanMove()) {
    move(move_widget_helper_.mouseMoveEvent(event));
  }
}
