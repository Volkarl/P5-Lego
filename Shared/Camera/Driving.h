//
// Created by karlz on 28/11/2017.
//

#include "IComponents.h"
#include "StayWithinLaneComponent.h"
#include "ObstacleDetectionComponent.h"
#include "BusStopDetectionComponent.h"
#include "SpeedZoneDetectionComponent.h"

#ifndef CAMERA_DRIVING_H
#define CAMERA_DRIVING_H


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


#endif //CAMERA_DRIVING_H
