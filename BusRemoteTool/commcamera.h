#ifndef COMMCAMERA_H
#define COMMCAMERA_H
#include "../Shared/Connectivity.h"
#include "includes/nxtcommfantom/nxtcommfantom.h"


class CommCamera
{
public:
    CommCamera();
    bool Connect();
    void Disconnect();
    bool SendPacket(char Packet);
    bool SendACK();
    size_t Receive(char* buffer, size_t length);
    void getObjects();

    // Variables
    static Rectangle_T rect[8];
    bool isConnected(); // Getter
    int amountOfObjects();

protected:
    ViChar nxt_id[MAX_DEV_ID];
    nFANTOM100_iNXT nxt;
    char buf[64];
    bool connected = false;
    int objectCount = 0;
};

#endif // COMMCAMERA_H
