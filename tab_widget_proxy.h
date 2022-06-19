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
using QtExtTabBarDef::TabState;

class TabWidgetProxy : public QWidget,
                   public std::enable_shared_from_this<TabWidgetProxy> {
  Q_OBJECT

 public:
  explicit TabWidgetProxy(QWidget *parent = 0);
  ~TabWidgetProxy();
  bool filterTabBarEvent(QObject *obj, QEvent *event);
  QTabBar *tabBar() const;

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
  void paintTabAddBtn(QPainter &painter, const QRect &rect, TabState tab_state);
  void paintTab(QPainter &painter, const QRect &rect, TabState tab_state,
                const QString &text, const QIcon &icon);
  void paintTabBar(QPainter &painter, const QRect &rect);
  void paintScrollBtn(bool left, QPainter &painter, const QRect &rect,
                      const QToolButton *tool_btn);
  QRect calcIconRect(bool left_button, const QSize &icon_size,
                     const QRect &btn_rect);
  bool isHasParent() const;

 private:
  NcFramelessHelper *frame_less_helper_ = nullptr;
  QtExtTabWidget *ext_tab_widget_;
  QPixmap tab_bar_bk_image_;
  QPixmap tab_bk_normal_image_;
  QPixmap tab_bk_hover_image_;
  QPixmap tab_bk_selected_image_;
  QList<QPixmap> left_arrow_pixmap_list_;
  QList<QPixmap> right_arrow_pixmap_list_;
  TabWidgetSignal signal_;
};

#endif  // TAB_WIDGET_PROXY_H
