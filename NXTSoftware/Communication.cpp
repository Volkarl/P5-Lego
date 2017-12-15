#include "stdio.h"
#include <cstring>

#include "Communication.h"
#include "../Shared/Connectivity.h"
#include "../Shared/Tools.h"
#include "../Shared/objects/camtools.h"
#include "Driving.h"


using namespace ecrobot;

Communication::Communication(Usb* usbobj, CamController* cam, ColorSensor* colorSensor, Driving* drive)
{
	this->usb = usbobj;
	this->m_Camera = cam;
	this->color = colorSensor;
	this->drive = drive;

	memset(data, 0, Usb::MAX_USB_DATA_LENGTH); // flush buffer
}

void Communication::handle()
{
	if (this->usb->isConnected()) // check usb connection
	{
		U32 len = usb->receive(this->data, 0, Usb::MAX_USB_DATA_LENGTH); // receive data

		if (len > 0)
		{
			const unsigned char& type = data[0];

			if (type == DISCONNECT_REQ)
				usb->close(); // close usb connection
			else if (type == PACKET_CAM || type == PACKET_POLL)
				this->sendData(this->data);
			else if (type == PACKET_DRIVE) // TEST
				this->driveCmd(this->data);

			memset(data, 0, Usb::MAX_USB_DATA_LENGTH); // flush buffer
		}

	}
}


void Communication::driveCmd(unsigned char *data)
{
	char forwardSpeed = (char)data[1];
	char angle = (char)data[2];

	if (forwardSpeed < -100 || forwardSpeed > 100 ||
		angle < -90 || angle > 90)
		return;

	this->drive->data.speed = forwardSpeed;
	this->drive->data.angle = angle;
}

/**
 * Heatmaps are ranges, as thus - if converted to signed they'll return -128 if they're
 * in that range. Remember, different colors can be saved (multiple 'profiles')
 *
 * RGB, 0-255
 *
 * 0|16|32|48|64|80|96|112|128|144|160|176|192|208|224|240|255
 */
void Communication::getCameraHeatmapTest()
{
	/*
	unsigned char buf[41];
	memset(buf, 0, 41);
	buf[41] = '\0';

	camera->sendCommand('G'); // Needed before reading colormaps
	this->clock.wait(10);
	camera->receive(0x80, buf, 41);

	usb->send(buf, 0, 41);
	 */
}



void Communication::sendData(unsigned char *data)
{
	size_t offset = 0;
	//Camera::Rectangle_T rec;
	data[0] = PACKET_END; data[1] = '\0';


	if (!this->m_Camera->UpdateBuffer()) {
		data[0] = 0;
		usb->send(data, 0, Usb::MAX_USB_DATA_LENGTH);
		return;
	}

	CamBuffer camBuffer = this->m_Camera->GetBuffer();


	data[offset++] = camBuffer.m_iCount;

	for (int i = 0; i < camBuffer.m_iCount; i++)
	{
		const Rectangle_T& rec = camBuffer.m_buffRects[i];

		data[offset++] = rec.width;
		data[offset++] = rec.height;
		data[offset++] = rec.upperLeftX;
		data[offset++] = rec.upperLeftY;
		data[offset++] = rec.objColor;
		data[offset] = PACKET_END;
	}

	data[offset++] = drive->data.speed;
	data[offset++] = (unsigned char) drive->getTurnCount();
	data[offset++] = drive->data.halt;
	// Color Sensor
	data[offset++] = drive->data.color.red;
	data[offset++] = drive->data.color.green;
	data[offset++] = drive->data.color.blue;

	usb->send(data, 0, Usb::MAX_USB_DATA_LENGTH);
}


/*void Communication::sendCameraData(unsigned char *data)
{
	size_t offset = 0;
	Camera::Rectangle_T rec;
	data[0] = PACKET_END; data[1] = '\0';

	int numOfObjects = camera->getNumberOfObjects();

	if (numOfObjects < 1 && numOfObjects > 8)
	{
		data[0] = 0;
		usb->send(data, 0, Usb::MAX_USB_DATA_LENGTH);
		return;
	}

	data[offset++] = numOfObjects;

	for (int i = 0; i < numOfObjects; i++)
	{
		camera->getRectangle(i, &rec);
		data[offset++] = rec.width;
		data[offset++] = rec.height;
		data[offset++] = rec.upperLeftX;
		data[offset++] = rec.upperLeftY;
		data[offset++] = camera->getObjectColor(i);
		data[offset] = PACKET_END;
	}

	usb->send(data, 0, Usb::MAX_USB_DATA_LENGTH);
}
*/
