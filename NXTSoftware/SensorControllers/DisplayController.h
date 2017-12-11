//
// Created by Andreas Laugård Hald on 11-12-2017.
//

#ifndef NXTSOFTWARE_DISPLAYCONTROLLER_H
#define NXTSOFTWARE_DISPLAYCONTROLLER_H

//#include <ntdef.h>  Where did this come from?
#include "Lcd.h"

namespace ecrobot {

    class DisplayController {
    public:
        DisplayController(Lcd *display);
        void SetText(char* DisplayText);
        void SetText(char* DisplayText, double Distance, int decimals);

    private:
        Lcd* lcd;
    };

};
#endif //NXTSOFTWARE_DISPLAYCONTROLLER_H
