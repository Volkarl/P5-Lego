//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H
#define P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H

#include "../../SteeringTypes.h"

class SpeedZoneDetectionComponent{
public:
    SpeedZoneDetectionComponent();
    bool DetectSpeedZone(SpeedZone* speedZone);
};

#endif //P5_LEGO_SPEEDZONEDETECTIONCOMPONENT_H
