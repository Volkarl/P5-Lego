//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H
#define P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H

#include "../../SensorControllers/UltrasonicSensorController.h"
#include "../../SteeringTypes.h"

namespace ecrobot{

class ObstacleDetectionComponent {
public:
    ObstacleDetectionComponent(UltrasonicSensorController* obstacleDetectionController);
    // It's coded to only take UltrasonicSensorController as input, which is sad.
    // It should be able to take any IObstacleDetectionController, because the logic works for either.
    // Removed because interface inheritance is dumb in c++.

    bool DetectObstacles();

private:
    UltrasonicSensorController* ObstacleDetectionSensor;
    int oldDistance;
};
};

#endif //P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H
