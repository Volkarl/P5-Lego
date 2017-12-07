//
// Created by karlz on 29/11/2017.
//

#include "DrivingComponent.h"

void Driving::Driving(IStayWithinLaneComponent *laneDetector, IObstacleDetectionComponent *obstacleDetector,
                      IBusStopDetectionComponent *busStopDetector, ISpeedZoneDetectionComponent *speedZoneDetector,
                      ISteeringController *steeringController, IObstacleDetectionController *obstacleDetectionController,
                      ILaneTrackingController *laneTrackingController, IColourSensorController *colourSensorController) {
    this->LaneCalculator = laneDetector;
    this->ObstacleDetectionControl = obstacleDetector;
    this->BusStopCalculator = busStopDetector;
    this->SpeedZoneCalculator = speedZoneDetector;

    this->SteeringControl = steeringController;
    this->ObstacleDetectionControl = obstacleDetectionController;
    this->CameraControl = laneTrackingController;
    this->ColourControl = colourSensorController;
}

void Driving::DetectLanes() {
    TurnData turn = LaneCalculator.CalculateSteering();
    if(turn != nullptr) StayWithinLaneSuggestion = turn;
}

void Driving::DetectObstacles() {
    SteeringSequence sequence = ObstacleCalculator.CalculateSteering();
    if(sequence != nullptr) ObstacleDetectionSuggestion = sequence;
}

void Driving::DetectBusStop() {
    SteeringSequence sequence = BusStopCalculator.CalculateSteering();
    if(sequence != nullptr) BusStopSuggestion = sequence;
}

void Driving::DetectSpeedZone() {
    SpeedZone currentSpeedZone = SpeedZoneCalculator.CalculateSteering();
    if(currentSpeedZone != nullptr) CurrentSpeedZone = currentSpeedZone;
}

void Driving::Steer() {
/* Executes the current index of the top-priority SteeringSequence with the MotorController
Then increments the index or removes the SteeringSequence entry */

    if(ObstacleDetectionSuggestion != nullptr)
        ExecuteSteeringCommand(ObstacleDetectionSuggestion);
        // ObstacleDetection overrides all other commands

    else if(BusStopSuggestion != nullptr)
        ExecuteSteeringCommandMaxSpeed(BusStopSuggestion, CurrentSpeedZone);
        // Uses the current speed zone as its maximum speed; cannot exceed that

    else ExecuteSteeringCommandSetSpeed(CurrentLaneTrackTurn, CurrentSpeedZone);
    // Whenever otherwise idle, the bus drives according to LaneTracking and CurrentSpeedZone
    // Uses the current speed zone as its actual speed, not only as max value
}

void Driving::ExecuteSteeringCommand(SteeringSequence sequence){
    if(sequence == nullptr) return;
    SteeringCommand command = sequence.GetNewActiveCommand(SteeringControl.GetCmDrivenSinceLast);
    if(command == nullptr) return;

    SteeringControl.SetSpeed(command.SpeedRotationsPerMin);
    SteeringControl.SetTurningAngle(command.Turn);
}

void Driving::ExecuteSteeringCommandMaxSpeed(SteeringSequence sequence, SpeedZone currentSpeedZone){
    if(sequence == nullptr) return;
    SteeringCommand command = sequence.GetNewActiveCommand(SteeringControl.GetCmDrivenSinceLast);
    if(command == nullptr) return;

    if(command.SpeedRotationsPerMin > currentSpeedZone.SpeedRpm)
        SteeringControl.SetSpeed(currentSpeedZone.SpeedRpm);
    else SteeringControl.SetSpeed(command.SpeedRotationsPerMin);

    SteeringControl.SetTurningAngle(command.Turn);
}

void Driving::ExecuteSteeringCommandSetSpeed(SteeringSequence sequence, SpeedZone currentSpeedZone){
    if(sequence == nullptr) return;
    SteeringCommand command = sequence.GetNewActiveCommand(SteeringControl.GetCmDrivenSinceLast);
    if(command == nullptr) return;

    SteeringControl.SetSpeed(currentSpeedZone.SpeedRpm);
    SteeringControl.SetTurningAngle(command.Turn);
}

void Driving::InitializeSystem() {
    // Initialize Controllers
    SteeringControl = new SteeringController();
    UltrasonicControl = new UltrasonicSensorController();
    CameraControl = new NxtCamLineTrackingController();
    ColourControl = new ColourSesnsorController();

    // Calibrate Sensors
    SteeringControl.Calibrate();
    UltrasonicControl.Calibrate();
    CameraControl.Calibrate();
    ColourControl.Calibrate();

    // Initialize Components
    LaneCalculator = new StayWithinLaneComponent(CameraControl);
    ObstacleCalculator = new ObstacleDetectionComponent(ObstacleDetectionControl);
    BusStopCalculator = new BusStopDetectionComponent(ColourControl);
    SpeedZoneCalculator = new SpeedZoneDetectionComponent(ColourControl);
}