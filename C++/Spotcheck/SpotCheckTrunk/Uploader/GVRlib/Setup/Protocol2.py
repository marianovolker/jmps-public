"""
.. module:: GVRlib.Setup.Protocol2 
   :platform: Unix, Windows
   :synopsis: Setup protocol management (Python 2).

Manage all command for Pinpad application using APDU object.

.. note::
    Aligned with Spot Programmers Guide v19.0.

"""


try:
	import pupyspot
except:
	from .. import pupyspot

# Default Variables
_apid    = 3
_TIMEOUT = 0

def send_downloadRequest(connection, filesize, cert, cmd, option):
	'''Download request: 0x10, 0x01'''
	apdu = protocol.create_apdu(_apid, '\x10\x01')
	apdu.put_dword(filesize)
	apdu.put_array(cert)
	apdu.put_byte(cmd)
	apdu.put_byte(option)
	connection.send_apdu(apdu)

def send_downloadBlock(connection, blksize = 0, blkdata = None):
	'''Download block: 0x10, 0x02'''
	apdu = protocol.create_apdu(_apid, '\x10\x02')
	apdu.put_word(blksize)
	if blksize:
		apdu.put_array(blkdata)
	connection.send_apdu(apdu)

def send_package_info(connection, _set):
    """
    Sends a **Package info** command to SPOT. This command queries for package info. 
    It is possible to select between two package sets: the currently active set or a new set of packages already downloaded but not activated yet.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x10     |      0x03      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *\_set* (byte): 
            - 0x00 = active package set,
            - 0x01 = new package set.

    """
	apdu = pupyspot.create_apdu(_apid, '\x10\x03')
	apdu.put_byte(_set)
	connection.send_apdu(apdu)

def send_activatePackage(connection, time = '0000.00.00-00:00'):
	'''Activate Package: 0x10, 0x04'''
	apdu = protocol.create_apdu(_apid, '\x10\x04')
	apdu.put_array(time)
	connection.send_apdu(apdu)

def recv_downloadRequest(connection, timeout = _TIMEOUT):
	'''Download response: 0x10, 0x01'''
	apdu = connection.wait_apdu(0x80 + _apid, '\x10\x01', timeout)
	if apdu is None:
		return None
	ack_code = apdu.pop_byte()
	if ack_code is None:
		return None
	if ack_code == 0x00:
		ack_params = {
			'ack_code': 0x00,
			'Offset': apdu.pop_dword()
		}
	else:
		ack_params = {
		 'ack_code': ack_code
		}
	return ack_params

def recv_downloadBlock(connection, timeout = _TIMEOUT):
	'''Download block: 0x10, 0x02'''
	apdu = connection.wait_apdu(0x80 + _apid, '\x10\x02', timeout)
	if apdu is None:
		return None
	ack_code = apdu.pop_byte()
	if ack_code is None:
		return None
	ack_params = {'ack_code': ack_code}
	return ack_params

def recv_package_info(connection, timeout = _TIMEOUT):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Package Info** *(0x10,0x03)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	'''Package info: 0x10, 0x03'''
	apdu = connection.wait_apdu(0x80 + _apid, '\x10\x03', timeout)
	if apdu is None:
		return None
	ack_code = apdu.pop_byte()
	if ack_code is None:
		return None
	ack_params = {
			'ack_code': ack_code
		}
	if ack_code == 0x00:
		ack_params['PackNum'] = apdu.pop_byte()
		ack_params['Pack'] = []
		for i in range (0, ack_params['PackNum']):
			ack_params['Pack'].append(apdu.pop_array(248))
	return ack_params

def recv_activationPackage(connection, timeout = 0):
	'''Activate Package: 0x10, 0x04'''
	apdu = connection.wait_apdu(0x80 + _apid, '\x10\x04', timeout)
	if apdu is None:
		return None
	ack_code = apdu.pop_byte()
	if ack_code is None:
		return None
	ack_params = {'ack_code': ack_code}
	return ack_params
