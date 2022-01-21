#ifndef ROUND_IMAGE_HELEPR
#define ROUND_IMAGE_HELEPR

#include <QLinearGradient>
#include <QPixmap>
#include <QRadialGradient>

#define DEFINE_CLASS_MEMBER_DEFAULT(type, mem_name, fun_name, value) \
 protected:                                                          \
  type mem_name##_ = value;                                          \
                                                                     \
 public:                                                             \
  type Get##fun_name() { return mem_name##_; };                      \
  void Set##fun_name(type val) { mem_name##_ = val; };

class RoundImageHelper {
 public:
  explicit RoundImageHelper() = default;
  explicit RoundImageHelper(const int shadow_width, const int radius);
  ~RoundImageHelper() = default;
  QPixmap RoundImage(const QPixmap& source_pixmap);

 private:
  void draw4BorderRectShadow(QPainter* painter,
                             int shadow_width,
                             int shadow_width2X,
                             const QSize& image_size);
  void draw4BorderArcShadow(QPainter* painter,
                            int shadow_width,
                            int shadow_width2X,
                            const QSize& image_size);
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
};

#endif  // ROUND_IMAGE_HELEPR
