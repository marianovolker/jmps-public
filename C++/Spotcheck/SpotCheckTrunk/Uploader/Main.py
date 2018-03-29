# __author__ = 'chaudk'
#!/usr/bin/python
# -*- coding: utf-8 -*-

from PySide.QtGui import *
import sys, socket, os, math, glob, shutil, re
import threading
from datetime import datetime
import showUI

import Event
import parsefile
from GVRlib import pupyspot, Interface
import spotPinpad
import Setup
import ctypes

########### GLOBAL PARAMETERS ###########
app = None
_max_count_ = 5

current_dir = os.path.dirname(os.path.abspath(sys.argv[0]))
_log_path_ = current_dir + '\\logs\\'
_resources_path_M3= os.path.join(current_dir, 'SPOT\\M3\\')
_resources_path_M5= os.path.join(current_dir, 'SPOT\\M5\\')
_resources_path = None
ack_hardware_info = None
_device_model_ = None

_ApplicationList_ ={'CRINDBIOS'}
########## Main Dialog CLASS ############
class MainWindow(QMainWindow, showUI.Ui_SPOTUPloader):
    def __init__(self, parent=None):
        super(MainWindow, self).__init__(parent)
        self.captureFile = None
        self.setupUi(self)
        if not os.path.exists(_log_path_):
            os.makedirs(_log_path_)
        self.ipaddress = None
        self.work=None

        self.threadEvent = threading.Event()
        self.parseFile = parsefile.ParseFile()
        self.spot = pupyspot.PupySPOT()
        self.SPOTPackages_updated = False
        self.logFilePath.setText(_log_path_)


    def event(self, e):
        if e.type()==Event.loggerEventType:
            self.Print(e.text,e.mon,e.log)
            return True
        if e.type()==Event.progressEventType:
            self.updateUI(e.percent, e.status)
            return True
        if e.type()==Event.disconnectEventType:
            self.processDisconnect()
            return True
        if e.type()==Event.updateUIEventType:
            self.updateGUI(e.status)
            return True
        return super(MainWindow, self).event(e)

    def updateUI(self, percent, status):
        self.upgradeProgressBar.setValue(percent)
        if percent == 0:
            self.Check.setPixmap(QPixmap(""))
        if percent == 100:
            self.Check.setPixmap(QPixmap("./Icons/Check.png"))
        if status == "FAILURE":
            self.Check.setPixmap(QPixmap("./Icons/Cross.png"))
            self.SPOTPackages_updated = False

    def processDisconnect(self):
        if self.SPOTPackages_updated:
            self.Print("SpotCheck uploaded successfully",True,True)
            
        self.spot.deregister_event('online_status', self.on_online_status)

        self.resetState()
        return
          

    def updateGUI(self, status):
         if status == "False":
           self.Exit.setEnabled(False)
           self.Save.setEnabled(False)
         else:
           self.Exit.setEnabled(True)
           self.Connect.setEnabled(True)
           self.Save.setEnabled(True)
           self.SpotUpdatelabel.setText("Not Connected")
    
    def on_online_status(self, is_online):
        if is_online == False:
            app.postEvent(self, Event.DisconnectEvent())

    def resetState(self):
        self.Connect.setEnabled(True)
        self.Exit.setEnabled(True)
        self.Save.setEnabled(True)
        if not self.spot.is_connected :
            self.SpotUpdatelabel.setText("Not Connected")


    def connectIP(self):
        ipaddress1 = self.Iptext1.text()
        ipaddress1 = ipaddress1 + "." + self.Iptext2.text()
        ipaddress1 = ipaddress1 + "." + self.Iptext3.text()
        ipaddress1 = ipaddress1 + "." + self.Iptext4.text()
        self.ipaddress = ipaddress1
        isValidIP = self.validateIP(self.ipaddress)
        if isValidIP:
            self.CreateLogFile()
            self.initConn(self.ipaddress)
            self.Connect.setEnabled(False)
            self.SpotUpdatelabel.setText("Connected")
            self.SPOTPackages_updated = False

            self.startThread()
        else:
            QMessageBox.information(self, "Error", "Invalid IP Address")
            return False

    def startThread(self):
        if self.spot.is_connected:
            self.upgradeProgressBar.setValue(0)
            self.Check.clear()
            self.spot.register_event('online_status', self.on_online_status)
            if self.work != None:
                if self.work.isAlive():
                    self.threadEvent.set()
                    self.work.join()
                    self.threadEvent.clear()
            self.work = Module(self.spot,self, self.threadEvent)
            self.work.start()
            self.SpotUpdatelabel.setText("Connected")
            self.Print("Connected to "+ self.ipaddress, True, True )
        else:
            self.Print("Could not connect to "+self.ipaddress, True, True )
            self.resetState()

    def disconnectIP(self, error = "Disconnected"):
        if self.spot.is_connected:
            Interface.send_logout_request(self.spot)
            ack_params = spotPinpad.recv_logout_result(self.spot, 5)
            if error:
                self.Print(error, False, True)
            self.spot.disconnect()
        #self.resetState()

    def Close(self):
        self.disconnectIP()
        if self.captureFile != None:
            self.captureFile.close()

        if self.work != None and self.work.isAlive():
            self.work.stop()
            self.work.join()

        self.reject()

    def validateIP(self, ip):
        try:
            socket.inet_aton(ip)
        except socket.error:
            return False
        return True

    def initConn(self, ip, timeout = 5):
        try:
            if self.spot.is_connected:
                self.disconnectIP()
            self.spot.connect(ip)
        except:
            self.Print("Already connected", True, True)

    def selectLogFile(self):
        fd = QFileDialog(None)
        fd.setFileMode(QFileDialog.Directory)
        fd.setOption(QFileDialog.ShowDirsOnly)
        result = fd.exec()
        if result > 0:
            self.logFilePath.setText(fd.selectedFiles()[0])
            for filename in glob.glob(os.path.join(_log_path_, '*.log')):
                try:
                    shutil.copy(filename, self.logFilePath.text())
                except EnvironmentError:
                    self.Print("Unable to copy file " + filename, True, True)

    def CreateLogFile(self):
         if self.captureFile is not None:
            self.captureFile.close()
            self.captureFile = None
         self.captureFile = open(_log_path_+'SpotUploader.'+ str(self.ipaddress) +'.log', 'a')
         self.captureFile.flush()

    def Print(self, text, mon, log):
            timestamp = datetime.now().strftime("%y:%m:%d:%H:%M:%S.%f")[:-4]
            mystr = timestamp + ' '
            mystr = mystr + str(text)
            mystr += '\n'

            # CAPTURE TO FILE
            if log:
                self.captureFile.write(str(mystr))
                self.captureFile.flush()

            #Write in the log pane
            if mon:
                fmt = QTextCharFormat()
                self.logTextEdit.moveCursor(QTextCursor.End,QTextCursor.MoveAnchor)
                self.logTextEdit.setCurrentCharFormat(fmt)
                self.logTextEdit.insertPlainText(mystr)
                self.logTextEdit.ensureCursorVisible()

    def exitSpotUploader(self):
        self.close()

    def errorDisplay(self, msg):
        ctypes.windll.user32.MessageBoxW(None, u""+msg+"", u"Error", 0)
        return
########## Module THREAD CLASS ##########
class Module(threading.Thread):
  def __init__ (self,s,window, event):
    threading.Thread.__init__(self)
    self.event = event
    self.s = s
    self.window=window


  def run(self):
    ack_params = Interface.login(self.s)
    if ack_params is False:
        app.postEvent(self.window, Event.LoggerEvent('Unable to Login',True,True))

    elif ack_params is True :
        rVal = self.install()
        if rVal is not None and rVal is not False:
            try:
                    self.upgradePackage(rVal)

            except:
                     pass

    window.disconnectIP()
    app.postEvent(self.window, Event.updateUIEvent("True"))

  def upgradePackage(self, pkgName):
    status = True
    if pkgName == None:
        status = False
    else:
         ack_params = None
         if pkgName == 'SpotCheckROMfs5':
             if window.parseFile.parseResFiles(_resources_path_, 5, _device_model_) == True :
                ack_params = self.update(window.parseFile.SpotCheckROMfs[5],'ROMfs5')
             else:
                  window.errorDisplay("Package not found. Please copy package in SPOT folder")
                  print("Resource path"+ _resources_path_)
                  return
         elif pkgName == 'SpotCheckROMfs6':
             if window.parseFile.parseResFiles(_resources_path_, 6, _device_model_) == True :
                ack_params = self.update(window.parseFile.SpotCheckROMfs[6], 'ROMfs6')
             else:
                  window.errorDisplay("Package not found. Please copy package in SPOT folder")
                  return
         elif pkgName == 'SpotCheckROMfs7':
             if window.parseFile.parseResFiles(_resources_path_, 7, _device_model_) == True:
                ack_params = self.update(self.window.parseFile.SpotCheckROMfs[7],'ROMfs7')
             else:
                   window.errorDisplay("Package not found. Please copy package in SPOT folder")
                   return
         elif pkgName == 'SpotCheckROMfs8':
             if window.parseFile.parseResFiles(_resources_path_, 8, _device_model_) == True:
                ack_params = self.update(self.window.parseFile.SpotCheckROMfs[8],'ROMfs8')
             else:
                   window.errorDisplay("Package not found. Please copy package in SPOT folder")
                   return
         if None == ack_params or ack_params['ack_code'] != 0x00 or ack_params == False:
             status = False

    if status != False:
        try:
            Setup.send_activatePackage(self.s)
            ack_params = Setup.recv_activationPackage(self.s, 5)
            window.SPOTPackages_updated = True
            if ack_params != None and ack_params['ack_code'] == 0x00:
                app.postEvent(self.window, Event.ProgressEvent(100))
            else:
                app.postEvent(self.window, Event.ProgressEvent(0, "FAILURE"))
                app.postEvent(self.window, Event.LoggerEvent("Package upload failed",True,True))
                window.errorDisplay("Package upload failed")
                window.SPOTPackages_updated = False
                window.resetState()
        except:
            pass
    else:
        app.postEvent(self.window, Event.ProgressEvent(0, "FAILURE"))
        app.postEvent(self.window, Event.LoggerEvent("Package upload failed",True,True))
        window.errorDisplay("Package upload failed")
        window.SPOTPackages_updated = False
        window.resetState()

  def update(self, pkgName,fileID ):
    ack_params = None
    pkgFile = None
    curFileSize = None
    ppn =None

    fileSize = os.path.getsize(pkgName)
    if fileSize < 248:
        app.postEvent(self.window, Event.LoggerEvent("size of " + pkgName + "is less than 248 bytes",False,True))
        return False
    else:
        app.postEvent(self.window, Event.LoggerEvent('Upgrading package ' + pkgName + '...',True,True))
        app.postEvent(self.window, Event.updateUIEvent("False"))
        try:
            spotPinpad.send_hardware_info(self.s)
            ack_hardware_info = spotPinpad.recv_hardware_info(self.s, 5)
            if ack_hardware_info !=None:
                ppn = ack_hardware_info['Tag_ppndata'][-8:]

                if _device_model_ == 'M3':
                    with open(pkgName, 'rb') as pkgFile:
                        pkgFile.seek(fileSize - 248)
                        certBuff = pkgFile.read(248)
                        cert = None
                        index = -1
                        while cert == None:
                            cert = self.window.parseFile.calcCert(ppn, pkgName, fileID, certBuff, index)
                            index += 1
                        cert = cert.encode('latin-1')
                        fileSize -= 248
                        curFileSize = fileSize
                        Setup.send_downloadRequest(self.s, fileSize, cert, 0x01, 0x00)
                        ack_params = Setup.recv_downloadRequest(self.s, 5)
                        self.download_block(fileSize, cert, pkgFile, curFileSize, ack_params)
                        return ack_params

                else:
                  with open(pkgName, 'rb') as pkgFile:
                        pkgFile.seek(fileSize - 248)
                        cert = pkgFile.read(248)
                        fileSize -= 248
                        curFileSize = fileSize
                        percent = 0
                        Setup.send_downloadRequest(self.s, fileSize, cert, 0x01, 0x00)
                        ack_params = Setup.recv_downloadRequest(self.s, 5)
                        self.download_block(fileSize, cert, pkgFile, curFileSize, ack_params)
                        return ack_params

        except:
            return ack_params

  def download_block(self,fileSize, cert, pkgFile, curFileSize, ack_params):
     try:
        if ack_params != None:
            if ack_params['ack_code'] == 0x00:
                no_of_blocks = math.ceil(fileSize/2048) + 1 #2048 is the max size of the data block
                pkgFile.seek(0)
                i = 0
                curPercent = 0
                prevPercent = 0
                while curFileSize > 0:
                    blkSize = min(2048, curFileSize)
                    curFileSize -= blkSize
                    blkData = pkgFile.read(blkSize)
                    Setup.send_downloadBlock(self.s, blkSize, blkData)
                    ack_params = Setup.recv_downloadBlock(self.s, 5)
                    if ack_params != None:
                        if ack_params['ack_code'] == 0x00:
                            i += 1
                            curPercent = math.ceil((i/no_of_blocks)*100)
                            if curPercent - prevPercent >= 2:
                                prevPercent = curPercent
                                app.postEvent(self.window, Event.ProgressEvent(curPercent))
                        else:
                            return ack_params
                    elif ack_params == None:
                        app.postEvent(self.window, Event.ProgressEvent(0, "FAILURE"))
                        return ack_params
                Setup.send_downloadBlock(self.s, 0)
                ack_params = Setup.recv_downloadBlock(self.s, 5)



        else:
            return ack_params
     except:
        return ack_params

  def install(self):

    global _resources_path_
    spotPinpad.send_hardware_info(self.s)
    ack_hardware_info = spotPinpad.recv_hardware_info(self.s, 5)
    if ack_hardware_info is not None and ack_hardware_info is not False:
        if ack_hardware_info['ack_code'] == 0x00:
            dev_type = ack_hardware_info['Tag_data'].rstrip(b'\0'b' ').decode('ascii')
            global _device_model_
            if dev_type[:7] == 'MCF532x':
                _resources_path_ = _resources_path_M3
                _device_model_ = 'M3'
            elif dev_type[:6] == 'OMAP35':
                _resources_path_ = _resources_path_M5
                _device_model_ = 'M5'
            elif dev_type[:4] == 'UPMx':
                _device_model_ = 'VGD'
                window.errorDisplay("This device model is not supported by Uploader")
                return


    spotPinpad.send_dispInfo(self.s)
    ack_params_disp = spotPinpad.recv_dispInfo(self.s, 5)
    spotPinpad.send_fileInfo(self.s)
    num_entries = spotPinpad.recv_fileInfo(self.s, 5)

    if num_entries is None or num_entries is False or num_entries > 0xFE :
       app.postEvent(self.window, Event.LoggerEvent("No free resource slots " , True, True))
       return False
    else:
       app.postEvent(self.window, Event.LoggerEvent("Resource slots available" , True, True))

    if ack_params_disp is not None and ack_params_disp['ack_code'] == 0x00:
        if ack_params_disp['count'] == b'\x00\x00':
            if _device_model_ == 'M5':
                spotPinpad.send_systemStatus(self.s, b'\x01')
                ack_params_sys = spotPinpad.recv_systemStatus(self.s, 5)
                if ack_params_sys is  None or ack_params_sys['ack_code'] != 0x00:
                    app.postEvent(self.window, Event.LoggerEvent('System Status command failed with ack code '+ ack_params_sys,True,True))
                    return False

                if ack_params_sys['num_entries'] == 0x00:
                    app.postEvent(self.window, Event.LoggerEvent('Bad System Status command. No devices in response', True,True))
                    return False

                if ack_params_sys['entries'][0]['status_extension'] == 0x02:
                    app.postEvent(self.window, Event.LoggerEvent('An external application is running on the SPOT. Please disable all external applications and retry',True,True))
                    window.errorDisplay('An external application is running on the SPOT. Please disable all external applications and retry.')
                    return False

            app.postEvent(self.window, Event.LoggerEvent("No external application is running in SPOT" , True, True))

            spotPinpad.send_rfsInfo(self.s)

            ack_params_sw = spotPinpad.recv_swInfo(self.s, 5)

            if ack_params_sw is not None and ack_params_sw['ack_code'] == 0x00:


                Romfs = self.processSwInfo(ack_params_sw)

                if Romfs is not None:

                    Romfsrange = [5,6,7,8]
                    overrideNum = None
                    EmptyNum = None

                    for rNumber in Romfsrange:
                        if Romfs[rNumber][:10] == 'DIAGNOSTIC':
                            app.postEvent(self.window, Event.LoggerEvent('Diagnostic Package found. Cannot proceed installation on this SPOT',True,True))
                            window.errorDisplay("Diagnostic Package found. Cannot proceed installation on this SPOT")
                            return False
                        if Romfs[rNumber][:9] == 'SPOTCHECK':
                            overrideNum = rNumber

                        if Romfs[rNumber] == '' :
                            EmptyNum = rNumber

                    if overrideNum is not None :
                        return 'SpotCheckROMfs'+ str(overrideNum)

                    if EmptyNum is not None :
                        return 'SpotCheckROMfs'+ str(EmptyNum)
                    else:
                        app.postEvent(self.window, Event.LoggerEvent('No free ROMfs slot. Cannot proceed installation',True,True))
                        window.errorDisplay("No free ROMfs slot. Cannot proceed installation")
                else:
                    app.postEvent(self.window, Event.LoggerEvent('Process software info failed ',True,True))
            else:
                app.postEvent(self.window, Event.LoggerEvent('Software Status command failed with ack code '+ ack_params_sw,True,True))
        else:
            app.postEvent(self.window, Event.LoggerEvent('Another external application is using the SPOT.\nPlease close all external applications and upload SPOTCheck again',True,True))
            window.errorDisplay("Another external application is using the SPOT.Please close it and upload SPOTCheck again")
    else:
        app.postEvent(self.window, Event.LoggerEvent('Display Status command failed with ack code '+ ack_params_disp,True,True))

    return False

  def processSwInfo(self, ack_params_sw):
     entries = ack_params_sw['entries']
     romfs = {}
     romfs[5] = entries[0]['tag_value'].rstrip(b'\0'b' ').decode('ascii')
     romfs[6] = entries[1]['tag_value'].rstrip(b'\0'b' ').decode('ascii')
     romfs[7] = entries[2]['tag_value'].rstrip(b'\0'b' ').decode('ascii')
     romfs[8] = entries[3]['tag_value'].rstrip(b'\0'b' ').decode('ascii')
     return romfs


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())


