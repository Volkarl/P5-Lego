#ifndef NXTSOFTWARE_DISPLAYCONTROLLER_H
#define NXTSOFTWARE_DISPLAYCONTROLLER_H

#include "Lcd.h"
#include "Clock.h"

namespace ecrobot {

    class DisplayController {
    public:
        DisplayController();
        void Calibrate();
        void SetText(const char* textToDisplay);
        void SetText(int textToDisplay);
        void SetText(const char* textToDisplay, double numberToDisplay, int spaceForNumber);
        //void RefreshDisplay();
        //void ClearDisplay();

    private:
        Lcd lcd;
    };

};
#endif //NXTSOFTWARE_DISPLAYCONTROLLER_H
