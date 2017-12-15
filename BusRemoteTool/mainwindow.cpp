#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../Shared/Connectivity.h"
#include "includes/nxtcommfantom/nxtcommfantom.h"
#include "controllers/car.h"

#include <QTimer>
#include <QDebug>

#define TIMER_REFRESH_RATE 175

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setupTable();
	
	ui->camWidget->m_Car = &this->m_Car;
	
	this->m_iTimerID = startTimer(TIMER_REFRESH_RATE);
	
	this->testVar = false;
	this->reverseTestVar = false;
}

MainWindow::~MainWindow()
{
	killTimer(this->m_iTimerID);
	
	this->m_Car.Deinit();
    delete ui;
}

/* Create event for button for this*/
void MainWindow::on_refreshButton_clicked()
{
	/*this->m_Car.Update();
    ui->camWidget->refreshView();
    this->fillTable();*/
	this->m_Car.m_Motor.SetAngle(0);
}

void MainWindow::setupTable()
{
    ui->tableWidget->clear();

    QStringList headers;
    headers << "W" << "H" << "X" << "Y" << "dist";
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderLabels(headers);

}

void MainWindow::fillTable()
{
	this->setupTable();
	
	CamBuffer cambuff = this->m_Car.m_Cam.GetBuffer();
	ui->tableWidget->setRowCount(cambuff.m_iCount);
	
	for (size_t i = 0; i < (size_t)cambuff.m_iCount; i++) {
		const Rectangle_T& line = cambuff.m_buffRects[i];
		
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(line.width)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(line.height)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(line.upperLeftX)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(line.upperLeftY)));
		
		const int distance = this->m_Car.m_Cam.m_Detector.GetDistance(cambuff.m_buffRects[i]);
		ui->tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(distance)));
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
	this->m_Car.Update();
    ui->camWidget->refreshView();
    this->fillTable();
	
	ui->currentAngleBox->setText(QString::number(this->m_Car.m_Cam.m_Detector.angle) + "°");
	
	qDebug() << "A: " << QString::number(-(this->m_Car.GetAngle()));
	
	const int angle = ui->angleBox->value();
	const int speed = ui->speedBox->value();
	
	
	if(reverseTestVar) // BEWARE: DESTROYS CAR
	{
		this->m_Car.m_Motor.SetForce(-30);
		this->m_Car.m_Motor.SetAngle(angle);
	} else {
		if (testVar)
		{
			this->m_Car.AllowDrive = true;
		}
		else
		{
			this->m_Car.AllowDrive = false;
		}
	}
	
	ui->currentSpeedBox->setText(QString::number(this->m_Car.m_Motor.GetSpeed()));
	
	/*
	//this->m_Car.m_Motor.SetAngle(-45);
	
	if (this->testVar)
	{
		this->m_Car.m_Motor.SetForce(25);
	} else {
		this->m_Car.m_Motor.SetForce(0);
	}
	
	this->m_Car.m_Motor.Send();
	*/
	
	/*CamBuffer cambuff = this->m_Car.m_Cam.GetBuffer();
	ui->colorRedBox->setText(QString::number((int) cambuff.m_buffRects[0].color.red));
	ui->colorGreenBox->setText(QString::number((int) cambuff.m_buffRects[0].color.green));
	ui->colorBlueBox->setText(QString::number((int) cambuff.m_buffRects[0].color.blue));*/
}

void MainWindow::on_testButton_clicked()
{
	testVar = !testVar;
	const int angle = ui->angleBox->value();
	
	//this->m_Car.m_Motor.SetAngle(angle);	
	
	/*if (testVar)
		this->m_Car.m_Motor.SetForce(30);
	else
		this->m_Car.m_Motor.SetForce(0);*/
	
	//this->m_Car.m_Motor.Send();
}

void MainWindow::on_connectButton_clicked()
{
    if (this->m_Car.Init())
    {
        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);

        ui->testButton->setEnabled(true);
		ui->testButton_2->setEnabled(true);
        ui->refreshButton->setEnabled(true);
    }
}

void MainWindow::on_disconnectButton_clicked()
{
	this->m_Car.Deinit();

    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);

    ui->testButton->setEnabled(false);
	ui->testButton_2->setEnabled(false);
    ui->refreshButton->setEnabled(false);
}

void MainWindow::on_testButton_2_clicked()
{
    reverseTestVar = !reverseTestVar;
	ui->testButton->setEnabled(!reverseTestVar);
}
