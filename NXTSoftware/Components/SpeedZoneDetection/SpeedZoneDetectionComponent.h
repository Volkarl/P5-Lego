//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H
#define P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H

#include "../IComponents.h"
#include "../../SensorControllers/ISensorController.h"

class SpeedZoneDetectionComponent : ISpeedZoneDetectionComponent {
public:
    SpeedZoneDetectionComponent(IColourSensorController colourSensor);
    ~SpeedZoneDetectionComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override ;

private:
    IColourSensorController ColourSensor;
};

#endif //P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H
