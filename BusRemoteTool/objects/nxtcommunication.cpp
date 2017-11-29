#include "nxtcommunication.h"
#include "ui_camerawidget.h"
#include <QDebug>

NXTCommunication::NXTCommunication()
{
}

bool NXTCommunication::Connect()
{
	if (NXTCommFantom_find(nxt_id) == 0)
		return false;

	nxt = NXTCommFantom_open(nxt_id);
	
	if (NXTCommFantom_connect(nxt) == 0)
	{
		NXTCommFantom_close(nxt);
		return false;
	}

	connected = true;
	return true;
}

void NXTCommunication::Disconnect()
{
	if (!connected) return;

	this->SendPacket(DISCONNECT_REQ);

	NXTCommFantom_close(nxt);
	connected = false;
}

bool NXTCommunication::SendPacket(char Packet)
{
	if (!connected)
		return false;
	
	buf[0] = Packet;
	buf[1] = '\0';

	NXTCommFantom_send(nxt, buf, 0, 2);
	return true;
}

bool NXTCommunication::SendArray(char* buf, size_t len)
{
	if(!connected)
		return false;
	
	NXTCommFantom_send(nxt, buf, 0, 4);
	return true;
}

bool NXTCommunication::SendACK()
{
	return this->SendPacket(PACKET_ACK);
}

size_t NXTCommunication::Receive(char* buffer, size_t length)
{
	return (size_t)NXTCommFantom_receive(nxt, buffer, 0, (int)length);
}

bool NXTCommunication::isConnected()
{
	return this->connected;
}
