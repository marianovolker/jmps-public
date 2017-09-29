QT += testlib
QT += xml
TEMPLATE = app
TARGET = runTestGURLMsgDataXML
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestGURLMsgDataXML.h
SOURCES += TestGURLMsgDataXML.cpp

# Common PRI Includes
ROOT = ../../../..
include($${ROOT}/unittest.pri)
INCLUDEPATH += $${ROOT}
QMAKE_DISTCLEAN += DisplayManager.log

# Library Includes
LIBS += $${ROOT}/control/src/libControl.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/config/libDisplayManagerConfig.a
LIBS += $${ROOT}/utils/configreader/src/libconfigReader.a
LIBS += $${ROOT}/utils/configenvironment/src/libconfigEnvironment.a
