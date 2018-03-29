QT += testlib
QT += xml
TEMPLATE = app
TARGET = runTestGdiDrawTextMsgDataXML
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestGdiDrawTextMsgDataXML.h
SOURCES += TestGdiDrawTextMsgDataXML.cpp

# Common PRI Includes
ROOT = ../../../..
include($${ROOT}/unittest.pri)
INCLUDEPATH += $${ROOT}
QMAKE_DISTCLEAN += DisplayManager.log

# Library Includes
LIBS += $${ROOT}/control/src/libControl.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a
LIBS += $${ROOT}/utils/logger/src/libLogger.a
