//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
#define P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H

#include "../IComponents.h"
#include "../../SensorControllers/ISensorController.h"

class BusStopDetectionComponent : IBusStopDetectionComponent {
public:
    BusStopDetectionComponent(IColourSensorController colorSensor);
    ~BusStopDetectionComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override ;

private:
    IColourSensorController ColourSensor;
};

#endif //P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
