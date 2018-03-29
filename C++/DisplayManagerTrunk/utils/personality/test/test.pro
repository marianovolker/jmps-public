CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = personality

# Common PRI Include
ROOT = ../../..
include($${ROOT}/unittest.pri)
