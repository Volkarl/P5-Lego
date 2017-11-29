//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
#define P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H

#include "IComponents.h"

class BusStopDetectionComponent : IBusStopDetectionComponent {
public:
    BusStopDetectionComponent();
    ~BusStopDetectionComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override ;
};

#endif //P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
