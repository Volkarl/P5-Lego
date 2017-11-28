//
// Created by karlz on 28/11/2017.
//

#include "IComponents.h"

#ifndef CAMERA_OBSTACLEDETECTIONCOMPONENT_H
#define CAMERA_OBSTACLEDETECTIONCOMPONENT_H


class ObstacleDetectionComponent : IObstacleDetectionComponent {
public:
    ObstacleDetectionComponent();
    ~ObstacleDetectionComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override ;
};


#endif //CAMERA_OBSTACLEDETECTIONCOMPONENT_H
