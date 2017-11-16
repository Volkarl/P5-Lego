#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QWidget>
#include "../Shared/Connectivity.h"
#include "commcamera.h"

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

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
    Ui::cameraWidget *ui;
};

#endif // CAMERAWIDGET_H
