#include "cam.h"
#include <QDebug>

Cam::Cam(NXTCommunication* comm) : m_nxtComm(comm), m_camBuffer() {
	for (int i = 0; i < CAM_OFFSET_BUFFER_SIZE; i++) {
 		this->m_Detector.m_iOffsetBuffer[i] = 0;
	}
}

CamBuffer Cam::GetBuffer() const {
	return this->m_camBuffer;
}

bool Cam::UpdateSight() {
	char buff[MAX_DATA_LEN];
	int speed, angle, halt;
	
	this->m_nxtComm->SendPacket(PACKET_POLL); // Previously PACKET_CAM
	this->m_nxtComm->Receive(buff, MAX_DATA_LEN);

	int offset = 0;
	CamBuffer newbuff;

	newbuff.m_iCount = (int) buff[offset++];

	// If either ending, or not within 0 - 8 bounds
	if (newbuff.m_iCount == PACKET_END || newbuff.m_iCount > 8 || newbuff.m_iCount < 0) {
		newbuff.m_iCount = 0; // TODO
		return false;
	}

	for (int i = 0; i < newbuff.m_iCount; i++) {
		Rectangle_T &rect = newbuff.m_buffRects[i];

		rect.width = (unsigned char) buff[offset++];
		rect.height = (unsigned char) buff[offset++];
		rect.upperLeftX = (unsigned char) buff[offset++];
		rect.upperLeftY = (unsigned char) buff[offset++];
		rect.objColor = (unsigned char) buff[offset++];
	}

	// Validate and vars
	this->m_Detector.speed = (unsigned char) buff[offset++];
	this->m_Detector.angle = (int) buff[offset++];
	this->m_Detector.halt = ((unsigned char) buff[offset++]) == 1;
	
	/*
	qDebug()<< "R: " << QString::number((unsigned char) buff[offset++])
			<< "G: " << QString::number((unsigned char) buff[offset++])
			<< "B: " << QString::number((unsigned char) buff[offset++]);
	*/
	
	this->m_camBuffer = newbuff;
	
	return true;
}
