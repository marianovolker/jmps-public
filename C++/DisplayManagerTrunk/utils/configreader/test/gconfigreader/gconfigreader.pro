QT += testlib
QT -= gui
TEMPLATE = app
TARGET = runTestGconfigReader
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestGConfigReader.h
SOURCES += TestGConfigReader.cpp

# Common PRI Includes
ROOT = ../../../..
INCLUDEPATH += $${ROOT}
include($${ROOT}/unittest.pri)

#Library Includes
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/utils/configreader/src/libconfigReader.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a

QMAKE_CLEAN += *.log
