#define TURN_ANGLE 30

// ECRobot++ API
#include "Lcd.h"
#include "Clock.h"
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

/**
 * Input:
 * PORT_1 Accelerometer
 */
AccelSensor     accel(PORT_1);


// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void)
{
	SleeperMonitor(); // needed for I2C device and Clock classes
}

TASK(TaskMain)
{
	Lcd lcd;
	Clock clock;

    S16 axesInit[3]; // Accel Sensor data
    S16 axes[3]; // Accel Sensor data
    accel.getAccel(axesInit);

	while(1)
	{
        accel.getAccel(axes);

        lcd.clear();
        lcd.putf("sn", "Accelerometer(1)");
        lcd.putf("sdsdn", "X: ", axesInit[0], 4, " X: ", axes[0], 4);
        lcd.putf("sdsdn", "Y: ", axesInit[1], 4, " Y: ", axes[1], 4);
        lcd.putf("sdsdn", "Z: ", axesInit[2], 4, " Z: ", axes[2], 4);

        lcd.putf("sn", "Difference");
        lcd.putf("sdn", "X: ", axes[0] - axesInit[0], 5);
        lcd.putf("sdn", "Y: ", axes[1] - axesInit[1], 5);
        lcd.putf("sdn", "Z: ", axes[2] - axesInit[2], 5);
		lcd.disp();

		clock.wait(100);
	}
}
}
