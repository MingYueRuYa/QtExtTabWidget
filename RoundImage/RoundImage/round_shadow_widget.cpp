#include "round_shadow_widget.h"

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

// 1.阴影的距离可以调整
// 2.阴影可以调整
// 3.圆角可以调整
// 4.圆角4个阴影可以调整
// RoundImageHelper::RoundImageHelper(const int shadow_width, const int radius)
//    : shadow_width_(shadow_width), radius_(radius) {}

RoundShadowWidget::RoundShadowWidget(QWidget* parent) : QWidget(parent) {
  setMouseTracking(true);
}

RoundShadowWidget::RoundShadowWidget(const int shadow_width,
                                     const int radius,
                                     QWidget* parent)
    : RoundShadowHelper(shadow_width, radius), QWidget(parent) {}

void RoundShadowWidget::paintEvent(QPaintEvent* ev) {
  const int kBORDER_DISTANCE = shadow_width_ / 2;
  const int kSHADOW_WIDTH = shadow_width_;
  const int kSHADOW_WIDTH2X = shadow_width_ * 2;

  QPainter painter(this);
  painter.setRenderHints(QPainter::Antialiasing, true);
  painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

  RoundShadow(&painter, QRect(0, 0, this->width(), this->height()));

  QPainterPath path;
  QRect rect(kBORDER_DISTANCE, kBORDER_DISTANCE,
             this->width() - kBORDER_DISTANCE * 2,
             this->height() - kBORDER_DISTANCE * 2);
  path.addRoundedRect(rect, radius_, radius_, Qt::AbsoluteSize);
  painter.setClipPath(path);
  painter.setPen(Qt::NoPen);
  painter.setBrush(Qt::white);
  painter.drawPath(path);
}

void RoundShadowWidget::mousePressEvent(QMouseEvent* event) {
  this->raise();
  move_widget_helper_.mousePressEvent(event, pos());
}

void RoundShadowWidget::mouseReleaseEvent(QMouseEvent* event) {
  move_widget_helper_.mouseReleaseEvent(event);
}

void RoundShadowWidget::mouseMoveEvent(QMouseEvent* event) {
  if (move_widget_helper_.CanMove()) {
    move(move_widget_helper_.mouseMoveEvent(event));
  }
}
