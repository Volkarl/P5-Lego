// ECRobot++ API
#include "Lcd.h"
#include "Clock.h"
#include "Nxt.h"
#include "Motor.h"

#include "Driving.h"

using namespace ecrobot;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

Driving drive;

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void)
{
	SleeperMonitor(); // needed for I2C device and Clock classes
}

TASK(TaskMain)
{
    Nxt nxt; // Used for buttons
	Lcd lcd;
	Clock clock;
	bool centerPls = false;

	// Positive = Right

	drive.calibrate();

	while(1)
	{
		if (nxt.getButtons() == Nxt::ENTR_ON)
			centerPls = true;

		if (centerPls)
		{
			if (drive.isCentered())
				centerPls = false;
			else
				drive.center();
		}


        lcd.clear();
        lcd.putf("sdnn", "Turn Count: ", drive.getTurnCount(), 0);

		if (drive.isCentered())
			lcd.putf("sn", "Centered");
		else
			lcd.putf("sn", "Not centered");

		lcd.disp();

		clock.wait(100);
	}
}
}
