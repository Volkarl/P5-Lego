#include "DisplayController.h"

using namespace ecrobot;

DisplayController::DisplayController()
{
    this->m_Lcd.clear();
}

void DisplayController::SetText(const char* text) {
    //m_Lcd->clear();
    this->m_Lcd.putf("sn", text);
    this->m_Lcd.disp();
}

void DisplayController::SetText(const char* text, int number, int space) {
    //m_Lcd->clear();
    this->m_Lcd.putf("sdn", text, number, space);
    this->m_Lcd.disp();
}

void DisplayController::SetText(int number) {
    //m_Lcd->clear();
    this->m_Lcd.putf("dn", number, 0);
    this->m_Lcd.disp();
}

/*
void DisplayController::RefreshDisplay(){
    // Can only refresh every 50 ms, otherwise it doesn't refresh properly
    m_Lcd->disp();
}
*/
void DisplayController::ClearDisplay(){
    this->m_Lcd.clear();
}


void DisplayController::Calibrate()
{
}
