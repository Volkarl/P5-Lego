#ifndef P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H
#define P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H

#include "../../SensorControllers/UltrasonicSensorController.h"
#include "../../SteeringTypes.h"

namespace ecrobot{

class ObstacleDetectionComponent {
public:
    ObstacleDetectionComponent(UltrasonicSensorController* UltrasonicSensorController);
    // It's coded to only take UltrasonicSensorController as input, which is sad.
    // It should be able to take any IObstacleDetectionController, because the logic works for either.
    // Removed because interface inheritance is dumb in c++.

    bool DetectObstacles(); // Terrible name, this sounds like a Update() function
	int GetDistance() const;

private:
    UltrasonicSensorController* m_UltrasonicSensorController;
    int oldDistance;
};
};

#endif //P5_LEGO_OBSTACLEDETECTIONCOMPONENT_H
