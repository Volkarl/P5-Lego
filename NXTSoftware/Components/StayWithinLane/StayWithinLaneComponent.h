//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_STAYWITHINLANECOMPONENT_H
#define P5_LEGO_STAYWITHINLANECOMPONENT_H

#include "../../SteeringTypes.h"

class StayWithinLaneComponent {
public:
    StayWithinLaneComponent();
    bool CalculateSteering(TurnData* turn);

private:
    //NxtCamLineTrackingController* LaneTrackingSensor;
};

#endif //P5_LEGO_STAYWITHINLANECOMPONENT_H
