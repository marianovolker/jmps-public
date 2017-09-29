CONFIG  += ordered warn_on
TEMPLATE = subdirs
SUBDIRS  = display imagemappingsimporterexporter model modelimporterexporter screen segment textfield

# Common PRI Includes
ROOT = ../../..
include($${ROOT}/unittest.pri)
