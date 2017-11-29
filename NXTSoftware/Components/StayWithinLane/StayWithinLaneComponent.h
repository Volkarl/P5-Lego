//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_STAYWITHINLANECOMPONENT_H
#define P5_LEGO_STAYWITHINLANECOMPONENT_H

#include "../IComponents.h"

class StayWithinLaneComponent : IStayWithinLaneComponent {
public:
    StayWithinLaneComponent();
    ~StayWithinLaneComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override;
};

#endif //P5_LEGO_STAYWITHINLANECOMPONENT_H
