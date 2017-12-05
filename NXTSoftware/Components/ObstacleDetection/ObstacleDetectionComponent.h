//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H
#define P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H

#include "../IComponents.h"
#include "../../SensorControllers/ISensorController.h"

class ObstacleDetectionComponent : IObstacleDetectionComponent {
public:
    ObstacleDetectionComponent(IObstacleDetectionController obstacleDetectionSensor);
    ~ObstacleDetectionComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override ;

private:
    IObstacleDetectionController ObstacleDetectionSensor;
};

#endif //P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H
