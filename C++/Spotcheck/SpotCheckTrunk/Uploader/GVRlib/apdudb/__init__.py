#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
.. module:: apdudb
   :platform: Unix, Windows
   :synopsis: APDU name database

.. moduleauthor:: Daniele Simonetti <cons_daniele.simonetti@gilbarco.com>


"""

__all__ = ['apdudb_spot', 'apdudb_pinpad', 'apdudb_emv', 'apdudb_setup', 'apdudb_opt']

from . import apdudb_spot
from . import apdudb_pinpad
from . import apdudb_emv
from . import apdudb_setup
from . import apdudb_opt

apdu_dbs = [
  apdudb_spot,
  apdudb_pinpad,
  apdudb_emv,
  apdudb_setup,
  apdudb_opt,
  None,
  None,
  None,
  None
  ]

def get_apdu_name(appid, apdu_cmd):
    '''Return the APDU name as specified in SPOT Programmers GUIDE'''
    appid = appid & 0x7F

    if appid < 0 or appid > 8:
        return None

    if apdu_dbs[appid] is None:
      return None

    return apdu_dbs[appid].get_apdu_name(apdu_cmd)