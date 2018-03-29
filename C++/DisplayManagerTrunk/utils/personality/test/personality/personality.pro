QT += testlib
QT += xml
TEMPLATE = app
TARGET = runTestGPersonality
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestGPersonality.h
SOURCES += TestGPersonality.cpp

# Common PRI Includes
ROOT = ../../../..
include($${ROOT}/unittest.pri)
INCLUDEPATH += $${ROOT}
QMAKE_DISTCLEAN += DisplayManager.log* Model.xml imagemappings.xml

# Library Includes
LIBS += $${ROOT}/utils/personality/src/libPersonality.a
LIBS += $${ROOT}/utils/charmapping/src/libCharMapping.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/config/libDisplayManagerConfig.a
LIBS += $${ROOT}/utils/configreader/src/libconfigReader.a
LIBS += $${ROOT}/utils/configenvironment/src/libconfigEnvironment.a
