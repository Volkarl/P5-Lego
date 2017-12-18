#include "stdio.h"
#include "../Shared/Tools.h"
#include "../Shared/Connectivity.h"

// Old stuff todo
#include "Driving.h"
#include "Communication.h"
#include "../Shared/objects/detector.h"

// Sensor Controllers
#include "SensorControllers/DisplayController.h"
#include "SensorControllers/UltrasonicSensorController.h"
#include "SensorControllers/CamController.h"

// Components
#include "Components/BusStopDetection/BusStopDetectionComponent.h"
#include "Components/Driver/DrivingComponent.h"
#include "Components/ObstacleDetection/ObstacleDetectionComponent.h"
#include "Components/SpeedZoneDetection/SpeedZoneDetectionComponent.h"
#include "Components/StayWithinLane/StayWithinLaneComponent.h"


// ECRobot++ API
#include "Nxt.h"
#include "Camera.h"
#include "SonarSensor.h"
#include "ColorSensor.h"
#include "Usb.h"

using namespace ecrobot;

extern "C" {

#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

/* OSEK DECLARATIONS */
DeclareCounter(SysTimerCnt);
//DeclareAlarm(AlarmUpdateSonar);
DeclareEvent(EventSleepI2C);
DeclareEvent(EventSleep);

DeclareAlarm(AlarmDetectBusStop);
DeclareAlarm(AlarmDetectObstacles);
DeclareAlarm(AlarmDetectSpeedZone);
DeclareAlarm(AlarmCamUpdate);
DeclareAlarm(AlarmDrive);

/* Nxt Input Ports */
ColorSensor colorSensor(PORT_1);
SonarSensor sonar(PORT_2);
Camera camera(PORT_4); // TODO: Remove this from here

/* Nxt Output Ports */
Motor motorForward(PORT_B);
Motor motorTurn(PORT_A);

/* NXT Objects */
Nxt nxt;
Clock clockfuck;
Usb usb;

/* Sensor Controllers */
DisplayController displayController;
UltrasonicSensorController ultrasonicSensorController(&sonar);
CamController cam(&clockfuck, &camera);
Detector detector;


/* Components */
BusStopDetectionComponent busStopDetectionComponent;
ObstacleDetectionComponent obstacleDetectionComponent(&ultrasonicSensorController);
SpeedZoneDetectionComponent speedZoneDetectionComponent;
StayWithinLaneComponent stayWithinLaneComponent;

DrivingComponent drivingComponent(&stayWithinLaneComponent, &obstacleDetectionComponent, &busStopDetectionComponent,
&speedZoneDetectionComponent, &displayController);

// TODO OLD Stuff below (not old)
Driving driving(&motorForward, &motorTurn);
Communication communication(&usb, &cam, &colorSensor, &driving, &displayController);

long msPassed = 0;

/* nxtOSEK hook to be invoked from an ISR in category 2 */
void user_1ms_isr_type2(void)
{
    msPassed++;
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

TASK(TaskUpdateSonar)
{
    drivingComponent.DetectObstacles();
    TerminateTask();

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

TASK(TaskDetectBusStop)
{
    drivingComponent.DetectBusStop();
    TerminateTask();
}

TASK(TaskDetectObstacles)
{
    if(obstacleDetectionComponent.DetectObstacles())
        driving.halt();
    else
        driving.data.halt = false;

    TerminateTask();
}

TASK(TaskDetectSpeedZone)
{
    drivingComponent.DetectSpeedZone();
    TerminateTask();
}

TASK(TaskCamUpdate)
{
	//drivingComponent.DetectLanes();

	cam.Update();
	cam.UpdateBuffer();

    TerminateTask();
}

TASK(TaskDrive)
{
	if(communication.m_Controller == Controller::NXT) {
		CamBuffer buffdata = cam.GetBuffer();
		detector.MarkData(buffdata);

		DrivingData detectordata = detector.GetAdvisedDrivingData();
		driving.data.speed = detectordata.speed;
		driving.data.angle = -detectordata.angle;
	}

	driving.update();

    TerminateTask();
}

/*
TASK(TaskMain)
{
	// Useless calibrations
    displayController.Calibrate();
    ultrasonicSensorController.Calibrate();

	driving.calibrate(); // Finding the center
	cam.Calibrate(); // Calling calibrate, as running it in the constructor seems to not work :S

    displayController.SetText("USB");

    SetRelAlarm(AlarmCamUpdate, 25, 175);
    //SetRelAlarm(AlarmDetectObstacles, 200, 1000);
	SetRelAlarm(AlarmDrive, 150, 150);

    while(1)
    {
        communication.handle();
    }
}
*/

void functionToTest(){
    cam.Update();
    cam.UpdateBuffer();
}

void TestingMain(){
    driving.calibrate(); // Finding the center
    cam.Calibrate(); // Calling calibrate, as running it in the constructor seems to not work :S
    communication.m_Controller = Controller::NXT;

    double duration = 0;
    double highest = 0;
    double all = 0;
    msPassed = 0;

    for (int i = 0; i < 1000; ++i) {
        all += msPassed;
        msPassed = 0;


        functionToTest();

        duration = msPassed;
        if(duration > highest) highest = duration;
    }

    displayController.SetText("All: ", (int) all, 0);

    clockfuck.wait(5000);

    displayController.SetText("Highest: ", (int) highest, 0);

    while (1){}
}


TASK(TaskMain){
        TestingMain();
}



}

