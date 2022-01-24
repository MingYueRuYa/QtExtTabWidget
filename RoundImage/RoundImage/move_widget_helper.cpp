#include "move_widget_helper.h"

void MoveWidgetHelper::mousePressEvent(QMouseEvent* event, QPoint mouse_point) {
  switch (event->button()) {
    case Qt::LeftButton:
      is_press_ = true;
      move_point_ = event->globalPos() - mouse_point;
      break;
    default:
      break;
  }
}

void MoveWidgetHelper::mouseReleaseEvent(QMouseEvent* event) {
  switch (event->button()) {
    case Qt::LeftButton:
      is_press_ = false;
      break;
    default:
      break;
  }
}

QPoint MoveWidgetHelper::mouseMoveEvent(QMouseEvent* event) {
  if (!is_press_)
    return QPoint(0, 0);

  return (event->globalPos() - move_point_);
}
