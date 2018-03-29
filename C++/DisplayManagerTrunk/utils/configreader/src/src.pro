TEMPLATE = lib
TARGET = configReader
CONFIG += static 
DEPENDPATH += .
INCLUDEPATH += .
 				
# Input
HEADERS += GConfigReader.h GConfigReaderException.h 
SOURCES += GConfigReader.cpp GConfigReaderException.cpp

# Common PRI Include
ROOT = ../../..
INCLUDEPATH += $${ROOT}
include($${ROOT}/displaymanager.pri)

