#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.Pinpad.Utility3
   :platform: Unix, Windows 
   :synopsis: Pinpad utility module (Python 3).
Contains utility functions for Pinpad package.
"""

import os, sys, time, re, hashlib
from struct import pack, unpack
from binascii import hexlify, unhexlify, crc32

parent_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
sys.path.append(parent_dir)

try:
    import config
except:
    from .. import config

try:
    import means
except:
    from .. import means

try:
    from .. import crypto
except:
    import crypto

try:
    import dukpt
except:
    from .. import dukpt

try:
    import Protocol
except:
    from . import Protocol

try:
    import Enums
except:
    from . import Enums


def get_pin(pinblock, accountNumber = None): # accountNumber must be printable string 
    """
    Get the pin associated with a pinblock adn an account number.
    :Parameters:
        - *pin\_block* (bytes array): The Pinblock.
        - *accountNumber* (string): The account number.
    :Returns: A string representing the pin.
    """
    pin = None
    n = int(pinblock[0]) & 0xf # low nibble defines pin len
    c = int(pinblock[0]) >> 4  # high nibble defines pinblock format type
    if c in [0, 8]:    # for pinblock format 0 (ansi pinblock 0) or 8 (null pinblock)...
        if accountNumber is None: pin = 'AccountNumber missing, it is mandatory for ' + str(c) + '-format'
        else:
            accountNumber = crypto.pad(accountNumber.split('=', 1)[0], side = 'LEFT', length = 17, value = '0')
            accountNumber = unhexlify(accountNumber[-17:-1]) # Remove check digit from Account Number, take the last 12 bytes and then unhexlify it
            pinblock = crypto.XOR(pinblock, accountNumber)                     # Remove accountNumber
    if c in [0, 1, 3]: # only for PinBlock with format type ANSI 0 / 1 / 3
        if 4 <= n <= 12:
            pinBuffer = hexlify(pinblock)
            if c == 0: # only for PinBlock with format type ANSI 0
                for s in pinBuffer[n+2:]:
                        if s != 102:            #chr(102) = 'f'
                            pin = 'Wrong PIN Block of type 0'
                        break
                else: pin = None
            elif c == 3:
                #Get last 12 digit from pan
                if accountNumber.rfind('=')>0:
                        pan = accountNumber[-13-accountNumber.rfind('='):accountNumber.rfind('=')-1]
                else:
                        #TBC
                        if len(accountNumber)<13:
                                #TBC padding method
                                accountNumber = crypto.pad(accountNumber, length = 13, side = 'LEFT', value = '0')
                        pan = '0000'+accountNumber[-13:-1]
                pinBuffer = hexlify(crypto.XOR(pinblock, unhexlify(pan.encode()))) 
            if pin is None:
                pin = pinBuffer[2:n+2]
                try:
                    int(pin) #check if pin contains only numbers
                    pin = pin.decode('latin-1')
                except: pin = 'Wrong PIN Block Format'
        else: pin = 'Invalid digits number for PIN'
    elif c == 8: # only for PinBlock with format type Null PIN block
        if n == 0 and pinblock[-1:] == b'\xff': pin = 'Null PIN Block'
        else: pin = 'Wrong Null PIN Block'
    else: pin = 'Wrong PIN Block Format'
    return pin

def get_APbancomat_pin(pinblock, accountNumber):
    """
    Get the Ap bancomat pin associated with a pinblock adn an account number.
    :Parameters:
        - *pin\_block* (bytes array): The Pinblock.
        - *accountNumber* (string): The account number.
    :Returns: A string representing the pin.
    """ 
    if len(pinblock) != 8:
        return 'Wrong pinblock length!'
    if accountNumber is None or len(accountNumber) < 66:
        return 'Card data Faulty!'

    pan = accountNumber[3:21]
    # Instead of :
#   sep = accountNumber.find('==')
#   pan = accountNumber[sep - 18:sep]
    if accountNumber[55] == b'=':
        CCS = accountNumber[58:66]
    else:
        CCS = accountNumber[61:69]
    if CCS.startswith(b'0000'):
        CCS = b'99499973'
    # reference input values (with PAN only, PIN = "00000")
    i1 = int(pan[:4] + b'00' + pan[6:9])
    i2 = int(pan[9:15] + b'000')
    # divisor modules:
    m1 = int(CCS[:4]) #first 4 digit of CCS  
    m2 = int(CCS[4:]) #last 4 digit of CCS
    #expected result (from PIN Block received):
    r1 = int(pinblock[:4])
    r2 = int(pinblock[4:])
    # pin code value components:
    v1 = 0
    while v1 <= 99000:
        if (i1 + v1) % m1 == r1:
            break;
        v1 += 1000
    v2 = 0
    while v2 <= 999:
        if (i2 + v2) % m2 == r2:
            break;
        v2 += 1
    if (v1 > 99000 or v2 > 999):
        return 'PinBlock Not calculable!'
    return str(v1 + v2)

def get_crc32(file_path):
    """
    This function computes the CRC-32 checksum for a specified file.
    :Parameters:
        - *file_path* (string): Path to file. 
    :Returns: CRC-32 of the file, ``None`` if file don't exist.
    """
    data = None
    with open(file_path, 'rb') as f:
        data = f.read()
    return None if data is None else (crc32(data) & 0xffffffff) 

def download_resource(connection, platform, file_path, resource_id, auth_flag, file_type, description = b'', block_size = 2048, timeout = 0, key = None):
    """
    TODO.
    """
    authent = b'\x00\x00\x00\x00'
    with open(file_path, 'rb') as file:
        data = file.read()
    # if file already resmarked
    mark = data.find(b'/resMark')
    if mark > 0:
        if (len(data) - mark) != 34:
            raise Exception('The Resource file is not resmarked correctly!')
        data = data[0:mark]
    if auth_flag in [1, 2, 3, 4] and key is not None:
        sha1digest = hashlib.sha1(data).digest()
        if platform == 'VGD':
            return False
        if platform == 'M3':
            if auth_flag == 1:
                sha1digest += b'PIN'
            elif auth_flag == 2:
                sha1digest += b'CLEAR'
            authent = crypto.mac_x919(sha1digest, key)
        elif platform == 'NGP':
            digest = b'\x00' + hexlify(sha1digest).upper()
            if auth_flag == 3:
                digest += b'\x00PIN\x00application\x00'
            elif auth_flag == 4:
                digest += b'\x00CLEAR\x00application\x00'
            digest += os.urandom(243 - len(digest))
            CRC32_digest = crc32(digest) & 0xffffffff
            digest += pack("!L", CRC32_digest) + b'\x00'
            priv_key = crypto.import_RSAkey(key, asFile = False)
            authent = crypto.rsa_decrypt(digest, priv_key)
    file_size = len(data)
    CRC32 = crc32(data) & 0xffffffff

    Protocol.send_file_download_request_cmd(connection, file_type, resource_id, file_size, CRC32, auth_flag, authent, description[:16])
    ack_params = Protocol.recv_file_download_request_cmd(connection, timeout)
    if ack_params['ack_code'] != 0x00:
        raise Exception('Download Request failed with ack_code: 0x{0:02x}'.format(ack_params['ack_code']))
    while ack_params['ack_code'] == 0x00:
        offset = ack_params['offset'] if 'offset' in ack_params else 0
        resource_block = data[offset:min(offset + block_size, file_size)]
        Protocol.send_file_block_download_cmd(connection, offset, resource_block)
        ack_params = Protocol.recv_file_block_download_cmd(connection, timeout)
    if ack_params['ack_code'] == 0x02:
        return True
    raise Exception('Download Block failed with ack_code: 0x{0:02x}'.format(ack_params['ack_code']))

def simmetric_keydata_generator(key_use, key_type, key_data, transport_key, SMIDR = b''):
    """
    Generate a suited DES/3DES/DUKPT keyblock for application key download command (0x40,0x04).
    :Parameters:
        - *key\_use* (byte): Key use.
        - *key_type* (byte): Key type.
        - *key_data* (bytes array): Key data.
        - *transport_key* (string): Transport key.
        - *SMIDR* (byte): TODO. 
    :Returns: The encrypted data.
    """
    rand = os.urandom(2)
    keyblock = rand + pack("!BB", key_type, key_use) + key_data
    keyblock_crc = pack("!L", crc32(keyblock) & 0xffffffff)
    keyblock += keyblock_crc
    return crypto.encrypt('DES3_CBC', keyblock, transport_key) + (b'' if SMIDR is None else SMIDR)

def simmetric_keydata_unpack(keyblock, transport_key = b'\x37\x36\x35\x34\x33\x32\x31\x30\x37\x36\x35\x34\x33\x32\x31\x30'):
    """
    TODO.
    :Parameters:
        - *keyblock* (byte): TODO.
        - *transport\_key* (bytes array): Transport key.
    :Returns: TODO.
    """
    if len(keyblock) > 24:
        SMIDR = keyblock[-10:]
        keyblock = keyblock[:-10]
    else: SMIDR = b''
    dec_keyblock  = crypto.decrypt('DES3_CBC', keyblock, transport_key)
    print('KEYBLOCK: ' + hexlify(dec_keyblock).decode('latin-1'))
    print('   RAND : ' + hexlify(dec_keyblock[:2]).decode('latin-1'))
    print('   TYPE : ' + hexlify(dec_keyblock[2]).decode('latin-1'))
    print('   USE  : ' + hexlify(dec_keyblock[3]).decode('latin-1'))
    print('   VALUE: ' + hexlify(dec_keyblock[4:-4]).decode('latin-1'))
    print('   CRC32: ' + hexlify(dec_keyblock[-4:]).decode('latin-1'))
    print('   SMIDR: ' + hexlify(SMIDR).decode('latin-1'))
    return dec_keyblock[:2], dec_keyblock[2], dec_keyblock[3], dec_keyblock[4:-4], dec_keyblock[-4:], SMIDR

def rsa_keydata_generator(modulo, exponent):
    """
    Generates a suited RSA keyblock for application key download command (0x40,0x04).
    :Parameters:
            - *modulo* (bytes array): RSA modulo.
            - *exponent* (bytes array): RSA exponeent.
            
        :Returns: *keyblock* + *keyblock crc* 
    """
    keyblock = pack("!HH", len(modulo), len(exponent)) + modulo + exponent
    keyblock_crc = pack("!L", crc32(keyblock) & 0xffffffff)
    return keyblock + keyblock_crc

def DeleteKeyM3(connection, key_id):
    """
    TODO.
    """
    apdu = pupyspot.create_apdu(0x01, bytes((key_id,)) + b'\x01\xFF\x03\x00\x00\x00\x00\x21\x44\xDF\x1C')
    connection.send_apdu(apdu)

def multiblock_encryption(c, key_id, versus, data, block_size, working_key_mode, working_key = b''):
    """
    TODO.
    """
    start = False
    chaining_data = b''
    result = b''
    while data != b'':
        if len(data) <= block_size and not start:
            buff = data
            data = b''
            flag = 0x03
            start = True
        else:
            if len(data) > block_size:
                buff = data[:block_size]
                data = data[block_size:]
                flag = 0x00 # body
            else:
                buff = data 
                data = b''
                flag = 0x02 # end
            if not start:
                flag = 0x01 # start
                start = True
        Protocol.send_multiblock_encryption(c, key_id, flag, versus, buff, chaining_data, working_key_mode, working_key)
        ack = Protocol.recv_multiblock_encryption(c, 4)
        if ack['ack_code'] == 0x00:
            chaining_data = ack['chaining_data']
            result += ack['data']
        else: break
    return ack['ack_code'], result


def multiblock_MAC_verification(connection, KeyId, WorkingKeyMode = None, WorkingKey = None, Versus = None, Data = None, IV = None, BlockSize = 16, MACData = None):
    """
    MultiBlock Encryption, MAC verification: Keyboard and Security  (0x40,0x16).
    :Parameters:
        -*connection* (PupySPOT): Object managing spot connection.
        - *KeyId* (integer): Slot ID of Master Key.
        - *WorkingKeyMode* (Enums.WKMode):  Working Key format. 
        - *WorkingKey* (bytes array): Working Key value encrypted with Master Key.
        - *Versus* (Enums.Versus):  Operation required.
        - *Data* (bytes array): Data block on which the operation is performed.
        - *IV* (bytes array): Initial vector IV. 
        - *BlockSize* (integer): Length of each block.
        - *MACData* (bytes array): MAC data buffer to verify.
    :Returns: Ack-Code indicating successful of the operation.
    """
    blkcount = 0
    data = Data
    cblock = IV
    macData = None

    # block split
    while True:
        block = data[:BlockSize]
        data = data[BlockSize:]
        #send
        if len(block) > 0 and len(data) > 0:
            flags = Enums.Flags.START_DATA_BLOCK if blkcount == 0 else Enums.Flags.INTERMEDIATE_DATA_BLOCK
        else:   
            flags = Enums.Flags.FIRSTANDLAST_DATA_BLOCK if blkcount == 0 else Enums.Flags.END_DATA_BLOCK
            macData = MACData
        Protocol.send_multiblock_verification(connection, KeyId = KeyId, WorkingKeyMode = Enums.WKMode[WorkingKeyMode], WorkingKey = WorkingKey, Flags = flags, Versus = Enums.Versus[Versus], Data = block, ChainingData = cblock, MACData = macData)
        #receive
        ack = Protocol.recv_multiblock_verification(connection, 120)
        if ack['ACKcode'] == 0x00:
            cblock = ack['ChainingData']
            if flags == Enums.Flags.FIRSTANDLAST_DATA_BLOCK or flags == Enums.Flags.END_DATA_BLOCK: break
        else: break
        blkcount += 1
    return ack['ACKcode']

def multiblock_encrypt(connection, KeyId, WorkingKeyMode = None, WorkingKey = None, Versus = None, Data = None, IV = None, BlockSize = 16):
    """
    MultiBlock Encryption, MAC Generation: Keyboard and Security (0x40,0x15).
    :Parameters:
        -*connection* (PupySPOT): Object managing spot connection.
        - *KeyId* (integer): Slot ID of Master Key.
        - *WorkingKeyMode* (Enums.WKMode):  Working Key format. 
        - *WorkingKey* (bytes array): Working Key value encrypted with Master Key.
        - *Versus* (Enums.Versus):  Operation required.
        - *Data* (bytes array): Data block on which the operation is performed.
        - *IV* (bytes array): Initial vector IV. 
        - *BlockSize* (integer): Length of each block.
    :Returns: Ack-Code indicating successful of the operation.
    """
    blkcount=0
    data = Data
    cblock = IV
    resultdata = b''
    
    # block split
    while True:
        block = data[:BlockSize]
        data = data[BlockSize:]
        #send
        if len(block) > 0 and len(data) > 0:
            flags = Enums.Flags.START_DATA_BLOCK if blkcount == 0 else Enums.Flags.INTERMEDIATE_DATA_BLOCK
        else:   
            flags = Enums.Flags.FIRSTANDLAST_DATA_BLOCK if blkcount == 0 else Enums.Flags.END_DATA_BLOCK
        Protocol.send_multiblock_encrypt(connection, Flags = flags, KeyId = KeyId, WorkingKeyMode = Enums.WKMode[WorkingKeyMode], WorkingKey = WorkingKey, Versus = Enums.Versus[Versus], Data = block, ChainingData = cblock)
        #receive
        ack = Protocol.recv_multiblock_encrypt(connection, 120)
        if ack['ACKcode'] == 0x00:
            cblock = ack['ChainingData']
            resultdata += ack['Data']
            if flags == Enums.Flags.FIRSTANDLAST_DATA_BLOCK or flags == Enums.Flags.END_DATA_BLOCK: break
        else: break
        blkcount += 1
    return ack['ACKcode'], resultdata

def data_encryption(connection, KeyId, Versus = None, Data = None, WorkingKey = b''):
    """
    Keyboard and Security, Data encryption (0x40, 0x03).
    :Parameters:
        -*connection* (PupySPOT): Object managing spot connection.
        - *KeyId* (integer): Slot ID of Master Key.
        - *Versus* (Enums.Versus):  Operation required.
        - *Data* (bytes array): Data block on which the operation is performed.
        - *WorkingKey* (bytes array): Working Key value encrypted with Master Key.
    :Returns: Ack-Code indicating successful of the operation.      
    """
    Protocol.send_data_encryption(connection, keyid = KeyId, versus = Versus, data = Data, mode = 0x00 if WorkingKey == b'' else 0x01, working_key = WorkingKey)
    ack = Protocol.recv_data_encryption(connection, timeout = 120)
    if ack['ack_code'] != 0x00: ack['Data'] = None
    return ack['ack_code'], ack['Data'] 

def MAC_verification(connection, KeyId, Versus = None, Data = None, MACData = None):
    """
    Keyboard and Security, MAC verification (0x40, 0x07).
    :Parameters:
        -*connection* (PupySPOT): Object managing spot connection.
        - *KeyId* (integer): Slot ID of Master Key.
        - *Versus* (Enums.Versus):  Operation required.
        - *Data* (bytes array): Data block on which the operation is performed.
        - *MACData* (bytes array): MAC data buffer to verify.
    :Returns: Ack-Code indicating successful of the operation.  
    """
    Protocol.send_mac_verification(connection, key_id = KeyId, versus = Versus, data = Data, mac = MACData)
    ack = Protocol.recv_send_mac_verification(connection, timeout = 120)
    return ack['ack_code'] 

# This function is used only for system with multiblock encryption and MAC verification
def calc_data_encrypt(key, data, type, versus, SMIDR = None, iv = None):
    """
    TODO.
    """
    if type is Enums.KeyKind.DES:
        algorithm = 'DES'
        if iv is None: iv = b'\x00' * crypto.block_size
    elif type is Enums.KeyKind.TDES:
        algorithm = 'DES3'
        if iv is None: iv = b'\x00' * crypto.block_size
    elif type is Enums.KeyKind.AES:
        algorithm = 'AES'
        if iv is None: iv = b'\x00' * crypto.iv_size
    elif type is Enums.KeyKind.RSA:
        raise Exception('Key Type NOT managed here: ' + str(type))
    else:
        raise Exception('Unknown Key Type Selected: ' + str(type))

    if SMIDR is not None and versus in [Enums.Versus.X919_MAC, Enums.Versus.DUKPT1992_MAC, Enums.Versus.DUKPT_MAC_IN, Enums.Versus.DUKPT_MAC_OUT, Enums.Versus.DUKPT_CBC_ENCRYPT, Enums.Versus.DUKPT_CBC_DECRYPT, Enums.Versus.DUKPT_MAC_FULL]:
        if len(SMIDR) != 10:
            raise Exception('Wrong size of SMIDR parameter: ' + str(len(SMIDR)))
        dukpt_key = dukpt.DUKPT_Key(len(key))
        dukpt_key.initializeKeys(SMIDR, key)

    #import pdb; pdb.set_trace()
    if versus is Enums.Versus.ECB_ENCRYPT:
        crypt_data = crypto.encrypt(algorithm + '_ECB', data, key)
    elif versus is Enums.Versus.ECB_DECRYPT:
        crypt_data = crypto.decrypt(algorithm + '_ECB', data, key)
    elif versus is Enums.Versus.CBC_ENCRYPT:
        crypt_data = crypto.encrypt(algorithm + '_CBC', data, key, iv = iv)
    elif versus is Enums.Versus.CBC_DECRYPT:
        crypt_data = crypto.decrypt(algorithm + '_CBC', data, key, iv = iv)
    elif versus is Enums.Versus.X919_MAC:
        if SMIDR is None: crypt_data = crypto.mac_x919(data, key)
        else: crypt_data = dukpt_key.mac_x919(data)
    elif versus is Enums.Versus.DUKPT1992_MAC:
        crypt_data = dukpt_key.mac_dukpt(data)
    elif versus is Enums.Versus.OFB_ENCRYPT:
        crypt_data = crypto.encrypt(algorithm + '_OFB', data, key, iv = iv)
    elif versus is Enums.Versus.MAB:
        crypt_data = crypto.MAB(data, key, 8)
    elif versus is Enums.Versus.DUKPT_MAC_IN:
        crypt_data = dukpt_key.mac_x919_nordic(data, True)   #Variant for Nordic Mac x9.19 (IN)
    elif versus is Enums.Versus.DUKPT_MAC_OUT:
        crypt_data = dukpt_key.mac_x919_nordic(data, False)  #Variant for Nordic Mac x9.19 (OUT)
    elif versus is Enums.Versus.DUKPT_CBC_ENCRYPT:
        crypt_data = dukpt_key.tdes_cbc(data, True)
    elif versus is Enums.Versus.DUKPT_CBC_DECRYPT:
        crypt_data = dukpt_key.tdes_cbc(data, False)
    elif versus is Enums.Versus.DUKPT_MAC_FULL:
        crypt_data = dukpt_key.mac_x919(data, 8)
    elif versus is Enums.Versus.CFB_ENCRYPT:
        crypt_data = crypto.encrypt(algorithm + '_CFB', data, key, iv = iv)
    elif versus is Enums.Versus.CFB_DECRYPT:
        crypt_data = crypto.decrypt(algorithm + '_CFB', data, key, iv = iv)
    elif versus is Enums.Versus.CTR_ENCRYPT:
        crypt_data = crypto.encrypt(algorithm + '_CTR', data, key)
    elif versus is Enums.Versus.CTR_DECRYPT:
        crypt_data = crypto.decrypt(algorithm + '_CTR', data, key)
    elif versus is Enums.Versus.MAC_ISO16609:
        crypt_data = crypto.mac_x919(data, key)
    elif versus is Enums.Versus.MAC_ISO9797_1:
        crypt_data = crypto.MAC1(data, key)
    elif versus is Enums.Versus.MAC_ISO9797_2:
        crypt_data = crypto.MAC2(data, key)
    elif versus is Enums.Versus.MAC_ISO9797_3:
        crypt_data = crypto.MAC3(data, key)
    elif versus is Enums.Versus.MAC_ISO9797_4:
        crypt_data = crypto.MAC4(data, key)
    elif versus is Enums.Versus.MAC_ISO9797_5:
        crypt_data = crypto.MAC5(data, key)
    else:
        raise Exception('Invalid Encryption Versus Selected ' + str(versus))
    return crypt_data

def get_platform(connection):
    """
    Gets the platform type.
    :Parameters:
        -*connection* (PupySPOT): Object managing spot connection.
    :Returns: A string containing the platform type.  
    """
    Protocol.send_hardware_configuration(connection, [0x01, 0x42])
    hw_conf_ack = Protocol.recv_hardware_configuration(connection, 0)
    connection.platform = None
    if hw_conf_ack['Entry']['66'].startswith(b'MCF532x'):
        connection.platform = 'M3'
    elif hw_conf_ack['Entry']['66'].startswith(b'OMAP35xx'):
        connection.platform = 'NGP'
    elif hw_conf_ack['Entry']['66'].startswith(b'UPMxxxxx'):
        connection.platform = 'VGD'
    return connection.platform

def get_card_reader_type(connection):
    """
    Gets the card reader type.
    :Parameters:
        -*connection* (PupySPOT): Object managing spot connection.
    :Returns: A string containing the card reader type.  
    """
    Protocol.send_hardware_configuration(connection, [0x34])
    hw_conf_ack = Protocol.recv_hardware_configuration(connection, 0)
    connection.cr_type = None
    if hw_conf_ack['Entry']['52'].startswith(b'ZU-'):
        connection.cr_type = 'PANASONIC'
    elif hw_conf_ack['Entry']['52'].startswith(b'MAGTEK-HCR'):
        connection.cr_type = 'HCR2'
    elif hw_conf_ack['Entry']['52'].startswith(b'Sankyo'):
        connection.cr_type = 'SANKYO'
    elif hw_conf_ack['Entry']['52'].startswith(b'SANKYO-HCR2'):
        connection.cr_type = 'SANKYO-HCR2'
    elif hw_conf_ack['Entry']['52'].startswith(b'Omron'):
        connection.cr_type = 'OMRON'
    return connection.cr_type

def get_bundle_version(connection, platform, login_answer):
    """
    Gets the bundle version.
    :Parameters:
        -*connection* (PupySPOT): Object managing spot connection.
        -*platform* (string): Platform type.
        -*login_answer* (string): TODO.
    :Returns: A string containing the bundle version.
    """
    version = None
    if platform == 'M3':
        Protocol.send_software_configuration(connection, [])
        ack = Protocol.recv_software_configuration(connection, 0)
        version = ack['Entry']['15'].split(b'\n')[0].split(b' ')[1]
    elif platform == 'NGP':
        version = '|'.join([str(e['app_id']) + '=' + e['app_ascii'].strip('\x00') for e in login_answer['entries']])
    elif platform == 'VGD':
        version = '|'.join([str(e['app_id']) + '=' + e['app_ascii'].strip('\x00') for e in login_answer['entries']])
    return version

class Resource_File(object):
    """
    TODO.
    """
    def __init__(self, file_path, file_type = 0, resource_id = 0, auth_flag = 0, description = b''):
        self.file_path = file_path
        self.file_type = file_type
        self.resource_id = resource_id
        self.auth_flag = auth_flag
        self.description = description
        self.file_size = os.path.getsize(file_path) - 34
        if self.file_size < 0: self.file_size = 0
        if auth_flag != 0:
            (self.resource_id, self.auth_flag, self.crc32, self.mac_x919, self.description) = self.get_resmark(file_path, 2)
        self.resource_file = open(self.file_path, 'rb')
        self.offset = 0

    def _del_(self):
        close(self.resource_file)

    def get_block(self, max_block_size = 2048):
        block_size = min(max_block_size, self.file_size - self.offset)
        if block_size > 0:
            resource_block = self.resource_file.read(block_size)
            self.offset = self.offset + block_size
            return resource_block
        return None

    def get_resmark(self, path_to_resource, auth_type = 0) :
            if auth_type in [0, 1, 2]:
                resource_file = open(path_to_resource, 'rb')
                resource_file.seek(-34, os.SEEK_END)
                resmark = resource_file.read(34)
                (resmark_tag, resource_id, auth_flag, crc32, mac_x919, description) =  unpack('!8sBBII16s', resmark)
                resource_file.close()
                if resmark_tag != b'/resMark':
                    raise Exception('resMark tag not found in resource file {0}'.format(path_to_resource) )
                else:
                    return (resource_id, auth_flag, crc32, mac_x919, description)
            else:
                raise Exception('Unexpected auth_type value')
                # TODO: rsa and description capture missing for other auth_flag
                return 0

class Resource_List(list):
    """
    TODO.
    """
    def walk_resource_tree(self, root_path):
        message_id     = 0x10
        image_id       = 0x11
        font_id        = 0x12
        template_id    = 0x15
        emv_table_id   = 0x20
        emv_message_id = 0x21

        resource_path = {
            os.path.join(root_path, 'Pinpad/Image'):               image_id,
            os.path.join(root_path, 'Pinpad/Image/Color'):         image_id,
            os.path.join(root_path, 'Pinpad/Image/BN'):            image_id,
            os.path.join(root_path, 'Pinpad/Messages'):            message_id,
            os.path.join(root_path, 'Pinpad/Font'):                font_id,
            os.path.join(root_path, 'Pinpad/DisplayScreen'):       template_id,
            os.path.join(root_path, 'Pinpad/DisplayScreen/BN'):    template_id,
            os.path.join(root_path, 'Pinpad/DisplayScreen/Color'): template_id,
            os.path.join(root_path, 'Pinpad/Templates'):           template_id,
            os.path.join(root_path, 'Emv/Messages'):               emv_message_id,
            os.path.join(root_path, 'Emv/Tables'):                 emv_table_id,
            }

        for dirname, dirnames, filenames in os.walk(root_path):
            if dirname in resource_path:
                for filename in filenames:
                    parse_filename = re.search('[Ii][Dd]([0-9]{3})_([012])_(.+)\....', filename)
                    try:
                        (resource_id, auth_flag, description) = parse_filename.groups()
                        self.append((os.path.join(dirname, filename), resource_path[dirname], resource_id, auth_flag, description))
                    except AttributeError:
                        print('skipped {0}'.format(filename))

    def set_current_resource(self, file_path, file_type, resource_id, auth_flag, description):
        self.crf = Resource_File(file_path, file_type, resource_id, auth_flag, description)
        
def format_system_status(cmdResponse):
    """
    *cmdResponse* is the answer received from pupySPOT connection.
    """
    device = {0x01: 'SPOT_SYS_MAIN', 0x02: 'SPOT_SYS_SECURITYMODULE', 0x04: 'SPOT_SYS_DISPLAY', 0x08: 'SPOT_SYS_KEYBOARD', 0x10: 'SPOT_SYS_CARDREADER', 0x20: 'SPOT_SYS_GEMCORE'}
    msg = ''
    try:
        for entry in cmdResponse['Entry']:
            msg = msg + 'Device:' + device[entry] + '(' + hex(entry) + ') Status:' + hex(cmdResponse['Entry'][entry]['Status']) +  ' Error code:' + hex(cmdResponse['Entry'][entry]['errorcode']) + ' Status ext:' + hex(cmdResponse['Entry'][entry]['StatusExt']) + '\n'
    except:
        msg = 'No entries for system status!\n'
    return msg
    
def wait_card_insert(connection, chip = False, timeout = 0):
    """
    TODO.
    """
    i = 0
    while timeout == 0 or (timeout > 0 and i <= timeout):
        print('Please insert a chip card ...')
        ack = Protocol.recv_system_status(connection, timeout)
        cr_status = ack['Entry'].get(0x10)
        if cr_status:
            sts = cr_status['Status']
            ext = cr_status['StatusExt']
            if sts == 1  and (((ext&3) == 3) if chip else ((ext&1) == 1)):
                return True
        i += 1
        time.sleep(1)
    return False

'''For VGD only:'''
def send_cert_browse_sequence(connection):
    """
    TODO.
    """
    Protocol.send_cert_browse(connection)
    ack_params = Protocol.recv_cert_browse(connection, timeout = 0)
    if ack_params['ack_code'] is not 0: return
    browse_size = ack_params['file_size']
    offset = 0
    blk_size = 0
    while offset + blk_size < browse_size:
        Protocol.send_cert_block_browse(connection, offset)
        ack_params = Protocol.recv_cert_block_browse(connection)
        if ack_params['ack_code'] is not 0 : return
        offset = ack_params['Offset']
        blk_size = ack_params['BlkSize']
        print('Cert browse => BlkData: ' + ack_params['BlkData'])

def mac_cbc(key, data):
    """
    TODO.
    """
    return crypto.encrypt("DES3_CBC", data,  key)[-8:-4]

def tr31_kba(kbpk, key, header):
    """
    TODO.
    """
    if header[0] == 'A':
        kbek = bytes((0x45 ^ x) for x in kbpk)
        kbmk = bytes((0x4D ^ x) for x in kbpk)
    elif header[0] == 'B':
        raise Exception('TODO: TR31 block version: '+header[0])
    else:
        raise Exception('Undefined TR31 block version: '+header[0])
    len_key = len(key)
    key_len = pack('!H', len_key * 8)
    if len_key == 8 or len_key == 16: rand_bytes = os.urandom(22 - len_key)
    else: raise Exception('Wrong length of the DES/DES3 Key!')
    key_block = key_len + key + rand_bytes
    enc_key_block = crypto.encrypt('DES3_CBC', key_block,  kbek, iv = header[:8])
    mac = mac_cbc(kbmk, header + enc_key_block)
    return header + hexlify(enc_key_block + mac)

def tr31_header(block_version, key_usage, algorithm, use_mode, version_num, exportability, optional_block = b'0000'):
    """
    TODO.
    """
    lenght = ('0'*(4-len(str(68 + len(optional_block))))+str(68 + len(optional_block))).encode()
    return pack('!c4s2scc2sc', block_version, lenght, key_usage, algorithm, use_mode, version_num, exportability) + optional_block

def tr31_kba_unpack(key_block, kbpk):
    """
    TODO.
    """
    key_block_version = key_block[0]
    length            = int(key_block[1:5])
    usage             = key_block[5:7]
    algorithm         = key_block[7]
    mode              = key_block[8]
    key_version_n     = int(key_block[9:11])
    exportability     = key_block[11]
    num_opt_block     = int(key_block[12:14])
    reserved          = key_block[14:16]
    opt_block         = {}
    l = 16
    for i in range(0, num_opt_block):
        id = key_block[l:l+2]
        l+=2
        opt_block[id]={
                        'lenght':int(key_block[l:l+2],16),
                        'data'  :key_block[l+2:l+2+int(key_block[l:l+2],16)-4]
                        }
        l+=opt_block[id]['lenght']-2
    mac = unhexlify(key_block[-8:])
    kbek = bytes((0x45 ^ x) for x in kbpk)
    if algorithm == 'D': key_len = 8
    elif algorithm == 'T': key_len = 16
    elif algorithm == 'A':
        if len(key_block[l:-8]) == 40: key_len = 16
        else: offset = 24
    if len(kbek) == 16: dec_key = crypto.decrypt('DES3_CBC', unhexlify(key_block[l:-8]), kbek, iv = key_block[:8])
    elif len(kbek) == 8: dec_key = crypto.decrypt('DES_CBC', unhexlify(key_block[l:-8]), kbek, iv = key_block[:8])
    else: raise Exception('Unknown length for KBPK key: '+str(len(kbek)))
    nbit = unpack('!H', dec_key[:2])[0]
    key = dec_key[2:2+key_len]
    return key_block_version, length, usage, algorithm, mode, key, mac

def makeTerminalCAcertificate(cert_request):
    """
    TODO.
    """
    content = b"It was a bright cold day in April, and the clocks were striking thirteen. Winston Smith, his chin nuzzled into his breast in an effort to escape the vile wind, slipped quickly through the glass doors of Victory Mansions, though not quickly enough to prevent a swirl of gritty dust from entering along with him."
    try:
        import OpenSSL
        basedir = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
        req = OpenSSL.crypto.load_certificate_request(OpenSSL.crypto.FILETYPE_ASN1, cert_request)
        pub_key = req.get_pubkey()
        if req.verify(pub_key):
            subject = req.get_subject()
            with open(os.path.join(basedir, config.certpath, 'ca_cert.pem'), 'rb') as cf:
                cacert_buffer = cf.read()
            ca_cert = OpenSSL.crypto.load_certificate(OpenSSL.crypto.FILETYPE_PEM, cacert_buffer)
            with open(os.path.join(basedir, config.certpath, 'priv_key.pem'), 'rb') as kf:
                privkey_buffer = kf.read()
            priv_key = OpenSSL.crypto.load_privatekey(OpenSSL.crypto.FILETYPE_PEM, privkey_buffer)
            signature = OpenSSL.crypto.sign(priv_key, content, 'sha1')
            OpenSSL.crypto.verify(ca_cert, signature, content, 'sha1') # This throws an exception if fail
            issuer = ca_cert.get_issuer()
            newCertificate = OpenSSL.crypto.X509()
            newCertificate.set_version(3)
            newCertificate.set_serial_number(int.from_bytes(os.urandom(64), byteorder = 'big'))
            newCertificate.set_issuer(issuer)
            from datetime import datetime, timedelta
            now = (datetime.now() - timedelta(days = 7)).strftime("%Y%m%d%H%M%SZ").encode('latin-1')
            expire  = (datetime.now() + timedelta(days = 365)).strftime("%Y%m%d%H%M%SZ").encode('latin-1')
            newCertificate.set_notBefore(now)
            newCertificate.set_notAfter(expire)
            newCertificate.set_subject(subject)
            newCertificate.set_pubkey(pub_key)
            newCertificate.sign(priv_key, 'sha1')
            terminal_certificate = OpenSSL.crypto.dump_certificate(OpenSSL.crypto.FILETYPE_ASN1, newCertificate)
            with open(os.path.join(basedir, config.certpath, 'master_ca_cert.pem'), 'rb') as mcf:
                mastercacert_buffer = mcf.read()
            cert_masterCA = OpenSSL.crypto.load_certificate(OpenSSL.crypto.FILETYPE_PEM, mastercacert_buffer)
            masterCA_certificate = OpenSSL.crypto.dump_certificate(OpenSSL.crypto.FILETYPE_ASN1, cert_masterCA)
            mac_data = len(terminal_certificate).to_bytes(2, byteorder='big') + terminal_certificate + len(masterCA_certificate).to_bytes(2, byteorder='big') + masterCA_certificate;
            import hashlib
            sha256_hash = hashlib.sha256(mac_data).digest()
            mac = crypto.mac_x919(sha256_hash, b'0123456701234567');
            return (int.from_bytes(mac, byteorder = 'big'), terminal_certificate, masterCA_certificate)
        else:
            means.error._lay('Verification of Certificate Request Failed!')
    except:
        means.error._lay(means.errExc('makeTerminalCertificate'))
    return None
