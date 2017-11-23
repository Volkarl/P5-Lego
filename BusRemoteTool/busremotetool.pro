#-------------------------------------------------
#
# Project created by QtCreator 2017-10-28T10:57:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = busremotetool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../Shared/Tools.cpp \
    camerawidget.cpp \
    main.cpp \
    mainwindow.cpp \
    includes/nxtcommfantom/nxtcommfantom.cpp \
    commcamera.cpp

HEADERS += \
    includes/nxtcommfantom/fantom/iFile.h \
    includes/nxtcommfantom/fantom/iFileIterator.h \
    includes/nxtcommfantom/fantom/iModule.h \
    includes/nxtcommfantom/fantom/iModuleIterator.h \
    includes/nxtcommfantom/fantom/iNXT.h \
    includes/nxtcommfantom/fantom/iNXTIterator.h \
    includes/nxtcommfantom/fantom/platform.h \
    includes/nxtcommfantom/fantom/tStatus.h \
    includes/nxtcommfantom/comm.h \
    includes/nxtcommfantom/linuxcomm.h \
    includes/nxtcommfantom/NXT++.h \
    includes/nxtcommfantom/visatype.h \
    ../Shared/Connectivity.h \
    ../Shared/Tools.h \
    camerawidget.h \
    mainwindow.h \
    includes/nxtcommfantom/nxtcommfantom.h \
    commcamera.h

INCLUDEPATH += \
    $$PWD/../Shared

FORMS += \
    mainwindow.ui \
    camerawidget.ui

LIBS += \
   $$PWD/includes/fantom/fantom.lib
