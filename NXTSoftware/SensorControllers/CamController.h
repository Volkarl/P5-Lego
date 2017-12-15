#ifndef CONNECTIVITY_CAMCONTROLLER_H
#define CONNECTIVITY_CAMCONTROLLER_H

#include "../../Shared/objects/camtools.h"
#include "../../Shared/Connectivity.h"
#include "Camera.h"
#include "Clock.h"


namespace ecrobot {
	class CamController {
		public:
			CamController(Clock* clock, Camera* cam);

			void Update();
			bool UpdateBuffer();
			CamBuffer GetBuffer() const;

			void Calibrate();

	private:
			Camera* m_Cam;
			Clock* m_Clock;
			CamBuffer m_camBuffer;


	};
};

#endif //CONNECTIVITY_CAMCONTROLLER_H
