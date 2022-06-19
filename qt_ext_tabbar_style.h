#ifndef TABBAR_STYLE_H
#define TABBAR_STYLE_H

#include <QProxyStyle>
#include <QStyle>
#include <functional>

class QToolButton;
class QtExtTabBar;

class QExtTabBarStyle : public QProxyStyle {
 public:
  QExtTabBarStyle(QtExtTabBar *tab_bar, QStyle *parent = Q_NULLPTR);
  ~QExtTabBarStyle() = default;

  virtual void drawPrimitive(
      PrimitiveElement pe, const QStyleOption *option, QPainter *painter,
      const QWidget *widget = Q_NULLPTR) const Q_DECL_OVERRIDE;
  virtual int pixelMetric(PixelMetric metric, const QStyleOption *option,
                          const QWidget *widget) const Q_DECL_OVERRIDE;

  virtual QRect subElementRect(SubElement element, const QStyleOption *option,
                               const QWidget *widget) const Q_DECL_OVERRIDE;

 private:
  QRect calcIconRect(bool left, const QStyleOption *option) const;
  void drawArrow(PrimitiveElement pe, const QStyleOption *option,
                 QPainter *painter, const QWidget *widget) const;

 private:
  QtExtTabBar *tab_bar_ = nullptr;
};

#endif  // TABBAR_STYLE_H