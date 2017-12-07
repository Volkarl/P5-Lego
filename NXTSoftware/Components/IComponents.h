//
// Created by karlz on 29/11/2017.
//
#ifndef P5_LEGO_ICOMPONENTS_H
#define P5_LEGO_ICOMPONENTS_H

#include "../SteeringTypes.h"


/////////////// Component Interfaces ///////////////

class IDriving
{
    // Handles all initialization and calls to the other components, prioritises and saves their results, and calls the MotorController to steer the bus
public:
    virtual void DetectLanes() = 0;
    virtual void DetectObstacles() = 0;
    virtual void DetectBusStop() = 0;
    virtual void DetectSpeedZone() = 0;

    virtual void Steer() = 0;
    virtual void InitializeSystem() = 0;

    SteeringSequence CurrentSteeringSequence;
    int CurrentSteeringSequenceIndex;
};


class IStayWithinLaneComponent
{
public:
    virtual TurnData CalculateSteering() = 0;
};

class IObstacleDetectionComponent
{
public:
    virtual SteeringSequence CalculateSteering() = 0;
};

class IBusStopDetectionComponent
{
public:
    virtual SteeringSequence CalculateSteering() = 0;
};

class ISpeedZoneDetectionComponent
{
public:
    virtual SpeedZone CalculateSteering() = 0;
};

#endif //P5_LEGO_ICOMPONENTS_H

