//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_STAYWITHINLANECOMPONENT_H
#define P5_LEGO_STAYWITHINLANECOMPONENT_H

#include "../IComponents.h"
#include "../../SensorControllers/ISensorController.h"

class StayWithinLaneComponent : IStayWithinLaneComponent {
public:
    StayWithinLaneComponent(ILaneTrackingController laneTrackingSensor);
    ~StayWithinLaneComponent() override;

    SteeringSequence CalculateSteering() override;
    void CalibrateSensors() override;

private:
    ILaneTrackingController LaneTrackingSensor;
};

#endif //P5_LEGO_STAYWITHINLANECOMPONENT_H
