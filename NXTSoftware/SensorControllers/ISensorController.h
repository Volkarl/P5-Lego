//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_ISENSORCONTROLLER_H
#define P5_LEGO_ISENSORCONTROLLER_H

#include "../SteeringTypes.h"

class ISensorController
{
public:
    virtual void Calibrate() = 0;
    virtual ~ISensorController() = 0;
};

class ISteeringController : public ISensorController
{
    // The inherited calibrate-method handles things like centering the wheels of the bus during setup
public:
    virtual void SetSpeed(int rpm) = 0;
    virtual void SetTurningAngle(TurnData turn) = 0;
    virtual ~ISteeringController() = 0;

    int CurrentSpeedRpm;
    TurnData CurrentTurningAngle;
};

class IObstacleDetectionController : public ISensorController
{
public:
    virtual int GetDistance() = 0;
    virtual ~IObstacleDetectionController() = 0;
    // This method sorts out invalid measurements and finds the median to remove sensor inaccuracies and more
    // Returns -1 if no objects are in range; not 255 which is the ultrasonic sensor standard return value
};

class ILaneTrackingController : public ISensorController
{
public:
    virtual std::vector<Vector2> TrackLanes() = 0;
    virtual ~ILaneTrackingController() = 0;
};

class ISoundSignalController : public ISensorController
{
public:
    virtual void SignalTurn(Direction direction) = 0;
    virtual ~ISoundSignalController() = 0;
};

class IColourSensorController : public ISensorController
{
public:
    virtual int DetectColour() = 0;
    // RETURNS INT? What does it return?
    // Sorts out bad or inconsistent measurements
    virtual ~IColourSensorController() = 0;
};

#endif //P5_LEGO_ISENSORCONTROLLER_H
