#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.apdudb.apdudb_setup
   :platform: Unix, Windows
   :synopsis: APDU names database for Setup application.
Manages the database of APDU names for Setup application.
"""

DB_ = {
  # Setup class
  (0x10, 0x01): "Setup Download",
  (0x10, 0x02): "Setup Download Block",
  (0x10, 0x03): "Setup Package Info",
  (0x10, 0x04): "Setup Activate Package",
  (0x10, 0x05): "Setup Request Info Debian",
  (0x10, 0x06): "Setup Clear Debian Repository",
  }

def get_apdu_name(apdu_cmd):
    """
    Returns the name of a Setup command.
    :Parameters:
        - *apdu_cmd* (tuple): Command code.
    :Returns: str -- The Name of Setup command if it exists, ``<unknown>`` otherwise.
    """
    if apdu_cmd not in DB_:
        return '<unknown>'
    return DB_[apdu_cmd]
