//
// Created by karlz on 5/12/2017.
//

#ifndef P5_LEGO_NXTCAMLINETRACKINGCONTROLLER_H
#define P5_LEGO_NXTCAMLINETRACKINGCONTROLLER_H

#include "Camera.h"

using namespace ecrobot;

class NxtCamLineTrackingController {
public:
    NxtCamLineTrackingController(Camera* cameraSensor, double CameraDistanceFromBusAxisY,
                                 double CameraDistanceFromBusAxisZ);
    void TrackLines(int* lanePointArray, int* size);
    void Calibrate();

    double CameraDistanceFromBusAxisY;
    // Elevation of the camera from the ground
    double CameraDistanceFromBusAxisZ;
    // How far the camera is ahead of the bus

private:
    Camera* CameraSensor;
};

#endif //P5_LEGO_NXTCAMLINETRACKINGCONTROLLER_H
