#include "camerawidget.h"
#include "ui_camerawidget.h"
#include <QPainter>
#include <QRect>
#include <QColor>
#include <QBrush>
#include <QPoint>
#include <QSize>

#include <QDebug>

cameraWidget::cameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraWidget)
{
    ui->setupUi(this);

	this->m_fDegree = 0.0f;
    //this->setFixedSize(88, 144);
}

cameraWidget::~cameraWidget()
{
    delete ui;
}


void cameraWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(0,0,width()-2, height()-2);

    //painter.setFont(QFont("Arial", 30));
    //painter.drawText(rect(), Qt::AlignCenter, "camera");
	
	CamBuffer cambuff = this->m_Car->m_Cam.GetBuffer();
	this->m_Detector.MarkData(cambuff);
	
	for (size_t i = 0; i < (size_t)cambuff.m_iCount; i++)
	{
		const Rectangle_T& line = cambuff.m_buffRects[i];
		
        QPoint upperLeft(line.upperLeftX * 2, line.upperLeftY * 2);
        QSize size(line.width * 2, line.height * 2);
		
		if (upperLeft.x() + size.width() > width()) {
			size.setWidth(width() - upperLeft.x());
		}
		
		if (upperLeft.y() + size.height() > width()) {
			size.setHeight(height() - upperLeft.y());
		}

        QRect rect(upperLeft, size);
        painter.setPen(Qt::NoPen);
        painter.setBrush(line.collision == false ? Qt::black : Qt::red);
        painter.drawRect(rect);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 15));
        painter.drawText(rect, Qt::AlignCenter, QString::number(i + 1));
    }
    //QRect rect1(10, 10, 50, 50);
    //painter.drawRect(rect1);


	const float turnMultiplier = 1.5f;
	const float maxTurnAngle = 46.0f;
	
    // LETS DRAW
	DirectionType dirtomove = this->m_Detector.ShouldEvade();
	switch(dirtomove) {
		case DirectionType::None:
			if (this->m_fDegree > -1.0f && this->m_fDegree < 1.0f) {
				this->m_fDegree = 0;
			} else {
				if (this->m_fDegree > 0) this->m_fDegree -= turnMultiplier;
				else if (this->m_fDegree < 0) this->m_fDegree += turnMultiplier;
			}
			break;
			
		case DirectionType::Left:
			this->m_fDegree -= turnMultiplier;
			break;
			
		case DirectionType::Right:
			this->m_fDegree += turnMultiplier;
			break;
	}
	
	if (this->m_fDegree > maxTurnAngle) this->m_fDegree = maxTurnAngle;
	if (this->m_fDegree < -maxTurnAngle) this->m_fDegree = -maxTurnAngle;
	
	QLineF angleline;
	angleline.setP1(QPointF(this->width() / 2, this->height() - 30));
	angleline.setLength(40);
	angleline.setAngle(90 - (int)this->m_fDegree);
	

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(10);

    painter.setPen(pen);
	painter.drawLine(angleline);
    //painter.drawPolygon(poly);
}

void cameraWidget::refreshView()
{
    repaint();
}
