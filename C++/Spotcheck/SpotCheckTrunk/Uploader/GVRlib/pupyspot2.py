#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""
.. module:: GVRlib.protocol
   :platform: Unix, Windows
   :synopsis: A Pure Python SPOT Protocol implementation.

This module contains a Python SPOT Protocol implementation.

"""

__author__       = "Daniele Simonetti"
__author_email__ = "cons_daniele.simonetti@gilbarco.com"
__version__      = "2.0"

import os
import sys
import time
import threading
import socket
import ssl
import types

from struct import pack, unpack
from collections import deque
from cStringIO import StringIO
from datetime import datetime, timedelta

try:
    import config
except:
    from . import config


def thread_proc(event, interval, action, exception):
    """
    Implements a loop that listens for events and then triggers a callback function when one of those events is detected.
    
    :Parameters:
        - *event* (Event): Event.
        - *interval* (integer): Timeout.
        - *action* (function): Callback function to call.
        - *exception* (Exception): Exception associated with the callback function.
        
    :Raises: ``exception``: exception passed as parameter.
    
    """
    while True:
        event.wait(interval)
        if event.isSet():
            break
        try:
            action()
        except:
            if exception:
                exception()

def raise_event(events, *args):
    """
    Initializes a list of events. 
    
    :Parameters:
        - *events* (list): List of events.
        - *\*args* (list): List of parameters to pass to the event.
        
    """
    for cb in events:
        cb(*args)

def total_seconds(td):
    """
    Return the total number of seconds contained in the time interval. 
    
    :Parameters:
        - *td* (datetime): time interval.
        
    :Returns: The total number of seconds contained in the time interval.
        
    """
    if sys.version >= (2, 7):
        return td.total_seconds()
    return td.seconds

### ------------------- PURE PYTHON SPOT INTERFACE CLASS ------------------- ###

class PupySPOT(object):
    """
    This class handle communication and SPOT Interface commands.
    
    Example usage::

        import pupyspot

        def on_online_status(status):
            # yada yada yada

        def on_incoming_message(appid, msg, tpdu):
            apdu = pupyspot.apdu_from_message(appid, msg)
            # foo bar baz

        spot = pupyspot.PupySPOT()
        spot.register_event('online_status', on_online_status   )
        spot.register_event('incoming_message', on_incoming_message)
        spot.register_event('ienqueue_message', ienqueue_message)
        spot.connect(my_spot_ip, my_spot_port, my_spot_timeout)
        
    """

    def __init__(self):
        """
        Initialize the *SpotInterface* instance.
        
        """
        self.ev_change_status = []
        self.ev_incoming_msg  = []
        self.ev_outgoing_msg  = []
        self.ev_ienqueue_msg  = []
        self.events = {
                      'online_status'   : self.ev_change_status,
                      'incoming_message': self.ev_incoming_msg,
                      'outgoing_message': self.ev_outgoing_msg,
                      'ienqueue_message': self.ev_ienqueue_msg
                      }

        self.io_thread  = None
        self.io_event   = None
        self.sock       = None

        self.poll_tmo   = 0
        self.last_poll  = None

        self.emit_ping  = False

        self.o_queue    = deque()
        self.or_queue   = deque()
        self.i_queue    = deque()
        
        self.connected  = False
        self.auto_send  = True

        self.platform    = None
        self.cr_type    = None

        self._exception = None

    ### API ###

    @property
    def receive_ping_message(self):
        """
        Receives ping messages.
        
        :Returns: ``True`` if ``incoming_message`` events are raised even on PING messages, ``False`` otherwise.
        """
        return self.emit_ping

    @receive_ping_message.setter
    def receive_ping_message(self, value):
        self.emit_ping = value
        
    @property
    def auto_send_messages(self):
        """
        :Returns: the ``auto_send`` value.
        """
        return self.auto_send

    @receive_ping_message.setter
    def auto_send_messages(self, value):
        self.auto_send = value

    @property
    def is_connected(self):
        return self.connected

    def connect(self, spot_ip, spot_port = 5200, spot_tmo = 0, use_ssl = False):
        """
        Try to establish a connection with the SPOT device at given host

        :Parameters:
            - *spot_ip* (string): SPOT IP address.
            - *spot_port* (integer): Port number.
            - *spot_tmo* (integer): Connection timeout.
            - *use_ssl* (integer): Use SSl protocol.
        
        :Returns: ``True`` if connection is established, ``False`` otherwise.
        
        """
        self.poll_tmo = timedelta( seconds=spot_tmo )

        def start_io_thread():
            self.io_event  = threading.Event()
            self.io_thread = threading.Thread(
                               target = thread_proc,
                               args   = (self.io_event,
                                        0.001,
                                        self._spot_io,
                                        self._exception))

            self.io_thread.daemon = True
            self.io_thread.start()

        def create_spot_socket():
            if self.sock is not None:
                raise Exception("already connected")
            # Create a socket (SOCK_STREAM means a TCP socket)
            self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.sock.settimeout(17.0 if use_ssl else 1.0)
            connected = False
            # Connect
            try:
                self.sock.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
                if use_ssl:
                    basedir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
                    self.sock = ssl.wrap_socket(self.sock,
                                                keyfile = os.path.join(basedir, config.certpath, 'client.key'),
                                                certfile = os.path.join(basedir, config.certpath, 'client.pem'),
                                                ca_certs = os.path.join(basedir, config.certpath, 'root_ca.pem'),
                                                do_handshake_on_connect = False,
                                                ssl_version = ssl.PROTOCOL_TLSv1,
                                                cert_reqs = ssl.CERT_REQUIRED)
                    self.sock.connect((spot_ip, spot_port + 1))
                    while True:
                        try: self.sock.do_handshake()
                        except ssl.SSLError as err:
                            if err.args[0] == ssl.SSL_ERROR_WANT_READ:
                                select.select([self.sock], [], [])
                            elif err.args[0] == ssl.SSL_ERROR_WANT_WRITE:
                                select.select([], [self.sock], [])
                            else: raise
                        else: break
                else:
                    self.sock.connect((spot_ip, spot_port))
                connected = True
            except Exception as e:
                self.sock = None
                connected = False
            return connected

        if create_spot_socket():
            self.connected = True
            start_io_thread()
            raise_event(self.ev_change_status, True)
        else:
            self.connected = False
            raise_event(self.ev_change_status, False)
        return self.connected

    def disconnect(self):
        """
        Disconnect from the SPOT.
        
        """
        if self.io_event is not None:
            self.io_event.set()
            self.io_thread.join()
            if self.sock!=None:
                self.sock.close()

        self.sock=None

        self.o_queue .clear()
        self.or_queue.clear()
        self.i_queue .clear()

        self.connected = False
        raise_event(self.ev_change_status, False)

    def process_events(self):
        """
        Send and receive queued messages and raise events.
        
        """
        self._send_queued_msg()
        self._recv_queued_msg()

    def register_event(self, ev_name, ev_cb):
        """
        Register a callback to an event. Event supported are:
            - ``online_status``,
            - ``incoming_message``.
            
        :Parameters:
            - *ev_name* (string): Event name.
            - *ev_cv* (function): Event callback.
            
        """
        if ev_name not in self.events:
            raise Exception('Event {0} not found'.format(ev_name))
        if ev_cb is None:
            raise Exception('Expected callback, found None')

        self.events[ev_name].append(ev_cb)

    def deregister_event(self, ev_name, ev_cb):
        """
        De\-register a callback from a previously registered event, after de\-registering the function won't be called anymore.
        
        :Parameters:
            - *ev_name* (string): Event name.
            - *ev_cv* (function): Event callback.
        
        """
        if ev_name not in self.events:
            raise Exception('Event {0} not found'.format(ev_name))
        if ev_cb not in self.events[ev_name]:
            raise Exception('Event not registered')

        self.events[ev_name].remove(ev_cb)

    def send_apdu(self, apdu):
        """
        Send an APDU message to the SPOT.
        The message should be an instance of :class:`pupyspot.APDU` class.
        
        :Parameters:
            - *apdu* (:class:`pupyspot.APDU`): APDU packet to send.
        
        """
        self._send_msg(apdu.app_id, apdu.data)
        if self.auto_send:
            self._send_queued_msg()

    def send_tpdu(self, tpdu):
        """
        Send a TDPU message to the SPOT.
        
        :Parameters:
            - *tdpu* (bytes array): TDPU message to send.
        
        """
        self.or_queue.append(tpdu)
        if self.auto_send:
            self._send_queued_msg()
        
    def wait_apdu(self, app_id, start, tmo = 0):
        """
        Wait an APDU reply.
        
        :Parameters:
            - *app_id* (byte): Application identifier.
            - *start* (integer): Start offset.
            - *tmo* (integer): timeout.
        
        """
        self.reply = None
        def _on_message(app, apdu, tpdu):
            if app != app_id:
                return
            if not apdu.startswith(start):
                return
            self.reply = apdu
            return

        self.register_event('incoming_message', _on_message)
        elapsed = 0
        now = time.time()
        while (self.reply is None) and (tmo==0 or elapsed < tmo):
            self.process_events()
            elapsed = time.time()-now
            time.sleep(0.001)

        if not self.reply:
            return None
        self.deregister_event('incoming_message', _on_message)
        return apdu_from_message(app_id, self.reply)

    def __del__(self):
        self.disconnect()

    ### HELPERS ###

    def _send_queued_msg(self):
        while True:
            msg  = None
            app  = None
            tpdu = None
            try:
                app, msg = self.o_queue.popleft()
                tpdu = pack('!HBB', len(msg)+4, app, 0) + msg
            except:
                try:
                    tpdu = self.or_queue.popleft()
                except:
                    break

            try:
                self.sock.send(tpdu)
                raise_event(self.ev_outgoing_msg, app, msg, tpdu)
            except:
                self.disconnect()

    def _recv_queued_msg(self):
        while True:
            apdu = None
            tpdu = None
            app  = None
            try:
                app, apdu, tpdu = self.i_queue.popleft()
            except:
                break

            # incoming message
            # check if ping and filter if needed
            if app == 0x80 and apdu[0] == '\x03' and not self.emit_ping:
                continue
            raise_event(self.ev_incoming_msg, app, apdu, tpdu)

    def _spot_io(self):
        """
        This method sends queued messages and reads incoming one.
        
        """
        def read_head():
            by_len_ = "\x00\x00"
            try:
                by_len_ = self.sock.recv(2)
            except:
                return None, 0
            len_ = unpack('!H', by_len_)[0]
            return by_len_, len_ - 2

        def check_polling():
            now = datetime.utcnow()
            if self.last_poll is None or \
               ( now - self.last_poll ) > self.poll_tmo:
               self.last_poll = now
               self._send_ping()

        if total_seconds(self.poll_tmo) > 0:
            check_polling()
        #send_queued_msg()

        tpdu_len, l = read_head()
        if l <= 0:
            return

        #print 'receiving %d bytes' % l

        tpdu = ""
        while l > len(tpdu):
            tpdu += self.sock.recv(l)
        app, ssk, apdu = unpack('!BB%ds' % (l-2), tpdu)
        tpdu = tpdu_len + tpdu
        #print app, ssk, repr(apdu)
        self.i_queue.append( (app, apdu, tpdu) )
        raise_event(self.ev_ienqueue_msg, app, apdu, tpdu)


    def _send_msg(self, app_id, apdu):
        """
        Send given APDU to SPOT.
        
        :Parameters:
            - *app_id* (byte): Application identifier.
            - *apdu* (:class:`pupyspot.APDU`): APDU packet to send.
        
        """
        self.o_queue.append( (app_id, apdu) )


    # HELPERS
    def _send_ping(self):
        """
        Sends polling command to SPOT.
        
        """
        apdu = pack('!B', 3)
        self.o_queue.append( (0, apdu) )

### --------------------- APDU MESSAGE CLASS ----------------------------- ###

def apdu_from_message(app_id, msg):
    """
    Opens an APDU to read an incoming message.
    
    :Parameters:
        - *app_id* (bytes array): Application identifier.
        - *msg* (bytes array): Incoming message.
    
    """
    apdu_ = APDU()
    apdu_.app_id = app_id
    if apdu_.open(msg):
        return apdu_
    return None

def create_apdu(app_id, cmd):
    """
    Creates new APDU to send a message.
    
    :Parameters:
        - *app_id* (bytes array): Application identifier.
        - *cmd* (bytes array): Command to send.

    """
    apdu_ = APDU()
    apdu_.app_id = app_id
    if apdu_.open():
        apdu_.put_array(cmd)
        return apdu_
    return None

def _hexdump(data, width, ascii):
    import string
    if not data:
        return '<empty>'
    io  = StringIO()
    io2 = StringIO()
    i   = 0
    for ch in data:
        if width > 0:
            if ascii and i and (i % width) == 0:
                io.write('    ')
                io.write(io2.getvalue())
                io.write('\n')
                io2.close()
                io2 = StringIO()
            elif i and (i % width) == 0:
                io.write('\n')

        io.write("%02X" % ord(ch))
        if ascii:
            if ch in string.printable and ch not in string.whitespace:
                io2.write(ch)
            else:
                io2.write('.')

        i += 1

    if width > 0 and ascii:
        miss = width - (i % width)
        io.write(' '*(miss*2+4))
        io.write(io2.getvalue())
    io2.close()
    hdump = io.getvalue()
    io.close()
    return hdump

def tpdu_hexdump(tpdu, width=-1, ascii=True):
    """
    Returns an hexdump of the given TPDU buffer.
    
    :Parameters:
        - *tdpu* (bytes array): TDPU message.
        - *width* (integer): length of TDPU message.
        - *ascii* (boolean): Is ASCII?.
        
    :Returns: A bytes array containing the hexdump of the TPDU buffer.   
            
    """
    return _hexdump(tpdu, width, ascii)

def apdu_hexdump(apdu, width=32, ascii=True):
    """
    Returns an hexdump of the given APDU object..
    
    :Parameters:
        - *apdu* (:class:`pupyspot.APDU`): APDU message.
        - *width* (integer): length of ADPU message.
        - *ascii* (boolean): Is ASCII?.
        
    :Returns: A bytes array containing the hexdump of the APDU buffer.        
    
    """
    return _hexdump(apdu.data, width, ascii)

class APDU(object):
    """
    This class represents an APDU message.
    Refer to "SPOT Programmers GUIDE" for further information on APDU and TPDU.
    
    """
    def __init__(self):
        """
        Creates an new APDU object.
        
        """
        self.app_id_ = None
        self.stream_ = None
        self.mode    = None

    @property
    def app_id(self):
        """
        Returns the APDU's Application ID.
        
        :Returns: An integer representing the Application ID. 
        
        """
        return self.app_id_

    @app_id.setter
    def app_id(self, value):
        """
        Sets the Application Id value.
        
        :Parameters:
            - *value* (integer): Value to set.
        
        """
        self.app_id_ = value

    @property
    def data(self):
        """
        Returns the APDU's data.
        
        :Returns: A bytes array with the APDU's data. 
        
        """
        if not self.stream_:
            return None
        return self.stream_.getvalue()

    @property
    def is_received(self):
        """ 
        :Returns: ``True`` if the message came from the SPOT, ``False`` otherwise.
        
        """
        return self.app_id_ & 0x80

    @property
    def cmd(self):
        """
        Returns the command as a tuple.
        
        :Returns: A tuple representing the command.
        
        """
        if not self.data:
            return None
        if (self.app_id_ & 0x7F) > 0:
            return (ord(self.data[0]), ord(self.data[1]))
        return (ord(self.data[0]), )

    def __eq__(self, other):
        """
        :Returns: ``True`` if other object is equals or compatible with this APDU object, ``False`` otherwise.
        
        """
        if type(other) == type(self):
            return other.data == self.data
        if type(other) != type(self.cmd):
            raise Exception('Expression not supported')
        return other == self.cmd

    def __len__(self):
        """
        Returns the total length of the APDU.
        
        """
        return len(self.data)

    def __del__(self):
        """
        Delete this APDU object.
        
        """
        self.close()

    ### API ###

    def open(self, source = None):
        try:
            self.mode  = 'r' if source else 'w'
            if not source:
                self.stream_ = StringIO()
            else:
                self.stream_ = StringIO(source)
                self.reset_read()
        except Exception as e:
            return False
        return True

    def close(self):
        if self.stream_:
            self.stream_.close()
            self.stream_ = None

    def reset_read(self):
        """
        Reset the APDU stream from the first byte after the command.
        
        """
        self.stream_.seek(len(self.cmd), os.SEEK_SET)

    def skip(self, n):
        """
        Skip n bytes ahead.
        
        :Parameters:
            - *n* (integer): Number of bytes to skip.
        
        """
        self._can_read(n)
        self.stream_.seek(n, os.SEEK_CUR)

    def pop_byte(self):
        """
        Pops a BYTE out of the buffer.
        
        """
        self._can_read(1)
        return unpack("!B", self.stream_.read(1))[0]

    def pop_word(self):
        """
        Pops a WORD (2 bytes) out of the buffer.
        
        :Returns: A tuple containing unpacked values.
        
        """
        self._can_read(2)
        return unpack("!H", self.stream_.read(2))[0]

    def pop_dword(self):
        """
        Pops a DWORD (4 bytes) out of the buffer.
        
        :Returns: A tuple containing unpacked values.
        
        """
        self._can_read(4)
        return unpack("!I", self.stream_.read(4))[0]

    def pop_array(self, len_):
        """
        Pops an array of the given length out of the buffer.

        :Parameters:
            - *len\_* (integer): Number of bytes to extract from buffer.
        
        :Returns: A bytes array containing ``len\_`` bytes.
        
        """
        
        self._can_read(len_)
        return self.stream_.read(len_)

    def put_byte(self, value):
        """
        Writes a BYTE into the buffer.
        
        :Parameters:
            - *value* (byte): Value to put into the buffer.
        
        :Returns: A reference to this object.
         
        """
        self._can_write(1)
        self.stream_.write(pack("!B", value))
        return self

    def put_word(self, value):
        """
        Writes a WORD into the buffer.
        
        :Parameters:
            - *value* (word): Value to put into the buffer.
        
        :Returns: A reference to this object.
         
        """
        self._can_write(2)
        self.stream_.write(pack("!H", value))
        return self

    def put_dword(self, value):
        """
        Writes a DWORD into the buffer.
        
        :Parameters:
            - *value* (dword): Value to put into the buffer.
        
        :Returns: A reference to this object.
         
        """
        self._can_write(4)
        self.stream_.write(pack("!I", value))
        return self

    def put_array(self, value):
        """
        Writes a DWORD into the buffer.
        
        :Parameters:
            - *value* (bytes array): Array to put into the buffer.
        
        :Returns: A reference to this object.
         
        """
        self._can_write(len(value))
        self.stream_.write(value)
        return self
        
    def hexdump(self):
        """
        :Returns: The hexdump of this APDU object.
        
        """
        return apdu_hexdump(self)

    def is_empty(self):
        """
        :Returns: A boolean that notify if this APDU object has been emptied.
        
        """
        return self.stream_.tell() == len(self)

    def remaining_length(self):
        """
        :Returns: The remaining number of data bytes.
        
        """
        return len(self) - self.stream_.tell()

    ### HELPERS ###

    def _can_read(self, n):
        if not self.stream_:
            raise Exception('Cannot read from empty APDU.')
        if 'r' not in self.mode:
            raise Exception('APDU is not open for reading.')
        if (self.stream_.tell() + n) > len(self):
            raise Exception('Reading beyond the end of the stream.')

    def _can_write(self, n):
        if not self.stream_:
            raise Exception('APDU is not open for writing.')
        if 'w' not in self.mode:
            raise Exception('APDU is not open for writing.')


if __name__ == '__main__':
    import apdudb

    spot = PupySPOT()

    def _my_send_apdu(apdu):
        spot.send_apdu(apdu)
        print("sent: {0}\n\x1b[36m{1}\x1b[0m".format(apdudb.get_apdu_name(apdu.app_id, apdu.cmd), apdu_hexdump(apdu)))

    def _send_login_msg(clientid, tmo):
        apdu = create_apdu(0, "\x01")
        apdu.put_byte(clientid)
        apdu.put_word(0)
        apdu.put_byte(tmo) # timeout in seconds
        _my_send_apdu(apdu)

    def on_online_status(is_online):
        print('is online: ' + str(is_online))
        if is_online:
            _send_login_msg(0x81, 30)

    def on_incoming_message(appid, msg, tpdu):
        apdu = apdu_from_message(appid, msg)
        print('received {0}:\n\x1b[31m{1}\x1b[0m\n\x1b[32m{2}\x1b[0m'.format(apdudb.get_apdu_name(apdu.app_id, apdu.cmd), tpdu_hexdump(tpdu), apdu_hexdump(apdu)))
        if apdu == (1,): # login
            logged_in = ( apdu.pop_byte() == 0 )
            print('Login? ' + str(logged_in))

    spot.register_event('online_status',    on_online_status   )
    spot.register_event('incoming_message', on_incoming_message)
    #spot.receive_ping_message = True
    ip_address = '192.168.0.101'
    try:
        message_string = 'IP Address [' + ip_address + ']: '
        if sys.version_info[0] < 3: new_ip = raw_input(message_string).strip()
        else: new_ip = input(message_string).strip()
    except EOFError:
        print(" <<< EOFError caught! >>>")
        sys.exit(1)
    except KeyboardInterrupt:
        print(" <<< Interrupted bu USER! >>>")
        sys.exit(1)
    except RuntimeError:
        sys.exit(1)
    if new_ip and len(new_ip) > 0:
        ip_address = new_ip
    spot.connect(ip_address, spot_tmo=10)

    run = True
    while run:
        try:
            spot.process_events()
            time.sleep(0.1)
        except KeyboardInterrupt:
            print('interrupted')
            run = False
        except Exception as ex:
            print(ex)

    spot.disconnect()
