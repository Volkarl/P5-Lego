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
#include "../../SensorControllers/UltrasonicSensorController.h"
#include "../../SensorControllers/NxtCamLineTrackingController.h"
#include "../../SensorControllers/ColourSensorController.h"
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
    SteeringSequence ObstacleDetectionSuggestion;
    SteeringSequence BusStopSuggestion;
    TurnData StayWithinLaneSuggestion;
    SpeedZone CurrentSpeedZone;

    // Sensor Controllers
    SteeringController* SteeringControl;
    //UltrasonicSensorController* ObstacleDetectionControl;
    //NxtCamLineTrackingController* CameraControl;
    //ColourSensorController* ColourControl;
    DisplayController* DisplayControl;

    void ExecuteSteeringCommand(SteeringSequence sequence);
    void ExecuteSteeringCommandMaxSpeed(SteeringSequence sequence, SpeedZone currentSpeedZone);
    void ExecuteSteeringCommandSetSpeed(SteeringSequence sequence, SpeedZone currentSpeedZone);
};

#endif //P5_LEGO_DRIVING_H
