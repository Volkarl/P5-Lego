//
// Created by karlz on 5/12/2017.
//

#ifndef P5_LEGO_NXTCAMLINETRACKINGCONTROLLER_H
#define P5_LEGO_NXTCAMLINETRACKINGCONTROLLER_H

#include "ISensorController.h"

class NxtCamLineTrackingController : public ILineTrackingController {
public:
    NxtCamLineTrackingController(Camera* cameraSensor, double CameraDistanceFromBusAxisY, double CameraDistanceFromBusAxisZ);
    std::vector<Vector2> TrackLanes() override;

    double CameraDistanceFromBusAxisY;
    // Elevation of the camera from the ground
    double CameraDistanceFromBusAxisZ;
    // How far the camera is ahead of the bus

private:
    Camera* CameraSensor;
};

#endif //P5_LEGO_NXTCAMLINETRACKINGCONTROLLER_H
