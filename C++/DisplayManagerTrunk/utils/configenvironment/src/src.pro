TEMPLATE = lib
TARGET = configEnvironment
CONFIG += static 
DEPENDPATH += .
INCLUDEPATH += .
 				
# Input
HEADERS += GConfigEnvironment.h 
SOURCES += GConfigEnvironment.cpp

# Common PRI Include
ROOT = ../../..
INCLUDEPATH += $${ROOT}
include($${ROOT}/displaymanager.pri)

