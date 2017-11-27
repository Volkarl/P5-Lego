//
// Created by karlz on 27/11/2017.
//
#include <vector>

// Todo add constructors and write the functions!


class SteeringSequence
{
    // If I want a smooth turn, speed up or down and similar, I just split it into multiple steeringCommands
    public:
        SteeringCommand GetNewActiveCommand(int cmDrivenSinceLast){}

        enum class SenderComponent { StayWithinLane, ObstacleDetection, BusStopDetection, SpeedZoneDetection };
        SenderComponent Sender;
        int CmToNextSteeringCommand;

        std::vector<SteeringCommand> Commands;
        // Can contain multiple commands because, for instance:
        // halting at a bus stop requires a sequence of commands while just braking only requires one
};

class SteeringCommand
{
    public:
        int SpeedRotationsPerMin;
        int OverDistance;
        Turn turn;
};

enum class Direction { Left, Right };

class Turn
{
public:
    Direction TurnDirection;
    int TurnDegrees;
};