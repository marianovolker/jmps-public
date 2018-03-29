CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = base image logger configreader charmapping configenvironment media personality

# Common PRI Include
ROOT = ../
include($${ROOT}/displaymanager.pri)

