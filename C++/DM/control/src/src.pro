TEMPLATE = lib
TARGET = Control
CONFIG += static
QT += xml
QPRO_PWD = $$PWD 

include(displaymanagermsgs/base/base.pri)
include(displaymanagermsgs/command/command.pri)
include(displaymanagermsgs/data/data.pri)
include(displaymanagermsgs/factory/factory.pri)
include(displaymanagermsgs/utils/utils.pri)
include(displaymanagermsgs/xml/xml.pri)

include(infoscreenmsg/base/base.pri)
include(infoscreenmsg/command/command.pri)
include(infoscreenmsg/data/data.pri)
include(infoscreenmsg/factory/factory.pri)
include(infoscreenmsg/utils/utils.pri)

# Common PRI Include
ROOT = ../..
include($${ROOT}/displaymanager.pri)

INCLUDEPATH += $${ROOT}
