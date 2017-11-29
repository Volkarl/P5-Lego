//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H
#define P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H

#include "../IComponents.h"

class SpeedZoneDetectionComponent : ISpeedZoneDetectionComponent {
public:
    SpeedZoneDetectionComponent();
    ~SpeedZoneDetectionComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override ;
};

#endif //P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H
