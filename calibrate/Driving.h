#ifndef TEST_DRIVING_H
#define TEST_DRIVING_H

#include "Motor.h"


#define TURN_ANGLE 35
#define TURN_SPEED 50
#define TURN_JITTER 2
#define FORWARD_SPEED 70

#define TURN_LEFT -1
#define TURN_RIGHT 1
#define TURN_CENTER 2
#define TURN_STOP 0

#define DRIVE_FORWARD 1
#define DRIVE_REVERSE -1
#define DRIVE_STOP 0

namespace ecrobot {
	class Driving {
		/*private:
			Motor motorDrive(unsigned char _port);
			Motor motorTurn(unsigned char _port);
		*/
		public:
			// Driving(Motor drive, Motor turn); // TODO: Not working
			Driving();

			void turn(int direction);

			void forward();
			void reverse();
			void stop();
			void halt();

			int getTurnCount();
			bool isCentered();
			bool isCalibrated();

			void calibrate();

			void center();
	};
};

#endif //TEST_DRIVING_H
