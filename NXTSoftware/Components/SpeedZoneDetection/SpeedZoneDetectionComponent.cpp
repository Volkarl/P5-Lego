//
// Created by karlz on 29/11/2017.
//

#include "SpeedZoneDetectionComponent.h"

SpeedZoneDetectionComponent::SpeedZoneDetectionComponent(ColourSensorController *colourSensor){
    this->ColourSensor = colourSensor;
}

bool SpeedZoneDetectionComponent::DetectSpeedZone(SpeedZone *speedZone) {
    return false;
}
