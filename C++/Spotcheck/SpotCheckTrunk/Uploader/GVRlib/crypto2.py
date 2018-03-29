#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.crypto2 
   :platform: Unix, Windows
   :synopsis: Encryption and decryption algorithms (Python 2).

This module collects various utility used for operation of encryption and decryption or similar (pinblock, MAC generation, etc.)

"""

# System modules
import sys, os

# Local Modules
try:
	import means
except:
	from . import means

cryRSA = False
cryDES = False
cryAES = False
cryRND = False
cryCTR = False


thirdparty = None
if __package__ is not None:
	parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
	if parent_dir not in sys.path:
		sys.path.append(parent_dir)
	import importlib
	try:
		thirdparty = importlib.import_module('3rdparty')
	except ImportError:
		print('due')
		thirdparty = None
	else:
		try:
			attrlist = thirdparty.__all__
		except AttributeError:
			attrlist = dir(thirdparty)
		for attr in attrlist:
			globals()[attr] = getattr(thirdparty, attr)

try:
	from Crypto.PublicKey import RSA
	cryRSA = True
except:
	pass

try:
	if thirdparty is None:
		import rsa
	else:
		__import__('3rdparty.rsa')
except:
	pass

try:
	from Crypto.Cipher import AES
	iv_size = AES.block_size
	cryAES = True
except:
	if thirdparty is None:
		import aes
	else:
		__import__('3rdparty.aes')
	iv_size = aes.AES.keySize['SIZE_128']

try:
	import Crypto.Random
	cryRND = True
except:
	pass

try:
	from Crypto.Cipher import DES
	from Crypto.Cipher import DES3
	cryDES = True
	block_size = DES3.block_size
except:
	if thirdparty is None:
		from pyDes import *
	else:
		pydes = __import__('3rdparty.pyDes', fromlist=['*'])
		for member_name in dir(pydes):
			globals()[member_name] = getattr(pydes, member_name)
	block_size = 8

def XOR(data1, data2):
	"""
	Calculates XOR between two strings or two bytes array.

	:Parameters:
		- *data1* (string|bytes array): First operator.
		- *data2* (string|bytes array): Second operator.
		
	:Returns: XOR of parameters.
	"""
	try:
		from Crypto.Util.strxor import strxor
	except:
		def strxor(data1, data2):
			if isinstance(data1, str):
				l = [ chr(ord(a) ^ ord(b)) for a, b in zip(data1, data2) ]
				return ''.join(l)
			else:
				return bytes([ a ^ b for a, b in zip(data1, data2) ])
	return strxor(data1, data2)

if sys.version_info[0] < 3:
	hexZero = '\x00'
	pyVersionTwo = True
else:
	hexZero = b'\x00'
	pyVersionTwo = False

try:
	from Crypto.Util.number import long_to_bytes as longToBytes, bytes_to_long as bytesToLong
except:
	if pyVersionTwo:
		def bytesToLong(array):
			import binascii
			return long(binascii.hexlify(array), 16)
		def longToBytes(number, size):
			return str(bytearray.fromhex(hex(number)[2:-1]))
	else:
		def bytesToLong(array):
			return int.from_bytes(array, byteorder = 'big')
		def longToBytes(number, size):
			return number.to_bytes(size, byteorder = 'big')
def bytes_to_long(array):
	return bytesToLong(array);
def long_to_bytes(number, size):
	return longToBytes(number, size)

try:
	from Crypto.Util import Counter as CTR_Counter
	cryCTR = True
except:
	class CTR_Counter(object):
		#Usage example: counter=CTR_Counter(nonce=os.urandom(block_size/2), blocksize=block_size)
		def __init__(self, nonce = None, blocksize = block_size, start = 1):
			if nonce is None: nonce = long_to_bytes(start, blocksize)
			if len(nonce) < block_size:
				self.__c = start
				self.__bs = blocksize - len(nonce)
				self.__nonce = nonce
			else:
				self.__c = bytes_to_long(nonce)
				self.__bs = blocksize
				self.__nonce = None
			self.__start = self.__c
			self.__mask = (1 << (8 * self.__bs)) - 1
		def __call__(self):
			actual_counter = self.__c
			self.__c = (self.__c + 1) & self.__mask
			increased_counter = long_to_bytes(actual_counter, self.__bs)
			if self.__nonce is None: return increased_counter
			return self.__nonce + increased_counter
		def reset(self):
			self.__c = self.__start

padding_algo = means.enum('NOPADDING', 'SKIPPING', 'ZERO', 'PKCS7', 'ISO10126', 'ANSI_X923', 'ISO_IEC7816_4')


TEST_MTK = '0123456701234567'
RSA_TEST_KEY = """-----BEGIN RSA PRIVATE KEY-----
MIIEdwIBAAKB+QC89IKq48PVR4OO0t4YBHfaG+Bj9sBe/9WoIGviGnqrinNVl0BB
HR7XU/Fwznd3nWvVwp1pNOBGpnO5cx2m8f93uU475SY79kKQ9ni59HlHbvqZXpOU
loOalENhJkOsXZKfJ2TMiCEHeiglKdk4h1y2VKphA67ekb/BdBX7v2YRxldInC1Q
uKuvSnoj8rnDxUO4soiO9wxbP+bOgazpz7MPv7n0D/seHiHRRLwH7dnDbOYcVdM5
NFEY3EVbyK8JoDiRSJM1meVypHb2AVl5QnBFeG1nHwXtQVlcWjYlXm6ebNmjWJbe
cCr77t6ZjAYV5azf7sHZhQtbPQIBAwKB+H34VxyX1+OFAl83PrqtpTwSlZf51ZSq
jnAVnUFm/HJcTOO6KtYTaeTioPXe+k++R+PXE5t4lYRu99D3aRn2qk/Q3tKYxCf5
gbX5pdFNpi+fUbuUYmMPAmcNgkDELR2TtxTE7d2wFgT8GsNxO3sE6HmNxutXyem2
f9ZNY/wERjogUpErUA8q4n3wQTrZA39ECCREMztHoA2w68vIu/l6ZxE/jTp0MxMx
gx8kD/l3amnkHsxyIshGbQWbcVxND+g274TlArns0kgk4052FpbMCOObUcwPtWCS
xdiHiio4UKE5PJOU2mi043NOuWinBTyUlsTyoOCZzXhrAn0A7CTC07FiJLSKf8ha
oRYz5NfwbaFmW+72CsGw4CYakb6StcvjFHFLF85NG9eftFffl5zOS1bFwo45LoYk
2rtjvCh2UbUdGPyD5fS8U4S9IykcPkNOpBAY4dl3aEh2CC+yc2QU1INwd1OSAgMB
NEq+QEAWnRkRjgu9ceTmlQJ9AMzX98IqDLaAr3iPl8ECF8hclHGWG/BMwADaldZf
AkLUYOJMksv00UngNDgFgtpMMiMdVkthI+gQ05wna5hf7ZjutvwcLNgSmtC1Shmd
Qy0OetIfoWtinCWcZPzoIYa3M5aRl8J7VYH7r17SzcR9vlE+JGmxpvTEyyxyQAkC
fQCdbdc3y5bDIwb/2ucWDs1DOqBJFkQ9SflcgSCVbrxhKbcj3UINoNy6iYi9Omp4
OpUPvd7c5IPXCXt0WW3nJ5fSxaQ2eL4QqFfuoyg3rdNsxhLULN8YCrtBO6TwME6w
H8xM7WM4V6BPjQwBV1Yi3H7VgA8TZgu0B9OhQ0RjAn0AiI/6gXFdzwB0+wplK1a6
hZMNoQ69St3VVecOjuoBgeLrQYhh3U3g2+rNeq5XPDLMF2jkMkDCmrXiaBpHuuqe
ZfR5/Wgd5WG8iyOGu74syLRR4WprnOxoGRLt/fAWWc93ubZlLFI5AVJ06eHegv5+
4NQYRnZvTdiHcvbVWwJ8V42dYpXJmbW710UHdahK+t8JgoO53ajO4sXg0Yds85z9
/4eWwstEOa0/fYWT1V3IaSbfajB9IHGQqhRNgYbU5VxRJsdj6Cqe5ww9KWQ1xoQq
8B7UfuwxY8yexZ1xh2NF2gYpeywScAEnXobi3gos9HDIQhvxLfNRQ4jrew==
-----END RSA PRIVATE KEY-----
"""


'''
################################################################MAC ALGORITHMS########################################################
Upper case function name => Generic function
'''

def mac_x919(data, key, size = 4):	#Based on MAC 3 algorithm
	"""
	Perform a *MAC X919* on a specific data with a specific key (based on *MAC 3 algorithm*).

	:Parameters:
		- *data* (bytes array): Data to be MACed.
		- *key* (bytes array): Key value.
		- *size* (integer): MAC length.
		
	:Returns: The encrypted data.	 
	"""
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	while len(data) % 8 != 0: data += valueZero
	if cryDES:
		des_key1 = DES.new(key[:8], DES.MODE_CBC, IV = block_size * valueZero)
		des_key2 = DES.new(key[:8], DES.MODE_ECB)
	else:
		des_key1 = des(key[:8], CBC, IV = block_size * valueZero)
		des_key2 = des(key[:8], ECB)
	buf = des_key1.encrypt(data)
	buf = buf[-8:]
	buf = des_key2.decrypt(buf)
	if cryDES:
		des3_key = DES3.new(key, DES3.MODE_ECB)
	else:
		des3_key = triple_des(key, ECB)
	buf = des3_key.encrypt(buf)
	return buf[0:size]

def mac_x919_des(data, key, size = 4):
	"""
	Perform a *MAC X919* on a specific data with a specific key (based on *DES algorithm*).

	:Parameters:
		- *data* (bytes array): Data to be MACed.
		- *key* (bytes array): Key value.
		- *size* (integer): MAC length.
		
	:Returns: The encrypted data.	 
	
	"""
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	while len(data) % 8 != 0: data += valueZero
	if cryDES:
		des_key = DES.new(key, DES.MODE_CBC, IV = block_size * valueZero)
	else:
		des_key = des(key, CBC, block_size * valueZero)
	buf = des_key.encrypt(data)
	return buf[-8:size-8 if size < 8 else None]

def MAB(data, key, size = 8):
	"""
	Perform a *MAB algorithm*, based on *MAC1*.
		
	:Parameters:
		- *data* (bytes array): Data to be MACed.
		- *key* (bytes array): Key value.
		- *size* (integer): MAC length.
		
	:Returns: The encrypted data.
	
	"""
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	oddment = len(data) % block_size
	if oddment != 0:
		oddment = block_size - oddment
		data += valueZero * oddment
	if len(key) in (16,24):
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CBC, valueZero * block_size)
		else:
			encryptor = triple_des(key, CBC, valueZero * block_size)
	else:
		if cryDES:
			encryptor = DES.new(key, DES.MODE_CBC, valueZero * block_size)
		else:
			encryptor = des(key, CBC, valueZero * block_size)
	mab = encryptor.encrypt(data)[-block_size:]
	return mab[:min(size, block_size)]

def mac1(data, tdes_key, size = 4):
	"""
	*MAC method 1* with Padding method 1 (AS 2805.4.1-2001).

	:Parameters:
		- *data* (bytes array): Data to be MACed.
		- *tdes\_key* (bytes array): Key value.
		- *size* (integer): MAC length.
		
	:Returns: The encrypted data.	
	
	.. note::
	TODO: move this function to specific AS2805/Utilit.py lib?
		
	"""
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	oddment = len(data) % block_size
	if oddment != 0:
		oddment = block_size - oddment
		data += valueZero * oddment
	if cryDES:
		encryptor = DES3.new(tdes_key, DES3.MODE_CBC, valueZero * block_size)
	else:
		encryptor = triple_des(tdes_key, CBC, valueZero * block_size)
	mab = encryptor.encrypt(data)[-block_size:]
	return mab[:min(size, block_size)]

def mac2(data, tdes_key, size = 4):
	"""
	*MAC method 2* with Padding method 1 (AS 2805.4.1-2001).
	
	:Parameters:
		- *data* (bytes array): Data to be MACed.
		- *tdes\_key* (bytes array): Key value.
		- *size* (integer): MAC length.
		
	:Returns: The encrypted data.	
	
	.. note::
	TODO: move this function to specific AS2805/Utilit.py lib?
		
	"""
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	oddment = len(data) % block_size
	if oddment != 0:
		oddment = block_size - oddment
		data += valueZero * oddment
	lkey = tdes_key[:block_size]
	rkey = tdes_key[-block_size:]
	if cryDES:
		encryptor = DES.new(lkey, DES.MODE_CBC, valueZero * block_size)
		r_decryptor = DES.new(rkey)
		l_encryptor = DES.new(lkey)
	else:
		encryptor = des(lkey, CBC, valueZero * block_size)
		r_decryptor = des(rkey, ECB)
		l_encryptor = des(lkey, ECB)
	encrypted_data = encryptor.encrypt(data)
	pre_mab = r_decryptor.decrypt(encrypted_data[-block_size:])
	mab = l_encryptor.encrypt(pre_mab)
	return mab[:min(size, block_size)]

def MAC1(data, key, size = 4):
	"""
	Generic MAC1 algorithm (padding method 1, final iteration 1, output transformation 1).

	:Parameters:
		- *data* (bytes array): Data to be MACed.
		- *key* (bytes array): Key value.
		- *size* (integer): MAC length.
		
	:Returns: The encrypted data.	
	
	"""
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	oddment = len(data) % block_size
	if oddment != 0:
		oddment = block_size - oddment
		data += valueZero * oddment
	if len(key) == 8:
		if cryDES:
			encryptor = DES.new(key, DES.MODE_CBC, valueZero * block_size)
		else:
			encryptor = des(key, CBC, valueZero * block_size)
	elif len(key) in (16,24):
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CBC, valueZero * block_size)
		else:
			encryptor = triple_des(key, CBC, valueZero * block_size)
	else:
		raise Exception('Bad length for MAC1 key: ' + str(len(key)))
	mac = encryptor.encrypt(data)[-block_size:]
	return mac[:min(size, block_size)]

def MAC2(data, key, size = 4):
	"""
	Generic MAC2 algorithm (padding method 1, final iteration 1, output transformation 2).
	
	:Parameters:
		- *data* (bytes array): Data to be MACed.
		- *key* (bytes array): Key value.
		- *size* (integer): MAC length.
		
	:Returns: The encrypted data.	
	
	"""
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	valueEffeZero = '\xF0' if isinstance(data, str) else b'\xF0'
	oddment = len(data) % block_size
	if oddment != 0:
		oddment = block_size - oddment
		data += valueZero * oddment
	if cryDES:
		encryptor = DES.new(key[:block_size], DES.MODE_CBC, valueZero * block_size)
	else:
		encryptor = des(key[:block_size], CBC, valueZero * block_size)
	encryptor = DES.new(key[:block_size], DES.MODE_CBC, valueZero * block_size)
	enc_data = encryptor.encrypt(data)
	if len(key) == 8:
		if cryDES:
			encryptor = DES.new(XOR(key, valueEffeZero * block_size), DES.MODE_CBC, valueZero * block_size)
		else:
			encryptor = des(XOR(key, valueEffeZero * block_size), CBC, valueZero * block_size)
	elif len(key) in (16,24):
		if cryDES:
			encryptor = DES.new(key[block_size:DES.block_size*2], DES.MODE_CBC, valueZero * block_size)
		else:
			encryptor = des(key[block_size:DES.block_size*2], CBC, valueZero * block_size)
	else:
		raise Exception('Bad length for MAC2 key: ' + str(len(key)))
	mac = encryptor.encrypt(enc_data[-block_size:])
	return mac[:min(size, block_size)]

def MAC3(data, key, size = 4):
	#TODO: develop general algorithm
	return mac_x919(data, key, size)

def MAC4(data, key, size = 4):
	#TODO: develop correct algorithm
	return MAC2(data, key, size)

def MAC5(data, key, size = 4):
	"""
	Generic MAC5 algorithm (padding method 1, final iteration 3, output transformation 1).
			
	:Parameters:
		- *data* (bytes array): Data to be MACed.
		- *key* (bytes array): Key value.
		- *size* (integer): MAC length.
		
	:Returns: The encrypted data.	
	
	"""
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	oddment = len(data) % block_size
	if oddment != 0:
		oddment = block_size - oddment
		data += valueZero * oddment
	lkey = key[:block_size]
	rkey = key[-block_size:]
	macA = MAC1(data, lkey, size)
	macB = MAC1(data, rkey, size)
	return XOR(macA,macB)[:min(size, block_size)]

####################################################################################################################################

def rsa2img(dump):
	if cryRSA:
		priv_key = RSA.load_key_string(RSA_TEST_KEY)
	else:
		priv_key = rsa.key.PrivateKey.load_pkcs1(RSA_TEST_KEY)
	buf = dump[:-248]
	cert = dump[-248:]
	if cryRSA:
		decrypted = priv_key.public_decrypt(cert, RSA.no_padding)
	else:
		pub_key = rsa.key.PublicKey(priv_key.n, priv_key.e)
		decrypted = rsa.transform.int2bytes(rsa.core.decrypt_int(rsa.transform.bytes2int(cert), pub_key.e, pub_key.n), rsa.common.byte_size(pub_key.n))
	byDecrypted = decrypted[0:243]
	if not isinstance(dump, str) and isinstance(byDecrypted, str):
		byDecrypted = byDecrypted.encode("latin-1")
	if isinstance(dump, str) and not isinstance(byDecrypted, str):
		byDecrypted = byDecrypted.decode("latin-1")
	valueZero = '\x00' if isinstance(dump, str) else b'\x00'
	test_mtk = TEST_MTK if isinstance(dump, str) else TEST_MTK.encode("latin-1")
	buf += byDecrypted
	mac = mac_x919(byDecrypted, test_mtk)
	buf += mac + valueZero
	return buf

def dump_bytestream(data):
	res = '' if isinstance(data, str) else b''
	if type(data) == str:
		for i in data:
			try:
				res += i.encode('latin-1')
			except:
				res += '{:02x}'.format(ord(i))
	else:
		for i in data:
			try:
				res += i.decode('latin-1')
			except:
				try:
					res += '{:02x}'.format(i).encode('latin-1')
				except:
					res += i
	return res

#def pad(data, *, length = block_size, side = 'RIGHT', value = hexZero):
def pad(data, length = block_size, side = 'RIGHT', value = hexZero):
	"""
	Padding data function.
	
	:Parameters:
		- *data* (bytes array): Data which to apply the padding.
		- *length* (integer): Data lenght.
		- *side* (string): Side which to apply the padding.
		- *value* (byte): Padding content.
		
	"""
	pad_add = length - len(data) % length
	if pad_add == length: return data
	if isinstance(data, str) and not isinstance(value, str):
		value = value.decode('latin-1')
	if not isinstance(data, str) and isinstance(value, str):
		value = value.encode('latin-1')
	if side is 'RIGHT': return data + pad_add * value
	if side is 'LEFT': return pad_add * value + data
	return None

def padding_append(data, block_length = block_size, algo = padding_algo.NOPADDING):
	if algo == padding_algo.NOPADDING: return data
	elif algo == padding_algo.ZERO: return pad(data, length = block_length)
	elif algo == padding_algo.PKCS7:
		numpads = block_length - (len(data) % block_length)
		if isinstance(data, str): return data + numpads * chr(numpads)
		else: return data + (numpads * chr(numpads)).encode("latin-1")
	elif algo == padding_algo.ISO10126:
		numpads = block_length - (len(data) % block_length)
		padded_part = os.urandom(numpads - 1) + (chr(numpads) if isinstance(data, str) else bytes((numpads,)))
		return data + (padded_part if isinstance(data, str) else padded_part.encode("latin-1"))
	elif algo == padding_algo.ANSI_X923:
		numpads = block_length - (len(data) % block_length)
		padded_part = (numpads - 1) * chr(0x00) + chr(numpads)
		return data + (padded_part if isinstance(data, str) else padded_part.encode("latin-1"))
	elif algo == padding_algo.ISO_IEC7816_4:
		numpads = block_length - (len(data) % block_length) - 1
		padded_part = chr(0x80) + numpads * chr(0x00)
		return data + (padded_part if isinstance(data, str) else padded_part.encode("latin-1"))
	else: raise ValueError("Unknown padding algorithm: %s" % algo)

def padding_strip(data, block_length = block_size, expected_length = None, algo = padding_algo.NOPADDING):
	len_data = len(data)
	if algo == padding_algo.NOPADDING: return data
	elif algo == padding_algo.SKIPPING:
		if not data or len_data % block_length: raise ValueError("Byte array of length %d can't be %s-padded" % (len_data, padding_algo.list[algo]))
		if expected_length is None:raise ValueError("No expected length found, failed to provides %s" % (len_data, padding_algo.list[algo]))
		return data[:expected_length]
	elif algo == padding_algo.ZERO:
		if not data or len_data % block_length: raise ValueError("Byte array of length %d can't be %s-padded" % (len_data, padding_algo.list[algo]))
		for c in data[expected_length:]:
			i = ord(c) if isinstance(data, str) else int(c)
			if i != 0x00: raise ValueError("Padding has a wrong byte 0x%02X so can't be %s-padded" % (i, padding_algo.list[algo]))
		return data[:expected_length]
	elif algo == padding_algo.PKCS7 or algo == padding_algo.ISO10126 or algo == padding_algo.ANSI_X923:
		if not data or len_data < block_length or len_data % block_length: raise ValueError("Byte array of length %d can't be %s-padded" % (len_data, padding_algo.list[algo]))
		try: numpads = ord(data[-1])
		except: numpads = data[-1]
		if 0 < numpads <= block_length: return data[:-numpads]
		raise ValueError("Byte array ending with 0x%02X can't be %s-padded" % (numpads, padding_algo.list[algo]))
	elif algo == padding_algo.ISO_IEC7816_4:
		if not data or len_data < block_length or len_data % block_length: raise ValueError("Byte array of length %d can't be ISO/IEC7816-4-padded" % len_data)
		idx = len_data - 1
		if isinstance(data, str):
			while idx > 0 and ord(data[idx]) == 0x00: idx -= 1
			if ord(data[idx]) != 0x80: len_data = -1
		else:
			while idx > 0 and int(data[idx]) == 0x00: idx -= 1
			if int(data[idx]) != 0x80: len_data = -1
		if len_data < 0: raise ValueError("Byte array padded with %r can't be ISO/IEC7816-4-padded" % data[idx:])
		return data[:idx]
	else: raise ValueError("Unknown padding algorithm: %s" % padding_algo.list[algo])


def generate_key(key_type, key_length = None):
	"""
	Generates a specific key.

	:Parameters:
		- *key\_type* (string): Key type:
			- RSA,
			- DES,
			- DES3.
		- *key_length* (bytes array): Key value.
		
	:Returns: The generated key.	
	
	"""
	key = None
	if key_type == "DES":
		key = os.urandom(block_size)
	elif key_type == "DES3":
		if (key_length == None): key = os.urandom(2 * block_size)
		else: key = os.urandom(key_length)
	elif key_type == "AES":
		if key_length not in (16, 24, 32):
			raise ValueError('Invalid keysize, %s. Should be one of (16, 24, 32).' % key_length)
		key = os.urandom(key_length)
	elif key_type == "RSA":
		if cryRSA:
			if key_length < 1024 or (key_length & 0xff) != 0:
				def genRSApair(length):
					'''
					Generate RSA private and public key
					length : RSA key size
					'''
# 					try:
# 						import rsa as RSA
# 					except:
# 						from . import rsa as RSA
					(pk, sk) = rsa.newkeys(length)
					if pyVersionTwo:
						pkObj = RSA.construct((long(pk.n), long(pk.e)))
						skObj = RSA.construct((long(sk.n), long(sk.e), long(sk.d), long(sk.q), long(sk.p)))
					else:
						pkObj = RSA.construct((int(pk.n), int(pk.e)))
						skObj = RSA.construct((int(sk.n), int(sk.e), int(sk.d), int(sk.q), int(sk.p)))
					return (pkObj, skObj)
				key = genRSApair(key_length)
			else: key = RSA.generate(key_length)
		else: key = rsa.newkeys(key_length)
	return key


def decrypt(algo, encrypted_text, key, iv = None, expected_length = None, pad_algo = padding_algo.NOPADDING, counter = None):
	"""
	Utility  function to decrypt encrypted data.

	:Parameters:
		- *algo* (string):
			- DES3\_CBC,
			- DES3\_ECB,
			- DES3\_OFB,
			- AES\_CBC.
		- *encrypted\_text* (bytes array): Text to be decrypted.
		- *key* (bytes array): Key value.
		- *iv* (bytes array): initial vector.
		
	:Returns: The plain text.
			
	"""
	encryptor = None
	plain_text = None
	if iv == None and not algo.endswith('_ECB') and not algo.endswith('_CTR'):
		valueZero = '\x00' if isinstance(encrypted_text, str) else b'\x00'
		iv = (iv_size if algo.startswith('AES') else block_size) * valueZero
	elif callable(iv):
		iv = iv(iv_size if algo.startswith('AES') else block_size)
	if algo == "DES_ECB":
		if cryDES:
			encryptor = DES.new(key, DES.MODE_ECB)
		else:
			encryptor = des(key, ECB)
		plain_text = encryptor.decrypt(encrypted_text)
	elif algo == "DES_CBC":
		if cryDES:
			encryptor = DES.new(key, DES.MODE_CBC, iv)
		else:
			encryptor = des(key, CBC, iv)
		plain_text = encryptor.decrypt(encrypted_text)
	elif algo == "DES_OFB":
		# Pad the encrypted_text
		length = len(encrypted_text)
		encrypted_text = pad(encrypted_text)
		if cryDES:
			encryptor = DES.new(key, DES.MODE_OFB, iv)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = des(key, CBC, iv)
			valueZero = '\x00' if isinstance(encrypted_text, str) else b'\x00'
			encrypted_zeros = encryptor.encrypt(valueZero * len(encrypted_text))
			plain_text = XOR(encrypted_zeros, encrypted_text)
		if expected_length is None: expected_length = length
		pad_algo = padding_algo.SKIPPING
	elif algo == "DES_CFB":
		# Pad the encrypted_text
		length = len(encrypted_text)
		encrypted_text = pad(encrypted_text)
		if cryDES:
			encryptor = DES.new(key, DES.MODE_CFB, iv, segment_size = 8 * block_size)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = des(key, ECB)
			portion_toencrypt = iv
			plain_text = '' if isinstance(encrypted_text, str) else b''
			while len(encrypted_text) > 0:
				encrypted_portion = encryptor.encrypt(portion_toencrypt)
				portion_toencrypt = encrypted_text[:block_size]
				plain_text += XOR(encrypted_portion, portion_toencrypt)
				encrypted_text = encrypted_text[block_size:]
		if expected_length is None: expected_length = length
		pad_algo = padding_algo.SKIPPING
	elif algo == "DES_CTR":
		# Pad the encrypted_text
		length = len(encrypted_text)
		encrypted_text = pad(encrypted_text)
		if counter is None:
			if cryCTR: counter = CTR_Counter(block_size)
			else: counter = CTR_Counter(blocksize = block_size)
		if cryDES:
			encryptor = DES.new(key, DES.MODE_CTR, counter = counter)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = des(key, ECB)
			plain_text = '' if isinstance(encrypted_text, str) else b''
			while len(encrypted_text) > 0:
				encrypted_portion = encryptor.encrypt(counter())
				plain_text += XOR(encrypted_portion, encrypted_text[:block_size])
				encrypted_text = encrypted_text[block_size:]
		if expected_length is None: expected_length = length
		pad_algo = padding_algo.SKIPPING
	elif algo == "DES3_ECB":
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_ECB)
		else:
			encryptor = triple_des(key, ECB)
		plain_text = encryptor.decrypt(encrypted_text)
	elif algo == "DES3_CBC":
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CBC, iv)
		else:
			encryptor = triple_des(key, CBC, iv)
		plain_text = encryptor.decrypt(encrypted_text)
	elif algo == "DES3_OFB":
		# Pad the encrypted_text
		length = len(encrypted_text)
		encrypted_text = pad(encrypted_text)
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_OFB, iv)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = triple_des(key, CBC, iv)
			valueZero = '\x00' if isinstance(encrypted_text, str) else b'\x00'
			encrypted_zeros = encryptor.encrypt(valueZero * len(encrypted_text))
			plain_text = XOR(encrypted_zeros, encrypted_text)
		if expected_length is None: expected_length = length
		pad_algo = padding_algo.SKIPPING
	elif algo == "DES3_CFB":
		# Pad the encrypted_text
		length = len(encrypted_text)
		encrypted_text = pad(encrypted_text)
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CFB, iv, segment_size = 8 * block_size)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = triple_des(key, ECB)
			portion_toencrypt = iv
			plain_text = '' if isinstance(encrypted_text, str) else b''
			while len(encrypted_text) > 0:
				encrypted_portion = encryptor.encrypt(portion_toencrypt)
				portion_toencrypt = encrypted_text[:block_size]
				plain_text += XOR(encrypted_portion, portion_toencrypt)
				encrypted_text = encrypted_text[block_size:]
		if expected_length is None: expected_length = length
		pad_algo = padding_algo.SKIPPING
	elif algo == "DES3_CTR":
		# Pad the encrypted_text
		length = len(encrypted_text)
		encrypted_text = pad(encrypted_text)
		if counter is None:
			if cryCTR: counter = CTR_Counter(block_size)
			else: counter = CTR_Counter(blocksize = block_size)
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CTR, counter = counter)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = triple_des(key, ECB)
			plain_text = '' if isinstance(encrypted_text, str) else b''
			while len(encrypted_text) > 0:
				encrypted_portion = encryptor.encrypt(counter())
				plain_text += XOR(encrypted_portion, encrypted_text[:block_size])
				encrypted_text = encrypted_text[block_size:]
		if expected_length is None: expected_length = length
		pad_algo = padding_algo.SKIPPING
	elif algo == "AES_ECB":
		if cryAES:
			encryptor = AES.new(key, AES.MODE_ECB)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = aes.AESModeOfOperation()
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			plain_text = encryptor.decrypt(encrypted_text, expected_length, encryptor.modeOfOperation["ECB"], key, len(key), None)
	elif algo == "AES_CBC":
		if cryAES:
			encryptor = AES.new(key, AES.MODE_CBC, iv)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = aes.AESModeOfOperation()
			if isinstance(iv, str): iv = [ord(i) for i in iv]
			else: iv = [i for i in iv]
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			plain_text = encryptor.decrypt(encrypted_text, expected_length, encryptor.modeOfOperation["CBC"], key, len(key), iv)
	elif algo == "AES_OFB":
		# Pad the encrypted_text
		length = len(encrypted_text)
		encrypted_text = pad(encrypted_text)
		if cryAES:
			encryptor = AES.new(key, AES.MODE_OFB, iv)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = aes.AESModeOfOperation()
			if isinstance(iv, str): iv = [ord(i) for i in iv]
			else: iv = [i for i in iv]
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			plain_text = encryptor.decrypt(encrypted_text, expected_length, encryptor.modeOfOperation["OFB"], key, len(key), iv)
		if expected_length is None: expected_length = length
		pad_algo = padding_algo.SKIPPING
	elif algo == "AES_CFB":
		# Pad the encrypted_text
		length = len(encrypted_text)
		encrypted_text = pad(encrypted_text)
		if cryAES:
			encryptor = AES.new(key, AES.MODE_CFB, iv, segment_size = 8 * iv_size)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = aes.AESModeOfOperation()
			if isinstance(iv, str): iv = [ord(i) for i in iv]
			else: iv = [i for i in iv]
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			plain_text = encryptor.decrypt(encrypted_text, expected_length, encryptor.modeOfOperation["CFB"], key, len(key), iv)
		if expected_length is None: expected_length = length
		pad_algo = padding_algo.SKIPPING
	elif algo == "AES_CTR":
		# Pad the encrypted_text
		length = len(encrypted_text)
		encrypted_text = pad(encrypted_text)
		if counter is None:
			if cryCTR: counter = CTR_Counter(block_size)
			else: counter = CTR_Counter(blocksize = block_size)
		if cryAES:
			encryptor = AES.new(key, AES.MODE_CTR, counter = counter)
			plain_text = encryptor.decrypt(encrypted_text)
		else:
			encryptor = aes.AESModeOfOperation()
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			plain_text = encryptor.decrypt(encrypted_text, expected_length, encryptor.modeOfOperation["CTR"], key, len(key), counter)
		if expected_length is None: expected_length = length
		pad_algo = padding_algo.SKIPPING
	else:
		raise Exception("Algo unknown %s" % algo)
	# UnPad the plain_text
	plain_text = padding_strip(plain_text, block_length = iv_size if algo.startswith('AES') else block_size, expected_length = expected_length, algo = pad_algo)
	return plain_text

def encrypt(algo, plain_text, key, iv = None, pad_algo = padding_algo.NOPADDING, counter = None):
	"""
	Utility to encrypt a plain text.

	:Parameters:
		- *algo* (string): 
			- DES3\_CBC,
			- DES3\_ECB,
			- DES3\_OFB,
			- AES\_CBC.
		- *plain\_text* (bytes array): Text to be encrypted.
		- *key* (bytes array): Key value.
		- *iv* (bytes array): initial vector.
		
	:Returns: The encrypted data.
			
	"""
	encryptor = None
	encrypted_text = None
	length = len(plain_text)
	if iv == None and not algo.endswith('_ECB') and not algo.endswith('_CTR'):
		valueZero = '\x00' if isinstance(plain_text, str) else b'\x00'
		iv = (iv_size if algo.startswith('AES') else block_size) * valueZero
	elif callable(iv):
		iv = iv(iv_size if algo.startswith('AES') else block_size)
	# Pad the plain_text
	plain_text = padding_append(plain_text, block_length = iv_size if algo.startswith('AES') else block_size, algo = pad_algo if algo.endswith('_CBC') or algo.endswith('_ECB') else padding_algo.ZERO)
	if algo == "DES_ECB":
		if cryDES:
			encryptor = DES.new(key, DES.MODE_ECB)
		else:
			encryptor = des(key, ECB)
		encrypted_text = encryptor.encrypt(plain_text)
	elif algo == "DES_CBC":
		if cryDES:
			encryptor = DES.new(key, DES.MODE_CBC, iv)
		else:
			encryptor = des(key, CBC, iv)
		encrypted_text = encryptor.encrypt(plain_text)
	elif algo == "DES_OFB":
		if cryDES:
			encryptor = DES.new(key, DES.MODE_OFB, iv)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
			encryptor = des(key, CBC, iv)
			valueZero = '\x00' if isinstance(plain_text, str) else b'\x00'
			encrypted_zeros = encryptor.encrypt(valueZero * len(plain_text))
			encrypted_text = XOR(encrypted_zeros, plain_text)
		if len(encrypted_text) > length:
			encrypted_text = encrypted_text[:length]
	elif algo == "DES_CFB":
		if cryDES:
			encryptor = DES.new(key, DES.MODE_CFB, iv, segment_size = 8 * block_size)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
			encryptor = des(key, ECB)
			portion_toencrypt = iv
			encrypted_text = '' if isinstance(plain_text, str) else b''
			while len(plain_text) > 0:
				encrypted_portion = encryptor.encrypt(portion_toencrypt)
				portion_toencrypt = XOR(encrypted_portion, plain_text[:block_size])
				encrypted_text += portion_toencrypt
				plain_text = plain_text[block_size:]
		if len(encrypted_text) > length:
			encrypted_text = encrypted_text[:length]
	elif algo == "DES_CTR":
		if counter is None:
			if cryCTR: counter = CTR_Counter(block_size)
			else: counter = CTR_Counter(blocksize = block_size)
		if cryDES:
			encryptor = DES.new(key, DES.MODE_CTR, counter = counter)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
			encryptor = des(key, ECB)
			encrypted_text = '' if isinstance(plain_text, str) else b''
			while len(plain_text) > 0:
				encrypted_portion = encryptor.encrypt(counter())
				encrypted_text += XOR(encrypted_portion, plain_text[:block_size])
				plain_text = plain_text[block_size:]
		if len(encrypted_text) > length:
			encrypted_text = encrypted_text[:length]
	elif algo == "DES3_ECB":
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_ECB)
		else:
			encryptor = triple_des(key, ECB)
		encrypted_text = encryptor.encrypt(plain_text)
	elif algo == "DES3_CBC":
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CBC, iv)
		else:
			encryptor = triple_des(key, CBC, iv)
		encrypted_text = encryptor.encrypt(plain_text)
	elif algo == "DES3_OFB":
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_OFB, iv)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
			encryptor = triple_des(key, CBC, iv)
			valueZero = '\x00' if isinstance(plain_text, str) else b'\x00'
			encrypted_zeros = encryptor.encrypt(valueZero * len(plain_text))
			encrypted_text = XOR(encrypted_zeros, plain_text)
		if len(encrypted_text) > length:
			encrypted_text = encrypted_text[:length]
	elif algo == "DES3_CFB":
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CFB, iv, segment_size = 8 * block_size)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
			encryptor = triple_des(key, ECB)
			portion_toencrypt = iv
			encrypted_text = '' if isinstance(plain_text, str) else b''
			while len(plain_text) > 0:
				encrypted_portion = encryptor.encrypt(portion_toencrypt)
				portion_toencrypt = XOR(encrypted_portion, plain_text[:block_size])
				encrypted_text += portion_toencrypt
				plain_text = plain_text[block_size:]
		if len(encrypted_text) > length:
			encrypted_text = encrypted_text[:length]
	elif algo == "DES3_CTR":
		if counter is None:
			if cryCTR: counter = CTR_Counter(block_size)
			else: counter = CTR_Counter(blocksize = block_size)
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CTR, counter = counter)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
			encryptor = triple_des(key, ECB)
			encrypted_text = '' if isinstance(plain_text, str) else b''
			while len(plain_text) > 0:
				encrypted_portion = encryptor.encrypt(counter())
				encrypted_text += XOR(encrypted_portion, plain_text[:block_size])
				plain_text = plain_text[block_size:]
		if len(encrypted_text) > length:
			encrypted_text = encrypted_text[:length]
	elif algo == "AES_ECB":
		if cryAES:
			encryptor = AES.new(key, AES.MODE_ECB)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
			encryptor = aes.AESModeOfOperation()
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			_, _, encrypted_text = encryptor.encrypt(plain_text, encryptor.modeOfOperation["ECB"], key, len(key), None)
	elif algo == "AES_CBC":
		if cryAES:
			encryptor = AES.new(key, AES.MODE_CBC, iv)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
#			encrypted_text =  aes.encryptData(key, iv + plain_text)
			encryptor = aes.AESModeOfOperation()
			if isinstance(iv, str): iv = [ord(i) for i in iv]
			else: iv = [i for i in iv]
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			_, _, encrypted_text = encryptor.encrypt(plain_text, encryptor.modeOfOperation["CBC"], key, len(key), iv)
	elif algo == "AES_OFB":
		if cryAES:
			encryptor = AES.new(key, AES.MODE_OFB, iv)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
			encryptor = aes.AESModeOfOperation()
			if isinstance(iv, str): iv = [ord(i) for i in iv]
			else: iv = [i for i in iv]
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			encrypted_text = encryptor.encrypt(plain_text, encryptor.modeOfOperation["OFB"], key, len(key), iv)
		if len(encrypted_text) > length:
			encrypted_text = encrypted_text[:length]
	elif algo == "AES_CFB":
		if cryAES:
			encryptor = AES.new(key, AES.MODE_CFB, iv, segment_size = 8 * iv_size)
			plain_text = encryptor.encrypt(encrypted_text)
		else:
			encryptor = aes.AESModeOfOperation()
			if isinstance(iv, str): iv = [ord(i) for i in iv]
			else: iv = [i for i in iv]
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			encrypted_text = encryptor.encrypt(plain_text, encryptor.modeOfOperation["CFB"], key, len(key), iv)
		if len(encrypted_text) > length:
			encrypted_text = encrypted_text[:length]
	elif algo == "AES_CTR":
		if counter is None:
			if cryCTR: counter = CTR_Counter(block_size)
			else: counter = CTR_Counter(blocksize = block_size)
		if cryAES:
			encryptor = AES.new(key, AES.MODE_CTR, counter = counter)
			encrypted_text = encryptor.encrypt(plain_text)
		else:
			encryptor = aes.AESModeOfOperation()
			if isinstance(key, str): key = [ord(k) for k in key]
			else: key = [k for k in key]
			encrypted_text = encryptor.encrypt(plain_text, encryptor.modeOfOperation["CTR"], key, len(key), counter)
		if len(encrypted_text) > length:
			encrypted_text = encrypted_text[:length]
	else:
		raise Exception("Algo unknown %s" % algo)
	return encrypted_text


def rsa_decrypt(encrypted_text, key_rsa):
	"""
	Utility function to decrypt an encrypted text  with the *RSA algorithm*.

	:Parameters:
		- *encrypted\_text* (bytes array): Text to decrypt.
		- *rsa\_key* (bytes array): Key value.
		
		
	:Returns: The decrypted text.
			
	"""	
	if isinstance(key_rsa, tuple):
		_, key_rsa = key_rsa
	try:
		decrypted_text = rsa.decrypt(encrypted_text, key_rsa, padding = False)
	except:
		try:
			decrypted_text = key_rsa.decrypt(encrypted_text)
		except:
			raise Exception("The key isn't a RSA Private key")
	return decrypted_text

def rsa_encrypt(plain_text, key_rsa):
	"""
	Utility function to encrypt a plain text with the *RSA algorithm*.

	:Parameters:
		- *plain\_text* (bytes array): Text to encrypt.
		- *rsa_key* (bytes array): Key value.
		
		
	:Returns: The encrypted text.
			
	"""		
	if isinstance(key_rsa, tuple):
		key_rsa, _ = key_rsa
	try:
		encrypted_text = rsa.encrypt(plain_text, key_rsa, padding = False)
	except:
		try:
			encrypted_text = key_rsa.encrypt(plain_text, 0)
		except:
			raise Exception("The key isn't a RSA Public key")
	return encrypted_text

def owf(data, key):
	"""
	*One Way Function* for AS2805 project. 

	.. note::
		*owf* for 8 bytes data long is different than OWF for data longer than 8 bytes.

	:Parameters:
		- *data* (string|bytes array): Data to convert.
		- *key* (bytes array): Key value to perform *owf*.
		
	.. note::	
	TODO: move this function to specific AS2805/Utilit.py lib?
		
	"""
	# Padding of the data that are not multiple of 8 bytes
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	hexEighty = '\x80' if isinstance(data, str) else b'\x80'
	toPad = block_size - (len(data) % block_size)
	if toPad > 0:
		data += hexEighty + (toPad - 1) * valueZero
	# OWF for data long up to 8 bytes is different than OWF for data longer than 8 bytes
	if len(data) == block_size:
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_ECB)
		else:
			encryptor = triple_des(key, ECB)
		result	 = encryptor.decrypt(data)
		output_owf = XOR(data, result)
	else:
		# Run DE3S CBC on data with IV equals to zero
		iv = block_size * valueZero
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CBC, iv)
		else:
			encryptor = triple_des(key, CBC, iv)
		digest = encryptor.encrypt(data)
		if cryDES:
			encryptor = DES3.new(key, DES3.MODE_CBC, digest[-8:])
		else:
			encryptor = triple_des(key, CBC, digest[-8:])
		result = encryptor.encrypt(data)
		output_owf = XOR(data, result)
	if toPad > 0: # Discard padded section of data
		output_owf = output_owf[-toPad:]
	return output_owf

def getPKMAN(dirPath = None, fileName = 'SBPKMAN.DAT', dataType = None):
	"""
	Get *PKMAN* from a *.dat* file.

	:Parameters:
		- *dirPath* (string): File complete path.
		- *name* (string): File name.
		- *dataType* (string): file type (*cryRSA* or *pyRSA*).
		
	:Returns: *PKMAN*.	
	"""
	
	filepath = os.path.join(config.dirdata if dirPath is None else dirPath, fileName)
	if os.path.exists(filepath) and os.path.getsize(filepath) == 521: # 521 : PKN=7 + MID=2 + MOD=256 + EXP=256
		if dataType is None:
			dataType = 'cryRSA' if cryRSA else 'pyRSA'
		with open(filepath, 'rb') as file:
			try:
				dataRead = file.read()
#				pkn = int(dataRead[:7], 16)
#				mid = int(dataRead[7:9], 16)
				mod = int(dataRead[9:265], 16)
				exp = int(dataRead[-6:], 16)
#				print('pkMAN info: PKN={:d}, MID={:x}, MOD={:x}, EXP={:x}'.format(pkn, mid, mod, exp))
				if dataType == 'cryRSA':
					from Crypto.PublicKey import RSA
					data = RSA.construct((mod, exp))
				elif dataType == 'pyRSA':
					import rsa
					data = rsa.PublicKey(mod, exp)
				else:
					means.error._lay('Unknown type of stored data: ' + dataType)
					print(means.error)
					return None
				return data
			except:
				means.error._lay('Unexpected restoring error: ' + str(sys.exc_info()[1]))
				print(means.error)
				return None
	else:
		means.error._lay('Not Found the file: ' + filepath)
		print(means.error)
		return None

def import_RSAkey(path, asFile = True):
	"""
	Imports value for RSA key from a text file or a string.

	:Parameters:
		- *path* (string): File path.
		- *asFile* (boolean).
		
	:Returns: The RSA key. 	

	"""
	keyfile = None
	if asFile:
		with open(path, 'r') as f:
			keyfile = f.read()
	else:
		keyfile = path
	if keyfile is None: return None
	if cryRSA: return RSA.importKey(keyfile)
	else:
		key = None
		try:
			privKey = rsa.key.PrivateKey()
			key = privKey.load_pkcs1(keyfile, format = 'PEM')
		except:
			try:
				key = privKey.load_pkcs1(keyfile, format = 'DER')
			except:
				try:
					pubKey = rsa.key.PublicKey()
					key = pubKey.load_pkcs1(keyfile, format = 'PEM')
				except:
					try:
						key = pubKey.load_pkcs1(keyfile, format = 'DER')
					except:
						pass
		return key

def	ofb_helper(data, key, Stan = None):
	"""
	Utility function for *GenerateMac\&Encrypt* process (for AS2805 project).
	
	:Parameters:
		- *data* (bytes array): Data to encrypt.
		- *key* (bytes array): Key value. 		
	
	:Returns: The encrypted data. 	
	
	.. note ::
		Move this function to specific AS2805/Utilit.py lib?
		
	"""
	constant = '\x01\x23\x45\x67\x00\x00\x00\x00'
	if not isinstance(data, str):
		constant = constant.encode('latin-1')
	if Stan is None:
		SV = constant
	else: # Set Starting Value (SV) as the STAN right-justified, zero-filled and XORed with a constant
		SV = XOR(constant, pad(Stan, length = len(constant), side = 'LEFT'))
	# Padding the plain_text
	pad_num = 0 if len(data) % block_size == 0 else block_size - len(data) % block_size
	valueZero = '\x00' if isinstance(data, str) else b'\x00'
	buffer = (len(data) + pad_num) * valueZero
	result = encrypt("DES3_CBC", buffer, key, SV)
	if pad_num == 0: return result
	else: return result[:-pad_num]


def prova():
	return 'Okey!!!'
def prova2(a):
	return 'Okey!!!' + ' - ' + a
def prova3(a, b):
	return 'Okey!!!' + ' - ' + a + ' - ' + b

if __name__ == "__main__" and __package__ is None:
	# boilerplate to allow running as script directly
	parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
	sys.path.insert(1, parent_dir)
	import importlib
	thirdparty = importlib.import_module('3rdparty')
	try:
		attrlist = thirdparty.__all__
	except AttributeError:
		attrlist = dir(thirdparty)
	for attr in attrlist:
		globals()[attr] = getattr(thirdparty, attr)
	if not cryAES:
		__import__('3rdparty.aes')
		iv_size = aes.AES.keySize['SIZE_128']
	if not cryDES:
		pydes = __import__('3rdparty.pyDes', fromlist=['*'])
		for member_name in dir(pydes):
			globals()[member_name] = getattr(pydes, member_name)
	if not cryRSA:
		__import__('3rdparty.rsa')

	def str2byt(data):
		if type(data) == str:
			return data.encode("latin-1")
		return data

	print("===== Testing PyCrypto =====\n")

	sss=b'RobertoCeleste\x07\x17\x03\x37\x57\x77\x80\x00\x00\x00'
#	print(padding_strip(sss, block_length = 8, expected_length = 16, algo = padding_algo.ZERO))
#	print(padding_strip(sss, block_length = 8, expected_length = 15, algo = padding_algo.ZERO))
#	print(padding_strip(sss, block_length = 8, expected_length = 13, algo = padding_algo.ZERO))
	print(padding_strip(sss, block_length = 8, algo = padding_algo.ISO_IEC7816_4))
#	print(padding_strip(sss, block_length = 8, algo = padding_algo.PKCS7))

	def random_IV(size = block_size):
		print(size)
		iv = os.urandom(size)
		print(iv)
		return iv

	# Encrypt data with DES3 CBC
	print("\n\nDES3 CBC")

	plain_text	 = str2byt("Some more random text")
	encrypted_text = encrypt("DES3_CBC", plain_text, 	 str2byt("0123456789012345"), iv = random_IV, pad_algo = padding_algo.PKCS7)
	decrypted_text = decrypt("DES3_CBC", encrypted_text, str2byt("0123456789012345"), iv = random_IV, pad_algo = padding_algo.PKCS7)

	print("\nPlain text: ", dump_bytestream(plain_text))
	print("\nEncrypted:  ", dump_bytestream(encrypted_text))
	print("\nDecrypted:  ", dump_bytestream(decrypted_text))

	# Encrypt data with DES3 OFB
	print("\n\nDES3 OFB",)

	iv			 = os.urandom(block_size)
	plain_text	 = str2byt("Some more random text")
	encrypted_text = encrypt("DES3_OFB", plain_text, 	 str2byt("0123456789012345"), iv)
	decrypted_text = decrypt("DES3_OFB", encrypted_text, str2byt("0123456789012345"), iv)

	print("\nPlain text: ", dump_bytestream(plain_text))
	print("\nEncrypted:  ", dump_bytestream(encrypted_text))
	print("\nDecrypted:  ", dump_bytestream(decrypted_text))

	# Encrypt data with AES CBC (THIS MAY WORK AS STR IN PYTHON3)
	print("\n\nAES CBC",)

	plain_text	 = str2byt("Some text to be encrypted with AES CBC")
	encrypted_text = encrypt("AES_CBC", plain_text, 	str2byt("0123456789012345"), pad_algo = padding_algo.ZERO)
	decrypted_text = decrypt("AES_CBC", encrypted_text, str2byt("0123456789012345"), expected_length = len(plain_text), pad_algo = padding_algo.SKIPPING)

	print("\nPlain text: ", dump_bytestream(plain_text))
	print("\nEncrypted:  ", dump_bytestream(encrypted_text))
	print("\nDecrypted:  ", dump_bytestream(decrypted_text))

	print("\n\n")

	print(aes.strip_PKCS7_padding(str2byt("12345678901234567890abcdef77\x04\x04\x04\x04")))

	print(XOR(str2byt('test123'), str2byt('321test')))
	# result: b'GWB\x00TAG'

	from struct import pack, unpack
	OPfields = {  1:123, 2:1234, 5:1834, 7:10942, 17:55555 }
	op_fields = b''.join([pack('!BH', field, value) for (field, value) in OPfields.items()])
	print(op_fields)

	print("\n\nDES3 CBC")

	plain_text = b"Some more random text"
	des3Key = b'0123456789012345'
	mac_res = mac1(plain_text, des3Key)
	print("The MAC1 is", mac_res)
	mac_res = mac2(plain_text, des3Key)
	print("The MAC2 is", mac_res)

	print("\n\nSTORE DES3")

	import DataStore
	ds = DataStore.DataStore(path = "..")

	print(ds.get('fun1'))
	print(ds.get('fun2'))
	print(ds.get('fun3'))

	strby = '\x01\x07\x17'
	ds.put('STRnotPRINTABLE', strby)
	diff_strby = ds.get('STRnotPRINTABLE')
	if strby == diff_strby:
		print('strby Matched')
	else:
		print('strby Dismatched')

	val1 = 17
	print(val1.__class__.__name__, str(val1))
	ds.put("Intero", val1)
	val2 = 17.77
	print(val2.__class__.__name__, str(val2))
	ds.put("Frazionale", val2)
	val3 = True
	print(val3.__class__.__name__, str(val3))
	ds.put("Booleano", val3)
	val4 = 4123456789
	print(val4.__class__.__name__, str(val4))
	ds.put("InteroLungo", val4)
	val5 = ds.get("InteroLungo")
	print("get long", val5.__class__.__name__, str(val5))
	val6 = 17
	print(val6.__class__.__name__, str(val6))
	ds.put("InteroVar", val6)
	val6 = ds.get("InteroVar")
	print("get Var", val6.__class__.__name__, str(val6))
	val6 += val4
	print(val6.__class__.__name__, str(val6))
	ds.put("InteroVar", val6)
	val6 = ds.get("InteroVar")
	print("get Var", val6.__class__.__name__, str(val6))
	val6 /= 3
	print(val6.__class__.__name__, str(val6))
	ds.put("InteroVar", val6)
	val6 = ds.get("InteroVar")
	print("get Var", val6.__class__.__name__, str(val6))

	val7 = ds.get("Frazionale")
	print("get float", val7.__class__.__name__, str(val7))

	val8 = [ 1, 7, 17.7, True, 'Roberto', '\x12\x34\x56', b'W ...' ]
	ds.put("ListaMista", val8)
	val8 = ds.get("ListaMista")
	print("get ListaMista", val8.__class__.__name__, val8)

	tls = ds.get("TLS")
	print("get Enabled", tls.__class__.__name__, str(tls))

	ss = ds.get("PKMAN")
	print(ss)

	dataMissing = ds.get("ThisKeyNotExists")
	if not dataMissing:
		print(means.error)

	AIIC = b'\x00\x01\x23\x45\x67\x89'
	key_des3 = generate_key('DES3')
	print("result des:", ds.put("DES3", key_des3, AIIC))
	dataNew = ds.get("DES3", AIIC)
	if key_des3 == dataNew:
		print('DES3 Matched')
	else:
		print('DES3 Dismatched')

	print("\n\nSTORE RSA")
	key_rsa = generate_key('RSA', 1024)
	if isinstance(key_rsa, tuple):
		pubkey, privkey = key_rsa
		print("result p_rsa:", ds.put("P_RSA1024", pubkey))
		print("result s_rsa:", ds.put("S_RSA1024", privkey))
		dataNew = ds.get("P_RSA1024")
		if pubkey == dataNew:
			print('P_RSA1024 Matched')
		else:
			print('P_RSA1024 Dismatched')
		dataNew = ds.get("S_RSA1024")
		if privkey == dataNew:
			print('S_RSA1024 Matched')
		else:
			print('S_RSA1024 Dismatched')
	else:
		print("result rsa:", ds.put("RSA1024", key_rsa))
		dataNew = ds.get("RSA1024")
		if key_rsa == dataNew:
			print('RSA1024 Matched')
		else:
			print('RSA1024 Dismatched')

	key_rsa = generate_key('RSA', 64)
	if isinstance(key_rsa, tuple):
		pubkey, privkey = key_rsa
		print("result p_rsa:", ds.put("P_RSA64", pubkey))
		print("result s_rsa:", ds.put("S_RSA64", privkey))
		dataNew = ds.get("P_RSA64")
		if pubkey == dataNew:
			print('P_RSA64 Matched')
		else:
			print('P_RSA64 Dismatched')
		dataNew = ds.get("S_RSA64")
		if privkey == dataNew:
			print('S_RSA64 Matched')
		else:
			print('S_RSA64 Dismatched')
	else:
		print("result rsa:", ds.put("RSA64", key_rsa))
		dataNew = ds.get("RSA64")
		if key_rsa == dataNew:
			print('RSA64 Matched')
		else:
			print('RSA64 Dismatched')

	iv			 = os.urandom(block_size)
	plain_text	 = str2byt("Some more random text")
	encrypted_text = encrypt("DES3_OFB", plain_text, 	 str2byt("0123456789012345"), iv)
	decrypted_text = decrypt("DES3_OFB", encrypted_text, str2byt("0123456789012345"), iv)
	print ("\nPlain text:", plain_text, "\nEncrypted: ", encrypted_text, "\nDecrypted: ", decrypted_text)

	print("")
	encrypted_text = ofb_helper(plain_text, str2byt("0123456789012345"))
	print ("OFB Encrypted [{:d}]:".format(len(encrypted_text)), encrypted_text);
	encrypted_text = ofb_helper(plain_text, str2byt("0123456789012345"), str2byt('012'))
	print ("OFB Encrypted [{:d}]:".format(len(encrypted_text)), encrypted_text);
	encrypted_text = ofb_helper(str2byt('Roberto Celeste!'), str2byt("0123456789012345"))
	print ("OFB Encrypted [{:d}]:".format(len(encrypted_text)), encrypted_text);
	encrypted_text = ofb_helper(str2byt('Roberto Celeste!'), str2byt("0123456789012345"), str2byt('012'))
	print ("OFB Encrypted [{:d}]:".format(len(encrypted_text)), encrypted_text);

	# Encrypt data with DES ECB
	print("\n\nDES3 ECB")

	plain_text	 = str2byt('\x00\x01\x02\x03\x04\x05\x06\x07')
	encrypted_text = encrypt("DES3_ECB", plain_text, 	 str2byt("0123456789012345"))
	decrypted_text = decrypt("DES3_ECB", encrypted_text, str2byt("0123456789012345"))

	print("\nPlain text: ", dump_bytestream(plain_text))
	print("\nEncrypted:  ", dump_bytestream(encrypted_text))
	print("\nDecrypted:  ", dump_bytestream(decrypted_text))

	# Encrypt data with DES3 CBC
	print("\n\nDES3 CBC")

	iv			 = os.urandom(block_size)
	plain_text	 = str2byt("Some more random text")
	encrypted_text = encrypt("DES3_CBC", plain_text, 	 str2byt("0123456789012345"), iv, pad_algo = padding_algo.PKCS7)
	decrypted_text = decrypt("DES3_CBC", encrypted_text, str2byt("0123456789012345"), iv, pad_algo = padding_algo.PKCS7)
#def decrypt(algo, encrypted_text, key, iv = None, expected_length = None, pad_algo = padding_algo.NOPADDING):

	print("\nPlain text: ", dump_bytestream(plain_text))
	print("\nEncrypted:  ", dump_bytestream(encrypted_text))
	print("\nDecrypted:  ", dump_bytestream(decrypted_text))

	# Encrypt data with DES3 OFB
	print("\n\nDES3 OFB",)

	iv			 = os.urandom(block_size)
	plain_text	 = str2byt("Some more random text")
	encrypted_text = encrypt("DES3_OFB", plain_text, 	 str2byt("0123456789012345"), iv)
	decrypted_text = decrypt("DES3_OFB", encrypted_text, str2byt("0123456789012345"), iv)

	print("\nPlain text: ", dump_bytestream(plain_text))
	print("\nEncrypted:  ", dump_bytestream(encrypted_text))
	print("\nDecrypted:  ", dump_bytestream(decrypted_text))

	# Encrypt data with AES CBC (THIS MAY WORK AS STR IN PYTHON3)
	print("\n\nAES CBC",)

	iv			 = os.urandom(iv_size)
	plain_text	 = str2byt("Some text to be encrypted with AES CBC")
	encrypted_text = encrypt("AES_CBC", plain_text, 	str2byt("0123456789012345"), iv, pad_algo = padding_algo.ZERO)
	decrypted_text = decrypt("AES_CBC", encrypted_text, str2byt("0123456789012345"), iv, expected_length = len(plain_text), pad_algo = padding_algo.SKIPPING)

	print("\nPlain text: ", dump_bytestream(plain_text))
	print("\nEncrypted:  ", dump_bytestream(encrypted_text))
	print("\nDecrypted:  ", dump_bytestream(decrypted_text))

	# Encrypt data with AES ECB (THIS MAY WORK AS STR IN PYTHON3)
	print("\n\nAES ECB",)

	iv			 = os.urandom(iv_size)
	plain_text	 = str2byt("Some text to be encrypted with AES ECB")
	encrypted_text = encrypt("AES_ECB", plain_text, 	str2byt("0123456789012345"), iv, pad_algo = padding_algo.ISO_IEC7816_4)
	decrypted_text = decrypt("AES_ECB", encrypted_text, str2byt("0123456789012345"), iv, pad_algo = padding_algo.ISO_IEC7816_4)

	print("\nPlain text: ", dump_bytestream(plain_text))
	print("\nEncrypted:  ", dump_bytestream(encrypted_text))
	print("\nDecrypted:  ", dump_bytestream(decrypted_text))

	# Generate RSA key of 2048 bits
	print("\nRSA key 2048 bit generated:")
	key = generate_key("RSA", 2048)
	print(key)

	# Generate RSA key of 1024 bits
	print("\nRSA key 1024 bit generated:")
	key = generate_key("RSA", 1024)
	print(key)

	# Generate DES key and encrypt it with RSA
	print("\n\nEncrypt DES key with RSA 1024",)

	key				  = generate_key("RSA", 1024)
	key_des			  = generate_key("DES3")
	key_des_encrypted = rsa_encrypt(key_des, key)
	key_des_decrypted = rsa_decrypt(key_des_encrypted, key)

	print("\nPlain key:	 ", dump_bytestream(key_des))
	print("\nEncrypted key: ", dump_bytestream(key_des_encrypted))
	print("\nDecrypted key: ", dump_bytestream(key_des_decrypted))
	print("")

	print("\nCalculate OWF:")

	key_des		= generate_key("DES3")
	plain_text	= str2byt("0123456701234567")
	owf_value	= owf(plain_text, key_des)

	print("\nPlain text: ", dump_bytestream(plain_text))
	print("\nKeys used:  ", dump_bytestream(key_des))
	print("\nOWF value:  ", dump_bytestream(owf_value))
	print("")
