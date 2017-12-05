//
// Created by karlz on 29/11/2017.
//

#include "ObstacleDetectionComponent.h"

ObstacleDetectionComponent::ObstacleDetectionComponent(IObstacleDetectionController obstacleDetectionSensor) {
    ObstacleDetectionSensor = obstacleDetectionSensor;
}

ObstacleDetectionComponent::~ObstacleDetectionComponent() = default;

SteeringSequence ObstacleDetectionComponent::CalculateSteering() {
    return SteeringSequence();
}

void ObstacleDetectionComponent::CalibrateSensors() {

}