#include "CamController.h"

using namespace ecrobot;

CamController::CamController(Clock* clock, Camera* cam)
{
	this->m_Clock = clock;
	this->m_Cam = cam;
}

void CamController::Calibrate()
{
	this->m_Cam->sendCommand('L'); // Line mode
	this->m_Clock->wait(10);
	this->m_Cam->sendCommand('X'); // Sort NONE
	this->m_Clock->wait(25);

	this->m_Cam->enableTracking(true);
}

void CamController::Update()
{
	this->m_Cam->update();
}

CamBuffer CamController::GetBuffer() const
{
	return this->m_camBuffer;
}

bool CamController::UpdateBuffer()
{
	Camera::Rectangle_T rec;

	int numOfObjects = this->m_Cam->getNumberOfObjects();

	if (numOfObjects < 1 && numOfObjects > 8) {
		this->m_camBuffer.m_iCount = 0;
		return false;
	}

	this->m_camBuffer.m_iCount = numOfObjects;

	for (int i = 0; i < this->m_camBuffer.m_iCount; i++) {
		this->m_Cam->getRectangle(i, &rec);

		m_camBuffer.m_buffRects[i].width = rec.width;
		m_camBuffer.m_buffRects[i].height = rec.height;
		m_camBuffer.m_buffRects[i].upperLeftX = rec.upperLeftX;
		m_camBuffer.m_buffRects[i].upperLeftY = rec.upperLeftY;
		m_camBuffer.m_buffRects[i].objColor = this->m_Cam->getObjectColor(i);
	}

	return true;
}
