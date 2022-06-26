#ifndef base_tab_bar_draw_helper_h
#define base_tab_bar_draw_helper_h

#include <qglobal.h>
#include <QToolButton>
#include <QPainter>

#include "qt_ext_tab_bar_define.h"

using QtExtTabBarDef::TabState;

class BaseTabBarDrawHelper {
 public:
  BaseTabBarDrawHelper() = default;
  ~BaseTabBarDrawHelper() = default;

  virtual void paintTabAddBtn(QPainter &painter, const QRect &draw_rect,
                              TabState tab_state) = 0;
  virtual void paintTab(QPainter &painter, const QRect &draw_rect,
                        TabState tab_state, const QString &text,
                        const QIcon &icon) = 0;
  virtual void paintTabBar(QPainter &painter, const QRect &draw_rect) = 0;
  virtual void paintScrollBtn(bool, QPainter &, const QRect &,
                              const QToolButton *) = 0;
  virtual QRect calcIconBtnRect(bool, const QSize &, const QRect &) = 0;

};

#endif  // base_tab_bar_draw_helper_h