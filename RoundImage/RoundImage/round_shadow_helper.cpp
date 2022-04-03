#include "round_shadow_helper.h"

#include <QPainter>
#include <QPainterPath>

using RoundDirc = RoundShadowHelper::RoundDirection;

// 1.阴影的距离可以调整
// 2.阴影可以调整
// 3.圆角可以调整
// 4.圆角4个阴影可以调整
RoundShadowHelper::RoundShadowHelper(const int shadow_width, const int radius)
    : shadow_width_(shadow_width), radius_(radius) {}

void RoundShadowHelper::RoundShadow(QPainter* painter,
                                    const QRect& source_rect,
                                    RoundDirection dirc) {
  //不处理空数据或者错误数据
  if (nullptr == painter)
    return;

  const int kBORDER_DISTANCE = shadow_width_ / 2;
  const int kSHADOW_WIDTH2X = shadow_width_ * 2;

  painter->setRenderHints(QPainter::Antialiasing, true);
  painter->setRenderHints(QPainter::SmoothPixmapTransform, true);

  draw4BorderRectShadow(painter, shadow_width_, kSHADOW_WIDTH2X, source_rect,
                        dirc);
  draw4BorderArcShadow(painter, shadow_width_, kSHADOW_WIDTH2X, source_rect,
                       dirc);

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

void RoundShadowHelper::FillRoundShadow(QPainter* painter,
                                        const QRect& rect,
                                        const QColor& fill_color,
                                        int radius) {
  painter->save();
  QPainterPath path;
  path.addRoundedRect(rect, radius, radius, Qt::AbsoluteSize);
  painter->setClipPath(path);
  painter->setPen(Qt::NoPen);
  painter->setBrush(fill_color);
  painter->drawPath(path);
  painter->restore();
}

void RoundShadowHelper::draw4BorderRectShadow(QPainter* painter,
                                              int shadow_width,
                                              int shadow_width2X,
                                              const QRect& dest_rect,
                                              RoundDirection dirc) {
  const int rect_width = dest_rect.width();
  const int rect_height = dest_rect.height();
  const int rect_pos_x = dest_rect.x();
  const int rect_pos_y = dest_rect.y();
  const QSize horizontal_size =
      QSize(rect_width - shadow_width2X, shadow_width);
  const QSize vertical_size = QSize(shadow_width, rect_height - shadow_width2X);

  QPoint start_point = QPoint(0, 0);
  QPoint end_point = QPoint(0, 0);
  // 上边
  if (dirc & RoundDirc::D_TOP) {
    start_point =
        QPoint(rect_pos_x + rect_width / 2.0, rect_pos_y + shadow_width);
    end_point = QPoint(rect_pos_x + rect_width / 2, 0 + rect_pos_y);
    drawShadowRect(
        painter, start_point, end_point,
        QRect(QPoint(shadow_width + rect_pos_x, rect_pos_y), horizontal_size));
  }
  // 下边
  if (dirc & RoundDirc::D_BOTTOM) {
    start_point = QPoint(rect_pos_x + rect_width / 2.0,
                         rect_pos_y + rect_height - shadow_width);
    end_point = QPoint(rect_pos_x + rect_width / 2.0, rect_pos_y + rect_height);
    drawShadowRect(painter, start_point, end_point,
                   QRect(QPoint(shadow_width + rect_pos_x,
                                rect_height - shadow_width + rect_pos_y),
                         horizontal_size));
  }

  // 左边
  if (dirc & RoundDirc::D_LEFT) {
    start_point =
        QPoint(rect_pos_x + shadow_width, rect_pos_y + rect_height / 2);
    end_point = QPoint(rect_pos_x, rect_pos_y + rect_height / 2);
    drawShadowRect(
        painter, start_point, end_point,
        QRect(QPoint(rect_pos_x, shadow_width + rect_pos_y), vertical_size));
  }
  // 右边
  if (dirc & RoundDirc::D_RIGHT) {
    start_point = QPoint(rect_pos_x + rect_width - shadow_width,
                         rect_pos_y + rect_height / 2);
    end_point = QPoint(rect_pos_x + rect_width, rect_pos_y + rect_height / 2);
    drawShadowRect(painter, start_point, end_point,
                   QRect(QPoint(rect_width - shadow_width + rect_pos_x,
                                shadow_width + rect_pos_y),
                         vertical_size));
  }
}

void RoundShadowHelper::draw4BorderArcShadow(QPainter* painter,
                                             int shadow_width,
                                             int shadow_width2X,
                                             const QRect& dest_rect,
                                             RoundDirection dirc) {
  const int rect_height = dest_rect.height();
  const int rect_width = dest_rect.width();
  const QSize rect_size = QSize(shadow_width2X, shadow_width2X);
  QPoint top_left_pos = dest_rect.topLeft();

  QPoint center_pos = QPoint(0, 0);
  QPainterPath arc_path;
  // 上左边
  if (dirc & RoundDirc::D_TOP) {
    center_pos = QPoint(shadow_width, shadow_width) + top_left_pos;
    arc_path.moveTo(center_pos);
    arc_path.arcTo(QRect(top_left_pos, rect_size), 90, 90);
    drawShadowArc(painter, center_pos, top_left_pos, arc_path);
  }

  // 下左边
  if (dirc & RoundDirc::D_LEFT) {
    QPoint bottom_left_pos =
        QPoint(dest_rect.x(), dest_rect.y() + dest_rect.height());
    center_pos = QPoint(shadow_width, -shadow_width) + bottom_left_pos;
    arc_path.moveTo(center_pos);
    arc_path.arcTo(
        QRect(QPoint(0, -shadow_width2X) + bottom_left_pos, rect_size), 180,
        90);
    drawShadowArc(painter, center_pos, bottom_left_pos, arc_path);
  }
  // 上右边
  if (dirc & RoundDirc::D_RIGHT) {
    QPoint top_right_pos = QPoint(dest_rect.x() + rect_width, dest_rect.y());
    center_pos = QPoint(-shadow_width, shadow_width) + top_right_pos;
    arc_path.moveTo(center_pos);
    arc_path.arcTo(QRect(QPoint(-shadow_width2X, 0) + top_right_pos, rect_size),
                   0, 90);
    drawShadowArc(painter, center_pos, top_right_pos, arc_path);
  }

  // 下右边
  if (dirc & RoundDirc::D_BOTTOM) {
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
