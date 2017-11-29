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

    // Steering Sequences Ordered by Priority
    SteeringSequence SuggestedSteeringSequences [4];
    // Todo: THIS SHOULD NOT JUST BE AN ARRAY! I want to be able access them by typing in the sender enum, but also access their priorities somehow.
};

#endif //P5_LEGO_DRIVING_H
