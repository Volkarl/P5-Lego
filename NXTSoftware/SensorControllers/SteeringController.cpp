//
// Created by karlz on 5/12/2017.
//

#include "SteeringController.h"

SteeringController::SteeringController(Motor* motorForSpeed, Motor* motorForTurning) {
    this->MotorForSpeed = motorForSpeed;
    this->MotorForTurning = motorForTurning;
}

void SteeringController::Calibrate() {

}

void SteeringController::SetSpeed(int rpm) {

}

void SteeringController::SetTurningAngle(TurnData turn) {

}

