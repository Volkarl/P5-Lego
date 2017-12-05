//
// Created by karlz on 5/12/2017.
//

#ifndef P5_LEGO_ULTRASONICSENSORCONTROLLER_H
#define P5_LEGO_ULTRASONICSENSORCONTROLLER_H

#include "ISensorController.h"
#include "../SteeringTypes.h"

class UltrasonicSensorController : public IObstacleDetectionController {
public:
    UltrasonicSensorController();
    ~UltrasonicSensorController() override;
    int GetDistance() override;
};

#endif //P5_LEGO_ULTRASONICSENSORCONTROLLER_H
