CONFIG += ordered warn_on
TEMPLATE = subdirs
SUBDIRS = gclearscreenmsgdataxml
SUBDIRS += gdownloadgraphicmsgdataxml
SUBDIRS += gflytextmsgdataxml
SUBDIRS += gdiclearscreenmsgdataxml
SUBDIRS += gdidrawlinemsgdataxml
SUBDIRS += gdidrawtextmsgdataxml
SUBDIRS += gdifillrectmsgdataxml
SUBDIRS += gdigetscreensizemsgdataxml
SUBDIRS += gdirefreshscreenmsgdataxml
SUBDIRS += ginfoscreenmsgdataxml
SUBDIRS += gkeyeventmsgdataxml
SUBDIRS += gkioskidmsgdataxml
SUBDIRS += gresponsemsgdataxml
SUBDIRS += gsetpersonalitymsgdataxml
SUBDIRS += gurlmsgdataxml
SUBDIRS += grequestclassifier


ROOT = ../../..
include($${ROOT}/unittest.pri)
