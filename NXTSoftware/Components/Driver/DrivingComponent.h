//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_DRIVING_H
#define P5_LEGO_DRIVING_H

#include "../StayWithinLane/StayWithinLaneComponent.h"
#include "../ObstacleDetection/ObstacleDetectionComponent.h"
#include "../BusStopDetection/BusStopDetectionComponent.h"
#include "../SpeedZoneDetection/SpeedZoneDetectionComponent.h"

#include "../../SensorControllers/SteeringController.h"
#include "../../SensorControllers/DisplayController.h"
//todo fix includes

class DrivingComponent {
public:
    DrivingComponent(StayWithinLaneComponent* laneDetector, ObstacleDetectionComponent* obstacleDetector,
            BusStopDetectionComponent* busStopDetector, SpeedZoneDetectionComponent* speedZoneDetector,
            SteeringController* steeringController, DisplayController* displayController);

    void DetectLanes();
    void DetectObstacles();
    void DetectBusStop();
    void DetectSpeedZone();

    void Steer();

private:
    // Components
    StayWithinLaneComponent* LaneCalculator;
    ObstacleDetectionComponent* ObstacleCalculator;
    BusStopDetectionComponent* BusStopCalculator;
    SpeedZoneDetectionComponent* SpeedZoneCalculator;

    // Component Results
    TurnData StayWithinLaneTurnSuggestion;
    SpeedZone CurrentSpeedZone;
    bool IsObstacleDetected, IsBusStopDetected;

    // Sensor Controllers
    SteeringController* SteeringControl;
    DisplayController* DisplayControl;

    // Methods
    void ExecuteSteeringCommandMaxSpeed(SteeringCommand command, int maxRpm);
    void Execute(int rpm, TurnData turn);

    void ExecuteFollowTrack();
    void ExecuteBusStop();
    void ExecuteHalt();
};

#endif //P5_LEGO_DRIVING_H
