"""
.. module:: GVRlib.Monitor
   :platform: Unix, Windows
   :synopsis: Utility functions for logging management.
This module contains some functions related to the logging management.
"""


import io
from binascii import hexlify

def register_logger_event(connection, output):
    """
    Adds a new event to the logger. 
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *output* (File|QT): Handler of the log file.
    """
    def dump_message(appid, msg, tpdu):
        sol = "<< " if tpdu[2] > 0x10 else ">> "
        wrap_tpdu = hexlify(tpdu[2:3] + tpdu[4:])
        tpdu = ' '.join(wrap_tpdu[i:i+2] for i in range(0, len(wrap_tpdu), 2))
        if isinstance(mon, io.IOBase):
            output.write(sol + tpdu + '\n')    #If mon is file pointer 
            output.flush()
        else:
            output.appendPlainText(sol + tpdu) #If mon is pyQside PlainText
    connection.register_event('ienqueue_message', dump_message)
    connection.register_event('outgoing_message', dump_message)
