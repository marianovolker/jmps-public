TEMPLATE = lib
TARGET = DisplayManagerConfig
CONFIG += static
QPRO_PWD = $$PWD 

HEADERS += 	GDisplayManagerConfig.h 
SOURCES += 	GDisplayManagerConfig.cpp


# Common PRI Include
ROOT = ..
include($${ROOT}/displaymanager.pri)

INCLUDEPATH += $${ROOT}
