#include "driving.h"
#include "Motor.h"

using namespace ecrobot;

/**
 * Output:
 *
 * PORT_A Turning Engine
 * PORT_B Forward Engine
 */
Motor motorTurn(PORT_A);
Motor motorForward(PORT_B);

/**
 * Constructor - TODO: requires motors
 * @param Motor drive
 * @param Motor turn
 */
/*Driving::Driving( Motor drive, Motor turn )
{
    direction = -1;
    motorDrive = drive;
    motorTurn = turn;
}*/

Driving::Driving() {}


/**
* Which direction to turn towards.
*
* @param direction
*/
void Driving::turn(int direction)
{
    if (direction == TURN_LEFT)
        motorTurn.setPWM(TURN_SPEED);

    else if (direction == TURN_RIGHT)
        motorTurn.setPWM(-(TURN_SPEED));

    else if (direction == TURN_CENTER) // TODO: Need to think through the approach on this one
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

/**
 * Select between drive / stop / reverse
 *
 * @param direction
 */
void Driving::drive(int direction)
{
    if (direction == DRIVE_FORWARD)
        motorForward.setPWM(FORWARD_SPEED);
    else if (direction == DRIVE_REVERSE)
        motorForward.setPWM(-(FORWARD_SPEED));
    else
        motorForward.setPWM(0);

    motorForward.setCount(0); // Counter is irrelevant and an increasing number would be a waste.
}


/**
 * Probably shouldn't be here as all driving logic should most likely be here
 * @return int count
 */
int Driving::getTurnCount()
{
	return motorTurn.getCount();
}
