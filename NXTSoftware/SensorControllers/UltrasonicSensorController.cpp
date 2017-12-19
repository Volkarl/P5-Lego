#include "UltrasonicSensorController.h"

using namespace ecrobot;

UltrasonicSensorController::UltrasonicSensorController(SonarSensor* ultrasonicSensor) {
    this->m_UltrasonicSensor = ultrasonicSensor;
}

void UltrasonicSensorController::Calibrate()
{
    // Nothing happens here
}

int UltrasonicSensorController::GetDistancePrecise() {
    // Clean up data from ultrasonic sensor
    // Returns -1 for invalid measurements!
    // If the added accuracy is not needed, then consider going back to the faster method.
    // Futhermore the current method would also have to be changed to allow for better scheduling.

    int tempvalue=0;
    int j=0;
    int distance[5];
    for (int i = 0; i < 5; i++) {//Fills the array with distances.
        distance[i] = m_UltrasonicSensor->getDistance();
        if(distance[i] != 255){
            tempvalue = tempvalue + distance[i];
            j++;
        }
        //clock.wait(1); //Should take 1 ms to get a new measurement. The bus will have moved a little, but it will hardly matter.
    }

    if(distance[0] == 255 && distance[1] == 255 && distance[2] == 255 && distance[3] == 255 && distance[4] == 255){
        return(-1);
    }
    else{
        tempvalue = tempvalue/j; //We lose a slight amount of precision since it's an int.
        return(tempvalue);
    }
}

int UltrasonicSensorController::GetDistanceFast(){
    int distance = m_UltrasonicSensor->getDistance();
    return distance == 255 ? -1 : distance;
}

int UltrasonicSensorController::GetDistance() const
{
    return this->m_UltrasonicSensor->getDistance();
}
