#include "Driving.h"
#include "Motor.h"
#include "Clock.h"
#include <climits>

using namespace ecrobot;

/**
 * Input:
 */

/**
 * Output:
 *
 * PORT_A Turning Engine
 * PORT_B Forward Engine
 */
Motor motorTurn(PORT_A);
Motor motorForward(PORT_B);


/**
 * Constructor
 */
Driving::Driving()
{
	calibrated = false;
}

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

	else if (direction == TURN_CENTER)
		this->center();
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
	int lastPos = INT_MAX; // TODO: Fix stupid value instead of null
	int direction = 1; // TODO: This variable is frankly pretty odd
	int count; // Used to reduce query counts, a bit unsure if this is a bit too slow because of processing & turning time
	int center;
	Clock clock;

	while(1) {
		count = motorTurn.getCount();

		// Right
		if (direction == 1)
		{
			motorTurn.setPWM(TURN_SPEED);
			count = this->getTurnCount();

			// Take account for jitter, albeit probably stupid
			if (count >= lastPos - 1 && count <= lastPos + 1) {
				rightPos = lastPos;
				lastPos = INT_MAX;
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

			if (this->getTurnCount() < center)
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
		return this->isAtAngle(0);
	else
		return false;
}

/**
 * Center wheels
 */
void Driving::center()
{
	this->setTurnAngle(0);
}

/**
 * @return bool Centered
 */
bool Driving::isAtAngle(int angle)
{
	if (calibrated)
	{
		int count = this->getTurnCount();

		if (count >= angle - TURN_JITTER && count <= angle + TURN_JITTER)
			return true;
		else
			return false;
	}
	else
		return false;
}

/**
 * Set turning angle
 */
void Driving::setTurnAngle(int angle)
{
	// Assuming 0 is center. Looks really awkward though.
	while (!this->isAtAngle(angle))
	{
		if (this->getTurnCount() < 0)
			motorTurn.setPWM(TURN_SPEED);
		else
			motorTurn.setPWM(-TURN_SPEED);
	}

	motorTurn.setPWM(0);
}
