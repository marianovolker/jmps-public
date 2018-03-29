CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = config utils view model control src

## Include project defaults from .pri
include(displaymanager.pri)

QMAKE_DEL_DIR = rm -rf
QMAKE_DEL_FILE = rm -rf
QMAKE_CLEAN += config/glibrary.min.js
QMAKE_CLEAN += src/bin build-stamp 
QMAKE_CLEAN += debian/dirs debian/displaymanager/ 
QMAKE_CLEAN += debian/displaymanager.postrm.debhelper
QMAKE_CLEAN += debian/displaymanager.prerm.debhelper 
QMAKE_CLEAN += debian/displaymanager.postinst.debhelper
QMAKE_CLEAN += debian/files 
QMAKE_CLEAN += debs
QMAKE_CLEAN += debian/tmp
QMAKE_CLEAN += debian/postrm.debhelper
QMAKE_CLEAN += debian/prerm.debhelper 
QMAKE_CLEAN += debian/postinst.debhelper

