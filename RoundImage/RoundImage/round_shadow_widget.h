#ifndef ROUND_SHADOW_WIDGET
#define ROUND_SHADOW_WIDGET

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
  virtual void paintEvent(QPaintEvent* ev) override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;

 private:
  bool is_press_ = false;
  QPoint move_point_;
};

#endif  // ROUND_SHADOW_WIDGET
