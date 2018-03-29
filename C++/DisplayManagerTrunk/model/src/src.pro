TEMPLATE = lib
TARGET = Model
CONFIG += static
QT += xml

include(base/base.pri)
include(infoscreenmodel/infoscreenmodel.pri)
include(gdimodel/gdimodel.pri)
include(urlmodel/urlmodel.pri)
    
# Common PRI Include
ROOT = ../..
include($${ROOT}/displaymanager.pri)

INCLUDEPATH += $${ROOT}

