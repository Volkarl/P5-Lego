//
// Created by karlz on 29/11/2017.
//

#include "StayWithinLaneComponent.h"

SpeedZoneDetectionComponent::SpeedZoneDetectionComponent(ILaneTrackingController* laneTrackingSensor) {
    this->LaneTrackingSensor = laneTrackingSensor;
}

SteeringSequence SpeedZoneDetectionComponent::CalculateSteering() {
    return SteeringSequence();
}

void SpeedZoneDetectionComponent::CalibrateSensors() {

}