#ifndef CAR_H
#define CAR_H

#include "objects/cam.h"
#include "../objects/motor.h"
#include "../Shared/objects/detector.h"
#include "../Shared/Connectivity.h"

class Car {
public:
	NXTCommunication m_nxtComm;
	CamBuffer m_camBuffer;
	Cam m_Cam;
    Motor m_Motor;
	
	Car();
	
    bool Init();
	void Deinit();

    bool MotorForward(int force);
    bool MotorTurning(int deg);

    void Update();
	
	float GetAngle() const;
	
	bool AllowDrive;
	
	bool SetController(Controller::Source);
};

#endif // CAR_H
