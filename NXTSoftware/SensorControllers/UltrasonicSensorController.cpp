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

    int tempvalue=0;
    int j=0;
    int distance[5];
    for (int i = 0; i < 5; i++) {//Fills the array with distances.
        distance[i] = UltrasonicSensor->getDistance();
        if(distance[i] != 255){
            tempvalue = tempvalue + distance[i];
            j++;
        }
        //clock.wait() is not included yet, since clock is out of this scope. Tried to #include "clock.h", but that didn't work.
        //clock.wait(1); //Should take 1 ms to get a new measurement. The bus will have moved a little, but it will hardly matter.
    }

    if(distance[0] == 255 && distance[1] == 255 && distance[2] == 255 && distance[3] == 255 && distance[4] == 255){
        return(-1);
    }
    else{
        tempvalue = tempvalue/j; //We lose a slight amount of precision since it's an int.
        return(tempvalue);
    }

    /*for (int i = 0; i < 3;) {//Fills the array with distances.
        distance[i] = UltrasonicSensor.getDistance();
        if (distance[i]!=255){
            i++;
        }
        clock.wait(1); //Should take 1 ms to get a new measurement. The bus will have moved a little, but it will hardly matter.
    }
    // Sort after size; return the median ?? Or the average?


    int distance = UltrasonicSensor->getDistance();
    return distance == 255 ? -1 : distance;
     */
}
