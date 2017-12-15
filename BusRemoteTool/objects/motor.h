#ifndef MOTOR_H
#define MOTOR_H

#include "nxtcommunication.h"

class Motor {
private:
	NXTCommunication* m_nxtComm;
	int driveSpeed;
	int driveAngle;

public:
	Motor(NXTCommunication* comm);

	bool SetForce(int force);
	bool SetAngle(int angle);
	
	int GetSpeed();
	
	bool Send();
};

#endif // MOTOR_H
