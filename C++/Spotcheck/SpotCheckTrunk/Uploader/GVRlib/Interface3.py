#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.Interface2 
   :platform: Unix, Windows
   :synopsis: Interface protocol management (Python 2).
Manage all command for Interface application using APDU object.
.. note::
    Aligned with Spot Programmers Guide v19.0.
"""

try:
    import pupyspot
except:
    from . import pupyspot

# Default Variables
_apid         = 0x00
_client_id    = 0xf0
_ping_timeout = 0x00
_type         = 0x02 # Enable extended login (only for VGD)


###Send Interface Command Methods Definition###

def login(connection, *, client_id = _client_id, ping_timeout = _ping_timeout, type = _type, modulo_len = 0, verbose = True, ack_expected = 0x00):
    """
    Sends a **Login** command and waits for an answer from the SPOT.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *client_id* (byte): Client ID:
            - 0x00\-0x01 = Not allowed,
            - 0x02\-0x7F = for SPOT local applications. An application logged to SPOT with Id <= 0x0F is intended internal; ids included in range 0x10\-0x7F are allowed but it is good practice to leave, them reserved for Gilbarco applications.
            - 0x80\-0xFF = available values for master programs running on PC.
        - *ping_timeout* (word): Length of PMK RSA module (may be zero = no Key required or in [0x0060,0x0200] range). Currently only the values 0x0000 and 0x0060 are supported (range is for future usage).  
        - * type* (byte):  Extended login (only for VGD).
        - *modulo_len* (byte): Specifies the maximum allowed time between two messages from master program (0x00=infinite seconds i.e.: no timeout).
        - *verbose* (boolean): Set verbose mode.
        - *ack_expected* (byte): Expected ``ack_code`` value.
    :Returns: ``True`` if check of ``ack_code`` is successful, ``False`` otherwise.
    """
    send_login_request(connection, client_id = client_id, ping_timeout = ping_timeout, type = type, modulo_len = modulo_len)
    ack_params = recv_login_request(connection, 5)
    if ack_params['ack_code' ] == ack_expected:
        if verbose:
            print('Login correctly executed!')
            print(' - Date&Time: ' + ack_params['date_time'].decode("latin-1"))
            entries = ack_params['entries']
            if len(entries) > 0:
                for entry in ack_params['entries']:
                    print(' - AppID: ' + str(entry['app_id']) + ' Sign: ' + hex(entry['sign']) + ' AppAscii: ' + entry['app_ascii'].decode("latin-1"))
            else:
                print('No entries in Login answer')
            pmk = ack_params['pmk_key']
            if pmk is None:
                print('No PMK key in Login answer')
            else:
                print(' - PMK Module: ' + pmk['rsa_mod'].decode("latin-1") + ' Exponent: ' + pmk['rsa_exp'].decode("latin-1"))
            if 'fwr_info' in ack_params:
                print(' - FWR Info: ' + ack_params['fwr_info'].decode("latin-1"))
            if 'hwr_info' in ack_params:
                print(' - HWR Info: ' + ack_params['hwr_info'].decode("latin-1"))
            if 'cert_status' in ack_params:
                print(' - Certificate Status: ' + str(ack_params['cert_status']))
        return True
    else:
        if verbose: print('ERROR: Login not executed (ACK = ' + hex(ack_params['ack_code' ]) + ')')
        return False

def send_login_request(connection, *, client_id = _client_id, ping_timeout = _ping_timeout, type = _type, modulo_len = 0):
    """
    Sends a **Login** request to SPOT.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x01      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *client\_id* (byte): Client ID:
            - 0x00\-0x01 = Not allowed,
            - 0x02\-0x7F = for SPOT local applications. An application logged to SPOT with Id <= 0x0F is intended internal; ids included in range 0x10\-0x7F are allowed but it is good practice to leave, them reserved for Gilbarco applications.
            - 0x80\-0xFF = available values for master programs running on PC.
        - *ping\_timeout* (word): Length of PMK RSA module (may be zero = no Key required or in [0x0060,0x0200] range). Currently only the values 0x0000 and 0x0060 are supported (range is for future usage).  
        - *type* (byte):  Extended login (only for VGD).
        - *modulo\_len* (byte): Specifies the maximum allowed time between two messages from master program (0x00=infinite seconds i.e.: no timeout). 
    """
    apdu = pupyspot.create_apdu(_apid, b'\x01')
    apdu.put_byte(client_id)     # client id
    apdu.put_word(modulo_len)    # PMK RSA module len (0x00 for no key required)
    apdu.put_byte(ping_timeout)  # timeout in seconds
    apdu.put_byte(type)          # extended login (only for VGD)
    connection.send_apdu(apdu)

def send_logout_request(connection, client_id = _client_id):
    """
    Sends a **Logout** request to SPOT.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x02      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *client_id* (byte): Client ID.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x02')
    connection.send_apdu(apdu)

def send_ping(connection):
    """
    Sends a **Ping** request to SPOT.
    This command is normally used only by external clients (controlling programs on PC) when logged to verify the connection.
    If no Ping or any APDU command is received from client within PingTimeout seconds an automatic logout is performed.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x03      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x03')
    connection.send_apdu(apdu)

def send_lock(connection, application_id):
    """
    Sends a **Lock** command to SPOT.
    This command is issued by a logged client to try to reserve the specified local application on SPOT.
    A specific error is returned to clients trying to lock a locked pinpad.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x04      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *application_id* (byte): Any of the available SPOT application in the range [0x01, 0x7F]. Values above 0x7F and the value 0x00 are not allowed (see TPDU format description for SPOT application identifier values).
    """
    apdu = pupyspot.create_apdu(_apid, b'\x04')
    apdu.put_byte(application_id)
    connection.send_apdu(apdu)

def send_unlock(connection, application_id):
    """
    Sends a **Unlock** command to SPOT.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x05      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *application_id* (byte): Any of the available SPOT application in the range [0x01, 0x7F]. Values above 0x7F and the value 0x00 are not allowed (see TPDU format description for SPOT application identifier values).
    """
    apdu = pupyspot.create_apdu(_apid, b'\x05')
    apdu.put_byte(application_id)
    connection.send_apdu(apdu) 

def send_reboot(connection):
    """
    Sends a **Reboot** command to SPOT.
    This command causes the SPOT to perform a system restart. This means:
        - That any connected client needs perform again the login (same as the SPOT was powered off),
        - Any processing in progress by internal application is interrupted and these application restarts in idle status,
        - All the applications are unlocked when they restart; so this command may be used by any external client (normally one acting as terminal supervisor) to prevent system blocking in case someone "forget" to unlock an application when finished to use it.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x06      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x06')
    connection.send_apdu(apdu)
def send_reboot_kernel_EMV(connection):
    """
    Sends a **Reboot KERNEL EMV** command to SPOT.
    This command causes the SPOT to reboot the EMV Kernel Application.
    The reboot is immediate and independent of the current EMV activity.
    The command is intended to be used mainly when the Kernel is in ERROR state and needs to restart to re\-read the configuration files (tables and parameters).
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x07      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x07')
    connection.send_apdu(apdu)
def send_init_log(connection, process_name):
    """
    Sends a **Init log** command to SPOT.
    The logging process is initialized and a name is provided by the application. Only for SPOT M5.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x08      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *process_name* (string): Name of the process to init.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x08')
    apdu.put_byte(len(process_name))
    apdu.put_array(process_name)
    connection.send_apdu(apdu)
def send_log_line(connection, log_type, datalib):
    """
    Sends a **log line** command to SPOT.
    Specifies the type of information to log as: error, warning, state change, command request, command received, debug, event and trace information.
    Only for SPOT M5.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x09      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *log_type* (byte): 
            - 0x00 = Error,
            - 0x01 = Warning,
            - 0x02 = InfoChangeState,
            - 0x03 = InfoCommanReceived,
            - 0x04 = InfoCommandRequest,
            - 0x05 = InfoData,
            - 0x06 = InfoTrace,
            - 0x07 = Debug,
            - 0x08 = Event.
        - *datalib* (dictionary): Log parameters.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x09')
    apdu.put_byte(log_type)
    if log_type == 0x00:
        apdu.put_byte(datalib['level'])
        apdu.put_byte(datalib['code'])
        apdu.put_byte(datalib['subcode'])
        apdu.put_byte(len(datalib['descr']))
        apdu.put_array(datalib['descr'])
    elif log_type == 0x01:
        apdu.put_byte(datalib['level'])
        apdu.put_byte(datalib['code'])
        apdu.put_byte(len(datalib['descr']))
        apdu.put_array(datalib['descr'])
    elif log_type == 0x02:
        apdu.put_byte(datalib['form_id'])
        apdu.put_byte(len(datalib['form_descr']))
        apdu.put_array(datalib['form_descr'])
        apdu.put_byte(datalib['to_id'])
        apdu.put_byte(len(datalib['to_descr']))
        apdu.put_array(datalib['to_descr'])
    elif log_type == 0x03:
        apdu.put_byte(datalib['protocol_type'])
        apdu.put_byte(datalib['sender'])
        apdu.put_byte(datalib['code'])
        apdu.put_byte(datalib['subcode'])
        apdu.put_byte(len(datalib['data']))
        apdu.put_array(datalib['data'])
    elif log_type == 0x04:
        apdu.put_byte(datalib['protocol_type'])
        apdu.put_byte(datalib['receiver'])
        apdu.put_byte(datalib['code'])
        apdu.put_byte(datalib['subcode'])
        apdu.put_byte(len(datalib['data']))
        apdu.put_array(datalib['data'])
    elif log_type == 0x05:
        apdu.put_byte(datalib['data_type'])
        apdu.put_byte(len(datalib['data']))
        apdu.put_array(datalib['data'])
    elif log_type == 0x06:
        apdu.put_byte(len(datalib['data']))
        apdu.put_array(datalib['data'])
    elif log_type == 0x07:
        apdu.put_byte(len(datalib['data']))
        apdu.put_array(datalib['data'])
    elif log_type == 0x08:  
        apdu.put_byte(datalib['event_id'])
        apdu.put_byte(len(datalib['event_descr']))
        apdu.put_array(datalib['event_descr'])
def send_session_keys(connection, modulo_len, data_buff):
    """
    Sends a **Session Keys** command to SPOT.
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x00     |      0x11      |
    +---------------+----------------+
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *modulo_len* (word): Length of RSA module or length of random bytes array (currently may be zero or 0x0060).
        - *data_buff* (binary string): Session Keys setup DataBuf RSA encrypted under PMK. It contains 5 3DES Session Keys (SK) and an initial padding starting with a fixed zero followed by 15 random bytes. SK\#1 is also used for Mkcur derivation.
    """
    apdu = pupyspot.create_apdu(_apid, b'\x11')
    apdu.put_word(modulo_len)
    apdu.put_array(data_buff)
    connection.send_apdu(apdu)

# Only for VGD
def send_maintenance_request(connection, tokendata):
    apdu = pupyspot.create_apdu(_apid, b'\x00\x0c')
    apdu.put_byte(0x01)
    apdu.put_array(tokendata)
    connection.send_apdu(apdu)


# Receive 

def recv_login_request(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Login** *(0x00,0x01)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.    
    """
    apdu = connection.wait_apdu(0x80 + _apid, b'\x01', timeout)
    ack_code = apdu.pop_byte()
    if ack_code is None:
        return False
    if ack_code == 0x00:
        ack_params = {
                'ack_code'      : 0x00,
                'date_time'     : apdu.pop_array(14),
                'num_apps'      : apdu.pop_byte(),
                'entries'       : [],
                'pmk_key'       : None
            }
        for i in range(0, ack_params['num_apps']):
            entry = {
                    'app_id'    : apdu.pop_byte(),
                    'sign'      : apdu.pop_dword(),
                    'app_ascii' : apdu.pop_array(40)
                    }
            ack_params['entries'].append(entry)
        rsa_mod_len = apdu.pop_word()
        rsa_exp_len = apdu.pop_word()
        if rsa_mod_len > 0 and rsa_exp_len > 0:
            ack_params['pmk_key'] = {
                                    'rsa_mod': apdu.pop_array(rsa_mod_len),
                                    'rsa_exp': apdu.pop_array(rsa_exp_len)
                                    }
        else:
            ack_params['pmk_key'] = None
        if not apdu.is_empty():
            info_len = apdu.pop_byte()
            if info_len > 0:
                ack_params['fwr_info'] = apdu.pop_array(info_len)
            info_len = apdu.pop_byte()
            if info_len > 0:
                ack_params['hwr_info'] = apdu.pop_array(info_len)
            ack_params['cert_status'] = apdu.pop_byte()
    else:
        ack_params = {
                'ack_code': ack_code
            }
    return ack_params

def recv_logout_result(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Logout** *(0x00,0x02)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.    
    """
    apdu = connection.wait_apdu(0x80 + _apid, b'\x02', timeout)
    ack_params = {
                'ack_code' : apdu.pop_byte()
                }
    return ack_params

def recv_ping_result(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Ping** *(0x00,0x03)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.    
    """

    apdu = connection.wait_apdu(0x80 + _apid, b'\x03', timeout)
    ack_params = {
                'ack_code' : apdu.pop_byte()
                }
    return ack_params

def recv_lock(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Lock** *(0x00,0x04)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.    
    """

    apdu = connection.wait_apdu(0x80 + _apid, b'\x04', timeout)
    ack_params = {
                'ack_code' : apdu.pop_byte()
                }
    return ack_params

def recv_unlock(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Unlock** *(0x00,0x05)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.    
    """

    apdu = connection.wait_apdu(0x80 + _apid, b'\x05', timeout)
    ack_params = {
                'ack_code' : apdu.pop_byte()
                }
    return ack_params

def recv_reboot_kernel_EMV(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Reboot KERNEL EMV** *(0x00,0x07)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.    
    """ 
    apdu = connection.wait_apdu(0x80 + _apid, b'\x07', timeout)
    ack_params = {
                'ack_code' : apdu.pop_byte()
                }
    return ack_params

def recv_init_log(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Init Log** *(0x00,0x08)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.
    """ 
    apdu = connection.wait_apdu(0x80 + _apid, b'\x08', timeout)
    ack_params = {
                'ack_code' : apdu.pop_byte()
                }
    return ack_params

def recv_log_line(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Log Line** *(0x00,0x09)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.    
    """ 
    apdu = connection.wait_apdu(0x80 + _apid, b'\x09', timeout)
    ack_params = {
                'ack_code' : apdu.pop_byte()
                }
    return ack_params

def recv_diagnostic(connection, timeout = 0):
    """
    Receives an unsolicited diagnostic message *(0x00,0x0F)* sent only in case of an error conditions on protocol management.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the *error code* parameter: 0x01 = Data encryption failure (other values reserved for future usage).    
    """
    apdu = connection.wait_apdu(0x80 + _apid, b'\x0F', timeout)
    ack_params = {
                'error_code' : apdu.pop_byte()
                }
    return ack_params

def recv_session_keys(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Session Keys** *(0x00,0x1)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.    
    """
    apdu = connection.wait_apdu(0x80 + _apid, b'\x11', timeout)
    ack_params = {
                'ack_code' : apdu.pop_byte()
                }
    return ack_params

# Only for VGD
def recv_maintenance_request(connection, timeout = 0):
    apdu = connection.wait_apdu(0x80 + _apid, b'\x00\x0c', timeout)
    ack_code = apdu.pop_byte()
    if ack_code == 0x00:
        ack_params = {
                'ack_code': 0x00,
                #TODO: complete answer decode
            }
    else:
        ack_params = {
                'ack_code': ack_code
            }
    return ack_params


if __name__ == '__main__':
    #Generic lib
    import sys, os, time
    from binascii import hexlify, unhexlify

    #Protocol lib
    import Logger
    import spotPinpad

    #Utility function
    def initConn(ip):
        """
        Initializes a PupySPOT connection to the SPOT.
        :Parameters:
            - *ip* (string): Ip address of the SPOT..
        :Returns: A *PupySPOT* object.
        """
        conn = pupyspot.PupySPOT()
        conn.emit_ping = True
        conn.connect(ip)
        return conn

    def checkACK(ack_params, ack_expected = 0x00):
        """
        Checks an "ack APDU" received from SPOT.
        :Parameters:
            - *ack_params* (dictionary): Dictionary containing the parameters of the "ack APDU".
            - *ack_expected* (dictionary): Expected value of  ``ack_code`` element.
        :Returns: ``True``if check is successful, ``False`` if check fails.    
        """
        if ack_params['ack_code'] == ack_expected:
            print('Check pass')
            return True
        else:
            print('Check fail: Expected => ' + hex(ack_expected) + ' Returned => ' + hex(ack_params['ack_code']))
            return False

    def loginTest(connection, client_id, modulo_len, ping_timeout, ack_expected,dologout, verbose, silence = False):
        send_login_request(connection, client_id, ping_timeout, modulo_len)
        ack_params = recv_login_request(connection, 4)
        if ack_params['ack_code' ] == ack_expected:
            if not silence:
                print('Login: Check pass!')
            if verbose and not silence:
                print(' - Date&Time: ' + ack_params['date_time'].decode("latin-1"))
                entries = ack_params['entries']
                if len(entries) > 0:
                    for entry in ack_params['entries']:
                        print(' - AppID: ' + str(entry['app_id']) + ' Sign: ' + hex(entry['sign']) + ' AppAscii: ' + entry['app_ascii'].decode("latin-1"))
                else:
                    print('No entries in Login answer')
                pmk = ack_params['pmk_key']
                if pmk is None:
                    print('No PMK key in Login answer')
                else:
                    print(' - PMK Module: ' + pmk['rsa_mod'].decode("latin-1") + ' Exponent: ' + pmk['rsa_exp'].decode("latin-1"))
                if 'fwr_info' in ack_params:
                    print(' - FWR Info: ' + ack_params['fwr_info'].decode("latin-1"))
                if 'hwr_info' in ack_params:
                    print(' - HWR Info: ' + ack_params['hwr_info'].decode("latin-1"))
                if 'cert_status' in ack_params:
                    print(' - Certificate Status: ' + str(ack_params['cert_status']))
            if dologout and ack_params['ack_code' ] == 0x00:
                send_logout_request(connection)
                ack_params = recv_logout_result(connection, 4)
                if not silence:
                    if ack_params['ack_code' ] == 0x00:
                        print('Logout: Check pass!')
                    else:
                        print('Logout: Check fail')
            return True
        else:
            print('Login: Check fail Exp=' + hex(ack_expected) + ' Ret=' + hex(ack_params['ack_code' ]))
            return False

    #Take parameters
    ip = input('Insert SPOT IP: ')

    platform_list=[ 'M3', 'NGP', 'VGD' ]
    platform = 0
    print('###SELECT PLATFORM###')
    for i in range(1, len(platform_list) + 1):
        print(str(i) + '. ' + platform_list[i - 1])

    while platform not in range(1, len(platform_list) + 1):
        platform = int(input('>> '))
    platform = platform_list[platform - 1]

    print('Platform selected: ' + platform)

    dist = ''
    while dist not in [ 'P', 'D' ]:
        dist = input('(P)rod / (D)evel: ')

    cycle = ''
    while cycle not in [ 'Y', 'N' ]:
        cycle = input('Do you want to execute test with all possible value (where is possible)? (Y)es / (N)o: ')

    #Initialize file
    br = '-----------------------------------------------------------------------------------------------'
    print(br)
    print(br)
    #################################################################################################
    #################################################################################################
    print('Login command:')
    #################################################################################################
    print('>>7.1.1 Positive case<<')
    Conn1 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 0, 0x00, True, True)
    Conn1.disconnect()
    print(br)
    #################################################################################################
    print('>>7.1.2 Positive case with timeout<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 10, 0x00, False, False)
    try:
        for i in range(1,12):
            print(i, '', end = "")
            sys.stdout.flush()
            time.sleep(1)
    except:
        pass
    print()
    Conn2 = initConn(ip)
    loginTest(Conn2, 0x20, 0, 10, 0x00, True, True)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    print('>>7.1.3 Negative case with timeout<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 10, 0x00, False, False)
    try:
        for i in range(1,6):
            print(i, '', end = "")
            sys.stdout.flush()
            time.sleep(1)
    except:
        pass
    print()
    loginTest(Conn2, 0x20, 0, 10, 0xA1, False, False)
    time.sleep(5)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    print('>>7.1.4 Negative case - Different client using the same id<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 10, 0x00, False, False)
    loginTest(Conn2, 0x20, 0, 10, 0xA1, False, False)
    time.sleep(10)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    print('>>7.1.5 Positive case - Different client using the same id<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 10, 0x00, True, True)
    loginTest(Conn1, 0x20, 0, 10, 0x00, True, True)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    print('>>7.1.6 Positive case - Different client using different id<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 10, 0x00, False, False)
    loginTest(Conn2, 0x20, 0, 10, 0xA1, False, False)
    time.sleep(10)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    print('>>7.1.7 Bad ModuloLen value<<')
    Conn1 = initConn(ip)
    if cycle == 'Y':
        r = range(0,0xFFFF)
    else:
        r = [0x0000,0x0060,0xFFFF]
    modLen_error=[]
    for i in r:
        if i == 0x0000 or 0x0060:
            ack = 0x00
        else:
            ack = 0xA4
        if loginTest(Conn1, 0x20, i, 5, ack, True, False, True) == False:
            modLen_error.append(i)
    if len(modLen_error) > 0:
        print('Error for ModuloLen field:')
        print(str(modLen_error))
    else:
        print('Check pass!')
    print(br)
    Conn1.disconnect()
    Conn2.disconnect()
    time.sleep(5)
    #################################################################################################
    print('>>7.1.8 Internal client ID<<')
    Conn1 = initConn(ip)
    if cycle == 'Y':
        r = range(1,16)
    else:
        r = range(9,10)
    for i in r:
        print('ID #'+str(i)),
        if dist == 'D':
            ack = 0x00
        else:
            ack = 0xA4
        loginTest(Conn1, i, 0, 5, ack, True, False)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    #################################################################################################
    print(br)
    print('Logout command:')
    #################################################################################################
    print('>>7.2.1 Positive case<<')
    Conn1 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 0, 0x00, True, True)
    Conn1.disconnect()
    print(br)
    #################################################################################################
    print('>>7.2.2 Without login<<')
    Conn1 = initConn(ip)
    send_logout_request(Conn1)
    ack_params = recv_logout_result(Conn1)
    print('Logout: '),
    checkACK(ack_params,0x80)
    Conn1.disconnect()
    print(br)
    #################################################################################################
    print('>>7.2.3 Different Client between Login and Logout<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 0, 0x00, False, True)
    loginTest(Conn2, 0x21, 0, 0, 0x00, False, True)
    send_logout_request(Conn2)
    ack_params = recv_logout_result(Conn2)
    print('Logout: '),
    checkACK(ack_params,0x00)
    send_lock(Conn1, 0x01)
    ack_params = recv_lock(Conn1)
    print('Lock: '),
    checkACK(ack_params)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    #################################################################################################
    print(br)
    print('Ping command:')
    #################################################################################################
    print('>>7.3.1 Positive case<<')
    Conn1 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 5, 0x00, False, False)
    send_ping(Conn1)
    ack_params = recv_ping_result(Conn1,4)
    print('Ping: '),
    checkACK(ack_params)
    send_logout_request(Conn1)
    Conn1.disconnect()
    print(br)
    #################################################################################################
    print('>>7.3.2 Without login<<')
    Conn1 = initConn(ip)
    send_ping(Conn1)
    ack_params = recv_ping_result(Conn1)
    print('Ping: '),
    checkACK(ack_params)
    Conn1.disconnect()
    print(br)
    #################################################################################################
    #################################################################################################
    print(br)
    print('Lock command:')
    #################################################################################################
    print('>>7.4.1 Positive case<<')
    Conn1 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 5, 0x00, False, False)
    send_lock(Conn1, 0x01)
    ack_params = recv_lock(Conn1)
    print('Lock: '),
    checkACK(ack_params)
    send_logout_request(Conn1)
    Conn1.disconnect()
    print(br)
    #################################################################################################
    print('>>7.4.2 Check lock<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 0, 0x00, False, False)
    loginTest(Conn2, 0x21, 0, 0, 0x00, False, False)
    send_lock(Conn1, 0x01)
    ack_params = recv_lock(Conn1)
    print('Lock: '),
    checkACK(ack_params)
    spotPinpad.send_system_status(Conn1, 0x10)
    ack_params = spotPinpad.recv_system_status(Conn1)
    print('System status: '),
    checkACK(ack_params, 0x00)
    spotPinpad.send_system_status(Conn2, 0x10)
    ack_params = spotPinpad.recv_system_status(Conn2)
    print('System status: '),
    checkACK(ack_params, 0x92)
    if platform == 'VGD':
        Pinpad_VGD.send_cert(Conn2, 'blablabla')
        ack_params = Pinpad_VGD.recv_cert(Conn2)
        print('Send certificates: '),
        checkACK(ack_params, 0x92)
        Pinpad_VGD.send_delete_key(Conn2, 0x01)
        ack_params = Pinpad_VGD.recv_send_delete_key(Conn2)
        print('Delete Key: '),
        checkACK(ack_params, 0x92)
    send_logout_request(Conn1)
    send_logout_request(Conn2)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    print('>>7.4.3 Automatic Unlock after logout<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 5, 0x00, False, False)
    loginTest(Conn2, 0x21, 0, 5, 0x00, False, False)
    send_lock(Conn1, 0x01)
    ack_params = recv_lock(Conn1)
    print('Lock: '),
    checkACK(ack_params)
    send_logout_request(Conn1)
    spotPinpad.send_system_status(Conn2, 0x10)
    ack_params = spotPinpad.recv_system_status(Conn2)
    print('System status: '),
    checkACK(ack_params, 0x00)
    if platform == 'VGD':
        Pinpad_VGD.send_cert(Conn2, 'blablabla')
        ack_params = Pinpad_VGD.recv_cert(Conn2)
        print('Send certificates: '),
        checkACK(ack_params, 0x00)
        Pinpad_VGD.send_delete_key(Conn2, 0x01)
        ack_params = Pinpad_VGD.recv_send_delete_key(Conn2)
        print('Delete Key: '),
        checkACK(ack_params, 0x00)
    send_logout_request(Conn2)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    print('>>7.4.4 Automatic Unlock after logout for timeout<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 5, 0x00, False, False)
    loginTest(Conn2, 0x21, 0, 10, 0x00, False, False)
    send_lock(Conn1, 0x01)
    ack_params = recv_lock(Conn1)
    print('Lock: '),
    checkACK(ack_params)
    time.sleep(5)
    spotPinpad.send_system_status(Conn2, 0x10)
    ack_params = spotPinpad.recv_system_status(Conn2)
    print('System status: '),
    checkACK(ack_params, 0x00)
    if platform == 'VGD':
        Pinpad_VGD.send_cert(Conn2, 'blablabla')
        ack_params = Pinpad_VGD.recv_cert(Conn2)
        print('Send certificates: '),
        checkACK(ack_params, 0x00)
        Pinpad_VGD.send_delete_key(Conn2, 0x01)
        ack_params = Pinpad_VGD.recv_send_delete_key(Conn2)
        print('Delete Key: '),
        checkACK(ack_params, 0x00)

    send_logout_request(Conn2)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    #################################################################################################
    print(br)
    print('Unlock command:')
    #################################################################################################
    print('>>7.5.1 Positive case<<')
    Conn1 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 5, 0x00, False, False)
    send_lock(Conn1, 0x01)
    ack_params = recv_lock(Conn1)
    print('Lock: '),
    checkACK(ack_params)
    send_unlock(Conn1, 0x01)
    ack_params = recv_unlock(Conn1)
    print('Unlock: '),
    checkACK(ack_params)
    send_logout_request(Conn1)
    Conn1.disconnect()
    print(br)
    #################################################################################################
    print('>>7.5.2 Unlock without lock<<')
    Conn1 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 5, 0x00, False, False)
    send_unlock(Conn1, 0x01)
    ack_params = recv_unlock(Conn1)
    print('Unlock: '),
    checkACK(ack_params,0x91)
    send_logout_request(Conn1)
    Conn1.disconnect()
    print(br)
    #################################################################################################
    print('>>7.5.3 Unlock from different client<<')
    Conn1 = initConn(ip)
    Conn2 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 5, 0x00, False, False)
    loginTest(Conn2, 0x21, 0, 5, 0x00, False, False)
    send_lock(Conn1, 0x01)
    ack_params = recv_lock(Conn1)
    print('Lock: '),
    checkACK(ack_params)
    send_unlock(Conn2, 0x01)
    ack_params = recv_unlock(Conn2)
    print('Lock: '),
    checkACK(ack_params,0x92)
    send_logout_request(Conn1)
    send_logout_request(Conn2)
    Conn1.disconnect()
    Conn2.disconnect()
    print(br)
    #################################################################################################
    print('>>7.5.4 Unlock with client logged out an re-logged in<<')
    Conn1 = initConn(ip)
    loginTest(Conn1, 0x20, 0, 5, 0x00, False, False)
    send_lock(Conn1, 0x01)
    ack_params = recv_lock(Conn1)
    print('Lock: '),
    checkACK(ack_params)
    send_logout_request(Conn1)
    loginTest(Conn1, 0x20, 0, 5, 0x00, False, False)
    send_unlock(Conn1, 0x01)
    ack_params = recv_unlock(Conn1)
    print('Unlock: '),
    checkACK(ack_params, 0x91)
    send_logout_request(Conn1)
    Conn1.disconnect()
    print(br)
    #################################################################################################
    #################################################################################################
    print(br)
    print('Reset command:')
    #################################################################################################
    print('>>7.6.1 Positive case<<')
    print(br)
    #################################################################################################
    print('>>7.6.2 Reset without login<<')
    print(br)
    #################################################################################################
    #################################################################################################
    print(br)
    print('ResetKernelEMV command:')
    #################################################################################################
    print('>>7.7.1 Positive case<<')
    print(br)
    #################################################################################################
    print('>>7.7.2 ResetKernelEMV without login<<')
    print(br)
    #################################################################################################
    print('>>7.7.3 ResetKernelEMV without login<<')
    print(br)
    #################################################################################################
    #################################################################################################
    print(br)
    print('InitLog command:')
    #################################################################################################
    print('>>7.8.1 Positive case<<')
    print(br)
    #################################################################################################
    #################################################################################################
    print(br)
    print('LogLine command:')
    #################################################################################################
    print('>>7.9.1 Positive case<<')
    print(br)
    #################################################################################################
