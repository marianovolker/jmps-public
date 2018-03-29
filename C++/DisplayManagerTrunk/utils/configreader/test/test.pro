CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = gconfigreader

# Common PRI Includes
ROOT = ../../..
include($${ROOT}/unittest.pri)
