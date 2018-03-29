#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.apdudb.apdudb_pinpad
   :platform: Unix, Windows
   :synopsis: APDU names database for Pinpad application.
Manages the database of APDU names for Pinpad application.
"""

DB_ = {
  # File Management Class
  (0x00, 0x01): "File Download",
  (0x00, 0x02): "File Block Download",
  (0x00, 0x03): "File Browse",
  (0x00, 0x04): "File Delete",
  (0x00, 0x05): "File Upload",
  (0x00, 0x06): "File Block Upload",
  # Information Request Class
  (0x10, 0x01): "System Status",
  (0x10, 0x02): "Hardware Configuration",
  (0x10, 0x03): "Software Configuration",
  (0x10, 0x04): "Diagnostic Info",
  (0x10, 0x05): "Set Date Time",
  (0x10, 0x06): "Get Date Time",
  # Card Reader Class
  (0x20, 0x01): "Card Reader Enable",
  (0x20, 0x02): "Card Reader Disable",
  (0x20, 0x03): "Card Reader Track Data",
  (0x20, 0x04): "Card Reader Write",
  (0x20, 0x07): "Card Reader Chip Reset",
  (0x20, 0x08): "Card Reader Enable Sankyo",
  # Display Class
  (0x30, 0x01): "Display Current Message File Selection",
  (0x30, 0x03): "Display Create Window",
  (0x30, 0x04): "Display Destroy Window",
  (0x30, 0x05): "Display Strings",
  (0x30, 0x06): "Display Images",
  (0x30, 0x07): "Display Show Window",
  (0x30, 0x08): "Display Hide Window",
  (0x30, 0x09): "Display Beep",
  (0x30, 0x0A): "Display Create Window From Resource File",
  (0x30, 0x0F): "Display Get Status",
  (0x30, 0x10): "Display Select Language",
  (0x30, 0x11): "Display Secure Media Connector (SMC) Switching",
  # Keyboard Class
  (0x40, 0x01): "Keyboard Customer Input Enable",
  (0x40, 0x02): "Keyboard Customer Input Abort",
  (0x40, 0x03): "Keyboard Data Encryption",
  (0x40, 0x04): "Keyboard Application Key Download",
  (0x40, 0x05): "Keyboard Key Derivation",
  (0x40, 0x06): "Keyboard Key Status",
  (0x40, 0x07): "Keyboard MAC Verification",
  (0x40, 0x08): "Keyboard Get Random Bytes",
  (0x40, 0x0D): "Keyboard Application Key Download in TR31 Format",
  (0x40, 0x0E): "Keyboard Get RKL Key Name",
  # SAM Interface-module Class
  (0x50, 0x01): "SAM Interface-module Status",
  (0x50, 0x02): "SAM Power Control",
  (0x50, 0x03): "SAM Data Transfer",
  (0x50, 0x04): "SAM Set Parameters",
  (0x50, 0x05): "SAM Get Parameters",
  (0x50, 0x06): "SAM Reset Parameters",
  # TLS terminal certificate generation
  (0x90, 0x01): "Retrieve Terminal Certificate Request",
  (0x90, 0x02): "Set TLS Terminal Certificate",
  }

def get_apdu_name(apdu_cmd):
    """
    Returns the name of a Pinpad command.
    :Parameters:
        - *apdu_cmd* (tuple): Command code.
    :Returns: str -- The Name of Pinpad command if it exists, ``<unknown>`` otherwise.
    """
    if apdu_cmd not in DB_:
        return '<unknown>'
    return DB_[apdu_cmd]
