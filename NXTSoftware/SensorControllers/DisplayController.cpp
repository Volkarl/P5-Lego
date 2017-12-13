//
// Created by Andreas Laugård Hald on 11-12-2017.
//

#include "DisplayController.h"

using namespace ecrobot;

DisplayController::DisplayController(Lcd *display) {
    this->lcd = display;
}

void DisplayController::SetText(char* textToDisplay) {
    lcd->clear();
    lcd->putf("sn", textToDisplay);
    lcd->disp();
}
void DisplayController::SetText(char* textToDisplay, double numberToDisplay, int spaceForNumber) {
    lcd->clear();
    lcd->putf("sdn", textToDisplay, numberToDisplay, spaceForNumber);
    lcd->disp();
}

void DisplayController::Calibrate() {

}
