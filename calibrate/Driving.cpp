#include "Driving.h"
#include "Motor.h"
#include "Clock.h"

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

bool calibrated = false;


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

/**
 * Perform calibration of wheels for getCount()
 */
void Driving::calibrate()
{
	int leftPos = 0;
	int rightPos = 0;
	int lastPos = -20000; // TODO: Fix stupid value instead of null
	int direction = 1; // TODO: This variable is frankly pretty odd
	int count; // Used to reduce query counts, a bit unsure if this is a bit too slow because of processing & turning time
	int center;
	Clock clock;

	while(1) {
		count = motorTurn.getCount();

		// Right
		if (direction == 1 && lastPos != -2000)
		{
			motorTurn.setPWM(TURN_SPEED);
			count = this->getTurnCount();

			// Take account for jitter, albeit probably stupid
			if (count >= lastPos - 1 && count <= lastPos + 1) {
				rightPos = lastPos;
				lastPos = -20000;
				direction = -1;
				motorTurn.setPWM(-TURN_SPEED);
			}
		// Left
		} else if (direction == -1) {
			motorTurn.setPWM(-TURN_SPEED);
			count = this->getTurnCount();

			// Take account for jitter, albeit probably stupid
			if (count >= lastPos - 1 && count <= lastPos + 1 && count < rightPos - 10) {
				leftPos = lastPos;
				direction = -2;
				motorTurn.setPWM(0);
			}
		}
		// Center
		else
		if (direction == -2 && count >= (leftPos + rightPos) / 2 - TURN_JITTER && count <= (leftPos + rightPos) / 2 + TURN_JITTER)
		{
			direction = 0;
			motorTurn.setCount(0);
			motorTurn.setPWM(0);
			calibrated = true;
			break;
		}
		else if (direction == -2) {
			center = (leftPos + rightPos) / 2;

			if (motorTurn.getCount() < center)
				motorTurn.setPWM(TURN_SPEED);
			else
				motorTurn.setPWM(-TURN_SPEED);

		}

		lastPos = this->getTurnCount();

		clock.wait(100);
	}
}

/**
 * Whether or not it is calibrated yet
 * @return bool Calibrated
 */
bool Driving::isCalibrated()
{
	return calibrated;
}

/**
 *
 * @return bool Centered
 */
bool Driving::isCentered()
{
	if (calibrated)
	{
		int count = this->getTurnCount();

		if (count >= 0 - TURN_JITTER && count <= 0 + TURN_JITTER)
			return true;
		else
			return false;
	}
	else
		return false;
}

/**
 * Center wheels
 */
void Driving::center()
{
	// Assuming 0 is center. Looks really awkward though.
	while (!this->isCentered())
	{
		if (this->getTurnCount() < 0)
			motorTurn.setPWM(TURN_SPEED);
		else
			motorTurn.setPWM(-TURN_SPEED);
	}

	motorTurn.setPWM(0);
}
