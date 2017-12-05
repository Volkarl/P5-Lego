//
// Created by jakob on 03-Dec-17.
//

#ifndef CONNECTIVITY_DETECTOR_H
#define CONNECTIVITY_DETECTOR_H

#include "../Shared/objects/camtools.h"
#include "../Shared/objects/rectangle.h"

#define CAM_OFFSET_BUFFER_SIZE 10

enum class DirectionType {
	None, Left, Right
};

class Detector {

public:
	void MarkData(CamBuffer& cambuff);
	DirectionType ShouldEvade() const;
	int m_iOffsetBuffer[CAM_OFFSET_BUFFER_SIZE]; // Change to private?	
	
	const int c_CamWidth = 176;
	const int c_CamHeight = 88;
};

#endif //CONNECTIVITY_DETECTOR_H
