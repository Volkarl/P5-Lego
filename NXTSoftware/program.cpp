#include "stdio.h"
#include "../Shared/Tools.h"
#include "../Shared/Connectivity.h"

#include "Driving.h"
#include "Communication.h"


// ECRobot++ API
#include "Lcd.h"
#include "Nxt.h"
#include "Clock.h"
#include "Camera.h"
#include "SonarSensor.h"
#include "Usb.h"

using namespace ecrobot;

extern "C"
{
#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* OSEK DECLARATIONS */
DeclareCounter(SysTimerCnt);
DeclareAlarm(AlarmUpdateCam);
DeclareAlarm(AlarmUpdateSonar);
DeclareAlarm(AlarmDrivingUpdate);
DeclareEvent(EventSleepI2C);
DeclareEvent(EventSleep);

/**
 * Input
 */
Camera camera(PORT_1);
SonarSensor sonar(PORT_4);
int distance;

/**
 * Variables
 */
Driving driving;
Usb usb;
Communication communication(&usb, &camera, &driving);

/**
 * NXT Objects
 */
Lcd lcd;
Nxt nxt;
Clock clock;


/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
	SleeperMonitor(); // needed for I2C device and Clock classes
	usb.commHandler(); // USB communication handler
    // fun times
    StatusType ercd;

    ercd = SignalCounter(SysTimerCnt); /* Increment OSEK Alarm Counter */
    if (ercd != E_OK)
    {
        ShutdownOS(ercd);
    }
}

TASK(TaskUpdateCam)
{
    camera.update();
    TerminateTask();
}

TASK(TaskUpdateSonar)
{
    distance = sonar.getDistance();
	if (distance < 10)
		driving.halt();
	else
		driving.data.halt = false; // TODO: Change to new function

    TerminateTask();
}

TASK(drivingUpdate)
{
	driving.update();
	TerminateTask();
}


TASK(TaskMain)
{
    distance = 0;
//	U8 data[MAX_USB_DATA_LEN]; // first byte is preserved for disconnect request from host

	driving.calibrate();

	lcd.clear();
	lcd.putf("sn", "USB");
	lcd.disp();

	camera.sendCommand('L'); // Line mode
	clock.wait(10);
	camera.sendCommand('X'); // Sort NONE
	clock.wait(25);
	camera.enableTracking(true);
    SetRelAlarm(AlarmUpdateCam, 25, 400);

	driving.calibrate();
	SetRelAlarm(AlarmDrivingUpdate, 150, 150);

	while(1)
  	{
		communication.handle();
        if (nxt.getButtons() == Nxt::ENTR_ON)
        {
            lcd.clear();
            lcd.putf("sdn", "Dist: ", distance, 0);
            lcd.disp();
        }
  	}
}

}

