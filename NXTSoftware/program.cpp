#include "stdio.h"
#include "../Shared/Tools.h"
#include "../Shared/Connectivity.h"

// Old stuff todo
#include "Driving.h"
#include "Communication.h"

// Sensor Controllers
#include "SensorControllers/ColourSensorController.h"
#include "SensorControllers/DisplayController.h"
#include "SensorControllers/NxtCamLineTrackingController.h"
#include "SensorControllers/SteeringController.h"
#include "SensorControllers/UltrasonicSensorController.h"

// Components
#include "Components/BusStopDetection/BusStopDetectionComponent.h"
#include "Components/Driver/DrivingComponent.h"
#include "Components/ObstacleDetection/ObstacleDetectionComponent.h"
#include "Components/SpeedZoneDetection/SpeedZoneDetectionComponent.h"
#include "Components/StayWithinLane/StayWithinLaneComponent.h"


// ECRobot++ API
//#include "Lcd.h"        Gets included in Displaycontroller.h
#include "Nxt.h"
//#include "Clock.h"      Gets included in UltrasonicSensorController.h.
#include "Camera.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "Usb.h"

using namespace ecrobot;

extern "C"{

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

/* Nxt Input Ports */
ColorSensor colorSensor(PORT_1);
SonarSensor sonar(PORT_2);
Camera camera(PORT_4);

/* Nxt Output Ports */
Motor motorForward(PORT_B);
Motor motorTurn(PORT_A);

/* NXT Objects */
Lcd lcd;
Nxt nxt;
Clock clock;
Usb usb;

/* Sensor Controllers */
ColourSensorController colourSensorController(&colorSensor);
DisplayController displayController(&lcd);
NxtCamLineTrackingController nxtCamLineTrackingController(&camera, 3, 20); //Todo: is 3 cm. correct??
SteeringController steeringController(&motorForward, &motorTurn);
UltrasonicSensorController ultrasonicSensorController(&sonar);

/* Components */
BusStopDetectionComponent busStopDetectionComponent(&colourSensorController);
ObstacleDetectionComponent obstacleDetectionComponent(&ultrasonicSensorController);
SpeedZoneDetectionComponent speedZoneDetectionComponent(&colourSensorController);
StayWithinLaneComponent stayWithinLaneComponent(&nxtCamLineTrackingController);

DrivingComponent drivingComponent(&stayWithinLaneComponent, &obstacleDetectionComponent, &busStopDetectionComponent,
&speedZoneDetectionComponent, &steeringController, &displayController);

// TODO OLD Stuff below
Driving driving(&motorForward, &motorTurn);
Communication communication(&usb, &camera, &colorSensor, &driving);


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

void DisplayAndWait(char* textToDisplay){
    displayController.SetText(textToDisplay);
    clock.wait(1000);
}

TASK(TaskUpdateSonar)
{

    drivingComponent.DetectObstacles();
    TerminateTask();

// Something that was already commented out prior to my involvement
//S16 color[3];

	//colorSensor.getRawColor(color);

	// Validate
	/*if (color[0] < 0 && color[0] > 255 &&
		color[1] < 0 && color[1] > 255 &&
		color[2] < 0 && color[2] > 255)
	{ return; }

	driving.data.color.red = color[0];
	driving.data.color.green = color[1];
	driving.data.color.blue = color[2];*/

// Jakob's old code for ultrasonic sensor
/*
    int distance = ultrasonicSensorController.GetDistanceFast();
	if (distance < 15 && distance > 5)
		driving.halt();
	else
		driving.data.halt = false; // TODO: Change to new function

    TerminateTask();
    */
}

TASK(drivingUpdate)
{
	driving.update();
	TerminateTask();
}

TASK(TaskMain)
{
//	U8 data[MAX_USB_DATA_LEN]; // first byte is preserved for disconnect request from host
    ultrasonicSensorController.Calibrate();  //Currently does nothing.

	driving.calibrate();

    displayController.SetText("USB");

	camera.sendCommand('L'); // Line mode
	clock.wait(10);
	camera.sendCommand('X'); // Sort NONE
	clock.wait(25);
	camera.enableTracking(true);
    SetRelAlarm(AlarmUpdateCam, 25, 350);
	SetRelAlarm(AlarmDrivingUpdate, 150, 150);

	while(1)
  	{
		communication.handle();
        if (nxt.getButtons() == Nxt::ENTR_ON)
        {
            displayController.SetText("Dist: ",ultrasonicSensorController.GetDistanceFast(), 0);
        }
  	}
}

}

