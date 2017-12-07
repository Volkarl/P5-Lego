//
// Created by karlz on 29/11/2017.
//

#include "BusStopDetectionComponent.h"

BusStopDetectionComponent::BusStopDetectionComponent(IColourSensorController colorSensor){
    ColourSensor = colorSensor;
}

BusStopDetectionComponent::~BusStopDetectionComponent() = default;

SteeringSequence BusStopDetectionComponent::CalculateSteering() {
    return SteeringSequence(); //The angle, how fast it drives, and how long it drives.
    //It needs to return an array the above mentioned items.
    //The first point in the array is to turn the bus right into the bus lane.
    //
}

void BusStopDetectionComponent::CalibrateSensors() {

}