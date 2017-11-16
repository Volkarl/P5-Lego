#ifndef CONNECTIVITY_COMMUNICATION_H
#define CONNECTIVITY_COMMUNICATION_H

#include "Usb.h"
#include "Camera.h"
#include "Clock.h"

namespace ecrobot {
	class Communication {
		public:
			Communication(Usb* usb, Camera* camera);
			void handle();

		private:
			void sendCameraData(unsigned char *data);
			void getCameraTest();
			void getCameraLineTest(unsigned char *data);
			int getCameraLineTestLoop(); // TODO
			void Communication::getCameraHeatmapTest();


		unsigned char data[64]; //Usb::MAX_USB_DATA_LEN];
			Usb* usb;
			Camera* camera;
			Clock clock;
	};
};



#endif //CONNECTIVITY_COMMUNICATION_H
