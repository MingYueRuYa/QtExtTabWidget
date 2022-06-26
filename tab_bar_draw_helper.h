#ifndef TAB_BAR_DRAW_HELPER_H
#define TAB_BAR_DRAW_HELPER_H

#include <qglobal.h>
#include "base_tab_bar_draw_helper.h"

class TabBarDrawHelper: public BaseTabBarDrawHelper
{
public:
    TabBarDrawHelper();
    ~TabBarDrawHelper();

  virtual void paintTabAddBtn(QPainter &painter, const QRect &draw_rect,
                              TabState tab_state) override;
  virtual void paintTab(QPainter &painter, const QRect &draw_rect,
                        TabState tab_state, const QString &text,
                        const QIcon &icon) override;
  virtual void paintTabBar(QPainter &painter, const QRect &draw_rect) override;
  virtual void paintScrollBtn(bool, QPainter &, const QRect &,
                              const QToolButton *) override;
  virtual QRect calcIconBtnRect(bool, const QSize &, const QRect &) override;

private:
  QPixmap tab_bar_bk_image_;
  QPixmap tab_bk_normal_image_;
  QPixmap tab_bk_hover_image_;
  QPixmap tab_bk_selected_image_;
  QList<QPixmap> left_arrow_pixmap_list_;
  QList<QPixmap> right_arrow_pixmap_list_;

   Q_DISABLE_COPY(TabBarDrawHelper);
};


#endif // TAB_BAR_DRAW_HELPER_H