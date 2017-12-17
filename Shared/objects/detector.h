#ifndef CONNECTIVITY_DETECTOR_H
#define CONNECTIVITY_DETECTOR_H

#include "../Shared/objects/camtools.h"
#include "../Shared/objects/rectangle.h"
#include <math.h>

#define CAM_OFFSET_BUFFER_SIZE 5

namespace DirectionType {
	enum Type {
		None, Left, Right
	};
}

struct DrivingData {
	bool halt;

	int speed;
	int angle;
	bool calibrated;

	RGB color;
};

class Detector {
public:
	void MarkData(CamBuffer& cambuff);
	DirectionType::Type ShouldEvade() const;
	int GetDistance(const Rectangle_T& wall);
	int GetDistance(RectangleF& wall);
	int GetDistanceFromNearest();
	int GetAngle() const; 	
	
	int m_iOffsetBuffer[CAM_OFFSET_BUFFER_SIZE]; // Change to private?
	
	static const int c_CamWidth;
	static const int c_CamHeight;
	
	// Not pretty
	int speed;
	int angle;
	bool halt;
	RectangleF m_closestwall;

	DrivingData GetAdvisedDrivingData();
	
	Detector();
	~Detector();
private:
	bool m_DetectedWall;
	float m_fDirectionAngle;
	float m_fDirectionAngleOld;
};

#endif //CONNECTIVITY_DETECTOR_H
