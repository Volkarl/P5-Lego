# Target specific macros
TARGET = usb

TARGET_CPP_SOURCES = \
        program.cpp \
        ../Shared/Tools.cpp \
        Driving.cpp \
        Communication.cpp \
        ../Shared/objects/rectangle.cpp \
        SensorControllers/UltrasonicSensorController.cpp \
        Components/ObstacleDetection/ObstacleDetectionComponent.cpp

TOPPERS_OSEK_OIL_SOURCE = ./program.oil

# Don't modify below part
O_PATH ?= build

# makefile for C++(.cpp) build
include ../../ecrobot/ecrobot++.mak
