__author__ = 'chaudk'

from GVRlib import protocol


# Default Variables
_apid = 0x03

def send_downloadRequest(connection, filesize, cert, cmd, option):
    ###Download request: 0x03 0x10 0x01
    apdu = protocol.create_apdu(_apid, b'\x10\x01')
    if apdu is None:
        return None
    apdu.put_dword(filesize)
    apdu.put_array(cert)
    apdu.put_byte(cmd)
    apdu.put_byte(option)
    connection.send_apdu(apdu)

def recv_downloadRequest(connection, timeout=0):
    ###Download response: 0x03 0x10 0x01
    apdu = connection.wait_apdu(0x80 + _apid, b'\x10\x01', timeout)
    if apdu is None:
        return None
    ack_code = apdu.pop_byte()
    if ack_code is None:
        return False
    if ack_code == 0x00:
        ack_params = {
            'ack_code': 0x00,
            'Offset': apdu.pop_array(4)
        }
    else:
        ack_params = {
         'ack_code': ack_code
        }
    return ack_params


def send_downloadBlock(connection, blksize = 0, blkdata = None):
    ###Download block: 0x03 0x10 0x02
    apdu = protocol.create_apdu(_apid, b'\x10\x02')
    if apdu is None:
        return None
    apdu.put_word(blksize)
    if blksize > 0:
        apdu.put_array(blkdata)
    connection.send_apdu(apdu)


def recv_downloadBlock(connection, timeout=0):
    ###Download block: 0x03 0x10 0x02
    apdu = connection.wait_apdu(0x80 + _apid, b'\x10\x02', timeout)
    if apdu is None:
        return None
    ack_code = apdu.pop_byte()
    if ack_code is None:
        return False
    ack_params = {'ack_code': ack_code}

    return ack_params


def send_packageInfo(connection, set):
    '''Package info: 0x03 0x10 0x03'''
    apdu = protocol.create_apdu(_apid, b'\x10\x03')
    if apdu is None:
        return None
    apdu.put_byte(set)
    connection.send_apdu(apdu)


def recv_packageInfo(connection, timeout=0):
    ###Package Info: 0x03 0x10 0x03
    apdu = connection.wait_apdu(0x80 + _apid, b'\x10\x03', timeout)
    if apdu is None:
        return None
    ack_code = apdu.pop_byte()
    if ack_code is None:
        return False
    if ack_code == 0x00:
        ack_params = {
            'ack_code': 0x00,
            'PackNum': apdu.pop_byte(),
            'entries': []
        }
        for i in range(0, ack_params['PackNum']):
            entry = {
                'certificate': apdu.pop_array(248)
            }
            ack_params['entries'].append(entry)
    else:
        ack_params = {
         'ack_code': ack_code
        }
    return ack_params

def send_activatePackage(connection):
    ###Activate Package: 0x03 0x10 0x04
    apdu = protocol.create_apdu(_apid, b'\x10\x04')
    if apdu is None:
        return None
    time = b'0000.00.00-00:00'
    apdu.put_array(time)
    connection.send_apdu(apdu)

def recv_activationPackage(connection, timeout=0):
    ###Activate Package: 0x03 0x10 0x04
    apdu = connection.wait_apdu(0x80 + _apid, b'\x10\x04', timeout)
    if apdu is None:
        return None
    ack_code = apdu.pop_byte()
    if ack_code is None:
        return False
    ack_params = {'ack_code': ack_code}
    return ack_params


