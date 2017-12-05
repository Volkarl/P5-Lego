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
    StayWithinLaneComponent LaneCalculator;
    ObstacleDetectionComponent ObstacleCalculator;
    BusStopDetectionComponent BusStopCalculator;
    SpeedZoneDetectionComponent SpeedZoneCalculator;

    // Steering Sequences
    SteeringSequence StayWithinLaneSuggestion;
    SteeringSequence ObstacleDetectionSuggestion;
    SteeringSequence BusStopSuggestion;
    SteeringSequence SpeedZoneSuggestion;

    // SteeringController
    SteeringController SteeringControl;
};

#endif //P5_LEGO_DRIVING_H
