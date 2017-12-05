#include "car.h"

Car::Car() : m_Cam(&m_nxtComm), m_Motor(&m_nxtComm)
{	
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

	DirectionType dirtomove = this->m_Cam.m_Detector.ShouldEvade();
	switch(dirtomove) {
		case DirectionType::None:
			// geradeaus
			break;
			
		case DirectionType::Left:
			// todo
			break;
			
		case DirectionType::Right:
			// todo
			break;
	}
}
