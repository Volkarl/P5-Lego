#include "stdio.h"
#include "../Shared/Tools.h"
#include "../Shared/Connectivity.h"

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
Clock clock;
Usb usb;

/* Sensor Controllers */
DisplayController displayController;
UltrasonicSensorController ultrasonicSensorController(&sonar);
CamController cam(&clock, &camera);
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

TASK(TaskDetectBusStop)
{
    drivingComponent.DetectBusStop();
    TerminateTask();
}

TASK(TaskDetectObstacles)
{
	const int dist = sonar.getDistance();

    if(dist < 20 && dist > 2)
        driving.halt();
    else
        driving.halt(false);

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
		driving.SetSpeed(detectordata.speed);
		driving.SetAngle(-detectordata.angle);
	}

	driving.Update();

    TerminateTask();
}

TASK(TaskMain)
{
	// Useless calibrations
    displayController.Calibrate();
    ultrasonicSensorController.Calibrate();

    driving.calibrate(); // Finding the center
    cam.Calibrate(); // Calling calibrate, as running it in the constructor seems to not work :S

    displayController.SetText("NXT");

    SetRelAlarm(AlarmCamUpdate, 25, 175);
    SetRelAlarm(AlarmDetectObstacles, 200, 800);
    SetRelAlarm(AlarmDrive, 150, 150);

    while(1)
    {
        communication.handle();
    }
}

}

