#ifndef CONNECTIVITY_DRIVING_H
#define CONNECTIVITY_DRIVING_H

#include "Motor.h"
#include "../Shared/Connectivity.h"


#define TURN_ANGLE 25
#define TURN_SPEED 50
#define TURN_JITTER 5
#define FORWARD_SPEED 38

#define DRIVE_FORWARD 1
#define DRIVE_REVERSE -1
#define DRIVE_STOP 0


struct DrivingData {
	bool halt;

	int speed;
	int angle;
	bool calibrated;

	RGB color;
};

class Driving {
	public:
		Driving();

		void forward();
		void forward(int speed);
		void reverse();
		void reverse(int speed);

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

#endif //CONNECTIVITY_DRIVING_H
