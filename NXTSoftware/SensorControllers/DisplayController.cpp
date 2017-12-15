#include "DisplayController.h"

using namespace ecrobot;

DisplayController::DisplayController()
{
    this->lcd.clear();
}

void DisplayController::SetText(const char* textToDisplay) {
    //lcd->clear();
    this->lcd.putf("sn", textToDisplay);
    this->lcd.disp();
}

void DisplayController::SetText(const char* textToDisplay, double numberToDisplay, int spaceForNumber) {
    //lcd->clear();
    this->lcd.putf("sdn", textToDisplay, numberToDisplay, spaceForNumber);
    this->lcd.disp();
}

void DisplayController::SetText(int number) {
    //lcd->clear();
    this->lcd.putf("dn", number, 0);
    this->lcd.disp();
}

/*
void DisplayController::RefreshDisplay(){
    // Can only refresh every 50 ms, otherwise it doesn't refresh properly
    lcd->disp();
}

void DisplayController::ClearDisplay(){
    lcd->clear();
}
*/

void DisplayController::Calibrate()
{
}
