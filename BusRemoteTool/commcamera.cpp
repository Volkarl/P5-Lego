#include "commcamera.h"
#include "ui_camerawidget.h"
#include <QDebug>

CommCamera::CommCamera()
{
}

bool CommCamera::Connect()
{
    if (NXTCommFantom_find(nxt_id) == 0)
    {
        //ui->textEdit->setText("NXT not found.");
        return false;
    }

    nxt = NXTCommFantom_open(nxt_id);

    if (NXTCommFantom_connect(nxt) == 0)
    {
        NXTCommFantom_close(nxt);
        return false;
    }

    connected = true;
    return true;
}

void CommCamera::Disconnect()
{
    //if (connected)
    //{
        this->SendPacket(DISCONNECT_REQ);

        NXTCommFantom_close(nxt);
    //}

    connected = false;
}

bool CommCamera::SendPacket(char Packet)
{
    if (connected)
    {
        buf[0] = Packet;
        buf[1] = '\0';

        NXTCommFantom_send(nxt, buf, 0, 2);
        return true;
    }
    else
    {
        return false;
    }
}

bool CommCamera::SendACK()
{
    return this->SendPacket(PACKET_ACK);
}

size_t CommCamera::Receive(char* buffer, size_t length)
{
    size_t len;

    len = (int)NXTCommFantom_receive(nxt, buffer, 0, (int)length);
    return len;
}

bool CommCamera::isConnected()
{
    return this->connected;
}

void CommCamera::getObjects()
{
    int offset = 0;

    this->SendPacket(PACKET_CAM);
    this->Receive(buf, MAX_DATA_LEN);

    if ((unsigned char) buf[0] == PACKET_END)
        return;
    else if ((int)buf[0] >= 0 && (int)buf[0] <= 8)
        this->objectCount = (int)buf[0];
    else
        this->objectCount = 0;

    qDebug() << "Count: " << QString::number(this->objectCount);


    for (int j = 0; j < this->objectCount; j++)
    {
        rect[j].width = (unsigned char)buf[++offset];
        rect[j].height = (unsigned char)buf[++offset];
        rect[j].upperLeftX = (unsigned char)buf[++offset];
        rect[j].upperLeftY = (unsigned char)buf[++offset];
        rect[j].objColor = (unsigned char)buf[++offset];
    }
}

int CommCamera::amountOfObjects()
{
    return this->objectCount;
}
