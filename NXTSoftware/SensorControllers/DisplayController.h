//
// Created by Andreas Laugård Hald on 11-12-2017.
//

#ifndef NXTSOFTWARE_DISPLAYCONTROLLER_H
#define NXTSOFTWARE_DISPLAYCONTROLLER_H

#include "Lcd.h"

using namespace ecrobot;

namespace ecrobot {

    class DisplayController {
    public:
        DisplayController(Lcd *display);
        void SetText(char* textToDisplay);
        void SetText(char* textToDisplay, double numberToDisplay, int spaceForNumber);

    private:
        Lcd* lcd;
    };

};
#endif //NXTSOFTWARE_DISPLAYCONTROLLER_H
