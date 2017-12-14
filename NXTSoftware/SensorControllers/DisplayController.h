//
// Created by Andreas Laugård Hald on 11-12-2017.
//

#ifndef NXTSOFTWARE_DISPLAYCONTROLLER_H
#define NXTSOFTWARE_DISPLAYCONTROLLER_H

#include "Lcd.h"
#include "Clock.h"

namespace ecrobot {

    class DisplayController {
    public:
        DisplayController(Lcd *display, Clock* clock);
        void Calibrate();
        void SetText(char* textToDisplay);
        void SetText(char* textToDisplay, double numberToDisplay, int spaceForNumber);
        //void RefreshDisplay();
        //void ClearDisplay();

    private:
        Lcd* lcd;
        Clock* clock;
    };

};
#endif //NXTSOFTWARE_DISPLAYCONTROLLER_H
