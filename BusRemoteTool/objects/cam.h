#ifndef CAM_H
#define CAM_H

#include <vector>

#include "objects/nxtcommunication.h"
#include "../Shared/objects/detector.h"
#include "../Shared/objects/camtools.h"


class NXTCommunication;

class Cam {
private:
	bool m_bConnected;
	NXTCommunication* m_nxtComm;
	CamBuffer m_camBuffer;

public:
	const int c_CamWidth = 176;
	const int c_CamHeight = 88;
	Cam(NXTCommunication* comm);
	
	Detector m_Detector; // Was private	

	bool IsConnected();
	bool Init();

	bool UpdateSight();

	CamBuffer GetBuffer() const;
	
	DirectionType::Type ShouldEvade();
};

#endif // CAM_H
