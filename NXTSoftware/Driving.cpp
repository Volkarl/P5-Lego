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
Driving::Driving() : data()
{
	data.angle = 0, data.speed = 0, data.calibrated = false, data.halt = false; // TODO: Necessary?
}

/**
 * Loop for task
 */
void Driving::update()
{
	if (data.halt)
		return;

	motorForward.setPWM(data.speed);
	this->setTurnAngle(data.angle);
}

/**
 * Drive Forwards
 */
void Driving::forward()
{
	//motorForward.setPWM(FORWARD_SPEED);
	data.speed = FORWARD_SPEED;
}

void Driving::forward(int speed)
{
	//motorForward.setPWM(speed);
	data.speed = speed;
}

/**
 * Reverse
 */
void Driving::reverse()
{
	//motorForward.setPWM(-(FORWARD_SPEED));
	data.speed = -(FORWARD_SPEED);
}

/**
 * Stop forward engine
 */
void Driving::stop()
{
	//motorForward.setPWM(0);
	data.speed = 0;
}

/**
 * Stop both forward and turn engines.
 * TODO: Remember that this should interrupt tasks as well
 */
void Driving::halt()
{
	motorForward.setPWM(0);
	motorTurn.setPWM(0);
	data.speed = 0;
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
			if (count >= lastPos - TURN_JITTER && count <= lastPos + TURN_JITTER) {
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
			if (count >= lastPos - TURN_JITTER && count <= lastPos + TURN_JITTER && count < rightPos - 10) {
				leftPos = lastPos;
				direction = -2;
				motorTurn.setPWM(0);
			}
		}
		// Center
		else if (direction == -2)
		{
			if (count >= (leftPos + rightPos) / 2 - TURN_JITTER && count <= (leftPos + rightPos) / 2 + TURN_JITTER)
			{
				direction = 0;
				motorTurn.setCount(0);
				motorTurn.setPWM(0);
				this->data.calibrated = true;

				break;
			} else {
				center = (leftPos + rightPos) / 2;

				if (this->getTurnCount() < center)
					motorTurn.setPWM(TURN_SPEED);
				else
					motorTurn.setPWM(-TURN_SPEED);
			}
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
	return this->data.calibrated;
}

/**
 *
 * @return bool Centered
 */
bool Driving::isCentered()
{
	if (this->isCalibrated())
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
	if (!this->isCalibrated())
		return false;

	const int count = this->getTurnCount();

	if (count >= angle - TURN_JITTER && count <= angle + TURN_JITTER)
		return true;
	else
		return false;
}

/**
 * Set turning angle
 */
bool Driving::setTurnAngle(int angle)
{
	if (!this->isCalibrated())
		return false;

	const int count = this->getTurnCount();

	if (!this->isAtAngle(angle))
	{

		if (count <= angle + TURN_JITTER)
			motorTurn.setPWM(TURN_SPEED);
		else if (count >= angle - TURN_JITTER)
			motorTurn.setPWM(-TURN_SPEED);
		else
			motorTurn.setPWM(0); // If we get in here something is broken

	} else {
		motorTurn.setPWM(0);
	}

	return true;
}
