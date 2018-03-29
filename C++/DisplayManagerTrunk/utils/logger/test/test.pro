CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = glogger

# Common PRI Includes
ROOT = ../../..
include($${ROOT}/unittest.pri)
