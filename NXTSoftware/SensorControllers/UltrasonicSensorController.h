#ifndef P5_LEGO_ULTRASONICSENSORCONTROLLER_H
#define P5_LEGO_ULTRASONICSENSORCONTROLLER_H

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
