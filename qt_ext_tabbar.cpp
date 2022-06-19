#include "qt_ext_tabbar.h"

#include <QDateTime>
#include <QDebug>
#include <QLabel>
#include <QPainter>
#include <QStyleOption>

#include "draw_helper.h"
#include "qt_ext_tabbar_style.h"
#include "round_shadow_helper.h"

// #define DEV_DEBUG

QtExtTabBar::QtExtTabBar(QWidget *parent, bool tab_add_button)
    : QTabBar(parent), tab_add_button_(tab_add_button) {
  this->setExpanding(false);
  this->setMovable(false);
  this->setStyle(new QExtTabBarStyle(this));
  if (tab_add_button_) addTab(0);
}

QSize QtExtTabBar::tabSize() const { return tab_size_; }

void QtExtTabBar::setTabSize(const QSize &tab_size) {
  if (tab_size_ == tab_size) return;
  tab_size_ = tab_size;
  update();
}

bool QtExtTabBar::tabAddButton() const { return tab_add_button_; }

void QtExtTabBar::setTabAddButton(bool draw_add) {
  if (tab_add_button_ == draw_add) return;
  if (tab_add_button_ && !draw_add) removeTab(count() - 1);
  tab_add_button_ = draw_add;
  update();
}

int QtExtTabBar::tabAddButtonWidth() const { return tab_add_button_width_; }

void QtExtTabBar::setTabAddButtonWidth(int width) {
  if (tab_add_button_width_ == width) return;
  tab_add_button_width_ = width;
  update();
}

int QtExtTabBar::scrollBtnWidth() const { return scroll_btn_width_; }

void QtExtTabBar::setScrollBtnWidth(int width) { scroll_btn_width_ = width; }

void QtExtTabBar::setPaintAddBtnFunc(PaintAddButtonFunc func) {
  paint_add_btn_func_ = func;
}

void QtExtTabBar::setPaintTabFunc(PaintTabFunc func) { paint_tab_func_ = func; }

void QtExtTabBar::setPaintTabBarFunc(PaintTabBarFunc func) {
  paint_tab_bar_func_ = func;
}

void QtExtTabBar::setPaintScrollBtnFunc(PaintScrollBtnFunc func) {
  paint_scroll_btn_func_ = func;
}

void QtExtTabBar::setCalcIconBtnRectFunc(CalcIconBtnRectFunc func) {
  calc_icon_btn_rect_func_ = func;
}

void QtExtTabBar::copyOtherTabBar(const QtExtTabBar *tab_bar) {
  if (nullptr == tab_bar || this == tab_bar) return;

  this->tab_size_ = tab_bar->tab_size_;
  this->press_pos_ = tab_bar->press_pos_;
  this->tab_btn_add_state_ = tab_bar->tab_btn_add_state_;
  this->dragging_ = tab_bar->dragging_;
  this->tab_add_button_ = tab_bar->tab_add_button_;
  this->tab_add_button_width_ = tab_bar->tab_add_button_width_;
  this->scroll_btn_width_ = tab_bar->scroll_btn_width_;
  this->paint_add_btn_func_ = tab_bar->paint_add_btn_func_;
  this->paint_tab_func_ = tab_bar->paint_tab_func_;
  this->paint_tab_bar_func_ = tab_bar->paint_tab_bar_func_;
  this->paint_scroll_btn_func_ = tab_bar->paint_scroll_btn_func_;
  this->calc_icon_btn_rect_func_ = tab_bar->calc_icon_btn_rect_func_;
}

int QtExtTabBar::tabCount() const {
  return tab_add_button_ ? count() - 1 : count();
}

const TabBarSingal &QtExtTabBar::getSignal() const { return signal_; }

void QtExtTabBar::setTabButton2(int index, QTabBar::ButtonPosition btn_pos,
                                QWidget *btn) {
  btn->setFixedSize(this->iconSize());
  if (QTabBar::RightSide == btn_pos)
    connect(btn, SIGNAL(clicked()), this, SLOT(OnRightWidgetClicked()));
  this->setTabButton(index, btn_pos, btn);
}

QSize QtExtTabBar::tabSizeHint(int index) const {
  if (index == count() - 1 && tab_add_button_)
    return QSize(tab_add_button_width_, tab_size_.height());
  return tab_size_;
}

void QtExtTabBar::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  if (nullptr != paint_tab_bar_func_)
    paint_tab_bar_func_(painter, this->rect());

  if (nullptr != paint_tab_func_) drawTab(&painter);

  if (tab_add_button_ && nullptr != paint_add_btn_func_) drawPlusBtn(&painter);
}

void QtExtTabBar::mouseReleaseEvent(QMouseEvent *e) {
  QTabBar::mouseReleaseEvent(e);
  //  if (dragging_ && e->button() == Qt::LeftButton &&
  //      !contentsRect().contains(e->pos())) {
  if (dragging_ && e->button() == Qt::LeftButton) {
    dragging_ = false;
  }
}

void QtExtTabBar::mousePressEvent(QMouseEvent *event) {
  QTabBar::mousePressEvent(event);

  int index = pointInTabRectIndex(event->pos());
  if (index == count() - 1 && tab_add_button_) {
    emit signal_.addBtnClicked();
    return;
  }

  if (event->button() == Qt::LeftButton && tabCount() >= 1) {
    dragging_ = true;
    press_pos_ = event->pos();
  }
}

void QtExtTabBar::mouseMoveEvent(QMouseEvent *event) {
  QTabBar::mouseMoveEvent(event);

  // 高度超过标签栏高度 或 标签数量=1 时，开启拖拽
  if (dragging_ && (event->buttons() & Qt::LeftButton) &&
      (tabCount() >= 1 || !contentsRect().contains(event->pos()))) {
    int index = this->currentIndex();
    if (index == -1 || (index == tabCount())) return;

    bool containts = false;
    for (int i = 0; i < count(); ++i) {
      if (tabRect(index).contains(event->pos())) {
        containts = true;
        break;
      }
    }

    // 拖拽到外面来了
    // if (count() == 1 || containts) {
    if (tabCount() >= 1 && containts) {
      emit signal_.signalStartDrag(index);
    }
  }
}

bool QtExtTabBar::event(QEvent *ev) {
  if (ev->type() == QEvent::HoverMove || ev->type() == QEvent::HoverEnter) {
    if (tab_add_button_) {
      QHoverEvent *hover_event = static_cast<QHoverEvent *>(ev);
      QRect tab_add_rect = tabRect(count() - 1);
      if (tab_add_rect.contains(hover_event->pos())) {
        tab_btn_add_state_ = TabState::hover_;
        update(tab_add_rect);
      } else {
        tab_btn_add_state_ = TabState::normal_;
        update(tab_add_rect);
      }
    }
  } else if (ev->type() == QEvent::HoverLeave) {
    QHoverEvent *hover_event = static_cast<QHoverEvent *>(ev);
    if (tab_add_button_) {
      QRect tab_add_rect = tabRect(count() - 1);
      tab_btn_add_state_ = TabState::normal_;
      update(tab_add_rect);
    }
  }

  return QTabBar::event(ev);
}

void QtExtTabBar::focusOutEvent(QFocusEvent *e) {
  dragging_ = false;
  return QTabBar::focusOutEvent(e);
}

void QtExtTabBar::setupUI() { this->setMouseTracking(true); }

void QtExtTabBar::updateUI() {}

int QtExtTabBar::pointInTabRectIndex(const QPoint &point) {
  for (int i = 0; i < count(); ++i) {
    QRect rect = tabRect(i);
    if (!rect.contains(point)) continue;
    return i;
  }
  return -1;
}

void QtExtTabBar::drawTab(QPainter *painter) {
  int tab_count = tab_add_button_ ? count() - 1 : count();
  QStyleOptionTabV3 option;
  TabState tab_state = TabState::normal_;
  for (int index = 0; index < tab_count; index++) {
    QRect rect = tabRect(index);
    initStyleOption(&option, index);
    if (QStyle::State_Selected & option.state)
      tab_state = TabState::selected_;
    else if (QStyle::State_MouseOver & option.state)
      tab_state = TabState::hover_;
    else
      tab_state = TabState::normal_;
    paint_tab_func_(*painter, rect, tab_state, option.text,
                    this->tabIcon(index));
  }
}

void QtExtTabBar::drawPlusBtn(QPainter *painter) {
  int last_index = count() - 1;
  QStyleOptionTabV3 option;
  initStyleOption(&option, last_index);
  painter->save();
  paint_add_btn_func_(*painter, tabRect(last_index), tab_btn_add_state_);
  painter->restore();
}

void QtExtTabBar::OnRightWidgetClicked() {
  QObject *obj = (QObject *)(sender());
  for (int i = 0; i < count(); ++i) {
    QWidget *widget = tabButton(i, QTabBar::RightSide);
    if (widget != obj) continue;
    emit signal_.closeTab(i);
    break;
  }
}
