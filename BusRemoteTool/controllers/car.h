#ifndef CAR_H
#define CAR_H

#include "objects/cam.h"
#include "../objects/motor.h"
#include "../Shared/objects/detector.h"

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
	
	float m_fDegree;
	float GetAngle() const;
	
	bool AllowDrive; // ghetto as fuck, but for UI and prolly on the car for collision? dunno
};

#endif // CAR_H
