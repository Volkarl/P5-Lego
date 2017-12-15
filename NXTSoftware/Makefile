# Target specific macros
TARGET = usb

TARGET_CPP_SOURCES = \
        program.cpp \
        ../Shared/Tools.cpp \
        Driving.cpp \
        Communication.cpp \
        ../Shared/objects/rectangle.cpp \
        ../Shared/objects/detector.cpp \
        SensorControllers/CamController.cpp \
        SensorControllers/UltrasonicSensorController.cpp \
        SensorControllers/DisplayController.cpp \
        Components/BusStopDetection/BusStopDetectionComponent.cpp \
        Components/Driver/DrivingComponent.cpp \
        Components/ObstacleDetection/ObstacleDetectionComponent.cpp \
        Components/SpeedZoneDetection/SpeedZoneDetectionComponent.cpp \
        Components/StayWithinLane/StayWithinLaneComponent.cpp



TOPPERS_OSEK_OIL_SOURCE = ./program.oil

# Don't modify below part
O_PATH ?= build

# makefile for C++(.cpp) build
include ../../ecrobot/ecrobot++.mak
