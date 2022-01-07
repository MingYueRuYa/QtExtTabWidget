#pragma once

#include "ui_round_image.h"
#include <QtWidgets/QWidget>

class RoundImage : public QWidget
{
    Q_OBJECT

public:
    RoundImage(QWidget *parent = Q_NULLPTR);
    QPixmap getRoundRectPixmap(QPixmap srcPixMap, const QSize& size, int radius);

protected:
    virtual void paintEvent(QPaintEvent *paintEvent);

private:
    void draw4BorderShadow();
    void drawShadowRect(QPainter &painter,
						const QPoint &startPoint, 
                        const QPoint &endPoint, 
                        const QRect &destRect);
    void drawShadowArc(QPainter &painter,
						const QPoint &startPoint, 
                        const QPoint &endPoint, 
                        const QPainterPath &painterPath);

private:
    Ui::RoundImageClass ui;
};
