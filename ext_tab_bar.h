#pragma once

#include <QTabBar>

class QExtTabBar : public QTabBar
{
public:
  QExtTabBar(QWidget* parent);
  ~QExtTabBar();

  QSize QExtTabBar::tabSizeHint(int index) const;

private:
  void mouse_release_event(QMouseEvent* event);

protected:
  bool QExtTabBar::eventFilter(QObject* obj, QEvent* event) override;

  virtual void dragEnterEvent(QDragEnterEvent* event);
  virtual void dragLeaveEvent(QDragLeaveEvent* event);
  virtual void dragMoveEvent(QDragMoveEvent* event);
  virtual void dropEvent(QDropEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* e);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);


private:
  int pointInTabRectIndex(const QPoint& point);
  void start_drag(int index);

private:
  bool dragging_ = false;
  QPoint press_pos_ = QPoint(0, 0);
};