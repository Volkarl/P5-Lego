#include "stdio.h"
#include <cstring>

#include "Communication.h"
#include "../Shared/Connectivity.h"
#include "../Shared/Tools.h"

using namespace ecrobot;

Communication::Communication(Usb* usbobj, Camera* cam)
{
	this->usb = usbobj;
	this->camera = cam;
}

void Communication::handle()
{
	if (this->usb->isConnected()) // check usb connection
	{
		memset(data, 0, Usb::MAX_USB_DATA_LENGTH); // flush buffer
		U32 len = usb->receive(this->data, 0, Usb::MAX_USB_DATA_LENGTH); // receive data

		if (len > 0)
		{
			if (data[0] == DISCONNECT_REQ)
				usb->close(); // close usb connection
			else if (data[0] == PACKET_CAM)
				this->sendCameraData(this->data);
			else if (data[0] == PACKET_TEST)
				this->getCameraLineTest(this->data);
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
void Communication::getCameraHeatmapTest()
{
	unsigned char buf[41];
	memset(buf, 0, 41);
	buf[41] = '\0';

	camera->sendCommand('G'); // Needed before reading colormaps
	this->clock.wait(10);
	camera->receive(0x80, buf, 41);

	usb->send(buf, 0, 41);
}


void Communication::sendCameraData(unsigned char *data)
{
	int amountOfObjects = 0;
	size_t offset = 0;
	Camera::Rectangle_T rec;
	data[0] = PACKET_END; data[1] = '\0'; // TODO: Ditch this? Seems a bit odd when you look at the next if clause

	//camera.update();
	//clock.wait(10); // Wait for camera data, we can possibly lower the time waited

	int numOfObjects = camera->getNumberOfObjects();

	if (numOfObjects >= 1 && numOfObjects <= 8)
		amountOfObjects = numOfObjects;
	else
	{
		data[0] = 0;
		usb->send(data, 0, Usb::MAX_USB_DATA_LENGTH);
		return;
	}

	data[offset++] = amountOfObjects;


	for (int i = 0; i < amountOfObjects; i++)
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

void Communication::getCameraLineTest(unsigned char *data)
{
	int amountOfObjects = 0;
	size_t offset = 0;
	Camera::Rectangle_T rec;

	camera->update();
	this->clock.wait(10);

	int numOfObjects = camera->getNumberOfObjects();

	if (numOfObjects == 8)
		amountOfObjects = 8;
	else
		amountOfObjects = 0;

	unsigned char countPacket[2];
	countPacket[0] = amountOfObjects;
	countPacket[1] = '\0';
	usb->send(countPacket, 0, 2);

	for (int i = 0; i < amountOfObjects; i++)
	{
		//memset(data, 0, Usb::MAX_USB_DATA_LENGTH); // flush buffer

		camera->getRectangle(i, &rec);
		offset = insertNumberToArray(data, offset, rec.width);
		offset = insertNumberToArray(data, offset, rec.height);
		offset = insertNumberToArray(data, offset, rec.upperLeftX);
		offset = insertNumberToArray(data, offset, rec.upperLeftY);
		offset = insertNumberToArray(data, offset, rec.lowerRightX);
		offset = insertNumberToArray(data, offset, rec.lowerRightY);
		data[offset] = PACKET_END;

		if (this->getCameraLineTestLoop() == -1)
			break;

		usb->send(data, 0, MAX_USB_DATA_LEN);
		offset = 0;
		//clock.wait(10);
	}

}

int Communication::getCameraLineTestLoop()
{
	Clock clock;
	int i = 0, timeout = 10;
	U32 len;
	U8 data[2];

	while (i <= timeout && usb->isConnected())
	{
		len = usb->receive(data, 0, 2);

		if (len > 0 && data[0] != PACKET_ACK)
			return -1;

		i++;
		this->clock.wait(5);
	}

	return 0;
}
