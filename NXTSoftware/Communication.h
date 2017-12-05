#ifndef CONNECTIVITY_COMMUNICATION_H
#define CONNECTIVITY_COMMUNICATION_H

#include "Usb.h"
#include "Camera.h"
#include "Clock.h"
#include "ColorSensor.h"
#include "Driving.h"

namespace ecrobot {
	class Communication {
		public:
			Communication(Usb* usb, Camera* camera, ColorSensor* colorSensor, Driving* drive);
			void handle();

		private:
			void sendData(unsigned char *data);
			void sendCameraData(unsigned char *data);
			//void getCameraLineTest(unsigned char *data);
			//int  getCameraLineTestLoop();
			void getCameraHeatmapTest();
			void driveCmd(unsigned char *data);


			unsigned char data[64]; //Usb::MAX_USB_DATA_LEN];
			Usb* usb;
			Camera* camera;
			ColorSensor* color;
			Driving* drive;
			Clock clock;
	};
};



#endif //CONNECTIVITY_COMMUNICATION_H
