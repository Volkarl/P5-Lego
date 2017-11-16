#include <string.h>
#include "stdio.h"
#include "../Shared/Tools.h"
#include "../Shared/Connectivity.h"

// ECRobot++ API
#include "Usb.h"
#include "Lcd.h"
#include "Nxt.h"
#include "Motor.h"
#include "Clock.h"
#include "Camera.h"
#include "SonarSensor.h"

using namespace ecrobot;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* OSEK DECLARATIONS */
DeclareCounter(SysTimerCnt);
DeclareAlarm(AlarmUpdateCam);
DeclareAlarm(AlarmUpdateSonar);
DeclareEvent(EventSleepI2C);
DeclareEvent(EventSleep);

Usb usb;
Motor motor(PORT_A);
Motor motor2(PORT_B);
Camera camera(PORT_1);
Lcd lcd;
SonarSensor sonar(PORT_4);

int distance;

void sendCameraData(unsigned char *data);
void getCameraTest();
void getCameraLineTest(unsigned char *data);
	int getCameraLineTestLoop();

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
	SleeperMonitor(); // needed for I2C device and Clock classes
	usb.commHandler(); // USB communication handler
    // fun times
    StatusType ercd;

    ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
    if (ercd != E_OK)
    {
        ShutdownOS(ercd);
    }
}

TASK(TaskUpdateCam)
{
    camera.update();
    TerminateTask();
}

TASK(TaskUpdateSonar)
{
    distance = sonar.getDistance();
    TerminateTask();
}


TASK(TaskMain)
{
    distance = 0;
	U32 len;
	U8 data[MAX_USB_DATA_LEN]; // first byte is preserved for disconnect request from host
	unsigned char buffer[MAX_USB_DATA_LEN];
	Nxt nxt;
	Clock clock;

	lcd.clear();
	lcd.putf("sn", "USB");
	lcd.disp();

	//camera.sortBy(Camera::NO_SORTING); // SIZE/COLOR/NO_SORTING
	//camera.setTrackingType(Camera::LINE);
	camera.sendCommand('L');
	clock.wait(10);
	camera.sendCommand('X'); // Sort NONE
	clock.wait(25);
	camera.enableTracking(true);
    SetRelAlarm(AlarmUpdateCam, 25, 400);

	while(1)
  	{
		if (usb.isConnected()) // check usb connection
  		{
			memset(data, 0, Usb::MAX_USB_DATA_LENGTH); // flush buffer
			len = usb.receive(data,0,Usb::MAX_USB_DATA_LENGTH); // receive data

  			if (len > 0)
  			{
  				if (data[0] == DISCONNECT_REQ)
  				{
  					usb.close(); // close usb connection
				}
				else
				{
					if (data[0] == PACKET_CAM)
					{
						lcd.clear();
						lcd.putf("sn", "SendCamera");
						lcd.disp();
						sendCameraData(buffer);
					} else if (data[0] == PACKET_TEST)
					{
						//getCameraHeatmapTest();
						lcd.clear();
						lcd.putf("sn", "GetCamera");
						lcd.disp();
						getCameraLineTest(buffer);
					} else if (data[0] == PACKET_POLL)
					{
						lcd.clear();
						lcd.putf("sn", "ENTER");

						unsigned char testBuffer[41];
						memset(testBuffer, 0, 41);

						camera.receive(0x42, testBuffer, 41); // 41 is count + 8 objects * 5 items (color, x, y, x2, y2)
						lcd.putf("sdn", "Count: ", testBuffer[0], 0);
						lcd.putf("sdsdn", "x0:", testBuffer[2], 3, " | y0: ", testBuffer[3], 3);
						lcd.putf("sdsdn", "x1:", testBuffer[7], 0, " | y1", testBuffer[8], 3);

						lcd.putf("nsn", "EXIT");
						lcd.disp();
					}
				}
  			}

			/*if (nxt.getButtons() == Nxt::ENTR_ON)
			{
				lcd.clear();
				lcd.putf("sdn", "Motor: ", motor.getCount(), 0);
				lcd.disp();
			}*/

		}
        if (nxt.getButtons() == Nxt::ENTR_ON)
        {
            lcd.clear();
            lcd.putf("sdn", "Dist: ", distance, 0);
            lcd.disp();
        }
  	}

}





/**
 * Heatmaps are ranges, as thus - if converted to signed they'll return -128 if they're
 * in that range. Remember, different colors can be saved (multiple 'profiles')
 *
 * RGB, 0-255
 *
 * 0|16|32|48|64|80|96|112|128|144|160|176|192|208|224|240|255
 */
void getCameraHeatmapTest()
{
	Clock clock;

	unsigned char buffer[41];
	memset(buffer, 0, 41);
	buffer[41] = '\0';

	camera.sendCommand('G'); // Needed before reading colormaps
	clock.wait(10);
	camera.receive(0x80, buffer, 41);

	for (int k = 0; k <= 4; k++)
		lcd.putf("ds", (int) buffer[k], 0, "|");

	usb.send(buffer, 0, 41);
}
}


void sendCameraData(unsigned char *data)
{
	Clock clock;

	int amountOfObjects = 0;
	size_t offset = 0;
	Camera::Rectangle_T rec;
	data[0] = PACKET_END; data[1] = '\0'; // TODO: Ditch this? Seems a bit odd when you look at the next if clause

	//camera.update();
	//clock.wait(10); // Wait for camera data, we can possibly lower the time waited

	int numOfObjects = camera.getNumberOfObjects();

	if (numOfObjects >= 1 && numOfObjects <= 8)
		amountOfObjects = numOfObjects;
	else
	{
		data[0] = 0;
		usb.send(data, 0, Usb::MAX_USB_DATA_LENGTH);
		return;
	}

	lcd.putf("sdsdn", "N:", numOfObjects, 0, "| C:", amountOfObjects, 0);
	lcd.disp();

	data[offset++] = amountOfObjects;


	for (int i = 0; i < amountOfObjects; i++)
	{
		camera.getRectangle(i, &rec);
		data[offset++] = rec.width;
		data[offset++] = rec.height;
		data[offset++] = rec.upperLeftX;
		data[offset++] = rec.upperLeftY;
		data[offset++] = camera.getObjectColor(i);
		data[offset] = PACKET_END;
	}

	usb.send(data, 0, Usb::MAX_USB_DATA_LENGTH);
}

void getCameraLineTest(unsigned char *data)
{
	Clock clock;

	int amountOfObjects = 0;
	size_t offset = 0;
	Camera::Rectangle_T rec;

	camera.update();
	clock.wait(10);

	int numOfObjects = camera.getNumberOfObjects();

	if (numOfObjects == 8)
		amountOfObjects = 8;
	else
		amountOfObjects = 0;

	lcd.putf("sdsdn", "N:", numOfObjects, 0, "| C:", amountOfObjects, 0);
	lcd.disp();

	unsigned char countPacket[2];
	countPacket[0] = amountOfObjects;
	countPacket[1] = '\0';
	usb.send(countPacket, 0, 2);

	for (int i = 0; i < amountOfObjects; i++)
	{
		//memset(data, 0, Usb::MAX_USB_DATA_LENGTH); // flush buffer

		camera.getRectangle(i, &rec);
		offset = insertNumberToArray(data, offset, rec.width);
		offset = insertNumberToArray(data, offset, rec.height);
		offset = insertNumberToArray(data, offset, rec.upperLeftX);
		offset = insertNumberToArray(data, offset, rec.upperLeftY);
		offset = insertNumberToArray(data, offset, rec.lowerRightX);
		offset = insertNumberToArray(data, offset, rec.lowerRightY);
		data[offset] = PACKET_END;

		if (getCameraLineTestLoop() == -1)
			break;

		usb.send(data, 0, MAX_USB_DATA_LEN);
		lcd.putf("sd", "PACK_I: ", i);

		offset = 0;
		//clock.wait(10);

		lcd.disp();
	}

	lcd.putf("sn", "EXIT");
	lcd.disp();

}

int getCameraLineTestLoop()
{
	Clock clock;
	int i = 0, timeout = 10;
	U32 len;
	U8 data[2];

	while (i <= timeout && usb.isConnected())
	{
		len = usb.receive(data, 0, 2);

		if (len > 0)
		{
			if (data[0] == PACKET_ACK)
			{
				lcd.putf("sn", " ACK");
				lcd.disp();
			}
			else
			{
				lcd.putf("sn", " ERR");
				lcd.disp();
				return -1;
			}
		}

		i++;
		clock.wait(5);
	}

	return 0;
}
