#include "detector.h"

void Detector::MarkData(CamBuffer& cambuff) {
	int carwidth = 80; // TODO: Modify value

	RectangleF collision;
	collision.x = (float)(this->c_CamWidth / 2 - carwidth / 2);
	collision.y = 0.0f;
	collision.w = (float)carwidth;
	collision.h = (float)this->c_CamHeight;

	RectangleF closestwall;
	bool evac = false;
	for (int i = 0; i < cambuff.m_iCount && cambuff.m_iCount < 8; i++) {
		Rectangle_T& rawwall = cambuff.m_buffRects[i];
		RectangleF wall(rawwall);

		if (collision.Intersects(wall)) {
			rawwall.objColor = 1; // TODO: add a field or alike to mark collision instead
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
