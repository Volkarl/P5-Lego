//
// Created by Andreas on 02/12/2017.
//

//So far it returns an int, it should be made to return an enum intstead.

#include "ColourSensorController.h"

using namespace ecrobot;

int ColourSensorController::DetectColour() {
    return -1; //TODO
}

int ColourSensorController::DetectColour(int red, int green, int blue){
    /*switch (red, green, blue) //Switches can't work like this in C++. Find something else.
    {
        case red >= 255 && green <= 0 && blue <= 0: return(1); //So far a number is returned. 1 for red, 2 for green, 3 of for blue, and 0 if it could not be determined, more colours can be added later.

        case red <= 0 && green >= 255 && blue <= 0: return(2);  //The numbers 255 are 0 are put here for now. Sensor needs to be tested, and appropriate numbers will be put in.

        case red <= 0 && green <= 0 && blue >= 255: return(3);

        default return(0);
    }*/

    //Logic:
    if (red >= 255 && green <= 0 && blue <= 0)
    {
        return(1);
    }
    else if (red <= 0 && green >= 255 && blue <= 0)
    {
        return(2);
    }
    else if (red <= 0 && green <= 0 && blue >= 255)
    {
        return(3);
    }
    else
    {
        return(0);
    }
}

ColourSensorController::ColourSensorController(ColorSensor *colourSensor) {
    this->ColourSensor = colourSensor;
}
