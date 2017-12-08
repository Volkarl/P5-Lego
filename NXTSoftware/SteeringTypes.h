//
// Created by karlz on 29/11/2017.
//

// Todo add constructors and write the functions!

#ifndef P5_LEGO_STEERINGTYPES_H
#define P5_LEGO_STEERINGTYPES_H


enum Direction { Left, Right };
// It's not happy about enum class for some reason


class TurnData
{
public:
    Direction TurnDirection;
    int TurnDegrees;
};

class SteeringCommand
{
public:
    int SpeedRotationsPerMin;
    int OverDistance;
    TurnData Turn;
};


class SpeedZone
{
public:
    int SpeedRpm;
};

class Coordinates2d{
public:
    Coordinates2d(double horizontal, double vertical){
        x = horizontal;
        y = vertical;
    }
    double x, y;
};

class SteeringSequence
{
    // If I want a smooth turn, speed up or down and similar, I just split it into multiple steeringCommands
public:
    SteeringCommand GetNewActiveCommand(int cmDrivenSinceLast){
        if(cmDrivenSinceLast >= CmToNextSteeringCommand)
            commandIndex++;
//        if(commandIndex >= maximumCommands)
        return Commands[commandIndex];
    }

    enum SenderComponent { StayWithinLane, ObstacleDetection, BusStopDetection, SpeedZoneDetection };
    SenderComponent Sender;
    int CmToNextSteeringCommand, commandIndex, maximumCommands;
    //todo initialize these

    SteeringCommand Commands[10];
    // Can contain multiple commands because, for instance:
    // halting at a bus stop requires a sequence of commands while just braking only requires one
};

#endif //P5_LEGO_STEERINGTYPES_H
