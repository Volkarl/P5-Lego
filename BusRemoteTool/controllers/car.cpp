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
	
	/*
	 * DIRECTION PART
	 */
	float dist = (float)this->m_Cam.m_Detector.GetDistanceFromNearest();
	
	const float maxdist = 60;
	if (dist > maxdist) dist = maxdist;
	
	//const float turnMultiplier = dist == -1 ? 0 : 20 + ((maxdist - dist) / 100 * 2);
	const float turnMultiplier = dist == -1 ? 5.5f : 5.5f + maxdist / dist;
	const float maxTurnAngle = 38.0f; // Much higher than it should be, but an attempt to make it not go back and forth
		
	DirectionType dirtomove = this->m_Cam.m_Detector.ShouldEvade();
	switch(dirtomove) {
		case DirectionType::None:
			if (this->m_fDegree > -3.5f && this->m_fDegree < 3.5f) {
				this->m_fDegree = 0;
			} else {
				if (this->m_fDegree > 0) this->m_fDegree -= 4.5;//turnMultiplier + 1.5f;
				else if (this->m_fDegree < 0) this->m_fDegree += 4.5;//turnMultiplier + 1.5f;
			}
			break;
			
		case DirectionType::Left:
			this->m_fDegree -= turnMultiplier;
			break;
			
		case DirectionType::Right:
			this->m_fDegree += turnMultiplier;
			break;
	}
	
	if (this->m_fDegree > maxTurnAngle) this->m_fDegree = maxTurnAngle;
	if (this->m_fDegree < -maxTurnAngle) this->m_fDegree = -maxTurnAngle;
	
	/*
	 * Driving
	 */
	
	const int speed = 37; // #ghettotemp
	
	if(this->AllowDrive) {	
		static float olddeg = 0;
		if (olddeg != this->GetAngle()) {
			olddeg = this->GetAngle();
			this->m_Motor.SetForce(20);
		} else {
			this->m_Motor.SetForce(speed);
		}
		this->m_Motor.SetAngle(-(this->GetAngle()));
	}
	
	
}

float Car::GetAngle() const
{
	return this->m_fDegree;
}
