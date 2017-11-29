#ifndef TEST_DRIVING_H
#define TEST_DRIVING_H

#include "Motor.h"


#define TURN_ANGLE 25
#define TURN_SPEED 45
#define TURN_JITTER 3
#define FORWARD_SPEED 38

#define DRIVE_FORWARD 1
#define DRIVE_REVERSE -1
#define DRIVE_STOP 0


struct DrivingData {
	bool halt;

	int speed;
	int angle;
	bool calibrated;
};

namespace ecrobot {
	class Driving {
		/*private:
			Motor motorDrive(unsigned char _port);
			Motor motorTurn(unsigned char _port);
		*/
		public:
			Driving();

			void forward();
			void forward(int speed);
			void reverse();
			void stop();
			void halt();

			int getTurnCount();
			bool isCentered();
			bool isCalibrated();
			bool isAtAngle(int angle);

			void calibrate();

			void center();
			bool setTurnAngle(int angle);

			void update();



		//private:
			DrivingData data;


	};
};

#endif //TEST_DRIVING_H
