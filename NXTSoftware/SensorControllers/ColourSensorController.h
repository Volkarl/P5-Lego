//
// Created by Andreas 03/12/2017
//
#ifndef P5_LEGO_IColourSENSORCONTROLLER_H
#define P5_LEGO_IColourSENSORCONTROLLER_H

#include "ISensorController.h"

class ColourSensorController : IColourSensorController
{
public:
	int DetectColour();
};

#endif //P5_LEGO_IColourSENSORCONTROLLER_H