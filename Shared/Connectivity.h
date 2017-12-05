#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#define DISCONNECT_REQ 0xFF

#define PACKET_END 0xFE
#define PACKET_SEPERATOR 0xFD
#define PACKET_CAM 0xE1
#define PACKET_TEST 0xE2
#define PACKET_POLL 0xE3
#define PACKET_DRIVE 0xE4
#define PACKET_ACK 0xFC

typedef struct {
	unsigned int red;
	unsigned int green;
	unsigned int blue;
} RGB;

typedef struct {
	signed int upperLeftX;
	signed int upperLeftY;
	signed int lowerRightX;
	signed int lowerRightY;
	signed int width;
	signed int height;
	signed int objColor;
	
	RGB color;
	bool collision;
} Rectangle_T;

#endif
