TEMPLATE = lib
TARGET = Personality
CONFIG += static 
DEPENDPATH += .
INCLUDEPATH += .

 				
# Input
HEADERS += GPersonality.h
HEADERS += GPersonalityException.h

SOURCES += GPersonality.cpp
SOURCES += GPersonalityException.cpp


# Common PRI Include
ROOT = ../../..
INCLUDEPATH += $${ROOT}
include($${ROOT}/displaymanager.pri)
