#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.apdudb.apdudb_opt
   :platform: Unix, Windows
   :synopsis: APDU names database for OPT application.
Manages the database of APDU names for OPT application.
"""


DB_ = {
  # Information Request class
  (0x10, 0x01): "OPT Status Request",
  (0x10, 0x02): "OPT Hardware Configuration Info",
  (0x10, 0x03): "OPT Software Configuration Info",
  (0x10, 0x04): "OPT Serial pass-thru Info",
  (0x10, 0x05): "OPT BNA Unsolicited Information",
  # Printer class
  (0x20, 0x01): "OPT Enqueue Data",
  (0x20, 0x02): "OPT Enqueue Metadata",
  (0x20, 0x03): "OPT Job Status",
  (0x20, 0x04): "OPT Create Job",
  (0x20, 0x05): "OPT Execute Job",
  (0x20, 0x06): "OPT Cancel Job",
  # Term I/O Class
  (0x30, 0x01): "OPT SoftKeys",
  (0x30, 0x02): "OPT Controls",
  (0x30, 0x03): "OPT Spare I/O",
  # Barcode Reader Class
  (0x40, 0x01): "OPT Barcode Enable/Disable",
  # Contact less Reader class
  (0x50, 0x01): "OPT Contactless Enable",
  (0x50, 0x02): "OPT Contactless Set MIFARE Key Table",
  (0x50, 0x03): "OPT Contactless Set MIFARE Read Table",
  (0x50, 0x04): "OPT Contactless Card Data",
  (0x50, 0x05): "OPT Contactless Disable",
  # Serial port pass-through class
  (0x60, 0x01): "OPT SerialPort Open Channel",
  (0x60, 0x02): "OPT SerialPort Close Channel",
  (0x60, 0x03): "OPT SerialPort Send Data",
  (0x60, 0x04): "OPT SerialPort Check Channel",
  (0x60, 0x05): "OPT SerialPort Get Data",
  (0x60, 0x06): "OPT SerialPort Set HWR Signal",
  # Fiscal Printer class
  (0x70, 0x01): "OPT Fiscal Cash Register Status",
  (0x70, 0x02): "OPT Fiscal Printer Status",
  (0x70, 0x03): "OPT Fiscal Memory Status",
  (0x70, 0x04): "OPT Fiscal Browse",
  (0x70, 0x05): "OPT Fiscal Get Journal File",
  (0x70, 0x06): "OPT Fiscal Enter Setting Mode",
  (0x70, 0x07): "OPT Fiscal Exit Setting Mode",
  (0x70, 0x08): "OPT Fiscal Set Date Time",
  (0x70, 0x09): "OPT Fiscal Set VAT Rate",
  (0x70, 0x0A): "OPT Fiscal Set VAT Table",
  (0x70, 0x0B): "OPT Fiscal Download User Data",
  (0x70, 0x0C): "OPT Fiscal Set Payment Mode",
  (0x70, 0x0D): "OPT Fiscal Begin Fiscal Day",
  (0x70, 0x0E): "OPT Fiscal Close Fiscal Day",
  (0x70, 0x0F): "OPT Fiscal Begin Fiscal Receipt",
  (0x70, 0x10): "OPT Fiscal Sell Item Request",
  (0x70, 0x11): "OPT Fiscal Pay",
  (0x70, 0x12): "OPT Fiscal End Fiscal Receipt",
  (0x70, 0x13): "OPT Fiscal Open Free Print",
  (0x70, 0x14): "OPT Fiscal Normal Printing",
  (0x70, 0x15): "OPT Fiscal Close Free Printing",
  (0x70, 0x16): "OPT Fiscal Begin Fiscal Invoice",
  (0x70, 0x17): "OPT Fiscal Close Fiscal Invoice",
  (0x70, 0x18): "OPT Fiscal Void Print",
  (0x70, 0x19): "OPT Fiscal Switch to Euro Mode",
  (0x70, 0x20): "OPT Fiscal X Report",
  # Banknote acceptor class
  (0x90, 0x01): "OPT BNA Enable",
  (0x90, 0x02): "OPT BNA Dsable",
  (0x90, 0x03): "OPT BNA Banknot Read Confirmation",
  (0x90, 0x04): "OPT BNA Status",
  (0x90, 0x05): "OPT BNA Version",
  (0x90, 0x06): "OPT BNA Start Firmware Download",
  (0x90, 0x07): "OPT BNA Firmware Download Status",
  (0x90, 0x08): "OPT BNA EnableAlternate",
  (0x90, 0x09): "OPT BNA Capture",
  (0x90, 0x0A): "OPT BNA Eject",
  }


def get_apdu_name(apdu_cmd):
    """
    Returns the name of an OPT command.
    :Parameters:
        - *apdu_cmd* (tuple): Command code.
    :Returns: str -- The Name of OPT command if it exists, ``<unknown>`` otherwise.
    """
    if apdu_cmd not in DB_:
        return '<unknown>'
    return DB_[apdu_cmd]
