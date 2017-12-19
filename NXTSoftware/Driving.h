#ifndef CONNECTIVITY_DRIVING_H
#define CONNECTIVITY_DRIVING_H

#include "Motor.h"
#include "../Shared/Connectivity.h"
#include "../Shared/objects/detector.h"


#define TURN_ANGLE 25
#define TURN_SPEED 50
#define TURN_JITTER 5
#define FORWARD_SPEED 38

#define DRIVE_FORWARD 1
#define DRIVE_REVERSE -1
#define DRIVE_STOP 0

namespace ecrobot{
	class Driving {
		public:
			Driving(Motor* propulsionMotor, Motor* turnMotor);

			void forward();
			void forward(int speed);
			void reverse();
			void reverse(int speed);

			void stop();
			void halt();
			void halt(bool state);

			int getTurnCount();
			bool isCentered();
			bool isCalibrated();
			bool IsHalted();
			bool isAtAngle(int angle);

			void calibrate();

			void center();
			bool SetSpeed(int speed);
			bool SetAngle(int angle);

			int GetSpeed();

			void Update();


			DrivingData data; // public is NOT optimal, but it's a stupid shortcut

	private:
			Motor* motorForward;
			Motor* motorTurn;
			bool setTurnAngle(int angle);

	};
};

#endif //CONNECTIVITY_DRIVING_H
