#include "round_image.h"
#include "round_image_helper.h"
#include "round_shadow_helper.h"
#include "ui_round_image.h"

#include <QDragEnterEvent>
#include <QFileInfo>
#include <QMimeData>
#include <QPainter>
#include <QPainterPath>

RoundImage::RoundImage(QWidget* parent)
    : QWidget(parent), ui_(new Ui::RoundImageClass) {
  ui_->setupUi(this);
  setAcceptDrops(true);
}

QPixmap RoundImage::getRoundRectPixmap(QPixmap srcPixMap,
                                       const QSize& size,
                                       int radius) {
  RoundImageHelper image_helper;
  return image_helper.RoundImage(srcPixMap);
}

void RoundImage::paintEvent(QPaintEvent* paintEvent) {
  //  QPainter painter(this);
  //  RoundShadowHelper round_shadow;
  //  round_shadow.RoundShadow(&painter, this->rect());
  //
  ////  return;
  if (pixmap_.isNull())
    return;
  QPainter painter(this);
  painter.drawPixmap(30, 30, pixmap_);
  QWidget::paintEvent(paintEvent);
}

void RoundImage::dragEnterEvent(QDragEnterEvent* event) {
  event->acceptProposedAction();
}

void RoundImage::dropEvent(QDropEvent* event) {
  const QMimeData* mimeData = event->mimeData();
  if (mimeData->hasImage()) {
    pixmap_ = qvariant_cast<QPixmap>(mimeData->imageData());
    update();
  } else if (mimeData->hasUrls()) {
    QList<QUrl> urls = mimeData->urls();
    QString pixmap_path = urls[0].toLocalFile();
    QPixmap temp_pixmap = QPixmap(pixmap_path);
    pixmap_ = getRoundRectPixmap(temp_pixmap, temp_pixmap.size(), 5);
    QFileInfo file_info(pixmap_path);
    QString save_file_name =
        file_info.path() + "/" + file_info.baseName() + "_rounder.png";
    pixmap_.save(save_file_name);
    update();
  }

  event->acceptProposedAction();
}
