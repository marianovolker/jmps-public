"""
.. module:: GVRlib.Emv.Protocol3
   :platform: Unix, Windows
   :synopsis: EMV protocol management (Python 3).
Manages commands related to the EMV status machine implemented by EMV application.
"""

from binascii import hexlify, unhexlify

try:
    import pupyspot
except:
    from .. import pupyspot

try:
    import Utility
except:
    from . import Utility

# Default Variables
EMV_BER_TLV_EXAMPLE = (
                       b"\x63\x04hell",             # simple tag, len
                       b"\x64\x04hell",             # test read length
                       b"\x1f\x81\x82\x82\x04\x00", # extended tag
                       b"\x65\x81\x01!",            # extended len
                       b"\x65\x83\x00\x00\x04!..!"  # mroe extended len
                       )

EMV_MULTI_BER_TLV_EXAMPLE = b"".join(EMV_BER_TLV_EXAMPLE)

_apid = 2

"""next command for start emv"""
START_NEXT_COMMAND_NONE                   = 0x00
START_NEXT_COMMAND_FORCED_OLA             = 0x01
START_NEXT_COMMAND_OLA_FORCED_BY_MERCHANT = 0x02
START_NEXT_COMMAND_ACTION                 = 0x03

"""next command for ola emv"""
ACTION_NEXT_COMMAND_NONE                  = 0x00
ACTION_NEXT_COMMAND_FORCED_BY_MERCHANT    = 0x01
ACTION_NEXT_COMMAND_CARD_REFUSED          = 0x02
ACTION_NEXT_COMMAND_FLOOR_LIMIT_EXCEEDED  = 0x03
ACTION_NEXT_COMMAND_NO_OLA                = 0x04

###Start EMV###
def send_START_EMV_cmd(connection, currentDateAndTime = None, amount = 0, nextCommand = START_NEXT_COMMAND_NONE):
    """
    Sends a **START_EMV** command to SPOT.
    This command starts the EMV kernel for initial processing of an EMV transaction. 
    It is assumed that an ICC has been already inserted and reset (the ATR is returned to EPS out of the Emv kernel context). 
    By using this command the EPS decides to try to process the ICC as an EMV card.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x10     |      0x01      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *currentDateAndTime* (binary string): Synchronisation of SPOT internal clock with the terminal one before starting the EMV transaction. Six ASCII hex for YYMMDDHHMMSS.
        - *amount* (dword): This is the amount used for pre-authorisation or pre-payment. It is 4 bytes binary data.
        - *nextCommand* (byte): This flag allows proceeding with the operations that normally are performed by next command.
    """
    if currentDateAndTime is None:
        currentDateAndTime = Utility.genDTS()
    apdu = pupyspot.create_apdu(_apid, b'\x10\x01')
    apdu.put_array(currentDateAndTime)
    apdu.put_dword(amount)
    apdu.put_byte(nextCommand)
    connection.send_apdu(apdu)

def recv_START_EMV_cmd(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **START_EMV_cmd** *(0x01)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters. 
    """ 
    return recv_EMV_ACK(connection, b'\x01', timeout)

def START_EMV_cmd(connection, currentDateAndTime, amount = 0, nextCommand = START_NEXT_COMMAND_NONE, timeout = 0):
    """
    Sends a **START_EMV** command and waits for an answer from the SPOT.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *currentDateAndTime* (binary string): Synchronisation of SPOT internal clock with the terminal one before starting the EMV transaction. Six ASCII hex for YYMMDDHHMMSS.
        - *amount* (dword): This is the amount used for pre-authorisation or pre-payment. It is 4 bytes binary data.
        - *nextCommand* (byte): This flag allows proceeding with the operations that normally are performed by next command.
        - *timeout* (integer): Timeout for ``recv_START_EMV_cmd`` function.
    :Returns: ``None`` if command is Successful.
    """ 
    send_START_EMV_cmd(connection, currentDateAndTime, amount, nextCommand)
    return Utility.assertSuccess(recv_START_EMV_cmd(connection, timeout))


###Action EMV###
def send_ACTION_EMV_cmd(connection, olaForced = ACTION_NEXT_COMMAND_NONE, amount = 0):
    """
    Sends a **ACTION_EMV** command to SPOT.
    This command starts the EMV kernel for performing the EMV steps related to Online/Offline decision.
    It requires the completion of the initial processing phase in the previous command START_EMV or START_EMV_1/START_EMV_2.
    If the offline authorisation is performed the transaction finishes here and the EPS can found in TAB_TRANS all the data required for printing the receipt and for data capture transaction (to be sent toacquirer).
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x10     |      0x02      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *olaForced* (byte): The flag parameter (if asserted) overrides the decision of ICC and EMV kernel and forces online decision. For example, EPS may consider suspiciously some transactions and require them to be forced online.
        - *amount* (dword): The Amount parameter (4 bytes binary data) is optional and when is specified overrides the amount specified with the ``START_EMV`` or ``START_EMV_1`` or ``START_EMV_2_PHASE_2`` command at the beginning of current transaction.
    """     
    apdu = pupyspot.create_apdu(_apid, b'\x10\x02')
    apdu.put_byte(olaForced)
    apdu.put_dword(amount)
    connection.send_apdu(apdu)

def recv_ACTION_EMV_cmd(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **ACTION_EMV** *(0x02)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters. 
    """ 
    return recv_EMV_ACK(connection, b'\x02', timeout)

def ACTION_EMV_cmd(connection, olaForced = ACTION_NEXT_COMMAND_NONE, amount = 0, timeout = 0):
    """
    Sends a **ACTION_EMV** command and waits for an answer from the SPOT.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *olaForced* (byte): The flag parameter (if asserted) overrides the decision of ICC and EMV kernel and forces online decision. For example, EPS may consider suspiciously some transactions and require them to be forced online.
        - *amount* (dword): The Amount parameter (4 bytes binary data) is optional and when is specified overrides the amount specified with the ``START_EMV`` or ``START_EMV_1`` or ``START_EMV_2_PHASE_2`` command at the beginning of current transaction.
        - *timeout* (integer): Timeout for ``recv_ACTION_EMV_cmd`` function.
    :Returns: ``None`` if command is Successful.    
    """ 
    send_ACTION_EMV_cmd(connection, olaForced, amount)
    return Utility.assertSuccess(recv_ACTION_EMV_cmd(connection, timeout))

###OLA EMV###
def send_OLA_EMV_cmd(connection, hostDecision, hostResponse, authCode, issData, script71Tuple = (), script72Tuple = ()):
    """
    Sends a **OLA_EMV** command to SPOT.
    This command starts the EMV kernel for performing the EMV steps related to Online authorisation and transaction closure.
    It requires the completion of the actions analysis phase in the previous command ACTION_EMV with the ICC and Emv kernel decision to continue online the transaction.
    The EPS can found in TAB_TRANS all the data required for printing the receipt and for data capture transaction (to be sent to acquirer).
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x10     |      0x03      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *hostDecision* (byte): 
            - 0x30 = authorised,
            - 0x31 = denied,
            - 0x32 = denied \- pick up the card,
            - 0x39 and other values = error or fail to connect to host.
        - *hostResponse* (word): This is normally the ISO8583/1987 two digits response/action code or "00" if not available.
        - *authCode* (binary string): Authorisation Code from ISO8583 bitmap 38 (six alphanum); it will be saved into DB with tag89.
        - *issData* (binary string): The "Iss data for authorisation" to be saved on DB as tag 91. First 8 bytes are the ARPC and are mandatory the rest should be optional but normally at least 2 bytes with ARC are present.
        - *script71Tuple* (tuple): List of "File Id" to use for download the EMV files type 0x22, used for issuer script 71.
        - *script72Tuple* (tuple): List of "File Id" to use for download the EMV files type 0x22, used for issuer script 72.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x10\x03')
    apdu.put_byte(hostDecision)
    apdu.put_word(hostResponse)
    apdu.put_array((authCode + b"      ")[0:6])
    apdu.put_byte(len(issData))
    apdu.put_array(issData)
    apdu.put_byte(len(script71Tuple))
    apdu.put_byte(len(script72Tuple))
    apdu.put_blist(script71Tuple)
    apdu.put_blist(script72Tuple)
    connection.send_apdu(apdu)

def recv_OLA_EMV_cmd(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **OLA_EMV** *(0x03)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters. 
    """ 
    return recv_EMV_ACK(connection, b'\x03', timeout)

def OLA_EMV_cmd(connection, hostDecision, hostResponse, authCode, issData, script71Tuple = (), script72Tuple = (), timeout = 0):
    """
    Sends a **OLA_EMV** command and waits for an answer from the SPOT.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *hostDecision* (byte): 
            - 0x30 = authorised,
            - 0x31 = denied,
            - 0x32 = denied \- pick up the card,
            - 0x39 and other values = error or fail to connect to host.
        - *hostResponse* (word): This is normally the ISO8583/1987 two digits response/action code or "00" if not available.
        - *authCode* (binary string): Authorisation Code from ISO8583 bitmap 38 (six alphanum); it will be saved into DB with tag89.
        - *issData* (binary string): The "Iss data for authorisation" to be saved on DB as tag 91. First 8 bytes are the ARPC and are mandatory the rest should be optional but normally at least 2 bytes with ARC are present.
        - *script71Tuple* (tuple): List of "File Id" to use for download the EMV files type 0x22, used for issuer script 71.
        - *script72Tuple* (tuple): List of "File Id" to use for download the EMV files type 0x22, used for issuer script 72.
        - *timeout* (integer): Timeout for ``recv_OLA_EMV_cmd`` function.
    :Returns: ``None`` if command is Successful.
    """ 
    send_OLA_EMV_cmd(connection, hostDecision, hostResponse, authCode, issData, script71Tuple, script72Tuple)
    return Utility.assertSuccess(recv_OLA_EMV_cmd(connection, timeout))

###END Emv###
def send_END_EMV_cmd(connection):
    """
    Sends a **END_EMV** command to SPOT.
    This command resets the EMV kernel for a new transaction. The cache of the transaction data (``TAB_TRANS``) is cleaned (will reload the default table).
    The ICC (if any inserted) is returned to the customer after a card reader "Disable" command (managed out of the EMV kernel context).
    If the EMV kernel receives this command during a processing state it aborts the transaction and moves in error state waiting for a new ``END_EMV`` command.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x10     |      0x04      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
    """ 
    apdu = pupyspot.create_apdu(_apid, b'\x10\x04')
    connection.send_apdu(apdu)

def recv_END_EMV_cmd(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **END_EMV** *(0x04)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters. 
    """ 
    return recv_EMV_ACK(connection, b'\x04', timeout) 

def END_EMV_cmd(connection, timeout = 0):
    """
    Sends a **END_EMV** command and waits for an answer from the SPOT.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Timeout for ``recv_END_EMV_cmd`` function.
    :Returns: ``None`` if command is Successful.    
    """ 
    send_END_EMV_cmd(connection)
    return Utility.assertSuccess(recv_END_EMV_cmd(connection,timeout))

###Start EMV 1###
def send_START_EMV_1_cmd(connection, currentDateAndTime, amount = 0, nextCommand = START_NEXT_COMMAND_NONE):
    """
    Sends a **START_EMV_1** command to SPOT.
    This command starts the task for initial processing of an EMV transaction. It is assumed that an ICC has been already inserted and reset.
    By using this command the EPS decides to try to process the ICC as anEMV card.
    After the execution of this command the EMV kernel stops the processing and waits for next command.
    The data coming from the inserted ICC card has been written to the ``TAB_TRANS_EXT`` table. 
    The EPS is allowed to modify these data in order to produce a candidates list different from the one obtained using the terminal automatic processing.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x10     |      0x05      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *currentDateAndTime* (binary string): Synchronisation of SPOT internal clock with the terminal one before starting the EMV transaction. Six ASCII hex for YYMMDDHHMMSS.
        - *amount* (dword): This is the amount used for pre-authorisation or pre-payment. It is 4 bytes binary data.
        - *nextCommand* (byte): This flag allows proceeding with the operations that normally are performed by next command.
                
    """ 
    apdu = pupyspot.create_apdu(_apid, b'\x10\x05')
    apdu.put_array(currentDateAndTime)
    apdu.put_dword(amount)
    apdu.put_byte(nextCommand)
    connection.send_apdu(apdu)

def recv_START_EMV_1_cmd(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **START_EMV_1** *(0x05)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters. 

    """ 
    return recv_EMV_ACK(connection, b'\x05', timeout)

def START_EMV_1_cmd(connection, currentDateAndTime, amount = 0, nextCommand = START_NEXT_COMMAND_NONE, timeout = 0):
    """
    Sends a **START_EMV_1** command and waits for an answer from the SPOT.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *currentDateAndTime* (binary string): Synchronisation of SPOT internal clock with the terminal one before starting the EMV transaction. Six ASCII hex for YYMMDDHHMMSS.
        - *amount* (dword): This is the amount used for pre-authorisation or pre-payment. It is 4 bytes binary data.
        - *nextCommand* (byte): This flag allows proceeding with the operations that normally are performed by next command.
        - *timeout* (integer): Timeout for ``recv_START_EMV_1_cmd`` function.
    :Returns: ``None`` if command is Successful.
    """ 
    send_START_EMV_1_cmd(connection, currentDateAndTime, amount, nextCommand)
    return Utility.assertSuccess(recv_START_EMV_1_cmd(connection, timeout))


###Start EMV 2###
def send_START_EMV_2_cmd(connection, nextCommand = 0):
    """
    Sends a **START_EMV_2** command to SPOT.
    This command is allowed only if preceded by ``START_EMV_1``.
    It resumes the task for completing initial processing of an EMV transaction. It is assumed that the ``TAB_TRANS_EXT`` table was updated as needed by the EPS.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x10     |      0x06      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - nextCommand (byte):
            - 0x00 = like START_EMV_2 without optional flag,
            - 0x01 = pausing processing just before EMV phase "Get Processing Options.
    """         
    apdu = pupyspot.create_apdu(_apid, b'\x10\x06')
    apdu.put_byte(nextCommand)
    connection.send_apdu(apdu)

def recv_START_EMV_2_cmd(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **START_EMV_2** *(0x06)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.
    """ 
    return recv_EMV_ACK(connection, b'\x06', timeout) 

def START_EMV_2_cmd(connection, nextCommand = 0, timeout = 0):
    """
    Sends a **START_EMV_2** command and waits for an answer from the SPOT.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - nextCommand (byte):
            - 0x00 = like START_EMV_2 without optional flag,
            - 0x01 = pausing processing just before EMV phase "Get Processing Options.
        - *timeout* (integer): Timeout for ``recv_START_EMV_2_cmd`` function.
    :Returns: ``None`` if command is Successful.    
    """
    send_START_EMV_2_cmd(connection, nextCommand)
    return Utility.assertSuccess(recv_START_EMV_2_cmd(connection, timeout))


###Start EMV 2 PHASE 2###
def send_START_EMV_2_PHASE_2_cmd(connection, amount = 0):
    """
    Sends a **START_EMV_2_PHASE_2** command to SPOT.
    This command is allowed only if preceded by ``START_EMV_2`` with ``OptionalFlag`` = 1.
    It resumes the processing paused before the execution of the "Get Processing Options" and stores the transaction amount present in the mandatory parameter``AmountBinary[4]`` overwriting the one previously sent with the ``START_EMV_1`` command.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x10     |      0x07      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *amount* (dword): This is the amount used for pre-authorisation or pre-payment.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x10\x07')
    apdu.put_dword(amount)
    connection.send_apdu(apdu)

def recv_START_EMV_2_PHASE_2_cmd(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **START_EMV_2_PHASE_2** *(0x07)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters. 
    """
    return recv_EMV_ACK(connection, b'\x07', timeout)

def START_EMV_2_PHASE_2_cmd(connection, amount = 0, timeout = 0):
    """
    Sends a **START_EMV_2_PHASE_2** command and waits for an answer from the SPOT.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *amount* (dword): This is the amount used for pre-authorisation or pre-payment.
        - *timeout* (integer): Timeout for ``recv_START_EMV_2_PHASE_2_cmd` function.
    :Returns: ``None`` if command is Successful.
    """
    send_START_EMV_2_PHASE_2_cmd(connection, amount)
    return Utility.assertSuccess(recv_START_EMV_2_PHASE_2_cmd(connection, timeout = 0))


###Utility method for 'ACK only' command answers reception###
def recv_EMV_ACK(connection, subcommand, timeout = 0):
    """
    Utility method for *ACK only* command answers reception.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *subcommand* (integer): Subcommand code.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.
    """
    apdu = connection.wait_apdu(0x80 + _apid, b'\x10' + subcommand, timeout)
    ack_code = apdu.pop_byte()
    ack_params = {
            'ack_code': ack_code
        }
    return ack_params


###Wait a status change###
def wait_EMV_Unsolicited(connection, timeout = 0):
    """
    Waits a status change.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the APDU parameters.  
    """
    apdu = connection.wait_apdu(0x80 + _apid, b'\x99\x02', timeout)
    status = apdu.pop_byte()
    ack_params = {'status': status}
    try:
        ack_params['arc'  ] = apdu.pop_word()
        ack_params['error'] = apdu.pop_word()
    except:
        pass
    return ack_params

def EMV_wait_valid_status(connection, *states):
    """
    Waits an ADPU containing a valid state parameter.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the APDU parameters.  
    """
    while True:
        ack = wait_EMV_Unsolicited(connection)
        if ack['status'] in states :
            return ack

#######################################
##  class 20 : EMV Database access   ##
#######################################

def send_read_record_cmd(connection, table, record, *fields):
    """
    Sends a **TLV or record field read** command to SPOT.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x20     |      0x04      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *table* (byte): table Id; it might be 0x00\-0x08 or 0xff for TLV.
        - *fields* (binary string): Tags or Id.s list in bytes.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x20\x04')
    apdu.put_byte(table)
    apdu.put_byte(record)
    apdu.put_word(len(fields))
    apdu.put_blist(fields)
    connection.send_apdu(apdu)
    

def send_read_tags_cmd(connection, *tags):
    """
    Sends a **Read tags** command to SPOT.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x20     |      0x04      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *table* (byte): table Id; it might be 0x00\-0x08 or 0xff for TLV.
        - *tags* (binary string): Tags list in bytes.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x20\x04')
    apdu.put_byte(0xff) #fake table id
    apdu.put_byte(0xff) #fake record number
    apdu.put_word(sum(len(tag) for tag in tags))
    for tag in tags: apdu.put_array(tag)
    connection.send_apdu(apdu)

def recv_read_record_cmd(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **TLV or record field read** *(0x02,0x04)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters. 
    """
    apdu = connection.wait_apdu(0x80 + _apid, b'\x20\x04', timeout)
    ack_code = apdu.pop_byte()
    length = apdu.pop_word()
    tlv = apdu.pop_array(length)
    sdata = {}
    ndata = {}
    parse_tlv(tlv, sdata, ndata)
    ack_params = {
        'ack_code': ack_code,
        'sdata'   : sdata,
        'ndata'   : ndata
        }
    return ack_params

def parse_tlv(tlv, sdata, ndata):
    """
    Parses a *tlv message*.
    TODO: Move to Utility module.
    """
    print("TLV received: " + hexlify(tlv).decode('latin-1'))
    ofs = 0
    while len(tlv) > ofs:
        stag = tlv[ofs:ofs+1]
        tag = tlv[ofs]
        ofs += 1             #consume the first byte of the TAG
        if (tag & 31) == 31: #test if tag is multybyte
            tagb = tlv[ofs]
            while (tagb & 0x80) == 0x80:
                tag = tag << 8 + tagb
                stag += tlv[ofs:ofs+1]
                ofs += 1     #consume the additional bytes of the TAG
                tagb = tlv[ofs]
            stag += tlv[ofs:ofs+1]
            tag = tag << 8 + tagb
            ofs += 1         #consume the second byte of the TAG if any
        vlen = 0
        vlenb = tlv[ofs]
        ofs += 1             #consume the 1st byte of the length
        if (vlenb & 0x80) == 0x80:
            for i in range(0, vlenb & 0x7f):
                vlen += tlv[ofs]
                ofs += 1
        else:
            vlen = vlenb
        value = tlv[ofs:ofs+vlen]
        ofs += vlen          #consume the value bytes
        print("Tag found " + str(tag) + " " + hexlify(stag).decode('latin-1') + " " + hexlify(value).decode('latin-1'))
        ndata[tag] = value
        sdata[stag] = value

# Table dump Load
def send_table_dump_load_cmd(connection, table):
    """
    Sends a **Table Dump Load** *(0x02,0x06)* command to SPOT. 
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x20     |      0x06      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *table* (byte): Table number or 0xff for all tables loading.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x20\x06')
    connection.send_apdu(apdu)

def send_dump_load_cmd(connection):
    """
    Sends a **Table Dump Load** command to SPOT with ``tableId`` field set to 0xff (TLV) and the ``errorEncoding`` field set to 0x02.    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x20     |      0x06      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x20\x06\xff\x02')
    connection.send_apdu(apdu)

def recv_table_dump_load_cmd(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Table Dump Load** *(0x02,0x06)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.
    """
    apdu = connection.wait_apdu(0x80 + _apid, b'\x20\x06', timeout)
    ack_code = apdu.pop_byte()
    ack_params = {
            'ack_code': ack_code
        }
    return ack_params
