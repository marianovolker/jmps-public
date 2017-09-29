CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = displaymanagermsgs infoscreenmsg

# Common PRI Include
ROOT = ../..
include($${ROOT}/unittest.pri)
