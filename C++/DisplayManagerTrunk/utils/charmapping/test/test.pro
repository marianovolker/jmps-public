CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = charmapping

# Common PRI Includes
ROOT = ../../..
include($${ROOT}/unittest.pri)
