#include "SteeringTypes.cpp"


/////////////// Component Interfaces ///////////////

class IDriving
{
   // Handles all initialization and calls to the other components, prioritises and saves their results, and calls the MotorController to steer the bus
   public:
        virtual void DetectLanes() = 0;
        virtual void DetectObstacles() = 0;
        virtual void DetectBusStop() = 0;
        virtual void DetectSpeedZone() = 0;

        virtual void Steer() = 0;
        virtual void InitializeSystem() = 0;
        virtual ~IDriving() = default;

        SteeringCommand CurrentSteeringCommand;
        int CurrentSteeringInstructionIndex;
};

class ISteeringSuggestionComponent
{
   public: 
        virtual SteeringSequence CalculateSteering() = 0;
        virtual ~ISteeringSuggestionComponent() = default;
};

class IStayWithinLane : public ISteeringSuggestionComponent
{
   public:
        virtual ~IStayWithinLane() = default;;
};

class IObstacleDetection : public ISteeringSuggestionComponent
{
   public:
        virtual ~IObstacleDetection() = default;
};

class IBusStopDetection : public ISteeringSuggestionComponent
{
   public:
        virtual ~IBusStopDetection() = default;
};

class ISpeedZoneDetection : public ISteeringSuggestionComponent
{
   public:
        virtual ~ISpeedZoneDetection() = default;
};
