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
	int GetDistance() const;

private:
    SonarSensor* m_UltrasonicSensor;
};

#endif //P5_LEGO_ULTRASONICSENSORCONTROLLER_H
