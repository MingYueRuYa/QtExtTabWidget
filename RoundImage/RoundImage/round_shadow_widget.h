#ifndef ROUND_SHADOW_WIDGET
#define ROUND_SHADOW_WIDGET

#include "move_widget_helper.h"
#include "round_shadow_helper.h"

#include <QWidget>

class QPaintEvent;

class RoundShadowWidget : public QWidget, public RoundShadowHelper {
  Q_OBJECT

 public:
  explicit RoundShadowWidget(QWidget* parent = nullptr);
  explicit RoundShadowWidget(const int shadow_width,
                             const int radius,
                             QWidget* parent = nullptr);
  ~RoundShadowWidget() = default;

 protected:
  virtual void paintEvent(QPaintEvent* ev);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent* event);
  virtual void mouseMoveEvent(QMouseEvent* event);

 private:
  bool is_press_ = false;
  QPoint move_point_;
  MoveWidgetHelper move_widget_helper_;
};

#endif  // ROUND_SHADOW_WIDGET
