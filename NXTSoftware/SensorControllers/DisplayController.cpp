//
// Created by Andreas Laugård Hald on 11-12-2017.
//

#include "DisplayController.h"

using namespace ecrobot;

DisplayController::DisplayController(Lcd *display) {
    this->lcd = display;
}

void DisplayController::SetText(char* DisplayText) {
    lcd->clear();
    lcd->putf("sdn", DisplayText);
    lcd->disp();
}
void DisplayController::SetText(char* DisplayText, double Display, int decimals) {
    lcd->clear();
    lcd->putf("sdn", DisplayText, Display, decimals);
    lcd->disp();
}
