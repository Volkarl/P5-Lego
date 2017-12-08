//
// Created by karlz on 29/11/2017.
//

#include "ObstacleDetectionComponent.h"

ObstacleDetectionComponent::ObstacleDetectionComponent(IUltrasonicSensorController* obstacleDetectionSensor) {
    this->ObstacleDetectionSensor = obstacleDetectionSensor;
}

SteeringSequence ObstacleDetectionComponent::CalculateSteering() {
    int dist = ObstacleDetectionSensor->GetDistance();
}
