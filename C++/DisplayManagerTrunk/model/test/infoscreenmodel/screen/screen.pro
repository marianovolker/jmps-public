TARGET = runTestScreen
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestScreen.h moc_GISView.h
SOURCES += TestScreen.cpp moc_GISView.cpp

# Common PRI Includes
ROOT = ../../../..
include($${ROOT}/unittest.pri)

INCLUDEPATH += $${ROOT}
QMAKE_DISTCLEAN += DisplayManager.log

# Library Includes
LIBS += $${ROOT}/model/src/libModel.a
LIBS += $${ROOT}/utils/logger/src/libLogger.a
LIBS += $${ROOT}/utils/image/src/libImageUtils.a
LIBS += $${ROOT}/utils/base/src/libBaseUtils.a
