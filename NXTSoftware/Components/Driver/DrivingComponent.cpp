//
// Created by karlz on 29/11/2017.
//

#include "DrivingComponent.h"

DrivingComponent::DrivingComponent(StayWithinLaneComponent* laneDetector,
                                   ObstacleDetectionComponent* obstacleDetector,
                                   BusStopDetectionComponent* busStopDetector,
                                   SpeedZoneDetectionComponent* speedZoneDetector,
                                   DisplayController* displayController){
    this->LaneCalculator = laneDetector;
    this->ObstacleCalculator = obstacleDetector;
    this->BusStopCalculator = busStopDetector;
    this->SpeedZoneCalculator = speedZoneDetector;

    this->DisplayControl = displayController;

    CurrentSpeedZone.SetData(10);
    StayWithinLaneTurnSuggestion.SetData(Direction(Left), 0);
    // Default values

    IsObstacleDetected = false;
    IsBusStopDetected = false;
}

void DrivingComponent::DetectLanes(){
    // DisplayControl->SetText("4 | LANE");

    TurnData turn;
    LaneCalculator->CalculateSteering(&turn);
    StayWithinLaneTurnSuggestion.SetData(turn.TurnDirection, turn.TurnDegrees);
}

void DrivingComponent::DetectObstacles()
{
	IsObstacleDetected = ObstacleCalculator->DetectObstacles();
}

void DrivingComponent::DetectBusStop()
{
    // DisplayControl->SetText("2 | BUS STOP");

    /*if(!BusStopCalculator->IsBusStopSequenceOngoing(SteeringControl->GetCmDrivenSinceLast())) {
        IsBusStopDetected = BusStopCalculator->DetectBusStop();
    }*/
    // If we're already executing a bus stop, we don't override it with a new measurement
}

void DrivingComponent::DetectSpeedZone()
{
    SpeedZone speedZone;

    if(SpeedZoneCalculator->DetectSpeedZone(&speedZone)){
        CurrentSpeedZone.SetData(speedZone.SpeedRpm);
    }
}

void DrivingComponent::Steer()
{
    // DisplayControl->SetText("1 | STEER");

    if(IsObstacleDetected)
        ExecuteHalt();

    else if(IsBusStopDetected)
        ExecuteBusStop();

    else
        ExecuteFollowTrack();
}

void DrivingComponent::ExecuteFollowTrack()
{
    //DisplayControl->SetText("STEER: Follow");

    // The bus drives according to LaneTracking turn and CurrentSpeedZone speed
    Execute(CurrentSpeedZone.SpeedRpm, StayWithinLaneTurnSuggestion);
}

void DrivingComponent::ExecuteHalt()
{
    //DisplayControl->SetText("STEER: Halt");

    Execute(0, TurnData(Direction(Left), 0));
}

void DrivingComponent::ExecuteBusStop()
{
    //DisplayControl->SetText("STEER: Bus Stop");

    SteeringCommand cmd;
    //BusStopCalculator->GetNextBusStopCommand(&cmd, SteeringControl->GetCmDrivenSinceLast());
    ExecuteSteeringCommandMaxSpeed(cmd, CurrentSpeedZone.SpeedRpm);
}

void DrivingComponent::ExecuteSteeringCommandMaxSpeed(SteeringCommand command, int maxRpm)
{
    int rpm;

    if(command.SpeedRotationsPerMin > maxRpm)
        rpm = maxRpm;
    else
        rpm = command.SpeedRotationsPerMin;

    Execute(rpm, TurnData(command.Turn->TurnDirection, command.Turn->TurnDegrees));
}

void DrivingComponent::Execute(int rpm, TurnData turn)
{
    //SteeringControl->SetTurningAngle(turn);
    //SteeringControl->SetSpeed(rpm);
}
