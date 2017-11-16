#include "camerawidget.h"
#include "ui_camerawidget.h"
#include <QPainter>
#include <QRect>
#include <QColor>
#include <QBrush>
#include <QPoint>
#include <QSize>

cameraWidget::cameraWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cameraWidget)
{
    ui->setupUi(this);

    //this->setFixedSize(88, 144);
}

cameraWidget::~cameraWidget()
{
    delete ui;
}


void cameraWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(0,0,width()-2, height()-2);

    //painter.setFont(QFont("Arial", 30));
    //painter.drawText(rect(), Qt::AlignCenter, "camera");

    for (int i = 0; i < 8; i++)
    {
        QPoint upperLeft(CommCamera::rect[i].upperLeftX * 2, CommCamera::rect[i].upperLeftY * 2);
        QSize size(CommCamera::rect[i].width * 2, CommCamera::rect[i].height * 2);

        QRect rect(upperLeft, size);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::black);
        painter.drawRect(rect);

        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 15));
        painter.drawText(rect, Qt::AlignCenter, QString::number(i + 1));
    }
    //QRect rect1(10, 10, 50, 50);
    //painter.drawRect(rect1);


    // LETS DRAW
    double angle = -30.0;
    double calc = sin(angle) * 40;

    QPolygon poly;
    poly << QPoint(this->width() / 2, this->height() - 2);
    poly << QPoint((this->width() / 2) - (int)calc, this->height() - 70);

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(10);

    painter.setPen(pen);
    painter.drawPolygon(poly);
}

void cameraWidget::refreshView()
{
//    CommCamera::rect[0].upperLeftX = 30;

    repaint();

}
