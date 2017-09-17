#define TURN_ANGLE 40
#define TURN_SPEED 15

// ECRobot++ API
#include "Lcd.h"
#include "Clock.h"
#include "Nxt.h"
#include "AccelSensor.h"
#include "Motor.h"

using namespace ecrobot;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/**
 * Output:
 *
 * PORT_A Turning Engine
 * PORT_B Forward Engine
 */
Motor motorA(PORT_A); // brake by default
Motor motorB(PORT_B); // brake by default

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
    int direction = -1;

	while(1)
	{
        if (nxt.getButtons() == Nxt::ENTR_ON)
            if (direction == 0)
                direction = -1;
            else
                direction = 0;

        if (direction == -1 && motorA.getCount() > TURN_ANGLE)
            direction = 1;
        else if (direction == 1 && motorA.getCount() < -(TURN_ANGLE))
            direction = -1;

        if (direction == -1)
            motorA.setPWM(TURN_SPEED);
        else if (direction == 1)
            motorA.setPWM(-(TURN_SPEED));
        else
            motorA.setPWM(0);

        lcd.clear();
        lcd.putf("sdn", "TurnA: ", motorA.getCount(), 0);
		lcd.disp();

		clock.wait(100);
	}
}
}
