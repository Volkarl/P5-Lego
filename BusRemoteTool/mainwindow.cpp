#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../Shared/Connectivity.h"
#include "includes/nxtcommfantom/nxtcommfantom.h"
#include "commcamera.h"

extern Rectangle_T CommCamera::rect[];
CommCamera cam;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setupTable();

    // Stuff, (144+32) 176 width

    // Dummy data
    CommCamera::rect[0].upperLeftX =  12; CommCamera::rect[0].upperLeftY =  0; CommCamera::rect[0].width = 11; CommCamera::rect[0].height = 18;
    CommCamera::rect[1].upperLeftX = 144; CommCamera::rect[1].upperLeftY = 13; CommCamera::rect[1].width = 32; CommCamera::rect[1].height = 18;
    CommCamera::rect[2].upperLeftX = 132; CommCamera::rect[2].upperLeftY = 32; CommCamera::rect[2].width = 23; CommCamera::rect[2].height = 18;
    CommCamera::rect[3].upperLeftX =  12; CommCamera::rect[3].upperLeftY = 19; CommCamera::rect[3].width =  7; CommCamera::rect[3].height = 18;
    CommCamera::rect[4].upperLeftX =  12; CommCamera::rect[4].upperLeftY = 38; CommCamera::rect[4].width =  7; CommCamera::rect[4].height = 18;
    CommCamera::rect[5].upperLeftX = 132; CommCamera::rect[5].upperLeftY = 51; CommCamera::rect[5].width = 15; CommCamera::rect[5].height = 18;
    CommCamera::rect[6].upperLeftX =  12; CommCamera::rect[6].upperLeftY = 57; CommCamera::rect[6].width =  7; CommCamera::rect[6].height = 18;
    CommCamera::rect[7].upperLeftX = 132; CommCamera::rect[7].upperLeftY = 70; CommCamera::rect[7].width = 11; CommCamera::rect[7].height = 18;

    // End
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Create event for button for this*/
void MainWindow::on_refreshButton_clicked()
{
    cam.getObjects();

    ui->camWidget->refreshView();
    this->fillTable();
}

void MainWindow::setupTable()
{
    ui->tableWidget->clear();

    QStringList headers;
    headers << "Width" << "Height" << "X" << "Y";
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderLabels(headers);

}

void MainWindow::fillTable()
{
    ui->tableWidget->setRowCount(cam.amountOfObjects()); // TODO: Not hardcoded

    for (int i = 0; i < cam.amountOfObjects(); i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(CommCamera::rect[i].width)));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(CommCamera::rect[i].height)));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(CommCamera::rect[i].upperLeftX)));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(CommCamera::rect[i].upperLeftY)));
    }
}

void MainWindow::on_testButton_clicked()
{
    cam.getObjects();

    ui->camWidget->refreshView();
    this->fillTable();
}

void MainWindow::on_connectButton_clicked()
{
    cam.Connect();
    if (cam.isConnected())
    {
        ui->connectButton->setEnabled(false);
        ui->disconnectButton->setEnabled(true);

        ui->testButton->setEnabled(true);
        ui->refreshButton->setEnabled(true);
    }
}

void MainWindow::on_disconnectButton_clicked()
{
    cam.Disconnect();

    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);

    ui->testButton->setEnabled(false);
    ui->refreshButton->setEnabled(false);
}
