#include "motor.h"

Motor::Motor(NXTCommunication* comm) : m_nxtComm(comm) {
	this->driveAngle = 0;
	this->driveSpeed = 0;
}

bool Motor::SetForce(int force)
{
	if (force < 0 && force > 100)
		return false;
	
	this->driveSpeed = force;
	return true;
}

bool Motor::SetAngle(int angle)
{
	if (angle < -90 && angle > 90)
		return false;
	
	this->driveAngle = angle;
	return true;
}

bool Motor::Send()
{
	char buff[MAX_DATA_LEN];
	buff[0] = PACKET_DRIVE;
	buff[1] = this->driveSpeed;
	buff[2] = this->driveAngle;
	buff[3] = '\0';
	
	return this->m_nxtComm->SendArray(buff, 4);
}
