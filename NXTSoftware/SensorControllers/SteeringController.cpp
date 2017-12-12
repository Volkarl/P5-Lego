//
// Created by karlz on 5/12/2017.
//

#include "SteeringController.h"

using namespace ecrobot;

SteeringController::SteeringController(Motor *motorForSpeed, Motor *motorForTurning){
    this->MotorForSpeed = motorForSpeed;
    this->MotorForTurning = motorForTurning;
}

void SteeringController::SetSpeed(int rpm) {

}

void SteeringController::SetTurningAngle(TurnData turn) {

}

