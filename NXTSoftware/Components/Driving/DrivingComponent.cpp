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

    if(ObstacleDetectionSuggestion != nullptr)
        ExecuteSteeringCommand(ObstacleDetectionSuggestion);

    // ObstacleDetectionSuggestion > StayWithinLaneSuggestion > BusStopSuggestion > SpeedZoneSuggestion ???
    // SpeedZone sets a speed, that can only be temporarily increased by ObstacleDetection
    // It can be decreased by busstop as well though
    // StayWithinLane decides turn, but this
}

void Driving::ExecuteSteeringCommand(SteeringSequence sequence){
    if(sequence == nullptr) return;
    if(sequence.GetNewActiveCommand(SteeringControl.GetCmDrivenSinceLast()))
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