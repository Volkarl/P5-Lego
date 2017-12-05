//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_DRIVING_H
#define P5_LEGO_DRIVING_H

#include "../IComponents.h"
#include "../StayWithinLane/StayWithinLaneComponent.h"
#include "../ObstacleDetection/ObstacleDetectionComponent.h"
#include "../BusStopDetection/BusStopDetectionComponent.h"
#include "../SpeedZoneDetection/SpeedZoneDetectionComponent.h"

#include "../../SensorControllers/ISensorController.h"

class Driving : IDriving {
public:
    Driving();
    ~Driving() override;

    void DetectLanes() override;
    void DetectObstacles() override;
    void DetectBusStop() override;
    void DetectSpeedZone() override;

    void Steer() override;
    void InitializeSystem() override;

private:
    // Components
    IStayWithinLaneComponent LaneCalculator;
    IObstacleDetectionComponent ObstacleCalculator;
    IBusStopDetectionComponent BusStopCalculator;
    ISpeedZoneDetectionComponent SpeedZoneCalculator;

    // Component Results
    SteeringSequence ObstacleDetectionSuggestion;
    SteeringSequence BusStopSuggestion;
    Turn StayWithinLaneSuggestion;
    SpeedZone CurrentSpeedZone;

    // Sensor Controllers
    ISteeringController SteeringControl;
    IObstacleDetectionController ObstacleDetectionControl;
    ILineTrackingController CameraControl;
    IColourSensorController ColourControl;
};

#endif //P5_LEGO_DRIVING_H
