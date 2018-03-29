CONFIG  += ordered warn_on
TEMPLATE = subdirs
SUBDIRS  = controlchannel

# Common PRI Includes
ROOT = ../
include($${ROOT}/unittest.pri)
