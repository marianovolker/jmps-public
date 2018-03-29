QT += testlib
QT -= gui
TEMPLATE = app
TARGET = runTestGConfigEnvironment
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestGConfigEnvironment.h
SOURCES += TestGConfigEnvironment.cpp

# Common PRI Includes
ROOT = ../../../..
INCLUDEPATH += $${ROOT}
include($${ROOT}/unittest.pri)

#Library Includes
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/utils/configenvironment/src/libconfigEnvironment.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a

QMAKE_CLEAN += *.log
