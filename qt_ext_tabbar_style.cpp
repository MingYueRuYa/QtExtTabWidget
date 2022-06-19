#include "qt_ext_tabbar_style.h"

#include <QToolButton>

#include "qt_ext_tabbar.h"

QExtTabBarStyle::QExtTabBarStyle(QtExtTabBar *tab_bar, QStyle *parent)
    : QProxyStyle(parent) {
  tab_bar_ = tab_bar;
}

void QExtTabBarStyle::drawPrimitive(PrimitiveElement pe,
                                    const QStyleOption *option,
                                    QPainter *painter,
                                    const QWidget *widget) const {
  if (pe == QStyle::PE_IndicatorArrowLeft ||
      pe == QStyle::PE_IndicatorArrowRight) {
    drawArrow(pe, option, painter, widget);
  } else if (pe == QStyle::PE_IndicatorTabClose) {
    // tab_bar_->paint_tab_close_btn_func();
  } else {
    QProxyStyle::drawPrimitive(pe, option, painter, widget);
  }
}

int QExtTabBarStyle::pixelMetric(PixelMetric metric, const QStyleOption *option,
                                 const QWidget *widget) const {
  if (PM_TabBarScrollButtonWidth == metric) {
    return tab_bar_->scrollBtnWidth();
  } else {
    return QProxyStyle::pixelMetric(metric, option, widget);
  }
}

QRect QExtTabBarStyle::subElementRect(SubElement element,
                                      const QStyleOption *option,
                                      const QWidget *widget) const {
  if (SE_TabBarTabLeftButton == element || SE_TabBarTabRightButton == element) {
    const QStyleOptionTab *tab_option =
        qstyleoption_cast<const QStyleOptionTab *>(option);
    if (nullptr != tab_bar_->calc_icon_btn_rect_func_) {
      return tab_bar_->calc_icon_btn_rect_func_(
          SE_TabBarTabLeftButton == element, tab_option->iconSize,
          tab_option->rect);
    } else {
      return calcIconRect(SE_TabBarTabLeftButton == element, tab_option);
    }
  } else {
    return QProxyStyle::subElementRect(element, option, widget);
  }
}

QRect QExtTabBarStyle::calcIconRect(bool left,
                                    const QStyleOption *option) const {
  const QStyleOptionTab *tab_option =
      qstyleoption_cast<const QStyleOptionTab *>(option);
  QSize icon_size = tab_option->iconSize;
  const QRect tab_rect = tab_option->rect;
  QPoint center_pos;
  QRect button_rect;
  const int icon_padding = 8;
  if (left) {
    center_pos =
        QPoint(icon_padding + icon_size.width() / 2 + tab_rect.x(),
               tab_rect.y() + (tab_rect.height() - icon_size.height()) / 2 +
                   icon_size.height() / 2);
  } else {
    center_pos = QPoint(
        tab_rect.x() + tab_rect.width() - icon_padding - icon_size.width() / 2,
        tab_rect.y() + (tab_rect.height() - icon_size.height()) / 2 +
            icon_size.height() / 2);
  }
  button_rect = QRect(QPoint(0, 0), icon_size);
  button_rect.moveCenter(center_pos);
  return button_rect;
}

void QExtTabBarStyle::drawArrow(PrimitiveElement pe, const QStyleOption *option,
                                QPainter *painter,
                                const QWidget *widget) const {
  const QToolButton *tool_btn = static_cast<const QToolButton *>(widget);
  bool left = (pe == QStyle::PE_IndicatorArrowLeft);
  if (nullptr != tool_btn && !tool_btn->isVisible()) return;
  if (nullptr != tab_bar_->paint_scroll_btn_func_) {
    tab_bar_->paint_scroll_btn_func_(left, *painter, option->rect, tool_btn);
  } else {
    QProxyStyle::drawPrimitive(pe, option, painter, widget);
  }
}