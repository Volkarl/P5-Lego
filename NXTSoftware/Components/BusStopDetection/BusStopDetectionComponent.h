//
// Created by karlz on 29/11/2017.
//

#ifndef P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
#define P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H

#include "../../SensorControllers/ColourSensorController.h"
#include "../../SteeringTypes.h"

class BusStopDetectionComponent {
public:
    BusStopDetectionComponent(ColourSensorController* colorSensor);
    //bool CalculateSteering(SteeringCommand* command);

    bool DetectBusStop();
    bool IsBusStopSequenceOngoing(int cmDrivenSinceLastCall);
    void GetNextBusStopCommand(SteeringCommand *command, int cmDrivenSinceLastCall);

private:
    ColourSensorController* ColourSensor;
    SteeringSequence StoppingSequence;
};

#endif //P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
