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
    virtual ~IDriving() = default;

    SteeringSequence CurrentSteeringSequence;
    int CurrentSteeringSequenceIndex;
};

class ISteeringSuggestionComponent
{
public:
    virtual SteeringSequence CalculateSteering() = 0;
    virtual void CalibrateSensors() = 0;
    virtual ~ISteeringSuggestionComponent() = default;
};

class IStayWithinLaneComponent : public ISteeringSuggestionComponent
{
public:
    virtual ~IStayWithinLane() = 0;
};

class IObstacleDetectionComponent : public ISteeringSuggestionComponent
{
public:
    virtual ~IObstacleDetection() = default;
};

class IBusStopDetectionComponent : public ISteeringSuggestionComponent
{
public:
    virtual ~IBusStopDetection() = default;
};

class ISpeedZoneDetectionComponent : public ISteeringSuggestionComponent
{
public:
    virtual ~ISpeedZoneDetection() = default;
};


/////////////// Other Stuff /////////////////
class IManoeuvre
{
    // The class containing high-level manoeuvres
public:
    virtual static SteeringSequence StopAtBusStop() = 0;
    virtual static SteeringSequence DriveFromBusStop() = 0;

    virtual ~IManoeuvre() = default;
    // Todo: Do I need this class? I could put it all into bus stop component
};

#endif //P5_LEGO_ICOMPONENTS_H
