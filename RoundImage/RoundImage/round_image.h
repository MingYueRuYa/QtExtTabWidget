#ifndef ROUNDIMAGE_ROUNDIMAGE_ROUND_IMAGE_H_
#define ROUNDIMAGE_ROUNDIMAGE_ROUND_IMAGE_H_

#include <QtWidgets/QWidget>

class RoundShadowPictureWidget;

namespace Ui {
class RoundImageClass;
}

class RoundImage : public QWidget {
  Q_OBJECT

 public:
  explicit RoundImage(QWidget* parent = Q_NULLPTR);
  QPixmap getRoundRectPixmap(QPixmap srcPixMap, const QSize& size, int radius);

 protected:
  void CreatePictureWidget(const QPixmap& pixmap);
  void CreatePictureWidget(const QString& pixmap_path);

 protected:
  virtual void paintEvent(QPaintEvent* paintEvent);
  virtual void dragEnterEvent(QDragEnterEvent* event);
  virtual void dropEvent(QDropEvent* event);

 protected slots:
  void DoWidgetExit(RoundShadowPictureWidget *widget);

 private:
  Ui::RoundImageClass* ui_;
  QPixmap pixmap_;
};

#endif  // ROUNDIMAGE_ROUNDIMAGE_ROUND_IMAGE_H_
