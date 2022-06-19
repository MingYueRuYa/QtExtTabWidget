#ifndef UI_DRAWHELPER_DRAWHELPER_H
#define UI_DRAWHELPER_DRAWHELPER_H

#include <QColor>
#include <QObject>
#include <QSize>
#include "common_ui_define.h"

class QPainter;

class DrawCharacter {
 public:
  DrawCharacter() = default;
  ~DrawCharacter() = default;

  static void DrawX(QPainter* painter,
                    const QRect& target_rect,
                    const QColor& text_color = X_CHARACTER_COLOR,
                    const QSize& text_size = QSize(8, 8),
                    const int pen_width = 2);

  static void DrawPlus(QPainter* painter,
                       const QRect& target_rect,
                       const QColor& text_color = X_CHARACTER_COLOR,
                       const QSize& text_size = QSize(8, 8),
                       const int pen_width = 2);
};

class DrawCircle {
 public:
  DrawCircle() = default;
  ~DrawCircle() = default;

  static void Draw(QPainter* painter,
                   const QRect& target_rect,
                   const QColor& bg_color = CIRCLE_BG_COLOR);
};

class DrawRoundRect {
 public:
  DrawRoundRect() = default;
  ~DrawRoundRect() = default;

  static void Draw(QPainter* painter,
                   const QRect& target_rect,
                   const QColor& bg_color = CIRCLE_BG_COLOR,
                   const int round_size = 5);
};

// 未完成
class DrawCircleArror {
 public:
  DrawCircleArror() = default;
  ~DrawCircleArror() = default;

  static void Draw(QPainter* painter,
                   const QRect& target_rect,
                   const QColor& bg_color = CIRCLE_BG_COLOR);
};

#endif  // UI_DRAWHELPER_DRAWHELPER_H
