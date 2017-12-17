#ifndef CONNECTIVITY_COMMUNICATION_H
#define CONNECTIVITY_COMMUNICATION_H

#include "Usb.h"
#include "SensorControllers/CamController.h"
#include "Clock.h"
#include "ColorSensor.h"

#include "Driving.h"
#include "../Shared/Connectivity.h"
#include "../Shared/Tools.h"
#include "../Shared/objects/camtools.h"
#include "Driving.h"

#include "SensorControllers/DisplayController.h"


namespace ecrobot {
	class Communication {
		public:
			Communication(Usb* usb, CamController* camera, ColorSensor* colorSensor, Driving* drive, DisplayController* lcd);
			void handle();
			Controller::Source m_Controller;

		private:
			void sendData(unsigned char* data);
			//void sendCameraData(unsigned char *data);
			//void getCameraLineTest(unsigned char *data);
			//int  getCameraLineTestLoop();
			void getCameraHeatmapTest();
			void driveCmd(unsigned char* data);
			void setMode(unsigned char* data);


			unsigned char data[64]; //Usb::MAX_USB_DATA_LEN];
			Usb* m_Usb;
			CamController* m_Camera;
			ColorSensor* m_Color;
			Driving* m_Drive;

			DisplayController* m_Lcd;
			//Clock clock;

	};
};



#endif //CONNECTIVITY_COMMUNICATION_H
