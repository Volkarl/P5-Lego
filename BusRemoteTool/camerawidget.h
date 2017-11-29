#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include "../Shared/Connectivity.h"
#include "controllers/car.h"

namespace Ui {
class cameraWidget;
}

class cameraWidget : public QWidget
{
    Q_OBJECT

public:
    explicit cameraWidget(QWidget *parent = 0);
    void refreshView();
    ~cameraWidget();
	const Car* m_Car;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::cameraWidget *ui;
	float m_fDegree;
};

#endif // CAMERAWIDGET_H
