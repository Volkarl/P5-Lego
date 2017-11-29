#ifndef CAM_H
#define CAM_H

#include "nxtcommunication.h"
#include <vector>
#include "rectangle.h"

#define CAM_OFFSET_BUFFER_SIZE 10

enum class DirectionType {
	None, Left, Right
};

class Cam {
private:
	bool m_bConnected;
	NXTCommunication& m_nxtComm;
	
	int m_iOffsetBuffer[CAM_OFFSET_BUFFER_SIZE];

public:
	const int c_CamWidth = 176;
	const int c_CamHeight = 88;
	
	Cam(NXTCommunication& comm);
	std::vector<Rectangle_T> m_lstObjects;

	bool IsConnected();
	bool Init();

	bool UpdateSight();

	DirectionType ShouldEvade() const;
};

#endif // CAM_H
