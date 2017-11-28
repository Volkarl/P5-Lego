//
// Created by karlz on 28/11/2017.
//

#include "IComponents.h"

#ifndef CAMERA_SPEEDZONEDETECTIONCOMPONENT_H
#define CAMERA_SPEEDZONEDETECTIONCOMPONENT_H


class SpeedZoneDetectionComponent : ISpeedZoneDetectionComponent {
public:
    SpeedZoneDetectionComponent();
    ~SpeedZoneDetectionComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override ;
};


#endif //CAMERA_SPEEDZONEDETECTIONCOMPONENT_H
