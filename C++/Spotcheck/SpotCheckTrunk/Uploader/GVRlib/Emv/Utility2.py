"""
.. module:: GVRlib.Emv.Utility2
   :platform: Unix, Windows 
   :synopsis: EMV utility module (Python 2).

Contains utility functions for Setup package.

"""

import time
from binascii import unhexlify



def genDTS(struct_time = None):
	"""
	Converts a ``time struct`` in its binary representation.
	
	:Parameters:
		- *struct_time* : This is the time in number of seconds to be formatted.
		
	:Returns: ``None`` if command is successful.	
	
	:Raises: Exception.
		
	"""
	return unhexlify(time.strftime('%y%m%d%H%M%S', time.localtime() if struct_time is None else struct_time))

def assertSuccess(result):
	"""
	Checks whether a command is successful.
	
	:Parameters:
		- *result* (dictionary): Dictionary containing APDU parameters.
		
	:Returns: ``None`` if command is successful.	
	
	:Raises: Exception.
		
	"""
	if result['ack_code'] != 0:
		raise Exception("EMV command failed: " + str(result))
