#ifndef ROUND_SHADOW_PICTURE_WIDGET_H
#define ROUND_SHADOW_PICTURE_WIDGET_H

#include "move_widget_helper.h"

#include <QMenu>
#include <QPixmap>
#include <QWidget>

class RoundShadowPictureWidget : public QWidget {
  Q_OBJECT
 public:
  RoundShadowPictureWidget(const QString& pixmap_path_,
                           QWidget* parent = nullptr);
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
  virtual void contextMenuEvent(QContextMenuEvent* event) override;

 signals:
  void Exit(RoundShadowPictureWidget *);

 protected slots:
  void on_action_save_triggered(bool);
  void on_action_exit_triggered(bool);

 private:
  QPixmap pixmap_;
  QPixmap round_shadow_pixmap_;
  MoveWidgetHelper move_widget_helper_;
  QMenu menu_;
  QString pixmap_path_;
};

#endif  // ROUND_SHADOW_PICTURE_WIDGET_H
