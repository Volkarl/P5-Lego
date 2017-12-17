#ifndef P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
#define P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H

#include "../../SteeringTypes.h"

class BusStopDetectionComponent {
public:
    BusStopDetectionComponent();
    //bool CalculateSteering(SteeringCommand* command);

    bool DetectBusStop();
    bool IsBusStopSequenceOngoing(int cmDrivenSinceLastCall);
    void GetNextBusStopCommand(SteeringCommand *command, int cmDrivenSinceLastCall);

private:
    SteeringSequence StoppingSequence;
};

#endif //P5_LEGO_BUSSTOPDETECTIONCOMPONENT_H
