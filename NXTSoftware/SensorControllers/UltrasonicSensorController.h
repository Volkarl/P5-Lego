//
// Created by karlz on 5/12/2017.
//

#ifndef P5_LEGO_ULTRASONICSENSORCONTROLLER_H
#define P5_LEGO_ULTRASONICSENSORCONTROLLER_H

#include "../SteeringTypes.h"
#include "SonarSensor.h"
#include "Clock.h"

using namespace ecrobot;

class UltrasonicSensorController {
public:
    UltrasonicSensorController(SonarSensor* ultrasonicSensor);
    void Calibrate();
    int GetDistancePrecise();
    int GetDistanceFast();

private:
    SonarSensor* UltrasonicSensor;
    Clock clock;
};

#endif //P5_LEGO_ULTRASONICSENSORCONTROLLER_H
