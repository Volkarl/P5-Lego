#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "commcamera.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_testButton_clicked();

    void on_connectButton_clicked();

    void on_disconnectButton_clicked();

    void on_refreshButton_clicked();

private:
    Ui::MainWindow *ui;
    void setupTable();
    void fillTable();

};

#endif // MAINWINDOW_H
