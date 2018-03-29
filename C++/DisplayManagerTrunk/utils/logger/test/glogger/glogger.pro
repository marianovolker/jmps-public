QT += testlib
TEMPLATE = app
TARGET = runTestGLogger
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestGLogger.h
SOURCES += TestGLogger.cpp

QMAKE_DISTCLEAN += DisplayManagerLvl1.log DisplayManagerLvl2.log DisplayManagerLvl3.log DisplayManagerLvl4.log

# Common PRI Includes
ROOT = ../../../..
INCLUDEPATH += $${ROOT}
include($${ROOT}/unittest.pri)

#Library Includes
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a
				