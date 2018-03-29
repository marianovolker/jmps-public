from builtins import print

#__author__ = 'sayyedm'

from GVRlib import protocol
import os, sys
current_dir = os.path.dirname(os.path.abspath(sys.argv[0]))
_resources_path_M3= os.path.join(current_dir, 'SPOT\\M3')
_resources_path_M5= os.path.join(current_dir, 'SPOT\\M5')

# Default Variables
_apid = 0x01

def send_systemStatus(connection, device):
    '''System Status : 0x01 0x010 0x01
    Accepts one parameter device BYTE
    '''

    if device is None:
        return None
    apdu = protocol.create_apdu(_apid, b'\x10\x01'+ device)
    if apdu is None:
        return None

    connection.send_apdu(apdu)

def recv_systemStatus(connection, timeout=0):
    '''System Status: 0x01 0x010 0x01'''
    apdu = connection.wait_apdu(0x80 + _apid, b'\x10\x01', timeout)
    if apdu is None:
        return None
    ack_code = apdu.pop_byte()
    if ack_code is None:
        return False
    if ack_code == 0x00:
        ack_params = {
            'ack_code': 0x00,
            'num_entries': apdu.pop_byte(),
            'entries': []
        }
        if ack_params['num_entries'] > 0x00:
            for i in range(0, ack_params['num_entries']):
                entry = {
                    'device': apdu.pop_byte(),
                    'status': apdu.pop_byte(),
                    'status_extension': apdu.pop_byte(),
                    'error_code':apdu.pop_byte()
                }
                ack_params['entries'].append(entry)

    else:
        ack_params = {
         'ack_code': ack_code
        }
    return ack_params




def send_rfsInfo(connection):
    '''Software configuration: 0x01 0x010 0x03'''
    apdu = protocol.create_apdu(_apid, b'\x10\x03')
    if apdu is None:
        return None
    apdu.put_byte(0x04)
    apdu.put_byte(0x07)
    apdu.put_byte(0x08)
    apdu.put_byte(0x09)
    apdu.put_byte(0x0a)
    connection.send_apdu(apdu)

def recv_swInfo(connection, timeout=0):
    '''Software configuration: 0x01 0x010 0x03'''
    apdu = connection.wait_apdu(0x80 + _apid, b'\x10\x03', timeout)
    if apdu is None:
        return None
    ack_code = apdu.pop_byte()
    if ack_code is None:
        return False
    if ack_code == 0x00:
        ack_params = {
            'ack_code': 0x00,
            'num_entries': apdu.pop_byte(),
            'entries': []
        }
        for i in range(0, ack_params['num_entries']):
            tag = apdu.pop_byte()
            tag_len = apdu.pop_byte()
            entry = {
                'tag_id': tag,
                'tag_len': tag_len,
                'tag_value': apdu.pop_array(tag_len)
            }
            ack_params['entries'].append(entry)

    else:
        ack_params = {
         'ack_code': ack_code
        }
    return ack_params

def send_dispInfo(connection):
    ''' Display  status: 0x01 0x30 0x0F'''
    apdu = protocol.create_apdu(_apid, b'\x30\x0F')
    if apdu is None:
        return None
    connection.send_apdu(apdu)


def recv_dispInfo(connection, timeout=0):
    ''' Receive Display Status Info'''
    ack_params = None
    apdu = connection.wait_apdu(0x80 + _apid, b'\x30\x0F', timeout)
    if apdu is None:
        return None
    ack_code = apdu.pop_byte()
    if ack_code is None:
        return False
    if ack_code == 0x00:
        ack_params = {
            'ack_code' :0x00,
            'count':apdu.pop_array(2)
        }
    return ack_params

def send_fileInfo(connection):
    ''' File Browse: 0x01 0x00 0x03 0x15'''
    apdu = protocol.create_apdu(_apid, b'\x00\x03\x15')
    if apdu is None:
        return None
    connection.send_apdu(apdu)

def recv_fileInfo(connection, timeout=0):
    ''' Receive File Browse Info'''
    apdu = connection.wait_apdu(0x80 + _apid, b'\x00\x03', timeout)
    if apdu is None:
        return None
    ack_code = apdu.pop_byte()
    if ack_code is None:
        return False
    if ack_code == 0x00:
        ack_params = {
            'ack_code': 0x00,
            'file_type' : apdu.pop_byte(),
            'num_entries': apdu.pop_byte()
        }
        return ack_params['num_entries']

def recv_logout_result(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Logout** *(0x00,0x02)* command.
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
    :Returns: A dictionary containing the "answer APDU" parameters.
    """
    apdu = connection.wait_apdu(0x80 + _apid, b'\x02', timeout)
    if apdu is None:
        return None
    ack_params = {
                'ack_code' : apdu.pop_byte()
                }
    return ack_params

def send_hardware_info(connection):
    apdu = protocol.create_apdu(_apid, b'\x10\x02\x02\x42\x01')
    if apdu is None:
        return None
    connection.send_apdu(apdu)

def recv_hardware_info(connection, timeout = 0):
     apdu = connection.wait_apdu(0x80 + _apid, b'\x10\x02', timeout)
     print('apdu '+ str(apdu))
     ack_params = None
     if apdu is None:
        return None
     ack_code = apdu.pop_byte()
     if ack_code is None:
            return False

     if ack_code == 0x00:
        num_entries = apdu.pop_byte()
        tag_id = apdu.pop_byte()
        tag_len = apdu.pop_byte()
        tag_data = apdu.pop_array(tag_len)
        tag_ppnid = apdu.pop_byte()
        tag_ppnlen = apdu.pop_byte()
        tag_ppndata = apdu.pop_array(tag_ppnlen)
        ack_params = {
                'ack_code' : 0x00,
				'num_entries' : num_entries,
				'Tag_Id' : tag_id,
				'Tag_len'  : tag_len,
                'Tag_data': tag_data,
                'Tag_ppnId' : tag_ppnid,
                'Tag_ppnlen' : tag_ppnlen,
                'Tag_ppndata' : tag_ppndata

	          }
     return ack_params






