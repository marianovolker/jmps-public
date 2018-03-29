# __author__ = 'chaudk'
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'showUI.ui'
#
# Created: Thu Mar 20 10:58:58 2014
#      by: pyside-uic 0.2.15 running on PySide 1.2.1
#
# WARNING! All changes made in this file will be lost!

from PySide import QtCore, QtGui

class Ui_SPOTUPloader(object):
    def setupUi(self, SpotUpdate):
        SpotUpdate.setObjectName("SpotUpdate")
        SpotUpdate.setWindowModality(QtCore.Qt.ApplicationModal)
        SpotUpdate.setEnabled(True)
        SpotUpdate.resize(495, 413)
        SpotUpdate.setMinimumSize(QtCore.QSize(495, 413))
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(SpotUpdate.sizePolicy().hasHeightForWidth())
        SpotUpdate.setSizePolicy(sizePolicy)
        SpotUpdate.setMaximumSize(QtCore.QSize(495, 413))
        icon = QtGui.QIcon()
        icon.addPixmap(QtGui.QPixmap("Icons/icogvr.jpg"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        SpotUpdate.setWindowIcon(icon)
        self.ipLabel = QtGui.QLabel(SpotUpdate)
        self.ipLabel.setGeometry(QtCore.QRect(20, 20, 51, 16))
        self.ipLabel.setFrameShape(QtGui.QFrame.NoFrame)
        self.ipLabel.setObjectName("ipLabel")
        self.upgradeProgressBar = QtGui.QProgressBar(SpotUpdate)
        self.upgradeProgressBar.setGeometry(QtCore.QRect(60, 350, 281, 21))
        self.upgradeProgressBar.setProperty("value", 0)
        self.upgradeProgressBar.setObjectName("upgradeProgressBar")
        self.logFilePath = QtGui.QLineEdit(SpotUpdate)
        self.logFilePath.setGeometry(QtCore.QRect(80, 60, 191, 21))
        self.logFilePath.setReadOnly(True)
        self.logFilePath.setObjectName("logFilePath")
        self.logFilelabel = QtGui.QLabel(SpotUpdate)
        self.logFilelabel.setGeometry(QtCore.QRect(20, 60, 41, 16))
        self.logFilelabel.setObjectName("logFilelabel")
        self.logTextEdit = QtGui.QPlainTextEdit(SpotUpdate)
        self.logTextEdit.setEnabled(True)
        self.logTextEdit.setGeometry(QtCore.QRect(10, 100, 321, 171))
        self.logTextEdit.setAutoFillBackground(False)
        self.logTextEdit.setFrameShape(QtGui.QFrame.Box)
        self.logTextEdit.setFrameShadow(QtGui.QFrame.Plain)
        self.logTextEdit.setVerticalScrollBarPolicy(QtCore.Qt.ScrollBarAlwaysOn)
        self.logTextEdit.setReadOnly(True)
        self.logTextEdit.setObjectName("logTextEdit")
        self.upgradeStatus = QtGui.QLabel(SpotUpdate)
        self.upgradeStatus.setGeometry(QtCore.QRect(340, 50, 51, 41))
        self.upgradeStatus.setFrameShape(QtGui.QFrame.NoFrame)
        self.upgradeStatus.setText("")
        self.upgradeStatus.setPixmap(QtGui.QPixmap("E:/CLO/HCR2 FW Upgrade/HCR2 Firmware Upgrade/stoneShadow.png"))
        self.upgradeStatus.setScaledContents(False)
        self.upgradeStatus.setAlignment(QtCore.Qt.AlignCenter)
        self.upgradeStatus.setTextInteractionFlags(QtCore.Qt.NoTextInteraction)
        self.upgradeStatus.setObjectName("upgradeStatus")
        self.updateStatuslabel = QtGui.QLabel(SpotUpdate)
        self.updateStatuslabel.setGeometry(QtCore.QRect(10, 340, 46, 41))
        self.updateStatuslabel.setObjectName("updateStatuslabel")
        self.Connect = QtGui.QPushButton(SpotUpdate)
        self.Connect.setGeometry(QtCore.QRect(400, 20, 75, 23))
        self.Connect.setObjectName("Install")
        self.Save = QtGui.QPushButton(SpotUpdate)
        self.Save.setGeometry(QtCore.QRect(400, 60, 75, 23))
        self.Save.setObjectName("Save")
        self.Exit = QtGui.QPushButton(SpotUpdate)
        self.Exit.setGeometry(QtCore.QRect(400, 100, 75, 23))
        self.Exit.setObjectName("Exit")
        self.Iptext1 = QtGui.QLineEdit(SpotUpdate)
        self.Iptext1.setGeometry(QtCore.QRect(80, 20, 41, 20))
        self.Iptext1.setObjectName("Iptext1")
        self.Iptext2 = QtGui.QLineEdit(SpotUpdate)
        self.Iptext2.setGeometry(QtCore.QRect(130, 20, 41, 20))
        self.Iptext2.setObjectName("Iptext2")
        self.Iptext3 = QtGui.QLineEdit(SpotUpdate)
        self.Iptext3.setGeometry(QtCore.QRect(180, 20, 41, 20))
        self.Iptext3.setObjectName("Iptext3")
        self.Iptext4 = QtGui.QLineEdit(SpotUpdate)
        self.Iptext4.setGeometry(QtCore.QRect(230, 20, 41, 20))
        self.Iptext4.setObjectName("Iptext4")
        self.SpotUpdatelabel = QtGui.QLabel(SpotUpdate)
        self.SpotUpdatelabel.setGeometry(QtCore.QRect(10, 390, 81, 21))
        self.SpotUpdatelabel.setObjectName("SpotUpdatelabel")
        self.Check = QtGui.QLabel(SpotUpdate)
        self.Check.setGeometry(QtCore.QRect(140, 280, 80, 60))
        self.Check.setText("")
        self.Check.setObjectName("Check")
        self.Check.setFrameShape(QtGui.QFrame.NoFrame)
        self.Check.setText("")
        self.GVR = QtGui.QPushButton(SpotUpdate)
        self.GVR.setGeometry(QtCore.QRect(394, 290, 80, 80))

        self.GVR.setObjectName("GVR")
        gvricon = QtGui.QIcon()
        gvricon.addPixmap(QtGui.QPixmap("Icons/gvr.jpg"), QtGui.QIcon.Normal, QtGui.QIcon.Off)
        self.GVR.setIcon(gvricon)
        self.GVR.setIconSize(QtCore.QSize(80, 500))

        self.retranslateUi(SpotUpdate)
        QtCore.QObject.connect(self.Connect, QtCore.SIGNAL("clicked()"), SpotUpdate.connectIP)
        QtCore.QObject.connect(self.Save, QtCore.SIGNAL("clicked()"), SpotUpdate.selectLogFile)
        QtCore.QObject.connect(self.Exit, QtCore.SIGNAL("clicked()"), SpotUpdate.exitSpotUploader)
        QtCore.QMetaObject.connectSlotsByName(SpotUpdate)

    def retranslateUi(self, SpotUpdate):
        SpotUpdate.setWindowTitle(QtGui.QApplication.translate("SpotUpdate", "Spot Uploader", None, QtGui.QApplication.UnicodeUTF8))
        self.ipLabel.setText(QtGui.QApplication.translate("SpotUpdate", "SPOT IP", None, QtGui.QApplication.UnicodeUTF8))
        self.logFilelabel.setText(QtGui.QApplication.translate("SpotUpdate", "Log File", None, QtGui.QApplication.UnicodeUTF8))
        self.updateStatuslabel.setText(QtGui.QApplication.translate("SpotUpdate", "Upload \n"
                                                                    " Status", None, QtGui.QApplication.UnicodeUTF8))
        self.Connect.setText(QtGui.QApplication.translate("SpotUpdate", "Install", None, QtGui.QApplication.UnicodeUTF8))
        self.Save.setText(QtGui.QApplication.translate("SpotUpdate", "Save", None, QtGui.QApplication.UnicodeUTF8))
        self.Exit.setText(QtGui.QApplication.translate("SpotUpdate", "Exit", None, QtGui.QApplication.UnicodeUTF8))
        self.SpotUpdatelabel.setText(QtGui.QApplication.translate("SpotUpdate", "Not Connected", None, QtGui.QApplication.UnicodeUTF8))
        self.Check.setText(QtGui.QApplication.translate("SpotUpdate", "", None, QtGui.QApplication.UnicodeUTF8))
        self.GVR.setText(QtGui.QApplication.translate("SpotUpdate", "", None, QtGui.QApplication.UnicodeUTF8))
