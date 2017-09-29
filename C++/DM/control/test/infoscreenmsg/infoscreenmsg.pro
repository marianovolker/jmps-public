CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = parser

# Common PRI Includes
ROOT = ../../..
include($${ROOT}/unittest.pri)
