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
        IComponents.cpp SteeringTypes.cpp ISensorControllers.cpp)

add_executable(Camera ${SOURCE_FILES})