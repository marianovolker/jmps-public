"""
.. module:: GVRlib.OPT.Protocol2 
   :platform: Unix, Windows
   :synopsis: OPT protocol management (Python 2).

Manage all command for OPT application using APDU object.

.. note::
    Aligned with Spot Programmers Guide v19.0.

"""

try:
	import pupyspot
except:
	from .. import pupyspot

# Default Variables

_apid = 4

def send_status(connection, device):
	"""
	Sends a **Status request** command to SPOT.
	This command allows to retrieve information about Printer, the OTI contactless reader (when connected) and to read the logical state of spare inputs. Status request for BNA device is not required with (0x10, 0x01) but rather with (0x90, 0x04).
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x10     |      0x01      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *device* (byte): The Device field is a bitmask: more than one device at a time can be queried.

	"""
	apdu = pupyspot.create_apdu(_apid, '\x10\x01')
	apdu.put_byte(device)
	connection.send_apdu(apdu)


def send_barcode_enable(connection):
	"""
	Sends a **Barcode Reader - Enable** command to SPOT.
	It enables/disables barcode reader.
	Champtek barcode reader will be enabled until a disable command is received: otherwise the Honeywell one will be disabled after a timeout about 10 seconds.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x40     |      0x02      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.

	"""
	apdu = pupyspot.create_apdu(_apid, '\x40\x02')
	connection.send_apdu(apdu)

def send_oti_enable_cmd(connection, transaction_amount = None, transaction_data = None, transaction_type = None, transaction_time = None):
	"""
	Sends a **Contact less reader - Enable** command to SPOT.
	It enables the contactless reader with the Card Application and Security schema selected according to the parameter ApplicationMask (see the *SPOT Programmers Guide*).
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x50     |      0x01      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *transaction_amount* (binary string) 
		- *transaction_data* (binary string)
		- *transaction_type* (byte)
		- *transaction_time* (binary string)

	"""
	apdu = pupyspot.create_apdu(_apid, '\x50\x01')
    # Put Application Mask for backward compatibily
	apdu.put_word(0x0000)

	if (transaction_amount):
		apdu.put_array(transaction_amount)
	else:
		apdu.put_array('\x00\x00\x00\x00\x11\x22')

	if (transaction_data):
		apdu.put_array(transaction_data)
	else:
		apdu.put_array('\x10\x01\x11')

	if (transaction_type):
		apdu.put_byte(transaction_type)
	else:
		apdu.put_byte(0x00)

	if (transaction_time):
		apdu.put_array(transaction_time)
	else:
		apdu.put_array('\x00\x00\x00')

	connection.send_apdu(apdu)

def send_oti_disable_cmd(connection):
	"""
	Sends a *Contact less reader disable* command to SPOT. It disables the contactless reader.
            
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x50     |      0x05      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.


	"""
	apdu = pupyspot.create_apdu(_apid, '\x50\x05')
	connection.send_apdu(apdu)

def send_enqueue_data(connection, jobID, seqNumber, _text):
	"""
	Sends a **Enqueue data** command to SPOT.
	This function is used to print both text and graphics, using Escape sequences (see the *SPOT Programmers Guide*) to switch from Text Mode to Graphics Mode and vice-versa. 
	Each printing "session" is identified as a "job", that has to be created before sending any data to SPOT; each job is identified with a ``JobID``, that has to be specified for all the subsequent messages related to the same session.This allows more printing "buffers" at the same time.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x01      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *jobID* (dword): Unique job identifier. 
		- *seqNumber* (word): Message sequence number; identifies single belonging to the same job.
		- *\_text* (binary string): Text to queue.  

	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x01')
	apdu.put_dword(jobID)
	apdu.put_word(seqNumber)
	apdu.put_word(len(_text))
	apdu.put_array(_text)
	connection.send_apdu(apdu)
	
def send_enqueue_metadata(connection, jobID, seqNumber, _text):
	"""
	Sends a **Enqueue data** command to SPOT.
	This function is used to print both text and graphics, using an XML format.
	Each printing "session" is identified as a "job", that has to be created before sending any data to SPOT; each job is identified with a ``JobID``, that has to be specified for all the subsequent messages related to the same session.
    This allows more printing "buffers" at the same time.
	
 	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x02      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *jobID* (dword): Unique job identifier. 
		- *seqNumber* (word): Message sequence number.
		- *\_text* (binary string): Text to queue.  

	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x02')
	apdu.put_dword(jobID)
	apdu.put_word(seqNumber)
	apdu.put_word(len(_text))
	apdu.put_array(_text)
	connection.send_apdu(apdu)

def send_job_status(connection, jobID):
	"""
	Sends a **Job status** command to SPOT. This command asks for job status.
	
 	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x03      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *jobID* (dword): Unique job identifier.
		 
	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x03')
	apdu.put_dword(jobID)
	connection.send_apdu(apdu)

def send_create_job(connection):
	"""
	Sends a **Create job** command to SPOT. This command Opens a new job for queuing data to print out.
	
 	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x04      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		
	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x04')
	connection.send_apdu(apdu)
	
def send_execute_job(connection, jobID):
	"""
	Sends a **Execute job** command to SPOT. This command starts print process of queued data.
	
 	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x05      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection. 
		 
	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x05')
	apdu.put_dword(jobID)
	connection.send_apdu(apdu)

def send_cancel_job(connection, jobID):
	"""
	Sends a **Cancel job** command to SPOT. This command aborts current job.
	
 	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x06      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection. 
		 
	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x06')
	apdu.put_dword(jobID)
	connection.send_apdu(apdu)


def recv_status(connection, time):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Information request** *(0x10,0x01)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *time* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	#apdu = connection.wait_apdu(0x80 + _apid, '\x10\x01', timeout)
	ack_params = {}
	#apdu.put_byte(device)
	#connection.send_apdu(apdu)
	return ack_params


def recv_barcode_enable(connection, timeout=0):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Barcode enable** *(0x40,0x02)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x02', timeout)
	ack_params = {'ack_code': apdu.pop_byte()}
	ack_params = {'bc_size': apdu.pop_byte()}
	ack_params = {'bc_data': apdu.pop_array(ack_params['bc_sizes'])}
	return ack_params

def recv_barcode_scan_event(connection, timeout = 0):
	"""
	Receives an APDU containing an **Bar scan event**  *(0x40,0x05)* unsolicited message. 
	This message provides by the Barcode after image is scanned.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x05', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'image_type': apdu.pop_word(),
			'status': apdu.pop_byte(),
			'image_data_length': apdu.pop_word(),
		}
		ack_params['image_data'] = apdu.pop_array(ack_params['image_data_length'])
	else:
		ack_params = {
			'ack_code': ack_code,
		}
	return ack_params

	
def recv_oti_enable_cmd(connection, timeout=0):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Contact less reader - Enable** *(0x50,0x01)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x50\x01', timeout)
	ack_code = apdu.pop_byte()

	ack_params = {
			'ack_code': ack_code
		}

	return ack_params

def recv_oti_track_data_cmd(connection, timeout=0):
	"""
	Receives an APDU containing a **Contact-less Card data** *(0x50,0x04)* message.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x50\x04', timeout)
	ack_code = apdu.pop_byte()

	ack_params = {
			'ack_code': ack_code
		}

	return ack_params


def recv_enqueue_data(connection, timeout = 0):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Enqueue data** *(0x20,0x01)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x01', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'job_id': apdu.pop_dword(),
			'seq_number': apdu.pop_word(),
		}
	else:
		ack_params = {
			'ack_code': ack_code,
		}
	return ack_params

def recv_enqueue_metadata(connection, timeout = 0):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Enqueue data**  *(0x20,0x02)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x02', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'job_id': apdu.pop_dword(),
			'seq_number': apdu.pop_word(),
		}
	else:
		ack_params = {
			'ack_code': ack_code,
		}
	return ack_params

def recv_job_status(connection, timeout = 0):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Job status**  *(0x20,0x03)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x03', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'job_id': apdu.pop_dword(),
			'seq_number': apdu.pop_word(),
			'status': apdu.pop_byte()
		}
	else:
		ack_params = {
			'ack_code': ack_code,
		}
	return ack_params

def recv_create_job(connection, timeout = 0):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Create job**  *(0x20,0x04)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x04', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'job_id': apdu.pop_dword(),
		}
	else:
		ack_params = {
			'ack_code': ack_code,
		}
	return ack_params

def recv_execute_job(connection, timeout = 0):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Execute job**  *(0x20,0x05)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x05', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'job_id': apdu.pop_dword(),
		}
	else:
		ack_params = {
			'ack_code': ack_code,
		}
	return ack_params

def recv_cancel_job(connection, timeout = 0):
	"""
	Receives an APDU containing the answer produced by the SPOT on the **Cancel job** *(0x20,0x06)* command.
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *timeout* (integer): Receive timeout.
		
	:Returns: A dictionary containing the "answer APDU" parameters.	

	"""
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x06', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'job_id': apdu.pop_dword(),
		}
	else:
		ack_params = {
			'ack_code': ack_code,
		}
	return ack_params
