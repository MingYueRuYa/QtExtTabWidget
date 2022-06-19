#ifndef TAB_WIDGET_PROXY_H
#define TAB_WIDGET_PROXY_H

#include <QWidget>
#include <QPixmap>
#include <QToolButton>
#include <memory>
#include <QTabBar>

#include "qt_ext_tab_bar_define.h"
#include "tab_widget_signal.h"

class QtExtTabWidget;
class NcFramelessHelper;
class BaseTabBarDrawHelper;
using QtExtTabBarDef::TabState;

class TabWidgetProxy : public QWidget,
                   public std::enable_shared_from_this<TabWidgetProxy> {
  Q_OBJECT

 public:
  explicit TabWidgetProxy(QWidget *parent = 0);
  ~TabWidgetProxy();
  bool filterTabBarEvent(QObject *obj, QEvent *event);
  void addTab2(QWidget *widget, const QString &label);
  QTabBar *tabBar() const;
  const TabWidgetSignal &getSignal() const;
  void setTabButton2(int index, QTabBar::ButtonPosition, QWidget *btn);
  void setTabIcon(int index, const QIcon &icon);
  void updateDrawHelp(BaseTabBarDrawHelper *helper);

 private:
  void setupUI();

 protected slots:
  void OnTabInserted(int index);
  void OnCloseTab(int index);
  void OnAddBtnClicked();
  void OnCloseWnd();
  void OnMinWnd();
  void OnMaxWnd();
  void OnRestoreWnd();
  void OnMaxOrRestore();

 private:
  bool isHasParent() const;
  void connectSignal();

 private:
  NcFramelessHelper *frame_less_helper_ = nullptr;
  QtExtTabWidget *ext_tab_widget_;
  TabWidgetSignal signal_;
};

#endif  // TAB_WIDGET_PROXY_H
