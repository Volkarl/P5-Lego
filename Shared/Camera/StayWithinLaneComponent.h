//
// Created by karlz on 28/11/2017.
//

#include "IComponents.h"

#ifndef CAMERA_STAYWITHINLANECOMPONENT_H
#define CAMERA_STAYWITHINLANECOMPONENT_H


class StayWithinLaneComponent : IStayWithinLaneComponent {
public:
    StayWithinLaneComponent();
    ~StayWithinLaneComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override;
};


#endif //CAMERA_STAYWITHINLANECOMPONENT_H
