//
// Created by karlz on 5/12/2017.
//

#ifndef P5_LEGO_STEERINGCONTROLLER_H
#define P5_LEGO_STEERINGCONTROLLER_H

#include "../SteeringTypes.h"
#include "Motor.h"

using namespace ecrobot;

class SteeringController {
public:
    SteeringController(Motor* motorForSpeed, Motor* motorForTurning);
    void SetSpeed(int rpm);
    void SetTurningAngle(TurnData turn);

    int CurrentSpeedRpm;
    TurnData CurrentTurningAngle;

private:
    Motor* MotorForSpeed;
    Motor* MotorForTurning;
};


#endif //P5_LEGO_STEERINGCONTROLLER_H
