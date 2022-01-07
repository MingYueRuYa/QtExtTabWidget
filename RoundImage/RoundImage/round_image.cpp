#include "round_image.h"

#include <QPainter>
#include <QPainterPath>

QPixmap RoundImage::getRoundRectPixmap(QPixmap srcPixMap, const QSize& size, int radius)
{
	//����������ݻ��ߴ�������
	if (srcPixMap.isNull()) {
		return srcPixMap;
	}

	const int kShadowWidth = 10;

	//��ȡͼƬ�ߴ�
	int imageWidth = srcPixMap.width();
	int imageHeight = srcPixMap.height();

	//�����ߴ��ͼƬ,��֤ͼƬ��ʾ��������
	QPixmap newPixMap = srcPixMap.scaled(imageWidth, (imageHeight == 0 ? imageWidth : imageHeight), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	QPixmap destImage(imageWidth+2*kShadowWidth, imageHeight+2*kShadowWidth);
	destImage.fill(Qt::transparent);
	QPainter painter(&destImage);
	// �����
	painter.setRenderHints(QPainter::Antialiasing, true);
	// ͼƬƽ������
	painter.setRenderHints(QPainter::SmoothPixmapTransform, true);

	int shadow_width = 20;
	// �ϱ�
	drawShadowRect(painter, 
					QPoint(destImage.width()/2, shadow_width), 
					QPoint(destImage.width()/2, 0),
					QRect(20, 0, destImage.width()-shadow_width*2, shadow_width));
	// �±�
	drawShadowRect(painter, 
					QPoint(destImage.width()/2, destImage.height()-shadow_width), 
					QPoint(destImage.width()/2, destImage.height()),
					QRect(20, destImage.height()-shadow_width, destImage.width()-2*shadow_width, destImage.height()));
	// ���
	drawShadowRect(painter, 
					QPoint(20, destImage.height()/2), 
					QPoint(0, destImage.height()/2),
					QRect(0, 20, 20, destImage.height()-2*shadow_width));
	// �ұ�
	drawShadowRect(painter, 
					QPoint(destImage.width()-shadow_width, destImage.height()/2), 
					QPoint(destImage.width(), destImage.height()/2),
					QRect(destImage.width()-shadow_width, 20, destImage.width()-20, destImage.height()-2*shadow_width));
	

	// �����
	QPainterPath myPath;
	myPath.moveTo(QPointF(20, 20));
	myPath.arcTo(QRect(0, 0, 40, 40), 90, 90);
	drawShadowArc(painter, QPoint(20, 20), QPoint(0, 0), myPath);

	// �����
	myPath.moveTo(QPointF(20, destImage.height()-20));
	myPath.arcTo(QRect(0, destImage.height()-40, 40, 40), 180, 90);
	drawShadowArc(painter, QPoint(20, destImage.height()-20), QPoint(0, 0), myPath);

	// ���ұ�
	myPath.moveTo(QPointF(destImage.width()-20, 20));
	myPath.arcTo(QRect(destImage.width()-40, 0, 40, 40), 0, 90);
	drawShadowArc(painter, QPoint(destImage.width()-20, 20), QPoint(destImage.width(), 0), myPath);

	// ���ұ�
	myPath.moveTo(QPointF(destImage.width()-20, destImage.height()-20));
	myPath.arcTo(QRect(destImage.width()-40, destImage.height()-40, 40, 40), 0, -90);
	drawShadowArc(painter, QPoint(destImage.width()-20, destImage.height()-20), QPoint(destImage.width(), 0), myPath);

	//// ��ͼƬ�ü�ΪԲ��
	QPainterPath path;
	QRect rect(kShadowWidth, kShadowWidth, imageWidth, imageHeight);
	path.addRoundedRect(rect, radius, radius, Qt::RelativeSize);
	painter.setClipPath(path);
	painter.drawPixmap(kShadowWidth, kShadowWidth, imageWidth, imageHeight, newPixMap);
	return destImage;
}

RoundImage::RoundImage(QWidget *parent)
    : QWidget(parent)
{
	ui_.setupUi(this);

	pixmap_ = getRoundRectPixmap(QPixmap(":/RoundImage/res/images/temp.png"), QSize(300, 500), 2);
}


void RoundImage::paintEvent(QPaintEvent* paintEvent)
{
	QPainter painter(this);
	painter.drawPixmap(30, 30, pixmap_);
	QWidget::paintEvent(paintEvent);
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
    // ������ʾģʽ
    linear.setSpread(QGradient::PadSpread);
    // ���û�����ɫ�����
    painter.setPen(Qt::NoPen);
    // ���û�ˢ���
    painter.setBrush(linear);
    // ������Բ
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
    // ������ʾģʽ
    linear.setSpread(QGradient::PadSpread);
	painter.setBrush(linear);
	painter.setPen(Qt::NoPen);
	painter.drawPath(painterPath);
	painter.restore();
}
