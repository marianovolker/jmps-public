TEMPLATE = lib
TARGET = MediaUtils
CONFIG += static 
DEPENDPATH += .
INCLUDEPATH += .
QT += xml


# Input
HEADERS += GAbsMediaManager.h
HEADERS += GGstLaunchMediaManager.h
HEADERS += GGstDaemonMediaManager.h
HEADERS += GMediaManagerFactory.h
SOURCES += GAbsMediaManager.cpp
SOURCES += GGstLaunchMediaManager.cpp
SOURCES += GGstDaemonMediaManager.cpp
SOURCES += GMediaManagerFactory.cpp

# Common PRI Include
ROOT = ../../..
include($${ROOT}/displaymanager.pri)

INCLUDEPATH += $${ROOT}
