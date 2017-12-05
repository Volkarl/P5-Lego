//
// Created by karlz on 5/12/2017.
//

#ifndef P5_LEGO_NXTCAMLINETRACKINGCONTROLLER_H
#define P5_LEGO_NXTCAMLINETRACKINGCONTROLLER_H

#include "ISensorController.h"

class NxtCamLineTrackingController : public ILineTrackingController {
public:
    NxtCamLineTrackingController(double CameraDistanceFromBusAxisY, double CameraDistanceFromBusAxisZ);
    ~NxtCamLineTrackingController() override;
    std::vector<Vector2> TrackLanes() override;

    double CameraDistanceFromBusAxisY;
    // Elevation of the camera from the ground
    double CameraDistanceFromBusAxisZ;
    // How far the camera is ahead of the bus
};

#endif //P5_LEGO_NXTCAMLINETRACKINGCONTROLLER_H
