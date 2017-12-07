//
// Created by karlz on 29/11/2017.
//

#include "BusStopDetectionComponent.h"

BusStopDetectionComponent::BusStopDetectionComponent(IColourSensorController* colorSensor){
    this->ColourSensor = colorSensor;
}

SteeringSequence BusStopDetectionComponent::CalculateSteering() {
    return SteeringSequence();
    //todo
}

void BusStopDetectionComponent::CalibrateSensors() {

}