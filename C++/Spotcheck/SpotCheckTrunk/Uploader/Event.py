__author__ = 'chaudk'

from PySide.QtCore import *

loggerEventType = QEvent.Type(QEvent.registerEventType())
progressEventType = QEvent.Type(QEvent.registerEventType())
disconnectEventType =  QEvent.Type(QEvent.registerEventType())
updateUIEventType = QEvent.Type(QEvent.registerEventType())
############# EVENT DISPATCH #############
class LoggerEvent(QEvent):
  def __init__(self,text,mon,log):
    super(LoggerEvent, self).__init__(loggerEventType)
    self.text=text
    self.mon=mon
    self.log=log

############# EVENT DISPATCH #############
class ProgressEvent(QEvent):
  def __init__(self,percent, status=None):
    super(ProgressEvent, self).__init__(progressEventType)
    self.percent = percent
    self.status = status

############# EVENT DISPATCH #############
class DisconnectEvent(QEvent):
  def __init__(self):
    super(DisconnectEvent, self).__init__(disconnectEventType)

############EVENT updateUI################

class updateUIEvent(QEvent):
 def __init__(self, status=None):
        super(updateUIEvent, self).__init__(updateUIEventType)
        self.status = status