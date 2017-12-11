//
// Created by karlz on 29/11/2017.
//

#include "ObstacleDetectionComponent.h"

using namespace ecrobot;

ObstacleDetectionComponent::ObstacleDetectionComponent(UltrasonicSensorController* obstacleDetectionSensor) {
    this->ObstacleDetectionSensor = obstacleDetectionSensor;
}

bool ObstacleDetectionComponent::CalculateSteering(SteeringSequence* sequenceToOverride) {
    int dist = ObstacleDetectionSensor->GetDistance();
    if(dist < 50) {
        //Direction direction = Left;
        //SteeringCommand halt[1] = SteeringCommand(0, 0, TurnData(direction, 0));
        //return &SteeringSequence(halt, 1);

        delete sequence;
        sequence = new SteeringSequence(SteeringCommand(0, 0, TurnData(Direction(Left), 0)));
        // Returns one steering command which stops the bus immediately
        return true;
        //TODO: We sure this doesn't cause null problems because SteeringCommand and TurnData objects die after return?
    }
    else return false;
}
