//
// Created by karlz on 29/11/2017.
//

#include "StayWithinLaneComponent.h"

SpeedZoneDetectionComponent::SpeedZoneDetectionComponent(ILaneTrackingController laneTrackingSensor) {
    LaneTrackingSensor = laneTrackingSensor;
}

SpeedZoneDetectionComponent::~SpeedZoneDetectionComponent() = default;

SteeringSequence SpeedZoneDetectionComponent::CalculateSteering() {
    return SteeringSequence();
}

void SpeedZoneDetectionComponent::CalibrateSensors() {

}