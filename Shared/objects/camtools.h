#ifndef CAMTOOLS_H
#define CAMTOOLS_H

#include "../Shared/Connectivity.h"
#define CAM_RECT_MAX_BUFFER_SIZE 8

class CamBuffer {
public:
	Rectangle_T m_buffRects[CAM_RECT_MAX_BUFFER_SIZE];
	int m_iCount;
};

#endif // CAMTOOLS_H
