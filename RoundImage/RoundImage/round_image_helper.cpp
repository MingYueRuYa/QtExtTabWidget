#include "round_image_helper.h"

#include <QPainter>
#include <QPainterPath>

// 1.阴影的距离可以调整
// 2.阴影可以调整
// 3.圆角可以调整
// 4.圆角4个阴影可以调整
RoundImageHelper::RoundImageHelper(const int shadow_width, const int radius)
    : shadow_width_(shadow_width), radius_(radius) {}

QPixmap RoundImageHelper::RoundImage(const QPixmap& source_pixmap) {
  //不处理空数据或者错误数据
  if (source_pixmap.isNull())
    return source_pixmap;

  const int kBORDER_DISTANCE = shadow_width_ / 2;
  const int kSHADOW_WIDTH2X = shadow_width_ * 2;

  //获取图片尺寸
  int image_width = source_pixmap.width();
  int image_height = source_pixmap.height();

  QPixmap new_pixmap = source_pixmap;
  QPixmap dest_image(image_width + shadow_width_, image_height + shadow_width_);
  dest_image.fill(Qt::transparent);
  QPainter painter(&dest_image);
  painter.setRenderHints(QPainter::Antialiasing, true);
  painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

  draw4BorderRectShadow(&painter, shadow_width_, kSHADOW_WIDTH2X,
                        dest_image.size());
  draw4BorderArcShadow(&painter, shadow_width_, kSHADOW_WIDTH2X,
                       dest_image.size());

  // 将图片裁剪为圆角
  QPainterPath path;
  QRect rect(kBORDER_DISTANCE, kBORDER_DISTANCE, image_width, image_height);
  path.addRoundedRect(rect, radius_, radius_, Qt::AbsoluteSize);
  painter.setClipPath(path);
  painter.drawPixmap(kBORDER_DISTANCE, kBORDER_DISTANCE, image_width,
                     image_height, new_pixmap);
  return dest_image;
}

void RoundImageHelper::draw4BorderRectShadow(QPainter* painter,
                                             int shadow_width,
                                             int shadow_width2X,
                                             const QSize& image_size) {
  // 上边
  drawShadowRect(painter, QPoint(image_size.width() / 2, shadow_width),
                 QPoint(image_size.width() / 2, 0),
                 QRect(shadow_width, 0, image_size.width() - shadow_width2X,
                       shadow_width));
  // 下边
  drawShadowRect(
      painter,
      QPoint(image_size.width() / 2, image_size.height() - shadow_width),
      QPoint(image_size.width() / 2, image_size.height()),
      QRect(shadow_width, image_size.height() - shadow_width,
            image_size.width() - shadow_width2X, image_size.height()));
  // 左边
  drawShadowRect(painter, QPoint(shadow_width, image_size.height() / 2),
                 QPoint(0, image_size.height() / 2),
                 QRect(0, shadow_width, shadow_width,
                       image_size.height() - shadow_width2X));
  // 右边
  drawShadowRect(
      painter,
      QPoint(image_size.width() - shadow_width, image_size.height() / 2),
      QPoint(image_size.width(), image_size.height() / 2),
      QRect(image_size.width() - shadow_width, shadow_width,
            image_size.width() - shadow_width,
            image_size.height() - shadow_width2X));
}

void RoundImageHelper::draw4BorderArcShadow(QPainter* painter,
                                            int shadow_width,
                                            int shadow_width2X,
                                            const QSize& image_size) {
  // 上左边
  QPainterPath arc_path;
  arc_path.moveTo(QPointF(shadow_width, shadow_width));
  arc_path.arcTo(QRect(0, 0, shadow_width2X, shadow_width2X), 90, 90);
  drawShadowArc(painter, QPoint(shadow_width, shadow_width), QPoint(0, 0),
                arc_path);

  // 下左边
  arc_path.moveTo(QPointF(shadow_width, image_size.height() - shadow_width));
  arc_path.arcTo(QRect(0, image_size.height() - shadow_width2X, shadow_width2X,
                       shadow_width2X),
                 180, 90);
  drawShadowArc(painter,
                QPoint(shadow_width, image_size.height() - shadow_width),
                QPoint(0, 0), arc_path);

  // 上右边
  arc_path.moveTo(QPointF(image_size.width() - shadow_width, shadow_width));
  arc_path.arcTo(QRect(image_size.width() - shadow_width2X, 0, shadow_width2X,
                       shadow_width2X),
                 0, 90);
  drawShadowArc(painter,
                QPoint(image_size.width() - shadow_width, shadow_width),
                QPoint(image_size.width(), 0), arc_path);

  // 下右边
  arc_path.moveTo(QPointF(image_size.width() - shadow_width,
                          image_size.height() - shadow_width));
  arc_path.arcTo(QRect(image_size.width() - shadow_width2X,
                       image_size.height() - shadow_width2X, shadow_width2X,
                       shadow_width2X),
                 0, -90);
  drawShadowArc(painter,
                QPoint(image_size.width() - shadow_width,
                       image_size.height() - shadow_width),
                QPoint(image_size.width(), 0), arc_path);
}

void RoundImageHelper::drawShadowRect(QPainter* painter,
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

void RoundImageHelper::drawShadowArc(QPainter* painter,
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

void RoundImageHelper::getGradient(const QPoint& start_point,
                                   const int radius,
                                   QRadialGradient& radial_gradient) {
  radial_gradient = QRadialGradient(start_point, radius);
  _getGradient(radial_gradient);
}

void RoundImageHelper::getGradient(const QPoint& start_point,
                                   const QPoint& end_point,
                                   QLinearGradient& linear_gradient) {
  linear_gradient = QLinearGradient(start_point, end_point);
  _getGradient(linear_gradient);
}

void RoundImageHelper::_getGradient(QGradient& gradient) {
  gradient.setColorAt(0, Qt::gray);
  gradient.setColorAt(0.5, QColor(0, 0, 0, 50));
  gradient.setColorAt(0.6, QColor(0, 0, 0, 30));
  gradient.setColorAt(1, QColor(0, 0, 0, 0));
  gradient.setSpread(QGradient::PadSpread);
}
