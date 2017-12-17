#include "ObstacleDetectionComponent.h"

using namespace ecrobot;

ObstacleDetectionComponent::ObstacleDetectionComponent(UltrasonicSensorController* obstacleDetectionSensor) {
    this->ObstacleDetectionSensor = obstacleDetectionSensor;
    oldDistance = 50;
    // Just a start value to avoid the bus screwing up if it starts with an invalid measurement
}

bool ObstacleDetectionComponent::DetectObstacles() {
    int dist = ObstacleDetectionSensor->GetDistanceFast();
    if(dist == -1) dist = oldDistance;
    else oldDistance = dist;
    // If it's an invalid measurement, we use a previous valid measurement

    if(dist < 15) {

        //Direction direction = Left;
        //SteeringCommand halt[1] = SteeringCommand(0, 0, TurnData(direction, 0));
        //return &SteeringSequence(halt, 1);

        //sequence->SetData(SteeringCommand(0, 100, TurnData(Direction(Left), 0)));
        // If within 15 cm, it returns one steering command which stops the bus immediately
        return true;
    }
    else return false;
    // No obstacles
}
