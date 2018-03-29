TEMPLATE = lib
TARGET = BaseUtils
CONFIG += static 
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += GAbsCmdDataFactory.h GAbsCommand.h GAbsData.h GException.h
SOURCES += GAbsCmdDataFactory.cpp GAbsCommand.cpp GAbsData.cpp GException.cpp

# Common PRI Include
ROOT = ../../..
include($${ROOT}/displaymanager.pri)

INCLUDEPATH += $${ROOT}

