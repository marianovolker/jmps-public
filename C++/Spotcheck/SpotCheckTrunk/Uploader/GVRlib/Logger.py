"""
.. module:: GVRlib.Logger
   :platform: Unix, Windows
   :synopsis: Logging management.
This module contains the *Logger* class.
"""


import sys, os, string, logging
from time import gmtime, strftime
from datetime import datetime
from binascii import hexlify

try:
    import pupyspot
except:
    from . import pupyspot

try:
    import apdudb
except:
    from . import apdudb


def print_trace(appid, apdu, apdu_name = '', logger_name = None):
    """
    Utility function which writes a trace line in the log file. 
    :Parameters:
        - *appid* (byte): Application Identifier.
        - *apdu* (struct): ADPU descriptor:
        - *apdu_name* (string): Name of APDU.
        - *logger_name* (string): Name of the logger to use.
    """
    ts = datetime.now().strftime("%Y-%m-%d %H:%M:%S.%f")
    if appid >> 4: ev = 'recv'
    else: ev = 'sent'
    if not apdu_name:
        apdu_name = apdudb.get_apdu_name(appid, apdu.cmd)
    try:
        trace_info = '{0} {1} {2:02X} {3} 0x{4[0]:02x}, 0x{4[1]:02x}'.format(ts, ev, appid, apdu_name, apdu.cmd)
    except:
        trace_info = '{0} {1} {2:02X} {3} 0x{4[0]:02x}'.format(ts, ev, appid, apdu_name, apdu.cmd)
    trace_debug = 'APDU hexdump:\n' + pupyspot.apdu_hexdump(apdu)
    if logger_name is None:
        print(trace_info)
        print(trace_debug)
    else:
        logger = logging.getLogger(logger_name)
        logger.info(trace_info)
        logger.debug(trace_debug)

def register_logger_event(connection, log_name = None):
    """
    Adds a new event to the logger. 
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *log_name* (string): Name of the logger.
    """
    def dump_message(appid, msg, tpdu, ln = log_name):
        apdu = pupyspot.apdu_from_message(appid, msg)
        print_trace(appid, apdu, logger_name = ln)
    connection.register_event('incoming_message', dump_message)
    connection.register_event('outgoing_message', dump_message)

class Logger:
    """
    This class implement a flexible event logging system for *pyTools* applications and libraries.
    """
    def __init__(self, path, title):
        """
        Initializes the *Logger* class. 
        :Parameters:
            - *path* (string): Path to the log file.
            - *title* (string): Name of log file.
        """
        if os.path.isfile(path):
            log_idx = path.index('.log')
            os.rename(path, (path[:log_idx] if log_idx > 0 else path) + '_' + strftime("%Y%m%d_%H%M%S", gmtime(os.path.getmtime(path))) + '.log')

        # DEFINE FORMATTERS
        self.formatter = logging.Formatter('%(asctime)s %(levelname)s - %(message)s')

        # DEFINE HANDLERS
        self.file_handler = logging.FileHandler(path, mode = 'a')
        self.file_handler.setLevel(logging.DEBUG)
        self.file_handler.setFormatter(self.formatter)
        self.console_handler = logging.StreamHandler()
        self.console_handler.setLevel(logging.DEBUG)
        self.console_handler.setFormatter(self.formatter)

        # DEFINE LOGGER
        self.logger = logging.getLogger(title)
        self.logger.setLevel(logging.DEBUG)
        self.logger.addHandler(self.file_handler)
        self.logger.addHandler(self.console_handler)

        # START
        self.info(title + ' START')

    def __del__(self):
        # END
        if self.logger.hasHandlers():
            self.close()

    def close(self):
        """
        Closes up any resources used by the class.
        """
        self.info('LOG END')
        self.logger.removeHandler(self.file_handler)
        self.logger.removeHandler(self.console_handler)
        self.file_handler.close()

    def info(self, msg):
        """
        Log a message string with the *INFO* level. 
        :Parameters:
            - *msg* (string): Message to log.
        """
        self.logger.info(msg)

    def debug(self, msg):
        """
        Log a message string with the *DEBUG* level. 
        :Parameters:
            - *msg* (string): Message to log.
        """
        self.logger.debug(msg)

    def critical(self, msg):
        """
        Log a message string with the *CRITICAL* level. 
        :Parameters:
            - *msg* (string): Message to log.
        """
        self.logger.critical(msg)

    def error(self, msg):
        """
        Log a message string with the *ERROR* level. 
        :Parameters:
            - *msg* (string): Message to log.
        """
        self.logger.error(msg)

    def checkResult(self, expectedDict, returnedDict, msg = ''):
        """
        This method checks the expected result comparing two different dictionaries.
        :Parameters:
            - *expectedDict* (dictionary): Template dictionary for comparison.
            - *returnedDict* (dictionary): Dictionary to compare.
        :Returns: ``True`` if both dictionaries must be equal (same keys and same values), ``False`` otherwise.
        """
        if expectedDict.keys() != returnedDict.keys():
            raise Exception("Mismatch of number of elements")
        if expectedDict == returnedDict:
            flag = 'PASS'
        else:
            flag = 'FAIL'
        if len(msg) > 0:
            msg = ' | ' + msg
        self.info('CHECK: ' + flag + msg + ' Expected values=> ' + self.dictToString(expectedDict) + ' Returned values=> ' + self.dictToString(returnedDict))
        return flag

    def dictToString(self, dictionary):
        """
        Utility method to convert dictionary to string.
        :Parameters:
            - *dictionary* (dictionary): Dictionary to convert.
        :Returns: A string representing dictionary content.
        """
        result = ''
        for key, value in dictionary.items():
            if isinstance(value, dict):
                entry = self.dictToString(value)
            else:
                entry = str(value)
                if not all(char in string.printable for char in entry):
                    entry = '{0} (hex)'.format(hexlify(entry))
            result += key + ':' + entry + '; '
        return result
