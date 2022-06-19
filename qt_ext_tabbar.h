#ifndef QTEXTTABBAR_H
#define QTEXTTABBAR_H

#include <QtWidgets>
#include <functional>

#include "common_ui_define.h"
#include "qt_ext_tab_bar_define.h"
#include "tab_bar_signal.h"
#include "weak_call_back.hpp"

class QtExtTabBar;
class RoundShadowHelper;

using QtExtTabBarDef::TabState;

class QtExtTabBar : public QTabBar {
  Q_OBJECT

 public:
  using PaintAddButtonFunc = std::function<void(
      QPainter &painter, const QRect &draw_rect, TabState tab_state)>;
  using PaintTabBarFunc =
      std::function<void(QPainter &painter, const QRect &draw_rect)>;
  using PaintTabFunc = std::function<void(
      QPainter &painter, const QRect &draw_rect, TabState tab_state,
      const QString &draw_text, const QIcon &icon)>;
  using PaintScrollBtnFunc =
      std::function<void(bool, QPainter &, const QRect &, const QToolButton *)>;
  using CalcIconBtnRectFunc =
      std::function<QRect(bool, const QSize &, const QRect &)>;

 public:
  explicit QtExtTabBar(QWidget *parent = 0, bool tab_add_button = true);

  QSize tabSize() const;
  void setTabSize(const QSize &tab_size);

  bool tabAddButton() const;
  void setTabAddButton(bool draw_add);

  int tabAddButtonWidth() const;
  void setTabAddButtonWidth(int width);

  int scrollBtnWidth() const;
  void setScrollBtnWidth(int width);

  void setTabButton2(int index, QTabBar::ButtonPosition btn_pos, QWidget *btn);
  int pointInTabRectIndex(const QPoint &point);
  void setPaintAddBtnFunc(PaintAddButtonFunc func);
  void setPaintTabFunc(PaintTabFunc func);
  void setPaintTabBarFunc(PaintTabBarFunc func);
  void setPaintScrollBtnFunc(PaintScrollBtnFunc func);
  void setCalcIconBtnRectFunc(CalcIconBtnRectFunc func);
  void copyOtherTabBar(const QtExtTabBar *bar);
  int tabCount() const;
  const TabBarSingal &getSignal() const;

 protected:
  QSize tabSizeHint(int index) const;
  virtual void paintEvent(QPaintEvent *event) override;
  virtual void mouseReleaseEvent(QMouseEvent *event) override;
  virtual void mousePressEvent(QMouseEvent *event) override;
  virtual void mouseMoveEvent(QMouseEvent *event) override;
  virtual bool event(QEvent *ev) override;
  virtual void focusOutEvent(QFocusEvent *e);

 private:
  void setupUI();
  void updateUI();
  void drawTab(QPainter *painter);
  void drawPlusBtn(QPainter *painter);

 Q_SIGNALS:
  void AddBtnClicked();
  void CloseTab(int index);
  void signalStartDrag(int index);
  void signalEndDrag();

 protected slots:
  void OnRightWidgetClicked();

 private:
  QSize tab_size_ = QSize(230, 40);
  QPoint press_pos_ = QPoint(0, 0);
  TabState tab_btn_add_state_ = TabState::normal_;
  bool dragging_ = false;
  bool tab_add_button_ = true;
  int tab_add_button_width_ = 30;
  int scroll_btn_width_ = 30;
  PaintAddButtonFunc paint_add_btn_func_ = nullptr;
  PaintTabFunc paint_tab_func_ = nullptr;
  PaintTabBarFunc paint_tab_bar_func_ = nullptr;
  PaintScrollBtnFunc paint_scroll_btn_func_ = nullptr;
  CalcIconBtnRectFunc calc_icon_btn_rect_func_ = nullptr;
  TabBarSingal signal_;

  friend class QExtTabBarStyle;
};

#endif  // QTEXTTABBAR_H
