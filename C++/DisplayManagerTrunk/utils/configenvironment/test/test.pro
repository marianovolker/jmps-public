CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = gconfigenvironment

# Common PRI Includes
ROOT = ../../..
include($${ROOT}/unittest.pri)
