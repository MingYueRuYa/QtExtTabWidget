#pragma once

#include <QTabBar>

class QButtonGroup;
class QPushButton;
class CusBtn;

class QExtTabBar : public QTabBar {
  Q_OBJECT
 public:
  enum class DragState { Unknown = 0, Merged, NoChanged };

 private:
  struct TabDragData {
    QTabBar* tab_bar;
    QString label;
    QPushButton* btn;
  };

 public:
  QExtTabBar(QWidget* parent);
  ~QExtTabBar();

  QSize QExtTabBar::tabSizeHint(int index) const;
  bool add_tab2(const QString& icon, const QString& title, QWidget* widget);
  bool remove_tab2(int index, QWidget *widget);

 private:
  void mouse_release_event(QMouseEvent* event);
  void tab_mouse_move_event(QObject* obj, QMouseEvent* event);

 signals:
  void switch_widget(QWidget* widget);
  void tab_click_closed(QWidget* widget);

 protected slots:
  void tab_clicked_changed();
  void tab_closed();

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
  void send_tab_bar_hover(const QPoint& pos);
  bool merge_tab(QDropEvent* event);
  void insert_tab(int index, QWidget* widget, const QString& title);
  QWidget *get_widget(CusBtn *btn);
  int get_btn_index(CusBtn *btn);
  CusBtn* get_btn(QWidget *widget);
  CusBtn* get_btn_by_index(int index);

 private:
  DragState drag_state_ = DragState::Unknown;
  bool dragging_ = false;
  QPoint press_pos_ = QPoint(0, 0);
  QButtonGroup* group_ = nullptr;
  std::vector<std::pair<CusBtn*, QWidget*>> btn_widget_map_;
};