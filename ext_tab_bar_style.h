#pragma once

#include <QProxyStyle>

class QTabBar;

class QExtTabBarStyle : public QProxyStyle {
 public:
    QExtTabBarStyle(QTabBar *bar, QStyle *parent = Q_NULLPTR):_bar(bar){}
    ~QExtTabBarStyle(){}


  virtual QRect subElementRect(SubElement element, const QStyleOption *option,
                               const QWidget *widget) const Q_DECL_OVERRIDE;

private:
  QTabBar* _bar;
};

