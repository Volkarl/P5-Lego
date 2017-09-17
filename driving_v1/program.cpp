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
 * Output:
 *
 * PORT_A Turning Engine
 * PORT_B Forward Engine
 */
Motor motorTurn(PORT_A);
Motor motorForward(PORT_B);

// nxtOSEK hook to be invoked from an ISR in category 2
void user_1ms_isr_type2(void)
{
	SleeperMonitor(); // needed for I2C device and Clock classes
}

void turn(int direction)
{
    if (direction == TURN_LEFT)
        motorTurn.setPWM(TURN_SPEED);

    else if (direction == TURN_RIGHT)
        motorTurn.setPWM(-(TURN_SPEED));

    else if (direction == TURN_CENTER)
    {
        int angle = motorTurn.getCount();

        if (angle > 3 && angle < -3)
            if (angle > 3)
                motorTurn.setPWM(-(TURN_SPEED));
            else if (angle < -3)
                motorTurn.setPWM(TURN_SPEED);
        else
            motorTurn.setPWM(0);
    }
}

void drive(int direction)
{
    if (direction == DRIVE_FORWARD)
        motorForward.setPWM(FORWARD_SPEED);
    else if (direction == DRIVE_REVERSE)
        motorForward.setPWM(-(FORWARD_SPEED));
    else
        motorForward.setPWM(0);
}

TASK(TaskMain)
{
    Nxt nxt; // Used for buttons
	Lcd lcd;
	Clock clock;
    int direction = -1;
    int distance;

    drive(DRIVE_FORWARD);

	while(1)
	{
        distance = sonar.getDistance();

        if (distance < 10)
        {
            direction = 0;
            // Reverse
            drive(DRIVE_REVERSE);
            turn(TURN_RIGHT);
            clock.wait(2000);
            drive(DRIVE_FORWARD);
            direction = TURN_LEFT;
        }
        else if (direction == DRIVE_STOP)
            direction = TURN_LEFT;

        // Left right spasm
        if (direction == TURN_LEFT && motorTurn.getCount() > TURN_ANGLE)
            direction = TURN_RIGHT;
        else if (direction == TURN_RIGHT && motorTurn.getCount() < -(TURN_ANGLE))
            direction = TURN_LEFT;

        turn(direction);

        lcd.clear();
        lcd.putf("sdn", "Distance: ", distance, 5);
        lcd.putf("sdn", "TurnA: ", motorTurn.getCount(), 0);
        lcd.putf("sdn", "Direction: ", direction);
		lcd.disp();

		clock.wait(100);
	}
};
}
