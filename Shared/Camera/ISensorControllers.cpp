//
// Created by karlz on 27/11/2017.
//
#include "CamCalcTypes.h"
#include "SteeringTypes.cpp"
#include <vector>



/////////////// Controller Class Interfaces ///////////////
class ISensorController
{
public:
    virtual void Calibrate() = 0;
    virtual ~ISensorController() = default;
};

class IMotorController : public ISensorController
{
    // The inherited calibrate-method handles things like centering the wheels of the bus during setup
public:
    virtual void SetSpeed(int rpm) = 0;
    virtual void SetTurningAngle() = 0;
    virtual IMotorController() = default;

    virtual ~IMotorController() override = default;

    int CurrentSpeedRpm;
    Turn CurrentTurningAngle;
};

class IUltrasonicSensorController : public ISensorController
{
public:
    virtual int GetDistance() = 0;
    virtual ~IUltrasonicSensorController() = default;
    // This method sorts out invalid measurements and finds the median to remove sensor inaccuracies and more
    // Returns -1 if no objects are in range; not 255 which is the ultrasonic sensor standard return value
};

class INxtCamLineTrackingController : public ISensorController
{
public:
    virtual std::vector<Vector2> TrackLanes() = 0;
    virtual ~INxtCamLineTrackingController() = default;

    double CameraDistanceFromBusAxisY;
    // Elevation of the camera from the ground
    double CameraDistanceFromBusAxisZ;
    // How far the camera is ahead of the bus

//NEEDS CAMERA HEIGHT IN ITS CONSTRUCTOR
};

class ISoundSignalController : public ISensorController
{
public:
    virtual void SignalTurn(Direction direction) = 0;
    virtual ~ISoundSignalController() = default;
};

class IColourSensorController : public ISensorController
{
public:
    virtual int DetectColour() = 0;
    // RETURNS INT? What does it return?
    // Sorts out bad or inconsistent measurements
    virtual ~IColourSensorController() = default;
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