#include "round_shadow_helper.h"

#include <QPainter>
#include <QPainterPath>

// 1.阴影的距离可以调整
// 2.阴影可以调整
// 3.圆角可以调整
// 4.圆角4个阴影可以调整
RoundShadowHelper::RoundShadowHelper(const int shadow_width, const int radius)
    : shadow_width_(shadow_width), radius_(radius) {}

void RoundShadowHelper::RoundShadow(QPainter* painter,
                                    const QRect& source_rect) {
  //不处理空数据或者错误数据
  if (nullptr == painter)
    return;

  const int kBORDER_DISTANCE = shadow_width_ / 2;
  const int kSHADOW_WIDTH2X = shadow_width_ * 2;

  painter->setRenderHints(QPainter::Antialiasing, true);
  painter->setRenderHints(QPainter::SmoothPixmapTransform, true);

  draw4BorderRectShadow(painter, shadow_width_, kSHADOW_WIDTH2X, source_rect);
  draw4BorderArcShadow(painter, shadow_width_, kSHADOW_WIDTH2X, source_rect);

  //  // 将图片裁剪为圆角
  //  QPainterPath path;
  //  QRect rect(kBORDER_DISTANCE, kBORDER_DISTANCE,
  //             image_width - kBORDER_DISTANCE * 2,
  //             image_height - kBORDER_DISTANCE * 2);
  //  path.addRoundedRect(rect, radius_, radius_, Qt::AbsoluteSize);
  //  painter->setClipPath(path);
  //  //  painter->drawPixmap(kBORDER_DISTANCE, kBORDER_DISTANCE, image_width,
  //  //                     image_height, new_pixmap);
  //  painter->setPen(Qt::NoPen);
  //  painter->setBrush(Qt::white);
  //  painter->drawPath(path);
}

void RoundShadowHelper::draw4BorderRectShadow(QPainter* painter,
                                              int shadow_width,
                                              int shadow_width2X,
                                              const QRect& dest_rect) {
  int rect_width = dest_rect.width();
  int rect_height = dest_rect.height();
  int rect_pos_x = dest_rect.x();
  int rect_pos_y = dest_rect.y();
  // 上边
  QPoint start_point =
      QPoint(rect_pos_x + rect_width / 2.0, shadow_width + rect_pos_y);
  QPoint end_point = QPoint(rect_pos_x + rect_width / 2, 0 + rect_pos_y);
  drawShadowRect(painter, start_point, end_point,
                 QRect(shadow_width + rect_pos_x, rect_pos_y,
                       rect_width - shadow_width2X, shadow_width));
  // 下边
  start_point = QPoint(rect_pos_x + rect_width / 2.0,
                       rect_pos_y + rect_height - shadow_width);
  end_point = QPoint(rect_pos_x + rect_width / 2.0, rect_pos_y + rect_height);
  drawShadowRect(
      painter, start_point, end_point,
      QRect(shadow_width + rect_pos_x, rect_height - shadow_width + rect_pos_y,
            rect_width - shadow_width2X, rect_height));

  // 左边
  start_point = QPoint(rect_pos_x + shadow_width, rect_pos_y + rect_height / 2);
  end_point = QPoint(rect_pos_x, rect_pos_y + rect_height / 2);
  drawShadowRect(painter, start_point, end_point,
                 QRect(0 + rect_pos_x, shadow_width + rect_pos_y, shadow_width,
                       rect_height - shadow_width2X));
  // 右边
  start_point = QPoint(rect_pos_x + rect_width - shadow_width,
                       rect_pos_y + rect_height / 2);
  end_point = QPoint(rect_pos_x + rect_width, rect_pos_y + rect_height / 2);
  drawShadowRect(
      painter, start_point, end_point,
      QRect(rect_width - shadow_width + rect_pos_x, shadow_width + rect_pos_y,
            rect_width - shadow_width, rect_height - shadow_width2X));
}

void RoundShadowHelper::draw4BorderArcShadow(QPainter* painter,
                                             int shadow_width,
                                             int shadow_width2X,
                                             const QRect& dest_rect) {
  const int rect_height = dest_rect.height();
  const int rect_width = dest_rect.width();
  const QSize rect_size = QSize(shadow_width2X, shadow_width2X);
  QPoint top_left_pos = dest_rect.topLeft();
  // 上左边
  QPoint center_pos = QPoint(shadow_width, shadow_width) + top_left_pos;
  QPainterPath arc_path;
  arc_path.moveTo(center_pos);
  arc_path.arcTo(QRect(top_left_pos, rect_size), 90, 90);
  drawShadowArc(painter, center_pos, top_left_pos, arc_path);

  // 下左边
  QPoint bottom_left_pos =
      QPoint(dest_rect.x(), dest_rect.y() + dest_rect.height());
  center_pos = QPoint(shadow_width, -shadow_width) + bottom_left_pos;
  arc_path.moveTo(center_pos);
  arc_path.arcTo(QRect(QPoint(0, -shadow_width2X) + bottom_left_pos, rect_size),
                 180, 90);
  drawShadowArc(painter, center_pos, bottom_left_pos, arc_path);

  // 上右边
  QPoint top_right_pos = QPoint(dest_rect.x() + rect_width, dest_rect.y());
  center_pos = QPoint(-shadow_width, shadow_width) + top_right_pos;
  arc_path.moveTo(center_pos);
  arc_path.arcTo(QRect(QPoint(-shadow_width2X, 0) + top_right_pos, rect_size),
                 0, 90);
  drawShadowArc(painter, center_pos, top_right_pos, arc_path);

  // 下右边
  QPoint bottom_right_pos =
      QPoint(dest_rect.x() + rect_width, dest_rect.y() + rect_height);
  center_pos = QPoint(-shadow_width, -shadow_width) + bottom_right_pos;
  arc_path.moveTo(center_pos);
  arc_path.arcTo(
      QRect(QPoint(-shadow_width2X, -shadow_width2X) + bottom_right_pos,
            rect_size),
      0, -90);
  drawShadowArc(painter, center_pos, bottom_right_pos, arc_path);
}

void RoundShadowHelper::drawShadowRect(QPainter* painter,
                                       const QPoint& startPoint,
                                       const QPoint& endPoint,
                                       const QRect& destRect) {
  painter->save();
  QLinearGradient linear(startPoint, endPoint);
  getGradient(startPoint, endPoint, linear);
  painter->setPen(Qt::NoPen);
  painter->setBrush(linear);
  painter->drawRect(destRect);
  painter->restore();
}

void RoundShadowHelper::drawShadowArc(QPainter* painter,
                                      const QPoint& startPoint,
                                      const QPoint& endPoint,
                                      const QPainterPath& painterPath) {
  painter->save();
  QRadialGradient radial_gardeint;
  getGradient(startPoint, shadow_width_, radial_gardeint);
  painter->setBrush(radial_gardeint);
  painter->setPen(Qt::NoPen);
  painter->drawPath(painterPath);
  painter->restore();
}

void RoundShadowHelper::getGradient(const QPoint& start_point,
                                    const int radius,
                                    QRadialGradient& radial_gradient) {
  radial_gradient = QRadialGradient(start_point, radius);
  _getGradient(radial_gradient);
}

void RoundShadowHelper::getGradient(const QPoint& start_point,
                                    const QPoint& end_point,
                                    QLinearGradient& linear_gradient) {
  linear_gradient = QLinearGradient(start_point, end_point);
  _getGradient(linear_gradient);
}

void RoundShadowHelper::_getGradient(QGradient& gradient) {
  QColor color = Qt::gray;
  gradient.setColorAt(0, color);
  color.setAlpha(50);
  gradient.setColorAt(0.5, color);
  color.setAlpha(30);
  gradient.setColorAt(0.6, color);
  color.setAlpha(0);
  gradient.setColorAt(1, color);
  gradient.setSpread(QGradient::PadSpread);
}
