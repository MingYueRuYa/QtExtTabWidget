#include "round_image.h"
#include "round_image_helper.h"
#include "round_shadow_helper.h"
#include "round_shadow_picture_widget.h"
#include "round_shadow_widget.h"
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

  RoundShadowWidget* widget = new RoundShadowWidget(this);
  widget->setFixedSize(QSize(600, 800));
  widget->move(50, 50);
}

QPixmap RoundImage::getRoundRectPixmap(QPixmap srcPixMap,
                                       const QSize& size,
                                       int radius) {
  RoundImageHelper image_helper;
  return image_helper.RoundImage(srcPixMap);
}

void RoundImage::CreatePictureWidget(const QPixmap& pixmap) {
  RoundShadowPictureWidget* widget = new RoundShadowPictureWidget(pixmap, this);
  widget->show();
}

void RoundImage::CreatePictureWidget(const QString& pixmap_path) {
  RoundShadowPictureWidget* widget =
      new RoundShadowPictureWidget(pixmap_path, this);
  widget->show();
}

void RoundImage::paintEvent(QPaintEvent* paintEvent) {
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
    for (auto& item : urls) {
      CreatePictureWidget(item.toLocalFile());
    }
  }

  event->acceptProposedAction();
}
