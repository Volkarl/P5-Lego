cmake_minimum_required(VERSION 3.8)
project(Camera)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        BezierCurve.cpp
        BezierCurve.h
        CamCalcTypes.h
        DriveLogic.cpp
        DriveLogic.h
        FieldOfViewCorrector.cpp
        FieldOfViewCorrector.h
        IComponents.h SteeringTypes.cpp ISensorControllers.cpp program.cpp Driving.cpp Driving.h StayWithinLaneComponent.cpp StayWithinLaneComponent.h ObstacleDetectionComponent.cpp ObstacleDetectionComponent.h BusStopDetectionComponent.cpp BusStopDetectionComponent.h SpeedZoneDetectionComponent.cpp SpeedZoneDetectionComponent.h)

add_executable(Camera ${SOURCE_FILES})