#include "ext_tab_bar_style.h"

#include <QStyleOption>

QRect QExtTabBarStyle::subElementRect(SubElement element,
  const QStyleOption* option,
  const QWidget* widget) const {
  if (SE_TabBarTabLeftButton == element) {
    const QStyleOptionTab* tab_option = qstyleoption_cast<const QStyleOptionTab*>(option);
    QRect rect = tab_option->rect;
    rect.setWidth(150);
    rect.setHeight(30);
    return rect;
  }
  else
  {
    return QProxyStyle::subElementRect(element, option, widget);
  }

}

