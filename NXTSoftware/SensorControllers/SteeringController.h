//
// Created by karlz on 5/12/2017.
//

#ifndef P5_LEGO_STEERINGCONTROLLER_H
#define P5_LEGO_STEERINGCONTROLLER_H

#include "ISensorController.h"
#include "../SteeringTypes.h"

class SteeringController : public ISteeringController {
public:
    SteeringController(Motor* motorForSpeed, Motor* motorForTurning);
    void SetSpeed(int rpm) override;
    void SetTurningAngle(TurnData turn);

    int CurrentSpeedRpm;
    TurnData CurrentTurningAngle;

private:
    Motor* MotorForSpeed;
    Motor* MotorForTurning;
};


#endif //P5_LEGO_STEERINGCONTROLLER_H
