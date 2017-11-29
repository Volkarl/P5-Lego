#ifndef CAR_H
#define CAR_H

#include "../objects/cam.h"
#include "../objects/motor.h"

class Car {
public:
	NXTCommunication m_nxtComm;
	Cam m_Cam;
    Motor m_Motor;
	
	Car();
	
    bool Init();
	void Deinit();

    bool MotorForward(int force);
    bool MotorTurning(int deg);

    void Update();
};

#endif // CAR_H
