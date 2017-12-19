#include "stdio.h"
#include <cstring>

#include "Communication.h"

using namespace ecrobot;

Communication::Communication(Usb* usb, CamController* cam, ColorSensor* colorSensor, Driving* drive, DisplayController* lcd)
{
	this->m_Usb = usb;
	this->m_Camera = cam;
	this->m_Color = colorSensor;
	this->m_Drive = drive;

	this->m_Controller = Controller::NXT; // Use NXT by default
	this->m_Lcd = lcd;

	memset(data, 0, Usb::MAX_USB_DATA_LENGTH); // flush buffer
}

void Communication::handle()
{
	if (this->m_Usb->isConnected()) // check usb connection
	{
		U32 len = m_Usb->receive(this->data, 0, Usb::MAX_USB_DATA_LENGTH); // receive data

		if (len > 0)
		{
			const unsigned char& type = data[0];

			if (type == DISCONNECT_REQ)
				m_Usb->close(); // close usb connection
			else if (type == PACKET_CAM || type == PACKET_POLL)
				this->sendData(this->data);
			else if (type == PACKET_DRIVE)
				this->driveCmd(this->data);
			else if(type == PACKET_SETMODE)
				this->setMode(this->data);

			memset(data, 0, Usb::MAX_USB_DATA_LENGTH); // flush buffer
		}

	}
}


void Communication::driveCmd(unsigned char* data)
{
	if(!this->m_Controller == Controller::PC)
		return;

	char forwardSpeed = (char)data[1];
	char angle = (char)data[2];

	if (forwardSpeed < -100 || forwardSpeed > 100 ||
		angle < -90 || angle > 90)
		return;

	this->m_Drive->SetSpeed(forwardSpeed);
	this->m_Drive->SetAngle(angle);
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

	m_Usb->send(buf, 0, 41);
	 */
}



void Communication::sendData(unsigned char* data)
{
	size_t offset = 0;
	//Camera::Rectangle_T rec;
	data[0] = PACKET_END; data[1] = '\0';


	if (!this->m_Camera->UpdateBuffer()) {
		data[0] = 0;
		m_Usb->send(data, 0, Usb::MAX_USB_DATA_LENGTH);
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

	data[offset++] = m_Drive->GetSpeed();
	data[offset++] = (unsigned char) m_Drive->getTurnCount();
	data[offset++] = m_Drive->IsHalted();
	// Color Sensor
	data[offset++] = m_Drive->data.color.red;
	data[offset++] = m_Drive->data.color.green;
	data[offset++] = m_Drive->data.color.blue;

	m_Usb->send(data, 0, Usb::MAX_USB_DATA_LENGTH);
}

void Communication::setMode(unsigned char *data)
{
	this->m_Controller = (Controller::Source) data[1];

	this->m_Lcd->ClearDisplay();
	if(this->m_Controller == Controller::PC)
		this->m_Lcd->SetText("PC");
	else if(this->m_Controller == Controller::NXT)
		this->m_Lcd->SetText("NXT");
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
		m_Usb->send(data, 0, Usb::MAX_USB_DATA_LENGTH);
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

	m_Usb->send(data, 0, Usb::MAX_USB_DATA_LENGTH);
}
*/
