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
 * Drive Forwards
 */
void Driving::forward()
{
	motorForward.setPWM(FORWARD_SPEED);
}

/**
 * Reverse
 */
void Driving::reverse()
{
	motorForward.setPWM(-(FORWARD_SPEED));
}

/**
 * Stop forward engine
 */
void Driving::stop()
{
	motorForward.setPWM(0);
}

/**
 * Stop both forward and turn engines.
 * TODO: Remember that this should interrupt tasks as well
 */
void Driving::halt()
{
	motorForward.setPWM(0);
	motorTurn.setPWM(0);
}


/**
 * Probably shouldn't be here as all driving logic should most likely be here
 * @return int count
 */
int Driving::getTurnCount()
{
	return motorTurn.getCount();
}
