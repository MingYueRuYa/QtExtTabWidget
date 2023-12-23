#pragma once

#include <QTabBar>

class QButtonGroup;

class QExtTabBar : public QTabBar {
  Q_OBJECT

 public:
  QExtTabBar(QWidget* parent);
  ~QExtTabBar();

  QSize QExtTabBar::tabSizeHint(int index) const;
  bool addTab2(const QString& icon, const QString& title, QWidget* widget);

 private:
  void mouse_release_event(QMouseEvent* event);

 signals:
  void switch_widget(QWidget* widget);
  void tab_click_closed(QWidget* widget);

protected slots:
    void tab_clicked_changed();

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
  QButtonGroup* group_ = nullptr;
  std::map<QWidget*, QWidget*> btn_widget_map_;
};