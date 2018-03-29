#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.apdudb.apdudb_spot
   :platform: Unix, Windows
   :synopsis: APDU names database for Spot application.
Manages the database of APDU names for Spot application.
"""

DB_ = {
  (0x01,): "Login",
  (0x02,): "Logout",
  (0x03,): "Ping",
  (0x04,): "Lock",
  (0x05,): "Unlock",
  (0x06,): "Reset",
  (0x07,): "Reboot KERNEL EMV",
  (0x08,): "InitLog",
  (0x09,): "LogLine",
  (0x0F,): "Diagnostic",
  (0x11,): "Session Keys",
  }

def get_apdu_name(apdu_cmd):
    """
    Returns the name of a Spot command.
    :Parameters:
        - *apdu_cmd* (tuple): Command code.
            
    :Returns: str -- The Name of Spot command if it exists, ``<unknown>`` otherwise.
    """
    if apdu_cmd not in DB_:
        return '<unknown>'
    return DB_[apdu_cmd]
