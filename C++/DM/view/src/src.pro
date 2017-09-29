TEMPLATE = lib
TARGET = View
CONFIG += static
QT += webkit

include(utils/utils.pri)
include(base/base.pri)
include(gdiview/gdiview.pri)
include(infoscreenview/infoscreenview.pri)
include(urlview/urlview.pri)
    
# Common PRI Include
ROOT = ../..
include($${ROOT}/displaymanager.pri)

INCLUDEPATH += $${ROOT}
