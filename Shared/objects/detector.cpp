#include "detector.h"
#include <string.h>

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
		curoffset = (this->c_CamWidth / 2) - (this->m_closestwall.x + this->m_closestwall.w / 2);
	}

	for (int i = 0; i < CAM_OFFSET_BUFFER_SIZE - 1; i++) {
		this->m_iOffsetBuffer[i] = this->m_iOffsetBuffer[i + 1];
	}

	this->m_iOffsetBuffer[CAM_OFFSET_BUFFER_SIZE - 1] = curoffset;
}

DirectionType Detector::ShouldEvade() const {
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

Detector::Detector() : speed(0), angle(0), halt(false) {
	memset(this->m_iOffsetBuffer, 0, CAM_OFFSET_BUFFER_SIZE * sizeof(int));
}

Detector::~Detector() {
}

int Detector::GetDistance(const Rectangle_T& wall)
{
	if(wall.upperLeftX <= this->c_CamWidth / 2 && wall.upperLeftX + wall.width >= this->c_CamWidth / 2) {
		return this->c_CamHeight - (wall.upperLeftY + wall.height);
	} else {
		const int x2 = this->c_CamWidth / 2 > wall.upperLeftX + wall.width ? wall.upperLeftX + wall.width : wall.upperLeftX;
	
		return abs(sqrt(
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
