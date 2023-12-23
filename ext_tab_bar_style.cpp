#include "ext_tab_bar_style.h"
#include "const.h"

#include <QStyleOption>

QRect QExtTabBarStyle::subElementRect(SubElement element,
  const QStyleOption* option,
  const QWidget* widget) const {
  if (SE_TabBarTabLeftButton == element) {
    const QStyleOptionTab* tab_option = qstyleoption_cast<const QStyleOptionTab*>(option);
    QRect rect = tab_option->rect;
    rect.setWidth(kTAB_BUTTON_WIDTH);
    rect.setHeight(kTAB_BUTTON_HEIGHT);
    return rect;
  }
  else
  {
    return QProxyStyle::subElementRect(element, option, widget);
  }

}

int QExtTabBarStyle::pixelMetric(PixelMetric metric, const QStyleOption* option,
                                 const QWidget* widget) const {
  if (PM_TabBarTabHSpace == metric) {
    return 0;
  } else {
    return QProxyStyle::pixelMetric(metric, option, widget);
  }
}

