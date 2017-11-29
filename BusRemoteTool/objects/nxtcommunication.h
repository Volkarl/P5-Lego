#ifndef NXTCOMMUNICATION_H
#define NXTCOMMUNICATION_H

#include "../Shared/Connectivity.h"
#include "includes/nxtcommfantom/nxtcommfantom.h"

class NXTCommunication {
public:
	bool Connect();

	NXTCommunication();
	void Disconnect();
	bool SendPacket(char Packet);
	bool SendArray(char* buffer, size_t len);
	bool SendACK();
	size_t Receive(char* buffer, size_t length);
	void getObjects();

	bool isConnected(); // Getter

protected:
	ViChar nxt_id[MAX_DEV_ID];
	nFANTOM100_iNXT nxt;
	char buf[64];
	bool connected = false;
};

#endif // NXTCOMMUNICATION_H
