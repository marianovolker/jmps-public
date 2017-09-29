QT += testlib
QT += xml
#QT -= gui
TEMPLATE = app
TARGET = runTestParser
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestParser.h
SOURCES += TestParser.cpp

# Mocking object 
HEADERS += moc_GISView.h
SOURCES += moc_GISView.cpp

# Common PRI Includes
ROOT = ../../../..
include($${ROOT}/unittest.pri)

INCLUDEPATH += $${ROOT}
QMAKE_DISTCLEAN += Model.xml DisplayManager.log

# Library Includes
LIBS += $${ROOT}/control/src/libControl.a
LIBS += $${ROOT}/model/src/libModel.a
LIBS += $${ROOT}/utils/image/src/libImageUtils.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/config/libDisplayManagerConfig.a
LIBS += $${ROOT}/utils/configreader/src/libconfigReader.a
LIBS += $${ROOT}/utils/configenvironment/src/libconfigEnvironment.a
