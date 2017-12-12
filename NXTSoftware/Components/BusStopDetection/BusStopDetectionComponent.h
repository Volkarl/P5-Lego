//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
#define P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H

#include "../../SensorControllers/ColourSensorController.h"
#include "../../SteeringTypes.h"

class BusStopDetectionComponent {
public:
    BusStopDetectionComponent(ColourSensorController* colorSensor);
    bool CalculateSteering(SteeringSequence* sequence);

private:
    ColourSensorController* ColourSensor;
};

#endif //P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
