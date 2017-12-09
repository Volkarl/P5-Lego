//
// Created by karlz on 5/12/2017.
//

#include "UltrasonicSensorController.h"

using namespace ecrobot;

UltrasonicSensorController::UltrasonicSensorController(SonarSensor* ultrasonicSensor) {
    this->UltrasonicSensor = ultrasonicSensor;
}

void ecrobot::UltrasonicSensorController::Calibrate(){
    // Nothing happens here
}

int UltrasonicSensorController::GetDistance() {
    // Clean up data from ultrasonic sensor
    // Returns -1 for invalid measurements!


    // At the moment, we're just using the raw data
    // Perhaps use median of multiple measurements? Perhaps save old measurement for this purpose?

    /*int distance[3];
    for (int i = 0; i < 3; ++i) {
        distance[i] = UltrasonicSensor.getDistance();
    }
    // Sort after size; return the median ?? Or the average?
    */

    int distance = UltrasonicSensor->getDistance();
    return distance == 255 ? -1 : distance;
}
