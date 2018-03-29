TEMPLATE = lib
TARGET = CharMapping
CONFIG += static 
DEPENDPATH += .
INCLUDEPATH += .

 				
# Input
HEADERS += GCharMapping.h
HEADERS += GCharMappingException.h

SOURCES += GCharMapping.cpp
SOURCES += GCharMappingException.cpp


# Common PRI Include
ROOT = ../../..
INCLUDEPATH += $${ROOT}
include($${ROOT}/displaymanager.pri)
