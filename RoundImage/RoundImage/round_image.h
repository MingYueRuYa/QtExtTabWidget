#ifndef ROUNDIMAGE_ROUNDIMAGE_ROUND_IMAGE_H_
#define ROUNDIMAGE_ROUNDIMAGE_ROUND_IMAGE_H_

#include <QtWidgets/QWidget>

namespace Ui {
class RoundImageClass;
}

class RoundImage : public QWidget {
  Q_OBJECT

 public:
  explicit RoundImage(QWidget* parent = Q_NULLPTR);
  QPixmap getRoundRectPixmap(QPixmap srcPixMap, const QSize& size, int radius);

 protected:
  virtual void paintEvent(QPaintEvent* paintEvent);
  virtual void dragEnterEvent(QDragEnterEvent* event);
  virtual void dropEvent(QDropEvent* event);

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
  void _getGradient(QGradient& gradient);

 private:
  Ui::RoundImageClass* ui_;
  QPixmap pixmap_;
};

#endif  // ROUNDIMAGE_ROUNDIMAGE_ROUND_IMAGE_H_
