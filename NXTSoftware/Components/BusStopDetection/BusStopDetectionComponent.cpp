//
// Created by karlz on 29/11/2017.
//

#include "BusStopDetectionComponent.h"

BusStopDetectionComponent::BusStopDetectionComponent(IColourSensorController colorSensor){
    ColourSensor = colorSensor;
}

BusStopDetectionComponent::~BusStopDetectionComponent() = default;

SteeringSequence BusStopDetectionComponent::CalculateSteering() {
    return SteeringSequence();
}

void BusStopDetectionComponent::CalibrateSensors() {

}