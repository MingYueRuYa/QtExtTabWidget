#include "round_shadow_picture_widget.h"
#include "round_image_helper.h"

#include <QAction>
#include <QFileDialog>
#include <QFileInfo>
#include <QPainter>

RoundShadowPictureWidget::RoundShadowPictureWidget(const QString& pixmap_path,
                                                   QWidget* parent)
    : pixmap_path_(pixmap_path), QWidget(parent) {
  setupUI();
  updateUI();
}

RoundShadowPictureWidget::RoundShadowPictureWidget(const QPixmap& pixmap,
                                                   QWidget* parent)
    : pixmap_(pixmap), QWidget(parent) {
  setupUI();
  updateUI();
}

RoundShadowPictureWidget::~RoundShadowPictureWidget() {}

void RoundShadowPictureWidget::setupUI() {
  if (pixmap_.isNull()) {
    pixmap_ = QPixmap(pixmap_path_);
  }
  if (!pixmap_.isNull()) {
    RoundImageHelper helper;
    round_shadow_pixmap_ = helper.RoundImage(pixmap_);
  }
  QAction* action = new QAction(tr("save"), this);
  action->setObjectName("action_save");
  connect(action, SIGNAL(triggered(bool)), this,
          SLOT(on_action_save_triggered(bool)));
  menu_.addAction(action);
}

void RoundShadowPictureWidget::updateUI() {
  if (!pixmap_.isNull()) {
    this->setFixedSize(pixmap_.size());
  }
}

void RoundShadowPictureWidget::paintEvent(QPaintEvent* event) {
  if (!round_shadow_pixmap_.isNull()) {
    QPainter painter(this);
    painter.drawPixmap(this->rect(), round_shadow_pixmap_);
  }
}

void RoundShadowPictureWidget::mousePressEvent(QMouseEvent* event) {
  move_widget_helper_.mousePressEvent(event, pos());
}

void RoundShadowPictureWidget::mouseReleaseEvent(QMouseEvent* event) {
  move_widget_helper_.mouseReleaseEvent(event);
}

void RoundShadowPictureWidget::mouseMoveEvent(QMouseEvent* event) {
  if (move_widget_helper_.CanMove()) {
    move(move_widget_helper_.mouseMoveEvent(event));
  }
}

void RoundShadowPictureWidget::contextMenuEvent(QContextMenuEvent* event) {
  menu_.exec(event->globalPos());
}

void RoundShadowPictureWidget::on_action_save_triggered(bool) {
  if (!pixmap_path_.isNull()) {
    QFileInfo file_info(pixmap_path_);
    QString file_path = file_info.absolutePath() + "/";
    file_path += file_info.baseName() + "_rounder." + file_info.suffix();
    round_shadow_pixmap_.save(file_path);
  } else {
    QString file_name = QFileDialog::getSaveFileName(
        this, tr("Save File"), "", tr("Images (*.png *.jpg)"));
    if (!file_name.isEmpty()) {
      round_shadow_pixmap_.save(file_name);
    }
  }
}
