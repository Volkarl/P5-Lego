//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H
#define P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H

#include "../../SensorControllers/ColourSensorController.h"
#include "../../SteeringTypes.h"

class SpeedZoneDetectionComponent{
public:
    SpeedZoneDetectionComponent(ColourSensorController* colourSensor);
    bool DetectSpeedZone(SpeedZone* speedZone);

private:
    ColourSensorController* ColourSensor;
};

#endif //P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H
