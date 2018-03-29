CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = infoscreenmodel

# Common PRI Include
ROOT = ../..
include($${ROOT}/displaymanager.pri)
