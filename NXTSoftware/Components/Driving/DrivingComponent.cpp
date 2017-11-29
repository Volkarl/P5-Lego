//
// Created by karlz on 29/11/2017.
//

#include "DrivingComponent.h"

Driving::Driving() = default;

Driving::~Driving() {
    delete LaneCalculator, ObstacleCalculator, BusStopCalculator, SpeedZoneCalculator;
}

void Driving::DetectLanes() {
    LaneCalculator.CalculateSteering();
    // Todo: Add the return value in the SuggestedSteeringSequence field
}

void Driving::DetectObstacles() {
    ObstacleCalculator.CalculateSteering();
    // Todo: Add the return value in the SuggestedSteeringSequence field
}

void Driving::DetectBusStop() {
    BusStopCalculator.CalculateSteering();
    // Todo: Add the return value in the SuggestedSteeringSequence field
}

void Driving::DetectSpeedZone() {
    SpeedZoneCalculator.CalculateSteering();
    // Todo: Add the return value in the SuggestedSteeringSequence field
}

void Driving::Steer() {
/* Executes the current index of the top-priority SteeringSequence with the MotorController
Then increments the index or removes the SteeringSequence entry */

}

void Driving::InitializeSystem() {
    LaneCalculator = new StayWithinLaneComponent();
    ObstacleCalculator = new ObstacleDetectionComponent();
    BusStopCalculator = new BusStopDetectionComponent();
    SpeedZoneCalculator = new SpeedZoneDetectionComponent();

    LaneCalculator.CalibrateSensors();
    ObstacleCalculator.CalibrateSensors();
    BusStopCalculator.CalibrateSensors();
    SpeedZoneCalculator.CalibrateSensors();
}