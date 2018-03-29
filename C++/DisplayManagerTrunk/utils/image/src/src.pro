TEMPLATE = lib
TARGET = ImageUtils
CONFIG += static 
DEPENDPATH += .
INCLUDEPATH += .
QT += xml


# Input
HEADERS += GColor.h GColorPalette.h GImageMap.h GImageMappings.h GImageMappingsExporter.h GImageMappingsImporter.h GImageJob.h GImageConverter.h GImageHelper.h
SOURCES += GColor.cpp GColorPalette.cpp GImageMap.cpp GImageMappings.cpp GImageMappingsExporter.cpp GImageMappingsImporter.cpp GImageJob.cpp GImageConverter.cpp GImageHelper.cpp

# Common PRI Include
ROOT = ../../..
include($${ROOT}/displaymanager.pri)

INCLUDEPATH += $${ROOT}
