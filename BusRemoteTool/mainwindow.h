#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "controllers/car.h"

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

	void on_testButton_2_clicked();
	
private:
	int m_iTimerID;
	Car m_Car;
	
    Ui::MainWindow *ui;
    void setupTable();
    void fillTable();
	
	bool testVar; // TODO: Remove
	bool reverseTestVar;
	
protected:
	void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
