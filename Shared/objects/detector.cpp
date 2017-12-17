#include "detector.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

const int Detector::c_CamWidth = 176;
const int Detector::c_CamHeight = 144;

Detector::Detector() : speed(0), angle(0), halt(false), m_DetectedWall(false), m_fDirectionAngle(0), m_fDirectionAngleOld(0) {
	memset(this->m_iOffsetBuffer, 0, CAM_OFFSET_BUFFER_SIZE * sizeof(int));
}

Detector::~Detector() {}

void Detector::MarkData(CamBuffer& cambuff) {
	int carwidth = 105; // TODO: Modify value

	RectangleF collision;
	collision.x = (float)(this->c_CamWidth / 2 - carwidth / 2);
	collision.y = 30.0f; //(float)this->c_CamHeight - 15.0f;
	collision.w = (float)carwidth;
	collision.h = (float)this->c_CamHeight - collision.y;

	this->m_DetectedWall = false;
	memset(&this->m_closestwall, 0, sizeof(RectangleF)); // reset struct
	
	for (int i = 0; i < cambuff.m_iCount && i < 8; i++)
	{
		Rectangle_T& rawwall = cambuff.m_buffRects[i];
		RectangleF wall(rawwall);

		if (collision.Intersects(wall)) {
			rawwall.collision = true;
			rawwall.dist = this->GetDistance(rawwall);

			if (this->m_closestwall.y + this->m_closestwall.h < wall.y + wall.h) {
				wall.dist = rawwall.dist;
				this->m_closestwall = wall;
				this->m_DetectedWall = true;
			}
		} else {
			rawwall.collision = false;
		}
	}

	int curoffset = 0;
	if (this->m_DetectedWall) {
		curoffset = (int)((this->c_CamWidth / 2) - (this->m_closestwall.x + this->m_closestwall.w / 2));
	}

	for (int i = 0; i < CAM_OFFSET_BUFFER_SIZE - 1; i++) {
		this->m_iOffsetBuffer[i] = this->m_iOffsetBuffer[i + 1];
	}

	this->m_iOffsetBuffer[CAM_OFFSET_BUFFER_SIZE - 1] = curoffset;
}

DrivingData Detector::GetAdvisedDrivingData() {
	DrivingData data;
	memset(&data, 0, sizeof(DrivingData));

	/*
	 * DIRECTION PART
	 */
	float dist = (float)this->GetDistanceFromNearest();

	const float maxdist = 60;
	if (dist > maxdist) dist = maxdist;

	//const float turnMultiplier = dist == -1 ? 0 : 20 + ((maxdist - dist) / 100 * 2);
	const float turnMultiplier = dist == -1 ? 5.5f : 5.5f + maxdist / dist;
	const float maxTurnAngle = 38.0f; // Much higher than it should be, but an attempt to make it not go back and forth

	DirectionType::Type dirtomove = this->ShouldEvade();
	switch(dirtomove) {
		case DirectionType::None:
			if (this->m_fDirectionAngle > -3.5f && this->m_fDirectionAngle < 3.5f) {
				this->m_fDirectionAngle = 0;
			} else {
				if (this->m_fDirectionAngle > 0) this->m_fDirectionAngle -= 4.5;//turnMultiplier + 1.5f;
				else if (this->m_fDirectionAngle < 0) this->m_fDirectionAngle += 4.5;//turnMultiplier + 1.5f;
			}
			break;

		case DirectionType::Left:
			this->m_fDirectionAngle -= turnMultiplier;
			break;

		case DirectionType::Right:
			this->m_fDirectionAngle += turnMultiplier;
			break;
	}

	if (this->m_fDirectionAngle > maxTurnAngle) this->m_fDirectionAngle = maxTurnAngle;
	if (this->m_fDirectionAngle < -maxTurnAngle) this->m_fDirectionAngle = -maxTurnAngle;


	const int speed = 37; // TODO: #ghettotemp, DEFINE CONFIG?

	if (this->m_fDirectionAngleOld != this->m_fDirectionAngle) {
		this->m_fDirectionAngleOld = this->m_fDirectionAngle;
		data.speed = 20;
	} else {
		data.speed = speed;
	}

	data.angle = (int)this->m_fDirectionAngle;

	return data;
}

DirectionType::Type Detector::ShouldEvade() const {
	int balancedoffset = 0;
	for (int i = 0; i < CAM_OFFSET_BUFFER_SIZE; i++) {
		balancedoffset += this->m_iOffsetBuffer[i];
	}
	balancedoffset /= CAM_OFFSET_BUFFER_SIZE;

	if (balancedoffset == 0) {
		return DirectionType::None;
	} else if (balancedoffset < 0) {
		return DirectionType::Left;
	} else {
		return DirectionType::Right;
	}
}

int Detector::GetDistance(const Rectangle_T& wall)
{
	if(wall.upperLeftX <= this->c_CamWidth / 2 && wall.upperLeftX + wall.width >= this->c_CamWidth / 2) {
		return this->c_CamHeight - (wall.upperLeftY + wall.height);
	} else {
		const int x2 = this->c_CamWidth / 2 > wall.upperLeftX + wall.width ? wall.upperLeftX + wall.width : wall.upperLeftX;
	
		return (int)abs(sqrt(
			pow( (x2 - this->c_CamWidth / 2), 2) + 
			pow( (wall.upperLeftY + wall.height) - this->c_CamHeight, 2)
		));
	}
}

int Detector::GetDistance(RectangleF& wall)
{
	if(wall.x <= this->c_CamWidth / 2 && wall.x + wall.w >= this->c_CamWidth / 2) {
		return this->c_CamHeight - (wall.y + wall.h);
	} else {
		const int x2 = this->c_CamWidth / 2 > wall.x + wall.w ? wall.x + wall.w : wall.x;
		
		return abs(sqrt(
			pow( (x2 - this->c_CamWidth / 2), 2) + 
			pow( (wall.y + wall.h) - this->c_CamHeight, 2)
		));
	}
}

int Detector::GetDistanceFromNearest()
{
	if(this->m_DetectedWall)
		return this->GetDistance(this->m_closestwall);
	else
		return -1;
}

int Detector::GetAngle() const
{
	return this->m_fDirectionAngle;
}
