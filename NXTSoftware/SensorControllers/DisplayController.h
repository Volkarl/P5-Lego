#ifndef NXTSOFTWARE_DISPLAYCONTROLLER_H
#define NXTSOFTWARE_DISPLAYCONTROLLER_H

#include "Lcd.h"
#include "Clock.h"

namespace ecrobot {

class DisplayController {
    public:
        DisplayController();
        void Calibrate();
        void SetText(const char* text);
        void SetText(int text);
        void SetText(const char* text, int number, int space);
        //void RefreshDisplay();
        void ClearDisplay();

    private:
        Lcd m_Lcd;
    };

};
#endif //NXTSOFTWARE_DISPLAYCONTROLLER_H
