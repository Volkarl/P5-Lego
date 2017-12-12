//
// Created by karlz on 5/12/2017.
//

#include "NxtCamLineTrackingController.h"

using namespace ecrobot;

NxtCamLineTrackingController::NxtCamLineTrackingController(Camera* cameraSensor, double cameraDistanceFromBusAxisY,
                                                           double cameraDistanceFromBusAxisZ) {
    this->CameraSensor = cameraSensor;
    this->CameraDistanceFromBusAxisY = cameraDistanceFromBusAxisY;
    this->CameraDistanceFromBusAxisZ = cameraDistanceFromBusAxisZ;
}

void NxtCamLineTrackingController::Calibrate() {

}

void NxtCamLineTrackingController::TrackLines(int *lanePointArray, int *size) {

}
