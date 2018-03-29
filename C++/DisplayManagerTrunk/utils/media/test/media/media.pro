QT += testlib
QT -= gui
TEMPLATE = app
TARGET = runTestGGstDaemonMediaManager
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestGGstDaemonMediaManager.h
SOURCES += TestGGstDaemonMediaManager.cpp

# Common PRI Includes
ROOT = ../../../..
INCLUDEPATH += $${ROOT}
include($${ROOT}/unittest.pri)

#Library Includes
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/utils/media/src/libMediaUtils.a
LIBS += $${ROOT}/config/libDisplayManagerConfig.a
LIBS += $${ROOT}/utils/configenvironment/src/libconfigEnvironment.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a
LIBS += $${ROOT}/utils/configreader/src/libconfigReader.a

QMAKE_CLEAN += *.log
