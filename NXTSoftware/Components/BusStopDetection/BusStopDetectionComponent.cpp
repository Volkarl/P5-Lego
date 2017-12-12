//
// Created by karlz on 29/11/2017.
//

#include "BusStopDetectionComponent.h"

BusStopDetectionComponent::BusStopDetectionComponent(ColourSensorController* colorSensor){
    this->ColourSensor = colorSensor;
}

bool BusStopDetectionComponent::CalculateSteering(SteeringSequence *sequence){

    int SteeringArray[5][3];   //Creates a 5*3 array of the steering sequence data.
    //The numbers in the rows are: 0=angle, 1=speed, 2=time.

    //Trying to hardcode it for now.
    SteeringArray[0][0] = 35; //Turns 35 degrees to the right.
    SteeringArray[0][1] = 100;  //100 means full power
    SteeringArray[0][2] = 1;  //1 seconds put here for now. This is a guesstimate.
    SteeringArray[1][0] = -35; //Turns 35 degrees to the left.
    SteeringArray[1][1] = 100;
    SteeringArray[1][2] = 1;
    SteeringArray[2][0] = -35;
    SteeringArray[2][1] = 0; //Put at 0 speed to let passengers on board.
    SteeringArray[2][2] = 5; //Giving the passengers 5 seconds to get on and off for now.
    SteeringArray[3][0] = -35;
    SteeringArray[3][1] = 100;
    SteeringArray[3][2] = 1;
    SteeringArray[4][0] = 35;
    SteeringArray[4][1] = 100;
    SteeringArray[4][2] = 1;

    //return (SteeringArray);
    return false; //The angle, how fast it drives, and how long it drives.
    //It needs to return an array the above mentioned items.
    //The first point in the array is to turn the bus right into the bus lane.
    //After it has driven a sufficient amount the wheels will turn to the left, and stay that way until
    //The bus has arrived at the bus stop.
    //After waiting for a short duration the bus will turn left and drive out into traffic again.
    //After a certain point the wheels will drive to the right again to straighten the bus out.
    //When the bus is straightened out the wheels will go back to being straight.


}
