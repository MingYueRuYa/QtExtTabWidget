#ifndef MOVE_WIDGET_HELPER_H
#define MOVE_WIDGET_HELPER_H

#include <QMouseEvent>

class MoveWidgetHelper {
 public:
  MoveWidgetHelper() = default;
  ~MoveWidgetHelper() = default;

  void mousePressEvent(QMouseEvent* event, QPoint mouse_point);
  void mouseReleaseEvent(QMouseEvent* event);
  QPoint mouseMoveEvent(QMouseEvent* event);
  bool CanMove() const { return is_press_; }

 private:
  bool is_press_ = false;
  QPoint move_point_;
};

#endif  // MOVE_WIDGET_HELPER_H
