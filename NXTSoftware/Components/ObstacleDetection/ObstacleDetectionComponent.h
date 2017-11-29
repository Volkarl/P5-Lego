//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H
#define P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H

#include "../IComponents.h"

class ObstacleDetectionComponent : IObstacleDetectionComponent {
public:
    ObstacleDetectionComponent();
    ~ObstacleDetectionComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override ;
};

#endif //P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H
