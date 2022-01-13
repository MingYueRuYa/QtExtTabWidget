#include "ui_round_image.h"
#include "round_image.h"

#include <QPainter>
#include <QFileInfo>
#include <QMimeData>
#include <QPainterPath>
#include <QDragEnterEvent>

RoundImage::RoundImage(QWidget *parent)
    : QWidget(parent), ui_(new Ui::RoundImageClass) {
	ui_->setupUi(this);
	setAcceptDrops(true);
}

QPixmap RoundImage::getRoundRectPixmap(QPixmap srcPixMap, 
										const QSize& size, 
										int radius)
{
	//不处理空数据或者错误数据
	if (srcPixMap.isNull()) {
		return srcPixMap;
	}

	const int kShadowWidth = 10;

	//获取图片尺寸
	int imageWidth = srcPixMap.width();
	int imageHeight = srcPixMap.height();

	//处理大尺寸的图片,保证图片显示区域完整
	QPixmap newPixMap = srcPixMap.scaled(imageWidth, 
                              (imageHeight == 0 ? imageWidth : imageHeight), 
                              Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPixmap destImage(imageWidth+2*kShadowWidth, imageHeight+2*kShadowWidth);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 图片平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

	int shadow_width = 20;
	int shadow_width2X = shadow_width * 2;

	draw4BorderRectShadow(painter, shadow_width, shadow_width2X, destImage.size());
	draw4BorderArcShadow(painter, shadow_width, shadow_width2X, destImage.size());

	// 将图片裁剪为圆角
	QPainterPath path;
	QRect rect(kShadowWidth, kShadowWidth, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius, Qt::AbsoluteSize);
	painter.setClipPath(path);
	painter.drawPixmap(kShadowWidth, kShadowWidth, imageWidth, imageHeight, newPixMap);
	return destImage;
}

void RoundImage::paintEvent(QPaintEvent* paintEvent)
{
  if (pixmap_.isNull())
    return;
	QPainter painter(this);
	painter.drawPixmap(30, 30, pixmap_);
	QWidget::paintEvent(paintEvent);
}

void RoundImage::dragEnterEvent(QDragEnterEvent* event)
{
	event->acceptProposedAction();
}

void RoundImage::dropEvent(QDropEvent* event)
{
	const QMimeData *mimeData = event->mimeData();
	if (mimeData->hasImage()) {
		pixmap_ = qvariant_cast<QPixmap>(mimeData->imageData());
		update();
	} else if (mimeData->hasUrls()) {
		QList<QUrl> urls= mimeData->urls();
		QString pixmap_path = urls[0].toLocalFile();
		QPixmap temp_pixmap = QPixmap(pixmap_path);
		pixmap_ = getRoundRectPixmap(temp_pixmap, temp_pixmap.size(), 5);
		QFileInfo file_info(pixmap_path);
		QString save_file_name = file_info.path() + "/" + file_info.baseName()  + "_rounder.png";
		pixmap_.save(save_file_name);
		update();
	}

	event->acceptProposedAction();
}

void RoundImage::draw4BorderRectShadow(QPainter &painter, int shadow_width, 
	int shadow_width2X, const QSize &image_size) {
	// 上边
	drawShadowRect(painter, 
					QPoint(image_size.width()/2, shadow_width), 
					QPoint(image_size.width()/2, 0),
					QRect(shadow_width, 0, image_size.width()-shadow_width2X, shadow_width));
	// 下边
	drawShadowRect(painter, 
					QPoint(image_size.width()/2, image_size.height()-shadow_width), 
					QPoint(image_size.width()/2, image_size.height()),
					QRect(shadow_width, image_size.height()-shadow_width, image_size.width()-shadow_width2X, image_size.height()));
	// 左边
	drawShadowRect(painter, 
					QPoint(shadow_width, image_size.height()/2), 
					QPoint(0, image_size.height()/2),
					QRect(0, shadow_width, shadow_width, image_size.height()-shadow_width2X));
	// 右边
	drawShadowRect(painter, 
					QPoint(image_size.width()-shadow_width, image_size.height()/2), 
					QPoint(image_size.width(), image_size.height()/2),
					QRect(image_size.width()-shadow_width, shadow_width, image_size.width()-shadow_width, image_size.height()-shadow_width2X));
}

void RoundImage::draw4BorderArcShadow(QPainter& painter, int shadow_width,
	int shadow_width2X, const QSize& image_size) {
	// 上左边
	QPainterPath myPath;
	myPath.moveTo(QPointF(shadow_width, shadow_width));
	myPath.arcTo(QRect(0, 0, shadow_width2X, shadow_width2X), 90, 90);
	drawShadowArc(painter, QPoint(shadow_width, shadow_width), QPoint(0, 0), myPath);

	// 下左边
	myPath.moveTo(QPointF(shadow_width, image_size.height()-shadow_width));
	myPath.arcTo(QRect(0, image_size.height()-shadow_width2X, shadow_width2X, shadow_width2X), 180, 90);
	drawShadowArc(painter, QPoint(shadow_width, image_size.height()-shadow_width), QPoint(0, 0), myPath);

	// 上右边
	myPath.moveTo(QPointF(image_size.width()-shadow_width, shadow_width));
	myPath.arcTo(QRect(image_size.width()-shadow_width2X, 0, shadow_width2X, shadow_width2X), 0, 90);
	drawShadowArc(painter, QPoint(image_size.width()-shadow_width, shadow_width), QPoint(image_size.width(), 0), myPath);

	// 下右边
	myPath.moveTo(QPointF(image_size.width()-shadow_width, image_size.height()-shadow_width));
	myPath.arcTo(QRect(image_size.width()-shadow_width2X, image_size.height()-shadow_width2X, shadow_width2X, shadow_width2X), 0, -90);
	drawShadowArc(painter, QPoint(image_size.width()-shadow_width, image_size.height()-shadow_width), QPoint(image_size.width(), 0), myPath);
}

void RoundImage::drawShadowRect(QPainter &painter, 
							const QPoint& startPoint,
							const QPoint& endPoint,
							const QRect& destRect)
{
	painter.save();
  QLinearGradient linear(startPoint, endPoint);
  getGradient(startPoint, endPoint, linear);
  painter.setPen(Qt::NoPen);
  painter.setBrush(linear);
  painter.drawRect(destRect);
  painter.restore();
}

void RoundImage::drawShadowArc(QPainter& painter,
	const QPoint& startPoint,
	const QPoint& endPoint,
	const QPainterPath& painterPath)
{
	painter.save();
  QRadialGradient radial_gardeint;
  getGradient(startPoint, 20, radial_gardeint);
	painter.setBrush(radial_gardeint);
	painter.setPen(Qt::NoPen);
	painter.drawPath(painterPath);
	painter.restore();
}

void RoundImage::getGradient(const QPoint &start_point, 
                                  const int radius, 
																	QRadialGradient &radial_gradient) {
  radial_gradient = QRadialGradient(start_point, radius);
	_getGradient(radial_gradient);
}

void RoundImage::getGradient(const QPoint& start_point,
	const QPoint& end_point,
	QLinearGradient& linear_gradient) {
  linear_gradient = QLinearGradient(start_point, end_point);
	_getGradient(linear_gradient);
}

void RoundImage::_getGradient(QGradient& gradient) {
  gradient.setColorAt(0, Qt::gray);
  gradient.setColorAt(0.5, QColor(0, 0, 0, 50));
  gradient.setColorAt(0.6, QColor(0, 0, 0, 30));
  gradient.setColorAt(1, QColor(0, 0, 0, 0));
  gradient.setSpread(QGradient::PadSpread);
}
