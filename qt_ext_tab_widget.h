#ifndef QTEXTTABWIDGET_H
#define QTEXTTABWIDGET_H

#include <QTabWidget>
#include <QWidget>

#include "tab_widget_signal.h"
#include "qt_ext_tabbar.h"

class ButtonWidget;

class QtExtTabWidget : public QTabWidget {
  Q_OBJECT

 public:
  enum class DragState { Unknown = 0, Merged, NoChanged };

 public:
  explicit QtExtTabWidget(QWidget *parent = 0);
  ~QtExtTabWidget() = default;
  void initProperty(bool useScrollButtons = true, bool isMain = true);
  void addTab2(QWidget *widget, const QString &label);
  void insertTab2(int index, QWidget *widget, const QString &label);
  void setTabButton2(int index, QTabBar::ButtonPosition, QWidget *btn);
  void setMaxRestoreBtnVisible(bool max_visible, bool restore_visible);
  int pointInTabRectIndex(const QPoint &point);

  QSize tabSize() const;
  void setTabSize(const QSize &tab_size);

  bool tabAddButton() const;
  void setTabAddButton(bool draw_add);

  int tabAddButtonWidth() const;
  void setTabAddButtonWidth(int width);

  QtExtTabBar *tabBar2() const;
  const TabWidgetSignal &getSignal() const;

  // call back
  void setPaintAddBtnFunc(QtExtTabBar::PaintAddButtonFunc func);
  void setPaintTabFunc(QtExtTabBar::PaintTabFunc func);
  void setPaintTabBarFunc(QtExtTabBar::PaintTabBarFunc func);
  void setPaintScrollBtnFunc(QtExtTabBar::PaintScrollBtnFunc func);
  void setCalcIconBtnRectFunc(QtExtTabBar::CalcIconBtnRectFunc func);

 protected:
  virtual void paintEvent(QPaintEvent *ev) override;
  virtual void tabInserted(int index) override;
  virtual void resizeEvent(QResizeEvent *ev) override;
  virtual void dragEnterEvent(QDragEnterEvent *event) override;
  virtual void dragMoveEvent(QDragMoveEvent *event) override;
  virtual void dropEvent(QDropEvent *event) override;
  virtual void dragLeaveEvent(QDragLeaveEvent *event) override;

 protected:
  void setTabBar2(QTabBar *tab_bar);

 protected Q_SLOTS:
  void OnCloseTab(int index);
  void OnTabBarDoubleClicked(int index);
  void OnStartDrag(int index);
  void OnCloseWnd();
  void OnMinWnd();
  void OnMaxWnd();
  void OnRestoreWnd();
  bool OnMergeDroppedLabel(QDropEvent *event);
  QtExtTabWidget *createDraggedNewWindow();

 private:
  void setupUI();
  bool isInMain();
  void deleteIfEmptyWindow();
  QtExtTabWidget *newTabWidget();

 private:
  static DragState drag_state_;
  static QString dragging_label_;
  static QIcon dragging_icon_;
  bool is_main_ = true;
  QPoint dragging_point_delta_ = QPoint(0, 0);
  int dragging_index_ = -1;
  QtExtTabBar *tab_bar_ = nullptr;
  ButtonWidget *button_widget_ = nullptr;
  QWidget *dragging_widget_ = nullptr;
  TabWidgetSignal signal_;
};

#endif  // QtExtTabWidget_H
