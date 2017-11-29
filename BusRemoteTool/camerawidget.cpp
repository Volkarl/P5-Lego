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
	
	const std::vector<Rectangle_T>& lines = this->m_Car->m_Cam.m_lstObjects;
	
	for (size_t i = 0; i < lines.size(); i++) {
		const Rectangle_T& line = lines[i];
		
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
        painter.setBrush(line.objColor == 0 ? Qt::black : Qt::red);
        painter.drawRect(rect);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 15));
        painter.drawText(rect, Qt::AlignCenter, QString::number(i + 1));
    }
    //QRect rect1(10, 10, 50, 50);
    //painter.drawRect(rect1);

    // LETS DRAW
	DirectionType dirtomove = this->m_Car->m_Cam.ShouldEvade();
	switch(dirtomove) {
		case DirectionType::None:
			if (this->m_fDegree > -0.2 && this->m_fDegree < 0.2) {
				this->m_fDegree = 0;
			} else {
				if (this->m_fDegree > 0) this->m_fDegree -= 0.5f;
				else if (this->m_fDegree < 0) this->m_fDegree += 0.5f;
			}
			break;
			
		case DirectionType::Left:
			this->m_fDegree -= 0.5f;
			break;
			
		case DirectionType::Right:
			this->m_fDegree += 0.5f;
			break;
	}
	
	if (this->m_fDegree > 25.0f) this->m_fDegree = 25.0f;
	if (this->m_fDegree < -25.0f) this->m_fDegree = -25.0f;
	
	/*if(dirtomove == DirectionType::None)
		qDebug() << "None: " << QString::number(this->m_fDegree);
	else if(dirtomove == DirectionType::Left)
		qDebug() << "Left: " << QString::number(this->m_fDegree);
	else if(dirtomove == DirectionType::Right)
		qDebug() << "Right: " << QString::number(this->m_fDegree);*/
	
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
