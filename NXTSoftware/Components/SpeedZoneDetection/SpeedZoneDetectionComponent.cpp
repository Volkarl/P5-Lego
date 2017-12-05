//
// Created by karlz on 29/11/2017.
//

#include "SpeedZoneDetectionComponent.h"

StayWithinLaneComponent::StayWithinLaneComponent() = default;

StayWithinLaneComponent::~StayWithinLaneComponent() = default;

SteeringSequence StayWithinLaneComponent::CalculateSteering() {
    return SteeringSequence();
}

void StayWithinLaneComponent::CalibrateSensors() {

}