#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.apdudb.apdudb_emv
   :platform: Unix, Windows
   :synopsis: APDU names database for EMV application.
Manages the database of APDU names for EMV application.
"""



DB_ = {
  # EMV Execution class
  (0x10, 0x01): "EMV START_EMV",
  (0x10, 0x02): "EMV ACTION_EMV",
  (0x10, 0x03): "EMV OLA_EMV",
  (0x10, 0x04): "EMV END_EMV",
  (0x10, 0x05): "EMV START_EMV_1",
  (0x10, 0x06): "EMV START_EMV_2",
  (0x10, 0x07): "EMV START_EMV_2_PHASE_2",
  # Table Management Class
  (0x20, 0x01): "EMV Record Count",
  (0x20, 0x02): "EMV Record Copy",
  (0x20, 0x03): "EMV Record Deletion",
  (0x20, 0x04): "EMV TLV or Record Field Read",
  (0x20, 0x05): "EMV TLV or Record Field Write",
  (0x20, 0x06): "EMV Table Dump Load",
  # Results class
  (0x99, 0x02): "EMV Status Change",
  }

def get_apdu_name(apdu_cmd):
    """
    Returns the name of an EMV command.
    :Parameters:
        - *apdu_cmd* (tuple): Command code.
    :Returns: str -- The Name of EMV command if it exists, ``<unknown>`` otherwise.
    """
    if apdu_cmd not in DB_:
        return '<unknown>'
    return DB_[apdu_cmd]
