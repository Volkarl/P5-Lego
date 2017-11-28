//
// Created by karlz on 28/11/2017.
//

#include "IComponents.h"

#ifndef CAMERA_BUSSTOPDETECTIONCOMPONENT_H
#define CAMERA_BUSSTOPDETECTIONCOMPONENT_H


class BusStopDetectionComponent : IBusStopDetectionComponent {
public:
    BusStopDetectionComponent();
    ~BusStopDetectionComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override ;
};


#endif //CAMERA_BUSSTOPDETECTIONCOMPONENT_H
