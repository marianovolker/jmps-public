CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS =

# Common PRI Includes
ROOT = ../..
include($${ROOT}/unittest.pri)
