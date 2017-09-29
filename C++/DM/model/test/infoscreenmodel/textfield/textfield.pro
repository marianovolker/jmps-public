TARGET = runTestTextField
DEPENDPATH += .
INCLUDEPATH += .

# Input
HEADERS += TestTextField.h moc_GISView.h
SOURCES += TestTextField.cpp moc_GISView.cpp

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
