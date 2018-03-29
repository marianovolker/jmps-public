CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = media

# Common PRI Includes
ROOT = ../../..
include($${ROOT}/unittest.pri)
