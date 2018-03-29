TEMPLATE = app
QT += network
QT += xml
QT += webkit
TARGET = dispmgrd
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += GControlChannelHandler.h
HEADERS += GControlChannelMgr.h
HEADERS += GControlChannelTcpServer.h
HEADERS += GDisplayManagerDaemon.h
HEADERS += GSignalHandler.h
HEADERS += GProcess.h
HEADERS += GDMQtApplication.h

SOURCES += GControlChannelHandler.cpp
SOURCES += GControlChannelMgr.cpp
SOURCES += GControlChannelTcpServer.cpp
SOURCES += GDisplayManagerDaemon.cpp
SOURCES += GSignalHandler.cpp
SOURCES += GProcess.cpp
SOURCES += mainapp.cpp
		 
# Common PRI Include
ROOT = ..
include($${ROOT}/displaymanager.pri)
INCLUDEPATH += $${ROOT}

# Library Includes
LIBS += $${ROOT}/control/src/libControl.a
LIBS += $${ROOT}/model/src/libModel.a
LIBS += $${ROOT}/view/src/libView.a
LIBS += $${ROOT}/utils/media/src/libMediaUtils.a
LIBS += $${ROOT}/utils/image/src/libImageUtils.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/config/libDisplayManagerConfig.a
LIBS += $${ROOT}/utils/configreader/src/libconfigReader.a
LIBS += $${ROOT}/utils/configenvironment/src/libconfigEnvironment.a
LIBS += $${ROOT}/utils/personality/src/libPersonality.a
LIBS += $${ROOT}/utils/charmapping/src/libCharMapping.a

QMAKE_DISTCLEAN += DisplayManager.log* Model.xml imagemappings.xml
