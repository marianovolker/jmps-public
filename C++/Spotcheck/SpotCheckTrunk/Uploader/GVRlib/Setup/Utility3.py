"""
.. module:: GVRlib.Setup.Utility3
   :platform: Unix, Windows 
   :synopsis: EMV utility module (Python 3).
Contains utility functions for Setup package.
"""

import time
from binascii import unhexlify, hexlify
try:
    import Protocol
except:
    from . import Protocol

def package_info(connection, _set, complete = False):
    """
    Sends a *Package info* command and waits for an answer from the SPOT.
    :Parameters:
     :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *\_set* (byte): 0x00 = active package set, 0\x01 = new package set.
        - *complete* (bool): True if the APDU message contains all parameters, False otherwise.
    :Returns: str -- String representing the received message.  
    """    
    Protocol.send_package_info(connection, _set)
    ack = Protocol.recv_package_info(connection, 0)
    pckg_info = ''
    try:
        if ack['ack_code'] == 0x00:
            if ack['Pack']:
                for pack in ack['Pack']:
                    pack = pack.decode('latin-1')
                    i = 0   #Initialize
                    i += 1  #Start marker
                    #Package name
                    if pack[i:i+5] == 'ROMfs':       
                        pckg_info += pack[i:i+6]
                        i += 6
                    elif pack[i:i+6] == 'kernel':
                        pckg_info += pack[i:i+6]
                        i += 6
                    elif pack[i:i+8] == 'recovery':
                        pckg_info += pack[i:i+8]
                        i += 8
                    elif pack.find('\x00', i + 1) > i:
                        pckg_info += pack[i:pack.find('\x00', i + 1)]
                        i += len(pack[i:pack.find('\x00', i + 1)])
                    else:
                        return False
                    pckg_info += ' '
                    #Package SHA
                    i += 1  #Start marker
                    if complete: pckg_info += 'SHA: '+ pack[i:i+40] + ' '
                    i += 40
                    #Vendor name
                    i += 1  #Start marker
                    if pack[i:i+8]=='Gilbarco': 
                        if complete:
                            pckg_info += pack[i:i+8] + ' '
                        i += 8
                    elif pack.find('\x00', i+1) > i:
                        if complete:
                            pckg_info+=pack[i:i+pack.find('\x00', i+1)]+' '
                        i += len(pack[i:i+pack.find('\x00', i+1)])
                    else: return False
                    #Product name or board name
                    i += 1  #Start marker
                    if pack.find('\x00', i+1) > i:
                        if complete:
                            pckg_info+=pack[i:pack.find('\x00', i+1)]+' ' 
                        i += len(pack[i:pack.find('\x00', i+1)])
                    else: return False
                    #Application name
                    i += 1  #Start marker
                    if pack.find('\x00', i+1) > i:
                        pckg_info+=pack[i:pack.find('\x00', i+1)]+' '
                        i += len(pack[i:pack.find('\x00', i+1)])
                    else: return False
                    #Version
                    i += 1  #Start marker
                    if pack.find('\x00', i+1) > i:
                        pckg_info+=pack[i:pack.find('\x00', i+1)]+' '
                        i += len(pack[i:pack.find('\x00', i+1)])
                    else: return False
                    #DateTime
                    i += 1  #Start Marker
                    if complete: pckg_info+=pack[i:i+16]+' '
                    i += 16
                    #Firmware MAC
                    i += 1  #Start Marker
                    if complete: pckg_info+='FW MAC:' + hexlify(pack[i:i+41].encode()).decode('latin 1')
                    i += 41
                    pckg_info += '\n'
            else:
                return False
        else:
            return False
    except:
        return False
    return pckg_info
