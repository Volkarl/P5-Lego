#define TURN_ANGLE 40
#define TURN_SPEED 15

// ECRobot++ API
#include "Lcd.h"
#include "Clock.h"
#include "Nxt.h"
#include "AccelSensor.h"
#include "SonarSensor.h"
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
 * PORT_A Forward Engine
 */
Motor motorA(PORT_A); // brake by default

/**
 * Input:
 *
 * PORT_1 SonarSensor
 */
SonarSensor sonar(PORT_1);


// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void)
{
	SleeperMonitor(); // needed for I2C device and Clock classes
}

TASK(TaskMain)
{
	Lcd lcd;
	Clock clock;
    int distance;


	while(1)
	{
        distance = sonar.getDistance();

        if (distance > 12)
            motorA.setPWM(-50);
        else
            motorA.setPWM(0);

        lcd.clear();
        lcd.putf("sdn", "Distance: ", distance, 5);
		lcd.disp();

		clock.wait(100);
	}
};
}
