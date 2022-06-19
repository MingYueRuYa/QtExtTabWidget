#include "draw_helper.h"

#include <QPainter>

void DrawCharacter::DrawX(QPainter* painter,
                          const QRect& target_rect,
                          const QColor& text_color,
                          const QSize& text_size,
                          const int pen_width) {
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing, true);
  QRect x_rect = {0, 0, text_size.width(), text_size.height()};
  x_rect.moveCenter(target_rect.center());
  QPen pen;
  pen.setBrush(QBrush(text_color, Qt::SolidPattern));
  pen.setStyle(Qt::SolidLine);
  pen.setWidth(pen_width);
  painter->setPen(pen);
  painter->drawLine(x_rect.topLeft(), x_rect.bottomRight());
  painter->drawLine(x_rect.bottomLeft(), x_rect.topRight());
  painter->restore();
}

void DrawCharacter::DrawPlus(QPainter* painter,
                             const QRect& target_rect,
                             const QColor& text_color,
                             const QSize& text_size,
                             const int pen_width) {
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing, true);
  QRect rect = {0, 0, text_size.width(), text_size.height()};
  rect.moveCenter(target_rect.center());
  QPen pen;
  pen.setBrush(QBrush(text_color, Qt::SolidPattern));
  pen.setStyle(Qt::SolidLine);
  pen.setWidth(pen_width);
  painter->setPen(pen);
  // 画 +
  painter->drawLine(
      QPoint(rect.left() + rect.width() / 2, rect.top()),
      QPoint(rect.left() + rect.width() / 2, rect.top() + rect.height()));
  // 画 -
  painter->drawLine(
      QPoint(rect.left(), rect.top() + rect.height() / 2),
      QPoint(rect.left() + rect.width(), rect.top() + rect.height() / 2));
  painter->restore();
}

void DrawCircle::Draw(QPainter* painter,
                      const QRect& target_rect,
                      const QColor& bg_color) {
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing, true);
  painter->setPen(Qt::NoPen);
  painter->setBrush(QBrush(bg_color));
  painter->drawEllipse(target_rect);
  painter->restore();
}

void DrawRoundRect::Draw(QPainter* painter,
                         const QRect& target_rect,
                         const QColor& bg_color,
                         const int round_size) {
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing, true);
  painter->setPen(Qt::NoPen);
  painter->setBrush(QBrush(bg_color));
  painter->drawRoundedRect(target_rect, round_size, round_size);
  painter->restore();
}

void DrawCircleArror::Draw(QPainter* painter,
                           const QRect& target_rect,
                           const QColor& bg_color) {
  painter->save();
  painter->setRenderHints(QPainter::Antialiasing, true);
  QRect rect = {0, 0, 15, 15};
  rect.moveCenter(target_rect.center());
  QPen pen;
  pen.setWidth(2);
  pen.setBrush(QBrush(QColor(95, 99, 104)));
  painter->setPen(pen);
  painter->drawArc(rect, 30 * 16, 310 * 16);
  painter->setPen(Qt::NoPen);

  painter->setBrush(QColor(Qt::green));
  //  static const QPointF points[3] = {
  //      QPointF(0.0, 0.0),
  //      QPointF(0.0, 20.0),
  //      QPointF(20.0, 00.0),
  //  };
  //  painter->drawPolygon(points, 3);
  QRect rect2 = {0, 0, 5, 5};
  painter->drawRect(rect2);
  painter->restore();
}
