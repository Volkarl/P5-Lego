//
// Created by Andreas 03/12/2017
//
#ifndef P5_LEGO_IColourSENSORCONTROLLER_H
#define P5_LEGO_IColourSENSORCONTROLLER_H

#include "ColorSensor.h"

using namespace ecrobot;

class ColourSensorController
{
public:
	ColourSensorController(ColorSensor* colourSensor);
	void Calibrate();
	int DetectColour();
    int DetectColour(int red, int green, int blue);

private:
	ColorSensor* ColourSensor;
};

//Todo add constructor with COLOUR SENSOR CLASS

#endif //P5_LEGO_IColourSENSORCONTROLLER_H
