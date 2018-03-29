TEMPLATE = lib
TARGET = Logger
CONFIG += static 
DEPENDPATH += .
INCLUDEPATH += .
 				
# Input
HEADERS += GLogger.h
HEADERS += GLogConfig.h
HEADERS += GLogManager.h

SOURCES += GLogger.cpp
SOURCES += GLogConfig.cpp
SOURCES += GLogManager.cpp

# Common PRI Include
ROOT = ../../..
INCLUDEPATH += $${ROOT}
include($${ROOT}/displaymanager.pri)
