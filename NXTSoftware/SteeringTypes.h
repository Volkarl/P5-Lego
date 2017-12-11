//
// Created by karlz on 29/11/2017.
//

// Todo add constructors and write the functions!

#ifndef P5_LEGO_STEERINGTYPES_H
#define P5_LEGO_STEERINGTYPES_H

namespace ecrobot{

enum Direction { Left, Right };
// It's not happy about enum class for some reason


class TurnData
{
public:
    TurnData(Direction direction, int degrees){
        TurnDirection = direction;
        TurnDegrees = degrees;
    }
    Direction TurnDirection;
    int TurnDegrees;
};

class SteeringCommand
{
public:
    SteeringCommand(int rpm, int distance, TurnData turn){
        SpeedRotationsPerMin = rpm;
        OverDistance = distance;
        Turn = &turn;
    }
    int SpeedRotationsPerMin;
    int OverDistance;
    TurnData* Turn;
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
    SteeringSequence(SteeringCommand commands[], int items){
        Commands = commands;
        Items = items;
        CmToNextSteeringCommand = 0;
        CommandIndex = 0;
    }

    SteeringSequence(SteeringCommand command){
        Commands = &command;
        Items = 1;
        CmToNextSteeringCommand = 0;
        CommandIndex = 0;
    }

    SteeringCommand* GetNewActiveCommand(int cmDrivenSinceLast){
        if(cmDrivenSinceLast >= CmToNextSteeringCommand){
            CommandIndex++;
        }

        if(CommandIndex >= Items) return 0;
        else return &Commands[CommandIndex];
    }

    int CmToNextSteeringCommand, CommandIndex;

    SteeringCommand* Commands;
    // Can contain multiple commands because, for instance:
    // halting at a bus stop requires a sequence of commands while just braking only requires one
    int Items;
};
};

#endif //P5_LEGO_STEERINGTYPES_H
