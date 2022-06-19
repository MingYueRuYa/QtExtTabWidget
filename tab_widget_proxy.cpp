#include "tab_widget_proxy.h"

#include <functional>

#include "NcFrameLessHelper.h"
#include "base_tab_bar_draw_helper.h"
#include "draw_helper.h"
#include "qt_ext_tab_widget.h"
#include "qt_ext_tabbar.h"
#include "round_shadow_helper.h"
#include "weak_call_back.hpp"

TabWidgetProxy::TabWidgetProxy(QWidget *parent) : QWidget(parent) { setupUI(); }

TabWidgetProxy::~TabWidgetProxy() { delete frame_less_helper_; }

void TabWidgetProxy::setupUI() {
  this->setWindowFlags(Qt::FramelessWindowHint |
                       Qt::X11BypassWindowManagerHint);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setSpacing(0);
  layout->setContentsMargins(0, 0, 0, 0);
  ext_tab_widget_ = new QtExtTabWidget(this);
  layout->addWidget(ext_tab_widget_);
  ext_tab_widget_->initProperty();
  connectSignal();
}

bool TabWidgetProxy::filterTabBarEvent(QObject *obj, QEvent *event) {
  // Only click on the tab bar non-tab area can be dragged
  if (event->type() != QEvent::MouseButtonPress) return false;

  QMouseEvent *mouse_event = dynamic_cast<QMouseEvent *>(event);
  // （只有在拖拽区域不在tab rect区域 || 只有一个tab标签时才可以拖动）
  if (isMaximized()) {
    return true;
  } else if (ext_tab_widget_->tabBar2() == obj &&
             (ext_tab_widget_->pointInTabRectIndex(mouse_event->pos()) < 0 ||
              ext_tab_widget_->tabBar2()->tabCount() == 1)) {
    return false;
  } else {
    return true;
  }
}

void TabWidgetProxy::addTab2(QWidget *widget, const QString &label) {
  ext_tab_widget_->insertTab2(-1, widget, label);
}

QTabBar *TabWidgetProxy::tabBar() const { return ext_tab_widget_->tabBar(); }

const TabWidgetSignal &TabWidgetProxy::getSignal() const { return signal_; }

void TabWidgetProxy::setTabButton2(int index, QTabBar::ButtonPosition btn_pos,
                                   QWidget *btn) {
  ext_tab_widget_->setTabButton2(index, btn_pos, btn);
}

void TabWidgetProxy::setTabIcon(int index, const QIcon &icon) {
  ext_tab_widget_->setTabIcon(index, icon);
}

void TabWidgetProxy::updateDrawHelp(BaseTabBarDrawHelper *helper) {
  ext_tab_widget_->setPaintAddBtnFunc(std::bind(
      &BaseTabBarDrawHelper::paintTabAddBtn, helper, std::placeholders::_1,
      std::placeholders::_2, std::placeholders::_3));
  ext_tab_widget_->setPaintTabFunc(
      std::bind(&BaseTabBarDrawHelper::paintTab, helper, std::placeholders::_1,
                std::placeholders::_2, std::placeholders::_3,
                std::placeholders::_4, std::placeholders::_5));
  ext_tab_widget_->setPaintTabBarFunc(
      std::bind(&BaseTabBarDrawHelper::paintTabBar, helper,
                std::placeholders::_1, std::placeholders::_2));
  ext_tab_widget_->setPaintScrollBtnFunc(std::bind(
      &BaseTabBarDrawHelper::paintScrollBtn, helper, std::placeholders::_1,
      std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
  ext_tab_widget_->setCalcIconBtnRectFunc(std::bind(
      &BaseTabBarDrawHelper::calcIconBtnRect, helper, std::placeholders::_1,
      std::placeholders::_2, std::placeholders::_3));
}

void TabWidgetProxy::OnTabInserted(int index) {
  if (isHasParent()) emit signal_.tabInserted(index);
}

void TabWidgetProxy::OnCloseTab(int index) {
  if (isHasParent()) emit signal_.tabClosed(index);
}

void TabWidgetProxy::OnAddBtnClicked() { emit signal_.addBtnClicked(); }

void TabWidgetProxy::OnCloseWnd() {
  if (isHasParent())
    emit signal_.closeWnd();
  else
    this->close();
}

void TabWidgetProxy::OnMinWnd() {
  if (isHasParent()) {
    emit signal_.minWnd();
    QWidget *parent = window();
    parent->showMinimized();
  }

  this->showMinimized();
}

void TabWidgetProxy::OnMaxWnd() {
  if (isHasParent()) {
    emit signal_.maxWnd();
    QWidget *parent = window();
    parent->showMaximized();
  } else {
    this->showMaximized();
    ext_tab_widget_->currentWidget()->setFocus();
  }
  ext_tab_widget_->setMaxRestoreBtnVisible(false, true);
}

void TabWidgetProxy::OnRestoreWnd() {
  if (isHasParent()) {
    emit signal_.restoreWnd();
    QWidget *parent = window();
    parent->showNormal();
  } else {
    this->showNormal();
  }
  ext_tab_widget_->setMaxRestoreBtnVisible(true, false);
}

void TabWidgetProxy::OnMaxOrRestore() {
  if (isHasParent()) {
    QWidget *parent = window();
    if (parent->isMaximized()) {
      parent->showNormal();
      ext_tab_widget_->setMaxRestoreBtnVisible(true, false);
    } else {
      parent->showMaximized();
      ext_tab_widget_->setMaxRestoreBtnVisible(false, true);
    }
    emit signal_.tabBarDoubleClicked();
  } else {
    if (isMaximized()) {
      this->showNormal();
      ext_tab_widget_->setMaxRestoreBtnVisible(true, false);
    } else {
      this->showMaximized();
      ext_tab_widget_->setMaxRestoreBtnVisible(false, true);
    }
  }
}

bool TabWidgetProxy::isHasParent() const { return nullptr != parentWidget(); }

void TabWidgetProxy::connectSignal() {
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(tabInserted(int)), this,
          SLOT(OnTabInserted(int)));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(addBtnClicked()), this,
          SLOT(OnAddBtnClicked()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(tabClosed(int)), this,
          SLOT(OnCloseTab(int)));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(closeWnd()), this,
          SLOT(OnCloseWnd()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(minWnd()), this,
          SLOT(OnMinWnd()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(maxWnd()), this,
          SLOT(OnMaxWnd()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(restoreWnd()), this,
          SLOT(OnRestoreWnd()));
  connect(&(ext_tab_widget_->getSignal()), SIGNAL(tabBarDoubleClicked()), this,
          SLOT(OnMaxOrRestore()));
}