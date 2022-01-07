#include "round_image.h"

#include <QPainter>
#include <QPainterPath>

QPixmap RoundImage::getRoundRectPixmap(QPixmap srcPixMap, const QSize& size, int radius)
{
	//不处理空数据或者错误数据
	if (srcPixMap.isNull()) {
		return srcPixMap;
	}

	const int kShadowWidth = 10;

	//获取图片尺寸
//	int imageWidth = size.width();
//	int imageHeight = size.height();
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

	// draw4BorderShadow();
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
	// painter.drawRect(QRect(destImage.width()-40, 0, 40, 40));
	drawShadowArc(painter, QPoint(destImage.width()-20, 20), QPoint(destImage.width(), 0), myPath);

	// 下右边
	myPath.moveTo(QPointF(destImage.width()-20, destImage.height()-20));
	myPath.arcTo(QRect(destImage.width()-40, destImage.height()-40, 40, 40), 0, -90);
	// painter.drawRect(QRect(destImage.width()-40, destImage.height()-40, 40, 40));
	drawShadowArc(painter, QPoint(destImage.width()-20, destImage.height()-20), QPoint(destImage.width(), 0), myPath);

//   QColor color(0, 255, 0, 255);
//       painter.setPen(color);
//	   painter.drawLine(QPoint(0, 10), QPoint(destImage.width(),10));
//	   painter.drawLine(QPoint(0, 9), QPoint(destImage.width(),9));
//	   painter.drawLine(QPoint(0, 8), QPoint(destImage.width(),8));

//   QColor color(0, 0, 0, 150);
//	 for (int i = 10; i > 0; i--)
//	 {
//		QPoint p1(0, i);
//		QPoint p2(destImage.width(), i);
//		painter.setPen(color);
//		color.setAlpha(150 - qSqrt(i)*50);
//		painter.drawLine(p1, p2);
//	 }

//   QColor color(0, 0, 0, 80);
//   int shadow_width = 20;
//    for(int i = 0; i < shadow_width; i++)
//    {
//        QPainterPath path;
//        path.setFillRule(Qt::WindingFill);
//        path.addRect(shadow_width-i, shadow_width-i, destImage.width()-(shadow_width-i)*2, destImage.height()-(shadow_width-i)*2);
//        painter.setPen(color);
//        painter.drawPath(path);
//        color.setAlpha(255 - qSqrt(i)*55);
//    }

	


	//// 将图片裁剪为圆角
	QPainterPath path;
	QRect rect(kShadowWidth, kShadowWidth, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius, Qt::RelativeSize);
	painter.setClipPath(path);
	painter.drawPixmap(kShadowWidth, kShadowWidth, imageWidth, imageHeight, newPixMap);

   //QColor color(0, 0, 0, 50);
   // for(int i = 0; i < 10; i++)
   // {
   //     QPainterPath path;
   //     path.setFillRule(Qt::WindingFill);
   //     path.addRect(10-i, 10-i, destImage.width()-(10-i)*2, destImage.height()-(10-i)*2);
   //     color.setAlpha(150 - qSqrt(i)*50);
   //     painter.setPen(color);
   //     painter.drawPath(path);
   // }

   //for (int i = 10; i > 0; i--)
   //{
	  // QPoint p1(0, i);
	  // QPoint p2(destImage.width(), i);
   //    painter.setPen(color);
	  // painter.drawLine(p1, p2);

   //    // color.setAlpha(150 - qSqrt(i)*50);
   //}

	return destImage;
}

RoundImage::RoundImage(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

//	QImage img;
//	if (!img.load("D:\\temp\\1.png")) {
//		return;
//	}
//
//	int pWidth = img.width();
//	int pHeight = img.height();
//	qDebug("读取成功%d, %d", pWidth, pHeight);
//	QImage pimg = img.copy(0, 0, pWidth / 2, pHeight / 2);
//	pimg.save("e://test2.png");


	QPixmap rounder_pixmap = getRoundRectPixmap(QPixmap("D:\\temp\\6.png"), QSize(300, 500), 2);
	rounder_pixmap.save("E://3.png");
}


void RoundImage::paintEvent(QPaintEvent* paintEvent)
{

#if 0
	QPainter painter(this);
	// 设置渐变色
    // QLinearGradient linear(QPointF(80, 80), QPointF(150, 150));
    QLinearGradient linear(QPointF(90, 0), QPointF(90, 90));
    linear.setColorAt(0, Qt::gray);
    linear.setColorAt(0.5, QColor(0, 0, 0, 50));
    linear.setColorAt(0.6, QColor(0, 0, 0, 30));
    linear.setColorAt(0.7, QColor(0, 0, 0, 10));
    linear.setColorAt(1, QColor(0, 0, 0, 0));
    // 设置显示模式
    linear.setSpread(QGradient::PadSpread);
    // 设置画笔颜色、宽度
    painter.setPen(QPen(QColor(0, 160, 230), 2));
    // 设置画刷填充
    painter.setBrush(linear);
    // 绘制椭圆
    painter.drawRect(QRect(0, 0, 180, 180));
	painter.drawLine(QPointF(80, 80), QPointF(150, 150));
#endif


	QRectF rectangle(10.0, 20.0, 80.0, 60.0);
	int startAngle = 30 * 16;
	int spanAngle = 90 * 16;

	QPainter painter1(this);
	painter1.setBrush(Qt::gray);
	painter1.drawArc(rectangle, startAngle, spanAngle);
	// painter1.drawRect(rectangle);

	QPainterPath myPath;
	myPath.moveTo(QPointF(50, 50));
	myPath.arcTo(QRect(0, 0, 100, 100), 60, 120);

	QPainter painter(this);
    QLinearGradient linear(QPoint(50, 50), QPoint(0, 0));
    linear.setColorAt(0, Qt::gray);
    linear.setColorAt(0.5, QColor(0, 0, 0, 50));
    linear.setColorAt(0.6, QColor(0, 0, 0, 30));
    linear.setColorAt(0.7, QColor(0, 0, 0, 10));
    linear.setColorAt(1, QColor(0, 0, 0, 0));
    // 设置显示模式
    linear.setSpread(QGradient::PadSpread);
	painter.setBrush(linear);
	// painter.setPen(myPen);
	painter.drawPath(myPath);

	return;
	//QColor color(0, 255, 0);
	//QPainterPath path;
	//QPainter painter(this);
	//// 抗锯齿
	//painter.setRenderHints(QPainter::Antialiasing, true);
	//// 图片平滑处理
	//painter.setRenderHints(QPainter::SmoothPixmapTransform, true);
	//path.addRect(0, 0, 20, 20);
	//painter.setPen(color);
	//painter.drawPath(path);	

	//path.addRoundedRect(QRect(50, 50, 250, 250), 10, 10);
	//painter.save();
	//painter.setClipPath(path);
	//painter.drawPath(path);	
	//painter.restore();
	//painter.drawRect(QRect(50, 50, 250, 250));	

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
	// QRadialGradient linear(QPoint(20,20), 20);
	QRadialGradient linear(startPoint, 20);
    linear.setColorAt(0, Qt::gray);
    linear.setColorAt(0.5, QColor(0, 0, 0, 50));
    linear.setColorAt(0.6, QColor(0, 0, 0, 30));
    // linear.setColorAt(0.7, QColor(0, 0, 0, 10));
    linear.setColorAt(1, QColor(0, 0, 0, 0));
    // 设置显示模式
    linear.setSpread(QGradient::PadSpread);
	painter.setBrush(linear);
	painter.setPen(Qt::NoPen);
	painter.drawPath(painterPath);
	painter.restore();
}
