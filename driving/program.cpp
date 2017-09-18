#include "Lcd.h"
#include "Clock.h"
#include "Nxt.h"
#include "AccelSensor.h"
#include "SonarSensor.h"
#include "Motor.h"

#include "driving.h"

using namespace ecrobot;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/**
 * Input:
 *
 * PORT_1 Sonar Sensor
 */
SonarSensor sonar(PORT_1);

/**
 * Classes
 */
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
	int distance;
	int direction = -1;

	drive.drive(DRIVE_FORWARD);

	while(1)
	{
		distance = sonar.getDistance();

		if (distance < 10)
		{
			direction = 0;
			// Reverse
			drive.drive(DRIVE_REVERSE);
			drive.turn(TURN_RIGHT);
			clock.wait(2000);
			drive.drive(DRIVE_FORWARD);
			direction = TURN_LEFT;
		}
		else if (direction == DRIVE_STOP)
			direction = TURN_LEFT;

		// Left right spasm
		if (direction == TURN_LEFT && drive.getTurnCount() > TURN_ANGLE)
			direction = TURN_RIGHT;
		else if (direction == TURN_RIGHT && drive.getTurnCount() < -(TURN_ANGLE))
			direction = TURN_LEFT;

		drive.turn(direction);

		lcd.clear();
		lcd.putf("sdn", "Distance: ", distance, 5);
		lcd.putf("sdn", "TurnA: ", drive.getTurnCount(), 0);
		lcd.putf("sdn", "Direction: ", direction, 1);
		lcd.disp();

		clock.wait(100);
	}
};
}
