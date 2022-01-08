#include "round_image.h"

#include <QPainter>
#include <QFileInfo>
#include <QMimeData>
#include <QPainterPath>
#include <QDragEnterEvent>

QPixmap RoundImage::getRoundRectPixmap(QPixmap srcPixMap, const QSize& size, int radius)
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
	QPixmap newPixMap = srcPixMap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPixmap destImage(imageWidth+2*kShadowWidth, imageHeight+2*kShadowWidth);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// 抗锯齿
	painter.setRenderHints(QPainter::Antialiasing, true);
	// 图片平滑处理
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

	int shadow_width = 20;
	// 上边
	drawShadowRect(painter, 
					QPoint(destImage.width()/2, shadow_width), 
					QPoint(destImage.width()/2, 0),
					QRect(20, 0, destImage.width()-shadow_width*2, shadow_width));
	// 下边
	drawShadowRect(painter, 
					QPoint(destImage.width()/2, destImage.height()-shadow_width), 
					QPoint(destImage.width()/2, destImage.height()),
					QRect(20, destImage.height()-shadow_width, destImage.width()-2*shadow_width, destImage.height()));
	// 左边
	drawShadowRect(painter, 
					QPoint(20, destImage.height()/2), 
					QPoint(0, destImage.height()/2),
					QRect(0, 20, 20, destImage.height()-2*shadow_width));
	// 右边
	drawShadowRect(painter, 
					QPoint(destImage.width()-shadow_width, destImage.height()/2), 
					QPoint(destImage.width(), destImage.height()/2),
					QRect(destImage.width()-shadow_width, 20, destImage.width()-20, destImage.height()-2*shadow_width));
	

	// 上左边
	QPainterPath myPath;
	myPath.moveTo(QPointF(20, 20));
	myPath.arcTo(QRect(0, 0, 40, 40), 90, 90);
	drawShadowArc(painter, QPoint(20, 20), QPoint(0, 0), myPath);

	// 下左边
	myPath.moveTo(QPointF(20, destImage.height()-20));
	myPath.arcTo(QRect(0, destImage.height()-40, 40, 40), 180, 90);
	drawShadowArc(painter, QPoint(20, destImage.height()-20), QPoint(0, 0), myPath);

	// 上右边
	myPath.moveTo(QPointF(destImage.width()-20, 20));
	myPath.arcTo(QRect(destImage.width()-40, 0, 40, 40), 0, 90);
	drawShadowArc(painter, QPoint(destImage.width()-20, 20), QPoint(destImage.width(), 0), myPath);

	// 下右边
	myPath.moveTo(QPointF(destImage.width()-20, destImage.height()-20));
	myPath.arcTo(QRect(destImage.width()-40, destImage.height()-40, 40, 40), 0, -90);
	drawShadowArc(painter, QPoint(destImage.width()-20, destImage.height()-20), QPoint(destImage.width(), 0), myPath);

	//// 将图片裁剪为圆角
	QPainterPath path;
	QRect rect(kShadowWidth, kShadowWidth, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius, Qt::AbsoluteSize);
	painter.setClipPath(path);
	painter.drawPixmap(kShadowWidth, kShadowWidth, imageWidth, imageHeight, newPixMap);
	return destImage;
}

RoundImage::RoundImage(QWidget *parent)
    : QWidget(parent)
{
	ui_.setupUi(this);

	setAcceptDrops(true);

	// pixmap_ = getRoundRectPixmap(QPixmap(":/RoundImage/res/images/temp.png"), QSize(300, 500), 2);
	// QPixmap dest_pixmap = QPixmap("D:\\browser_close_check_error\\0.png");
	// pixmap_ = getRoundRectPixmap(dest_pixmap, dest_pixmap.size(), 8);
	// pixmap_.save("D:\\0_rounder.png");
}


void 
RoundImage::paintEvent(QPaintEvent* paintEvent)
{
	QPainter painter(this);
	painter.drawPixmap(30, 30, pixmap_);
	QWidget::paintEvent(paintEvent);
}

void 
RoundImage::dragEnterEvent(QDragEnterEvent* event)
{
	event->acceptProposedAction();
}

void 
RoundImage::dropEvent(QDropEvent* event)
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

void 
RoundImage::drawShadowRect(QPainter &painter, 
							const QPoint& startPoint,
							const QPoint& endPoint,
							const QRect& destRect)
{
	painter.save();
    QLinearGradient linear(startPoint, endPoint);
    linear.setColorAt(0, Qt::gray);
    linear.setColorAt(0.5, QColor(0, 0, 0, 50));
    linear.setColorAt(0.6, QColor(0, 0, 0, 30));
    // linear.setColorAt(0.7, QColor(0, 0, 0, 10));
    linear.setColorAt(1, QColor(0, 0, 0, 0));
    // 设置显示模式
    linear.setSpread(QGradient::PadSpread);
    // 设置画笔颜色、宽度
    painter.setPen(Qt::NoPen);
    // 设置画刷填充
    painter.setBrush(linear);
    // 绘制椭圆
    painter.drawRect(destRect);
	painter.restore();
}

void 
RoundImage::drawShadowArc(QPainter& painter,
	const QPoint& startPoint,
	const QPoint& endPoint,
	const QPainterPath& painterPath)
{
	painter.save();
	QRadialGradient linear(startPoint, 20);
    linear.setColorAt(0, Qt::gray);
    linear.setColorAt(0.5, QColor(0, 0, 0, 50));
    linear.setColorAt(0.6, QColor(0, 0, 0, 30));
    linear.setColorAt(1, QColor(0, 0, 0, 0));
    // 设置显示模式
    linear.setSpread(QGradient::PadSpread);
	painter.setBrush(linear);
	painter.setPen(Qt::NoPen);
	painter.drawPath(painterPath);
	painter.restore();
}
