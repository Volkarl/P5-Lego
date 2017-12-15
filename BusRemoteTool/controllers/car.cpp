#include "car.h"

Car::Car() : m_Cam(&m_nxtComm), m_Motor(&m_nxtComm)
{	
	this->m_fDegree = 0.0f;
}

bool Car::Init()
{
	if (!this->m_nxtComm.Connect()) return false;
	return true;
}

void Car::Deinit()
{
	this->m_nxtComm.Disconnect();
}

void Car::Update()
{
	if (!this->m_nxtComm.isConnected()) {
		return;
	}

	this->m_Cam.UpdateSight();
	
	CamBuffer cambuff = this->m_Cam.GetBuffer();
	this->m_Cam.m_Detector.MarkData(cambuff);
	
	if(this->AllowDrive) {
		DrivingData data = this->m_Cam.m_Detector.GetAdvisedDrivingData();
		
		this->m_Motor.SetForce(data.speed);
		this->m_Motor.SetAngle(-(data.angle));
	} else {
		this->m_Motor.SetForce(0);
	}
	
	this->m_Motor.Send();
}

float Car::GetAngle() const
{
	return this->m_fDegree;
}
