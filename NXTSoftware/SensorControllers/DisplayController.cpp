//
// Created by Andreas Laugård Hald on 11-12-2017.
//

#include "DisplayController.h"

using namespace ecrobot;

DisplayController::DisplayController(Lcd *display, Clock *clock){
    this->lcd = display;
    this->clock = clock;
}

void DisplayController::SetText(char* textToDisplay) {
    clock->wait(100);
    //lcd->clear();
    lcd->putf("sn", textToDisplay);
    lcd->disp();
}

void DisplayController::SetText(char* textToDisplay, double numberToDisplay, int spaceForNumber) {
    clock->wait(100);
    //lcd->clear();
    lcd->putf("sdn", textToDisplay, numberToDisplay, spaceForNumber);
    lcd->disp();
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

void DisplayController::Calibrate() {

}
