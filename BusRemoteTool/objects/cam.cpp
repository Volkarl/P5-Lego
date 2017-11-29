#include "cam.h"

#include <QDebug>

Cam::Cam(NXTCommunication &comm) : m_nxtComm(comm) {
	for (int i = 0; i < CAM_OFFSET_BUFFER_SIZE; i++) {
		this->m_iOffsetBuffer[i] = 0;
	}
}

bool Cam::UpdateSight() {
	char buff[MAX_DATA_LEN];
	this->m_nxtComm.SendPacket(PACKET_POLL); // Previously PACKET_CAM
	this->m_nxtComm.Receive(buff, MAX_DATA_LEN);
	
	int offset = 0;
	int count = (int)buff[offset++];
	
	// If either ending, or not within 0 - 8 bounds
	if (count == PACKET_END || count > 8 || count < 0) {
		return false;
	}

	this->m_lstObjects.clear();
	for (int i = 0; i < count; i++) {
		Rectangle_T rect;

		rect.width = (unsigned char)buff[offset++];
		rect.height = (unsigned char)buff[offset++];
		rect.upperLeftX = (unsigned char)buff[offset++];
		rect.upperLeftY = (unsigned char)buff[offset++];
		rect.objColor = (unsigned char)buff[offset++];

		rect.objColor = 0; // TODO: temp for marking collision, problem is modifying Rectangle_T
		
		this->m_lstObjects.push_back(rect);
	}
	
	int carwidth = 80; // TODO: Modify value
	
	RectangleF collision;
	collision.x = (float)(this->c_CamWidth / 2 - carwidth / 2);
	collision.y = 0.0f;
	collision.w = (float)carwidth;
	collision.h = (float)this->c_CamHeight;
	
	RectangleF closestwall;
	bool evac = false;
	for (Rectangle_T& rawwall : this->m_lstObjects) {
		RectangleF wall(rawwall);
		
		if (collision.Intersects(wall)) {
			rawwall.objColor = 1; // todo add a field or so to mark this
			evac = true;
			
			if (closestwall.y + closestwall.h < wall.y + wall.h) {
				closestwall = wall;
			}
		}
	}
	
	int curoffset = 0;
	if (evac) {
		curoffset = (this->c_CamWidth / 2) - (closestwall.x + closestwall.w / 2);
	}
	
	for (int i = 0; i < CAM_OFFSET_BUFFER_SIZE - 1; i++) {
		this->m_iOffsetBuffer[i] = this->m_iOffsetBuffer[i + 1];
	}
	
	this->m_iOffsetBuffer[CAM_OFFSET_BUFFER_SIZE - 1] = curoffset;
}

DirectionType Cam::ShouldEvade() const {
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
