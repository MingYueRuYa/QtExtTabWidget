#ifndef ROUND_SHADOW_PICTURE_WIDGET_H
#define ROUND_SHADOW_PICTURE_WIDGET_H

#include "move_widget_helper.h"

#include <QPixmap>
#include <QWidget>

class RoundShadowPictureWidget : public QWidget {
  Q_OBJECT
 public:
  RoundShadowPictureWidget(const QPixmap& pixmap, QWidget* parent = nullptr);
  ~RoundShadowPictureWidget();

 protected:
  void setupUI();
  void updateUI();

 protected:
  virtual void paintEvent(QPaintEvent* event) override;
  virtual void mousePressEvent(QMouseEvent* event) override;
  virtual void mouseReleaseEvent(QMouseEvent* event) override;
  virtual void mouseMoveEvent(QMouseEvent* event) override;

 private:
  QPixmap pixmap_;
  QPixmap round_shadow_pixmap_;
  MoveWidgetHelper move_widget_helper_;
};

#endif  // ROUND_SHADOW_PICTURE_WIDGET_H
