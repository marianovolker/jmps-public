QT += testlib
QT += xml
QT += network

TEMPLATE = app
TARGET = runTestControlChannel
DEPENDPATH += .
INCLUDEPATH += .

# Common PRI Includes
ROOT = ../..
include($${ROOT}/unittest.pri)
INCLUDEPATH += $${ROOT}

# Input
HEADERS += TestControlChannel.h
HEADERS += ../../src/GControlChannelHandler.h
HEADERS += ../../src/GControlChannelMgr.h
HEADERS += ../../src/GControlChannelTcpServer.h
SOURCES += TestControlChannel.cpp
SOURCES += ../../src/GControlChannelHandler.cpp
SOURCES += ../../src/GControlChannelMgr.cpp
SOURCES += ../../src/GControlChannelTcpServer.cpp

# Mocking object 
HEADERS += moc_GISView.h
SOURCES += moc_GURLModel.cpp
SOURCES += moc_GGDIModel.cpp
SOURCES += moc_GISView.cpp
SOURCES += moc_GDownloadGraphicMsgCommand.cpp
SOURCES += moc_GSetPersonalityMsgCommand.cpp

# Library Includes
LIBS += $${ROOT}/control/src/libControl.a
LIBS += $${ROOT}/model/src/libModel.a
LIBS += $${ROOT}/utils/media/src/libMediaUtils.a
LIBS += $${ROOT}/utils/image/src/libImageUtils.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/config/libDisplayManagerConfig.a
LIBS += $${ROOT}/utils/configreader/src/libconfigReader.a
LIBS += $${ROOT}/utils/configenvironment/src/libconfigEnvironment.a
LIBS += $${ROOT}/utils/personality/src/libPersonality.a
LIBS += $${ROOT}/utils/charmapping/src/libCharMapping.a
