//
// Created by karlz on 29/11/2017.
//

#include "StayWithinLaneComponent.h"

StayWithinLaneComponent::StayWithinLaneComponent(NxtCamLineTrackingController *laneTrackingSensor) {
    this->LaneTrackingSensor = laneTrackingSensor;
}

bool StayWithinLaneComponent::CalculateSteering(TurnData *turn){
    return false;
}
