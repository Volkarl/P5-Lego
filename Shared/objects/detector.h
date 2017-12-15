//
// Created by jakob on 03-Dec-17.
//

#ifndef CONNECTIVITY_DETECTOR_H
#define CONNECTIVITY_DETECTOR_H

#include "../Shared/objects/camtools.h"
#include "../Shared/objects/rectangle.h"
#include <math.h>

#define CAM_OFFSET_BUFFER_SIZE 5

enum class DirectionType {
	None, Left, Right
};

class Detector {

public:
	void MarkData(CamBuffer& cambuff);
	DirectionType ShouldEvade() const;
	int GetDistance(const Rectangle_T& wall);
	int GetDistance(RectangleF& wall);
	int GetDistanceFromNearest();
	
	int m_iOffsetBuffer[CAM_OFFSET_BUFFER_SIZE] = {}; // Change to private?	
	
	const int c_CamWidth = 176;
	const int c_CamHeight = 144; // was 88?
	
	// Not pretty
	int speed;
	int angle;
	bool halt;
	RectangleF m_closestwall;
	
	Detector();
	~Detector();
private:
	bool m_DetectedWall;
};

#endif //CONNECTIVITY_DETECTOR_H
