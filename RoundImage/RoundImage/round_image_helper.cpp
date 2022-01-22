#include "round_image_helper.h"

#include <QPainter>
#include <QPainterPath>

// 1.阴影的距离可以调整
// 2.阴影可以调整
// 3.圆角可以调整
// 4.圆角4个阴影可以调整
RoundImageHelper::RoundImageHelper(const int shadow_width, const int radius)
    : RoundShadowHelper(shadow_width, radius) {}

QPixmap RoundImageHelper::RoundImage(const QPixmap& source_pixmap) {
  if (source_pixmap.isNull())
    return source_pixmap;

  const int kBORDER_DISTANCE = shadow_width_ / 2;
  const int kSHADOW_WIDTH = shadow_width_;
  const int kSHADOW_WIDTH2X = shadow_width_ * 2;

  //获取图片尺寸
  int image_width = source_pixmap.width();
  int image_height = source_pixmap.height();

  QPixmap new_pixmap = source_pixmap;
  QPixmap dest_image(image_width + kSHADOW_WIDTH, image_height + kSHADOW_WIDTH);
  dest_image.fill(Qt::transparent);
  QPainter painter(&dest_image);
  painter.setRenderHints(QPainter::Antialiasing, true);
  painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

  RoundShadow(&painter, QRect(0, 0, dest_image.width(), dest_image.height()));

  // 将图片裁剪为圆角
  QPainterPath path;
  QRect rect(kBORDER_DISTANCE, kBORDER_DISTANCE, image_width, image_height);
  path.addRoundedRect(rect, radius_, radius_, Qt::AbsoluteSize);
  painter.setClipPath(path);
  painter.drawPixmap(kBORDER_DISTANCE, kBORDER_DISTANCE, image_width,
                     image_height, new_pixmap);
  return dest_image;
}
