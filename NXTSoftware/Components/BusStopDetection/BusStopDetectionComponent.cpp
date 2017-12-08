//
// Created by karlz on 29/11/2017.
//

#include "BusStopDetectionComponent.h"

BusStopDetectionComponent::BusStopDetectionComponent(IColourSensorController* colorSensor){
    this->ColourSensor = colorSensor;
}

SteeringSequence BusStopDetectionComponent::CalculateSteering() {
    return SteeringSequence(); //The angle, how fast it drives, and how long it drives.
    //It needs to return an array the above mentioned items.
    //The first point in the array is to turn the bus right into the bus lane.
    //After it has driven a sufficient amount the wheels will turn to the left, and stay that way until
    //The bus has arrived at the bus stop.
    //After waiting for a short duration the bus will turn left and drive out into traffic again.
    //After a certain point the wheels will drive to the right again to straighten the bus out.
    //When the bus is straightened out the wheels will go back to being straight.
}

void BusStopDetectionComponent::CalibrateSensors() {

}