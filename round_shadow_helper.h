#ifndef ROUND_SHADOW_HELPER_H
#define ROUND_SHADOW_HELPER_H

#include <QLinearGradient>
#include <QPainter>
#include <QPixmap>
#include <QRadialGradient>

#define DEFINE_CLASS_MEMBER_DEFAULT(type, mem_name, fun_name, value) \
 protected:                                                          \
  type mem_name##_ = value;                                          \
                                                                     \
 public:                                                             \
  type Get##fun_name() { return mem_name##_; };                      \
  void Set##fun_name(type val) { mem_name##_ = val; };

class RoundShadowHelper {
 public:
  enum RoundDirection {
    D_NONE = 0,
    D_TOP = 0x1 << 1,
    D_BOTTOM = 0x1 << 2,
    D_LEFT = 0x1 << 3,
    D_RIGHT = 0x1 << 4,
    D_ALL = D_TOP | D_BOTTOM | D_LEFT | D_RIGHT
  };

 public:
  explicit RoundShadowHelper() = default;
  explicit RoundShadowHelper(const int shadow_width, const int radius);
  ~RoundShadowHelper() = default;
  void RoundShadow(QPainter* painter,
                   const QRect& rect,
                   RoundDirection dire = D_ALL);
  void FillRoundShadow(QPainter* painter,
                       const QRect& rect,
                       const QColor& fill_color,
                       int radius);

 private:
  void draw4BorderRectShadow(QPainter* painter,
                             int shadow_width,
                             int shadow_width2X,
                             const QRect& dest_rect,
                             RoundDirection dirc);
  void draw4BorderArcShadow(QPainter* painter,
                            int shadow_width,
                            int shadow_width2X,
                            const QRect& dest_rect,
                            RoundDirection dirc);
  void drawShadowRect(QPainter* painter,
                      const QPoint& startPoint,
                      const QPoint& endPoint,
                      const QRect& destRect);
  void drawShadowArc(QPainter* painter,
                     const QPoint& startPoint,
                     const QPoint& endPoint,
                     const QPainterPath& painterPath);

  void getGradient(const QPoint& start_point,
                   const int radius,
                   QRadialGradient& radial_gradient);
  void getGradient(const QPoint& start_point,
                   const QPoint& end_point,
                   QLinearGradient& linear_gradient);

 private:
  void _getGradient(QGradient& gradient);

  DEFINE_CLASS_MEMBER_DEFAULT(int, shadow_width, ShadowWidth, 20)
  DEFINE_CLASS_MEMBER_DEFAULT(int, radius, Radius, 5)

  friend class RoundImageHelper;
  friend class RoundShadowWidget;
};

#endif  // !ROUND_SHADOW_HELPER_H
