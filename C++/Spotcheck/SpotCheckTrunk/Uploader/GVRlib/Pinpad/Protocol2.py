"""
.. module:: GVRlib.Pinpad.Protocol2 
   :platform: Unix, Windows
   :synopsis: Pinpad protocol management (Python 2).

Manage all command for Pinpad application using APDU object.

.. note::
    Aligned with Spot Programmers Guide v19.0.

"""

try:
	import pupyspot
except:
	from .. import pupyspot

# Default Variables
_apid = 1
_MIN_INPUT = 4
_MAX_INPUT = 12
_AUTOENTER = 0
_DELETE_ENABLE = 1
_TIMEOUT = 0
_BEEPECHO = 0
_ECHOMODE = 0
_WIN_ID = 1
_TAG_ID = 1
_INPUT_MODE = 1


def send_file_download_request_cmd(connection, file_type, file_id, file_size, crc32, auth_type, auth, description):
	"""
	Sends a **File Download Request** command to SPOT.
	When (``FileId``, ``FileType``) refer to an existing file, it is necessary to first delete the old file and then download the new one.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x00     |      0x01      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *file_type* (byte): 
			- b3-b0 = File types client specific (see the tables with files types at each application-APDU section on "Spot Programmers Guide"),  
		  	- b7-b4 = SPOT local application that owns this type of file: 
				- 0x00 = reserved, 
				- 0x01 = pinpad application, 
				- 0x02 = EMV application,
				- 0x03 = Setup/Download application, 
				- 0x04 = OPT application,
				- 0x05 = acquirer application, 
				- 0x06-0x0F = reserved for future applications. 
				
		- *file_id* (byte): File unique identification number.
		- *file_size* (dword): File size.
		- *crc32* (dword): CRC32 on whole file.
		- *auth_type* (byte): 
			- 0x00 = none,
			- 0x01 = PIN-resource authenticator (SPOT M3/M4 only),
			- 0x02 = NotPIN-resource authenticator (SPOT M3/M4 only),
			- 0x03 = PIN-resource RSA authenticator (SPOT M4/M5 only),
			- 0x04 = NotPIN-resource RSA authenticator (SPOT M4/M5 only).
		- *auth* (binary string): Optional secret authenticator. Content and size depend on ``auth_type``:
			- AuthenType = 0x00 It consists of four zeroes (0x00000000).
			- AuthenType = 0x01, 0x02 It consists in a four bytes ANSI 9.19 MAC calculated in secure room.
			- AuthenType = 0x03, 0x04 It consists in a 248 bytes RSA-certificate calculated in secure room.
		- *description* (binary string): Ascii description of the file.

	"""
	apdu = pupyspot.create_apdu(_apid, '\x00\x01')
	apdu.put_byte(file_type)
	apdu.put_byte(file_id)
	apdu.put_dword(file_size)
	apdu.put_dword(crc32)
	apdu.put_byte(auth_type)
	apdu.put_array(auth)
	apdu.put_array(description[0:16].ljust(16, '\x00'))
	connection.send_apdu(apdu)

def send_file_block_download_cmd(connection, blk_offset, blk_data):
	"""
	Sends a **File Block Download** command to SPOT.
	This command is used once or more times to send to SPOT the file presented with the previous command. 
	A block write error occurs when the block size exceeds the available space on the SPOT during a file block download.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x00     |      0x02      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *blk_offset* (dword): Current block position.
		- *blk_data* (binary string): Block data to be sent to SPOT.
		
	"""
	apdu = pupyspot.create_apdu(_apid, '\x00\x02')
	apdu.put_dword(blk_offset)
	apdu.put_word(len(blk_data))
	apdu.put_array(blk_data)
	connection.send_apdu(apdu)

def send_file_browse_cmd(connection, filetype):
	"""
	Sends a **File Browse** command to SPOT.
	This command asks to SPOT to list the FileIds currently loaded for a specific FileType and the available space.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x00     |      0x03      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *filetype* (byte): Types of files to be listed.
		
	"""
	apdu = pupyspot.create_apdu(_apid, '\x00\x03')
	apdu.put_byte(filetype)
	connection.send_apdu(apdu)

def send_file_delete_cmd(connection, filetype, fileid):
	"""
	Sends a **File delete** command to SPOT.
	This command asks to delete a specific file of a specific type. Log files can not be deleted.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x00     |      0x04      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *filetype* (byte): Types of files to be deleted.
		- *fileid* (byte): Id of file to be deleted.
		
	"""
	apdu = pupyspot.create_apdu(_apid, '\x00\x04')
	apdu.put_byte(filetype)
	apdu.put_byte(fileid)
	connection.send_apdu(apdu)

def send_file_upload_request_cmd(connection, filetype, fileid):
	"""
	Sends a **File Upload request** command to SPOT.
	This command asks to upload a specific file of a specific type.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x00     |      0x05      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *filetype* (byte): Types of files to be uploaded.
		- *fileid* (byte): Id of file to be uploaded.
		
	"""
	apdu = pupyspot.create_apdu(_apid, '\x00\x05')
	apdu.put_byte(filetype)
	apdu.put_byte(fileid)
	connection.send_apdu(apdu)
            
def send_file_block_upload_cmd(connection, blk_offset):
	"""
	Sends a **File Block Upload request** command to SPOT.
	This command is used once or more times to receive from SPOT the file specified with the previous command.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x00     |      0x06      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *blk_offset* (dword): Current block position.
		
	"""
	apdu = pupyspot.create_apdu(_apid, '\x00\x06')
	apdu.put_dword(blk_offset) 
	connection.send_apdu(apdu)


###Send Information Request Class Command Methods Definition###

def send_system_status(connection, device):
	"""
	Sends a **System status** request to SPOT.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x10     |      0x01      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *device* (dword): Device to be queried for status. This parameter is a bits mask that allows asking information about one or more SPOT sub systems (internal module or device driver).
		
	"""	
	apdu = pupyspot.create_apdu(_apid, '\x10\x01')
	apdu.put_byte(device)
	connection.send_apdu(apdu)

def send_hardware_configuration(connection, item_list):
	"""
	Sends an **Hardware configuration** request to SPOT.
	This command asks info about one or more hardware items contained in SPOT.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x10     |      0x02      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *item_list* (binary string): If count is not zero, this is a list of hardware Items to be queried.
		
	"""	
	count = len(item_list)
	apdu = pupyspot.create_apdu(_apid, '\x10\x02')
	if item_list is None:
		apdu.put_byte(0x00)
	else:
		apdu.put_byte(count)
		for item_code in item_list:
			apdu.put_byte(item_code)
	connection.send_apdu(apdu)

def send_software_configuration(self, item_list):
	"""
	Sends a **Software configuration** request to SPOT.
	It asks info about one or more software items contained in SPOT.
		
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x10     |      0x03      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *item_list* (binary string): If count is not zero, this is a list of software Items to be queried.
		
	"""
	count = len(item_list)
	apdu = pupyspot.create_apdu(_apid, '\x10\x03')
	if item_list is None:
		apdu.put_byte(0x00)
	else:
		apdu.put_byte(count)
		for item_code in item_list:
			apdu.put_byte(item_code)
	connection.send_apdu(apdu)

def send_diagnostic_info_(connection, item_list):
	"""
	Sends a **Diagnostic Info** request to SPOT.
	It asks the value of one or more diagnostic items managed by SPOT. These are counters for specific operations performed on SPOT.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x10     |      0x04      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *item_list* (binary string): This is a list of diagnostic Items to be queried. SPOT will answer anyway with all defined diagnostic items, this parameter have this structure for future use and to be in line with the previous commands.
		
	"""
	count = len(item_list)
	apdu = pupyspot.create_apdu(_apid, '\x10\x04')
	if item_list is None:
		apdu.put_byte(0x00)
	else:
		apdu.put_byte(count)
		for item_code in item_list:
			apdu.put_byte(item_code)
	connection.send_apdu(apdu)


###Date and time management###

def send_set_date_time(connection, date_time):
	"""
	Sends a **Set Date-Time** command to SPOT.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x10     |      0x05      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *date_time* (dword): Number of seconds since 1/1/1970.
		
	"""
	apdu = pupyspot.create_apdu(_apid, '\x10\x05')
	apdu.put_dword(date_time)
	connection.send_apdu(apdu)


def send_get_date_time(connection):
	"""
    Sends a **Get Date-Time** request to SPOT.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x10     |      0x06      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x10\x06')
	connection.send_apdu(apdu)


###Send Card Reader Class Command Methods Definition###

def send_cr_enable_cmd(connection, card_types = []):
	"""
	Sends a **Card Reader - Enable** command to SPOT.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x01      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *card_types* (binary string): These optional bytes are used for supporting up to 16 specific protocols for Memory Cards. In particular, the support for memory cards is activated by feeding the system with a list of up to 16 one-byte codes (CardTypes parameter) that identify univocally the card, currently according to the following list:
			- 0x30 = SLE 4442,
			- 0x31 = SLE 4432,
			- 0x32 = SLE 4428,
			- 0x33 = SLE 4418 (supported only by non-motorized readers),
			- 0x37 = GPM 896,
			- 0x38 = PRIMEFLEX MEMBER 1K.
		If more than one byte is provided in the CardTypes parameter then the reader attempts to open the memory card using the protocol type indicated by the first byte of the list and, in case of failure, proceeds trying all the following protocols (in the order provided by the list) until a reading is successful or the list is walked through to the end.
				
	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x01')
	for card_code in card_types: #optional for OMRON V2BF card reader
		apdu.put_byte(card_code)
	connection.send_apdu(apdu)

def send_cr_disable_cmd(connection):
	"""
	Sends a **Card Reader - Disable** command to SPOT.
	After this command the reader no longer accept a card at its inlet. If a card it is inside it is ejected/unlocked. LED is turned off (if any).
	For Panasonic reader this command is required only to unlock an ICC. Magnetic-only cards can be freely removed without disable command.	
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x02      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
				
	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x02')
	connection.send_apdu(apdu)

def send_cr_write_cmd(connection, dest_bit_mask, data):
	"""
	Sends a **Card Reader - Write** command to SPOT.
	This command allows to write on an already activated ICC.	
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x04      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *dest_bit_mask* (byte): Mask to specify the destination of data buffer:
			- 0x01= write buffer to track1,
			- 0x02= write buffer to track2,
			- 0x04= write buffer to track3,
			- 0x10= send buffer to ICC.
		- *data* (binary string): Data buffer.
				
	"""
	data_size = len(data)
	apdu = pupyspot.create_apdu(_apid, '\x20\x04')
	apdu.put_byte(0x01) # reserved byte
	apdu.put_byte(dest_bit_mask)
	apdu.put_word(data_size)
	apdu.put_array(data)
	connection.send_apdu(apdu)

def send_cr_chip_reset_cmd(connection):
	"""
	Sends a **Card Reader - Chip Reset** command to SPOT. 
	This command has effects only if an ICC is present inside the reader.
	It turns the power off and then on, causing the ICC to generate again the ATR data. These will be returned immediately to the client without ISO tracks data.
	
	.. note::
	    This command is mandatory for Panasonic reader.
		
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x07      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
				
	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x07')
	connection.send_apdu(apdu)

def send_cr_enable_sankyo_reader_cmd(connection, pre_head, track_bitmask):
	"""
	Sends a **Card Reader - Enable Sankyo Reader** command to SPOT. 

	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x08      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *pre_head* (byte): This parameter will have effect only on card readers that support it (namely the Sankyo card reader) and will be otherwise ignored:
			- 0x30 = off (the command is identical to *Card Reader - Enable*),
			- 0x31 = on (only cards with magnetic band on the proper side are allowed to open the, shutter to be moved inside the reader).
		- *track_bitmask* (byte): The Track Bitmask allows to selectively suppress unwanted tracks, whose data will not appear in the answer. In the answer message a dropped track will be marked with a dedicated status flag (0xff) in order to distinguish the case when the track has been voluntarily suppressed from that when it is corrupted or not present. Bitmask representing date to hide:
			- 0x01 = track 1,
			- 0x02 = track 2,
			- 0x04 = track 3,
			- 0x08 = ATR.
				
	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x08')
	apdu.put_byte(pre_head)
	apdu.put_byte(track_bitmask)
	connection.send_apdu(apdu)

def send_get_emvcard_signature_cmd(connection, num_aid, aid_list):
	"""
	Sends a **Card Reader - Get EMV Signature request** command to SPOT. 
	This command has effects only if an ICC is present inside the reader; If NumAIDs is empty the Application Identifier list is built reading the EMV table SCHEMES.tab initialised during the configuration of the EMV application.
	If NumAID is filled, the Application Identifier list is built reading AID fields. Pinpad will busy during processing of this command since a communication between Pinpad and Card Reader will start; in the meantime some unsolicited messages could be issued by Pinpad to inform connected clients at every state change. 
	When the procedure will be finished a dedicated answer will be sent to the sender client.

	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x20     |      0x09      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *num_aid* (byte): It indicates how many AIDs are reported in the following fields (It is an optional field).
		- *aid_list* (list): Application Identifier Data in hexadecimal format.
				
	"""
	apdu = pupyspot.create_apdu(_apid, '\x20\x09')
	apdu.put_byte(num_aid)
	for aid in aid_list:
		apdu.put_byte(len(aid))
		apdu.put_array(aid)
	connection.send_apdu(apdu)


###Display Class Command Methods Definition###

def send_current_message_file_selection(connection, normal_msg, clear_input_msg, pin_input_msg):
	"""
	Sends a **Display - Current MessageFiles selection** command to SPOT. 
    This command allows to select the current message file to be then referred by the following display-commands using the "%" special char .
    The message file is selected by specifying its ``FileId``. The ``FileId`` needs to refer to an already downloaded file with ``FileType``=0x10 and eventually with secret authenticator. 
    Instead of a single FileId parameters a mask of the positions is used to indicate also the type of secret authenticator that the file has. If one of the three mask positions contain a not existent FileId value or a not valid authenticator, an error is returned.

    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x01      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *normal_msg* (byte): ``FileId`` for a msg file without any secure authenticator or zero.
        - *clear_input_msg* (byte): ``FileId`` for a msg file with a secret authenticator to allow the contained messages to be used also when a clear text input is required, or zero.
        - *pin_input_msg* (byte): ``FileId``  for a msg file with a secret authenticator to allow the contained messages to be used also when a PIN input is required, or zero.
        
            	
	"""
	apdu = pupyspot.create_apdu(_apid, '\x30\x01')
	apdu.put_byte(normal_msg)
	apdu.put_byte(clear_input_msg)
	apdu.put_byte(pin_input_msg)
	connection.send_apdu(apdu)

def send_create_window_from_resource_cmd(connection, winid, resid, allign = 0xFF, posX = 0x00, posY = 0x00):
	"""
	Sends a **Display - Create window from resource file** command to SPOT.
	This command creates a window using a previously downloaded template file. 
	This type of file is, actually a dump of CreateWindow command with proper *TAGareas* parameters but without ``WinId``.
	At the moment the window is created from the file, the ``WinId`` is assigned and it also is possible to redefine some ``TAGarea(0)`` parameters.
	This allows to create multiple windows (with different ``WinId``) with different positioning using the same template file.
	When SPOT is in secure input mode all the display commands need to refer to a Window created from a previously downloaded template file with the proper secret authenticator.
	
	+---------------+----------------+
	| Command class | Command type   |
	+===============+================+
	|      0x30     |      0x0A      |
	+---------------+----------------+
	
	:Parameters:
		- *connection* (PupySPOT): Object managing spot connection.
		- *winid* (byte): ``WinId`` to be assigned to the currently created window.
		- *allign* (byte): Window align override (respect to ``TAGarea(0)`` on template file):
			- Bit 0/1 0=left, 1=center, 2=right,
			- Bit 2/3 0=top, 1=middle, 2=bottom,
			- Bit 4/7 0, 
			- 0xFF means use the original align parameter with no overriding,
		- *posX* (byte, *posY* (byte):  New origin in pixels respect to what specified on file.
		
	"""
	apdu = pupyspot.create_apdu(_apid, '\x30\x0A')
	apdu.put_byte(winid) 
	apdu.put_byte(resid) 
	apdu.put_byte(allign)
	apdu.put_word(posX)
	apdu.put_word(posY)
	connection.send_apdu(apdu)

def send_destroy_window_cmd(connection, win_id, tags):
	"""
    Sends a **Display - Destroy window** command to SPOT.
    This command allows to destroy a whole window or just some of its contained TAG areas.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x04      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *win_id* (byte): Identifier of the window.
        - *tags* (byte): Tags unique ids within this window.
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x30\x04')
	apdu.put_byte(win_id) #WinId
	apdu.put_byte(len(tags)) #NumTags
	for tag in tags:
		apdu.put_byte(tag) #TagId
	connection.send_apdu(apdu)

def send_display_string_cmd(connection, win_id, output_list):
	"""
    Sends a **Display - Strings** command to SPOT.
    This command needs to refer to one or more *Text TAGareas* in an already displayed window.
    The string parameters can be a text to be displayed or a "%label" to be retrieved in currently selected message file.
    If this command is sent when SPOT is in a secure input mode only "%label" referring to a proper authenticated message files are allowed. Moreover the referred window and its resource must have been previouslydownloaded with the proper secret authenticator. Otherwise the command output will result not visible.
    If the specified *TAGareas* contains a default text defined at window creation time, it will result overwritten by the currently passed string.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x05      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *win_id* (byte): Identifier of destination window.
        - *output_list* (list): list of "Text TAGareas" to display.
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x30\x05')
	apdu.put_byte(win_id) #WinId
	apdu.put_byte(len(output_list)) #NumOutputStrings
	for tag in output_list:
		apdu.put_byte(tag[0]) #TagId
		apdu.put_byte(tag[1]) #Align
		apdu.put_word(tag[2]) #POSX_in_TAG
		apdu.put_word(tag[3]) #POSY_in_TAG
		apdu.put_word(len(tag[4]))
		apdu.put_array(tag[4]) # String
	connection.send_apdu(apdu)
	
def send_display_string_lite(connection, string_tag, win_id, tag_id):
	"""
    Wrapper function of ``send_display_string_cmd`` to display a single string.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *string_tag* (byte): String to display.
        - *win_id* (byte): Identifier of destination window.
        - *tag_id* (byte): Identifier of the string within ``win_id``-window.
        
    """
	send_display_string_cmd(connection, win_id, [(tag_id, 0xff, 0x00, 0x00, string_tag)])
	
def send_display_image(connection, win_id, output_list):
	"""
    Sends a **Display - Images** command to SPOT.
    This command needs to refer to one or more *Image TAGareas* in an already displayed window.
    The image parameter needs to be the FileId of an already downloaded image file.
    If this command is sent when SPOT is in a secure input mode only FileId referring to a proper authenticated image files are allowed.
    Moreover the referred window and its resource must have been previously downloaded with the proper secret authenticator; otherwise the command output will result not visible.
    If the specified *TAGareas* contains a default image file defined at window creation time, it will result overwritten by the currently specified FileId.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x06      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *win_id* (byte): Identifier of destination window.
        - *output_list* (list): list of "Image TAGareas" to display.
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x30\x06')
	apdu.put_byte(win_id) #WinId
	apdu.put_byte(len(output_list)) #NumOutputStrings
	for tag in output_list:
		apdu.put_byte(tag[0]) #TagId
		apdu.put_byte(tag[1]) #Align
		apdu.put_word(tag[2]) #POSX_in_TAG
		apdu.put_word(tag[3]) #POSY_in_TAG
		apdu.put_byte(tag[4]) # Image FileId
		connection.send_apdu(apdu)

def send_display_image_lite(connection, image_id, win_id, tag_id):
	"""
    Wrapper function of ``send_display_image`` to display a single image.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *image_tag* (byte): image to display.
        - *win_id* (byte): Identifier of destination window.
        - *tag_id* (byte): Identifier of the image within ``win_id``-window.
        
    """
	send_display_image(connection, win_id, [(tag_id, 0x00, 0x00, 0x00, image_id)])
			
def send_show_window(connection,winid): # TODO: minimal implementation, to be improved 
	"""
    Sends a **Display - Show windows** command to SPOT.
    This command displays a previously created window referred by its WindowId. During this displaying is possible to select which TAGareas, within of the originally created set, must be visible and their z-order.
    That is, since TAGareas in a window may also overlaps each other, it is possible to define the foreground priority of the various TAGareas.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x07      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *win_id* (byte): windows identifier.
        
    """	
	apdu = pupyspot.create_apdu(_apid, '\x30\x07')
	apdu.put_byte(winid)
	apdu.put_byte(1)
	apdu.put_byte(0)
	apdu.put_byte(0)
	connection.send_apdu(apdu)
            
def send_hide_window(connection,winid): # TODO: minimal implementation, to be improved 
	"""
    Sends a **Display - Hide windows** command to SPOT.
    This command hides a previously created and shown window referred by its WindowId. 
    During this hiding is possible to select which TAGareas, within of the originally created set, must become invisible.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x08      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *win_id* (byte): windows identifier.
        
    """	
	apdu = pupyspot.create_apdu(_apid, '\x30\x08')
	apdu.put_byte(winid)
	apdu.put_byte(1)
	apdu.put_byte(0)
	connection.send_apdu(apdu)
            
def send_beep(connection,time_to_buzz):  
	"""
    Sends a **Display - Beep** command to SPOT.
    This command activated the buzzer for the specified time.
    Depending on SPOT HW configuration the buzzer can be: inside the reader module, on a cabled external board, or not present.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x09      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *time_to_buzz* (word): Time to buzz in milliseconds.
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x30\x09')
	apdu.put_word(time_to_buzz)
	connection.send_apdu(apdu)

def send_get_display_status(connection,win_id):  
	"""
    Sends a **Display - Get display status** request to SPOT.
    Get info about the displaying status of the currently available windows.
    
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x0F      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *win_id* (byte): Optional parameter. If present the command output is the displaying status of the window identified by ``win_id``.
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x30\x0F')
	apdu.put_byte(win_id)
	connection.send_apdu(apdu)
	
def send_select_language(connection, language):  
	"""
    Sends a **Display - Select language** command to SPOT.
    This command is reserved for internal application only and is used to set the language of the string retrieved in currently selected message file.
    It overrides the default language of the currently selected message file and the language set in the label string.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x10      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *language* (word): ISO639-1 two-letters code for the language. Examples:
			- IT = 0x4954 = Italian EN = 0x454E = English,
			- DE = 0x4445 = Deutsch ES = 0x4553 = Spanish,
			- FR = 0x4652 = French.
			
    """
	apdu = pupyspot.create_apdu(_apid, '\x30\x10')
	apdu.put_array(language)
	self.spot.send_apdu(apdu)
	
def send_secure_media_connector_switching(connection, mode):	
	"""
    Sends a **Display - Secure Media Connector switching** command to SPOT.
    This command is available only on SPOT M4 and M5.
    It switches the display video-input between internal SPOT CPU and the externally connected PC video-output.
    Switch externally the display cause the keypad to be disabled. 
    The command fails in case a SMC switch to extern is required while an input command is in progress.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x30     |      0x11      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *mode* (byte): 
            - I=0x49=Internal SPOT CPU (default; Secure in SPOT M5),
            - E=0x45=External PC connected on SMC,
            - F=0x46=Internal SPOT CPU (Not secure; only on SPOT M5).			
    """
	apdu = pupyspot.create_apdu(_apid, '\x30\x11')
	apdu.put_array(mode)
	connection.send_apdu(apdu)


###Send Keyboard and security Class Command Methods Definition###

def send_customer_input_enable_cmd(connection, min_input=_MIN_INPUT, max_input=_MAX_INPUT, autoenter=_AUTOENTER, delete_enable=_DELETE_ENABLE, timeout=_TIMEOUT, beepecho=_BEEPECHO, echo_mode=_ECHOMODE, win_id=_WIN_ID, tag_id=_TAG_ID, input_mode=_INPUT_MODE, **kargs):
	"""
    Sends a **Keyboard and Security - Customer input enable** command to SPOT.
    This command sets the keypad to capture the specified customer input.
    This could be an encrypted PIN that causes the secure input mode. Alternatively, it could be a clear text input mode (NotPIN data capture).
    I this case the secure input mode is triggered automatically when more than 3-digits are required or for any digits number in case auto-enter is used.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x01      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *min_input* (byte), *max_input* (byte): See "Spot Programmers Guide".
        - *autoenter* (byte): 0 = No, otherwise = Yes. If asserted, when a digit string reaches MaxInputLength it is automatically returned, otherwise the OK button pressure is required.
        - *delete_enable* (byte): 0 = No, otherwise = Yes. This works only when a PIN is required. In case of ``Not PIN`` the ``AlphaEnable`` parameter is rather used. If asserted, ``F4`` or ``DELETE`` yellow button (Depending on keypad layout) are no longer returned but rather used as backspace to delete single char a time to correct wrong typing.
        - *timeout* (word): 0 = infinite wait, n = seconds to wait for customer input completion. For PIN input an infinite timeout is not permitted. In this case the maximum timeout is 255 seconds (default value if n > 0xFF).
        - *beepecho* (byte): 0 = No, otherwise = Yes (always the same tone for all the digits)'
        - *echo_mode* (byte): 0 = no echo, otherwise = echo, > 0x20 = char mask code.
        - *win_id* (byte): Window identifier of a properly authenticated window that will contain echoes.
        - *tag_id* (byte): This individuates echo area for displaying echoes. It is the ``tagID`` of a tag area contained in a properly authenticated window identified by ``WinforInput``.
        - *input_mode* (byte): 
            - 0x00 = NotPIN,
            - 0x01 = PIN.
        - *\*\*kargs* (list):  See "Spot Programmers Guide".          
    """
	apdu = pupyspot.create_apdu(_apid, '\x40\x01')
	apdu.put_byte(min_input)                   #min input
	apdu.put_byte(max_input)                   #max input
	apdu.put_byte(autoenter)                   #Autoenter
	apdu.put_byte(delete_enable)               #delete enable
	apdu.put_word(timeout)                     #timeout
	apdu.put_byte(beepecho)                    #beepecho
	apdu.put_byte(echo_mode)                   #echomode
	apdu.put_byte(win_id)                      #winforinput
	apdu.put_byte(tag_id)                      #Tagid
	apdu.put_byte(input_mode)                  #input mode
	if input_mode == 0 or input_mode == 0x00:
		apdu.put_byte(kargs['alpha_enable'])        #alpha enable
		apdu.put_byte(kargs['input_format'])        #input format
		apdu.put_byte(kargs['spec'])                #spec
	elif input_mode == 0x01:
		apdu.put_byte(kargs['remember'])           #remember PIN value = 0-1-2
		apdu.put_byte(kargs['enctype'])            #enc type value = 0-1-2-3-4
		apdu.put_byte(kargs['encmode'])            #enc mode value = 0-1-2-3
		apdu.put_byte(kargs['pin_format'])         #format ansi pin block
		apdu.put_byte(kargs['keyid'])
		try:# card data could be not present
			apdu.put_byte(len(kargs['card_data']))      #card data size
			apdu.put_array(kargs['card_data'])
		except:
			apdu.put_byte(0)      #card data size = 0
		if 'working_key' in kargs:
			apdu.put_array(kargs['working_key'])
	else:
		raise ValueError('Unknown input_mode 0x{0:0x}'.format(input_mode))
	connection.send_apdu(apdu)  

def send_customer_input_abort_cmd(connection):
	"""
    Sends a **Keyboard and Security - Customer input abort** command to SPOT.
    This command causes the customer input to be aborted.
    
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

def send_app_key_download(connection,key_id,key_use,transport_key,key_type,key_data):
	"""
    Sends a **Keyboard and Security - Application key download** command to SPOT.
    This command downloads a new key to one of the available key-slots. RSA keys are loaded in clear text since, normally, they are not allowed with ``KeyUse`` = PIN key.
    The only exception is when the PIN is captured-and passed to the ICC within the ``PIN VERIFY`` command (public key encryption using random pattern generated by ``GET_CHALLENGE`` command). 
    But in any case, it is not possible to ask to this interface to capture a PIN and return it with RSA encryption.
    
    .. note::
        This command is not supported in *SPOT M5*.
        On *SPOT M3* versions greater than 03.02 after the TR-31 switch activation, this command is available only for asymmetric keys injection.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x04      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *key_id* (byte): Slot ID of Master Key.
        - *key_use* (Enums. KeyUse):
            - 0x01 = PIN key,
            - 0x02 = Data key
            - 0x03 = Transport key
            - 0x04 = MAC key (implemented only in PINPAD version destined to Canadian market)
            - 0x05 = OFFLINE PIN key
            - 0x06 = TR-31 KBPK Since version 03.02 these keys are no longer allowed to be downloaded with this command.
        - *transport_key* (byte): 0 = use MTK (factory loaded) or \>0 previously loaded DES or 3DES transport key. On PINPAD 03.02 the MTK can be use as long as the terminal is not operated in TR-31 mode. 
        - *key_type* (Enum.KeyType): Refer to appendix for key types. DUKPT and DUKPT2002 keys need to be loaded with ``KeyUse`` = 0x01. TR-31 KBPK (SPOT M3c 2.1 only) and transport keys need to be loaded with ``KeyType`` = 0x04 = 3DES.
        - *key_data* (byte): The encrypted key data contain also the intended values of ``key_use`` and ``key_type`` (that were inserted at factory-creation time). If these values don't match the ones specified as command parameters the key download fails. The maximum key data length accepted is 576 bytes.
        
    """    
	apdu = pupyspot.create_apdu(_apid, '\x40\x04')
	apdu.put_byte(key_id) 
	apdu.put_byte(key_use)
	apdu.put_byte(transport_key)	
	apdu.put_byte(key_type)
	apdu.put_array(key_data)
	connection.send_apdu(apdu)
	
def send_data_encryption(connection, keyid, versus, data, mode, working_key = ''):
	"""
    Sends a **Keyboard and Security - Data Encryption** command to SPOT.
    This command encrypts or decrypts or calculates MAC on the passed data.
    The encryption used depends on the type of the specified key; in case DES/3DES CBC or ANSI9.19 the initial vector (IV) is zeroed. 
    ``key_id`` must have KeyUse set for Data Encryption.  ``key_id`` referring to Transport or PIN are not allowed here.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x03      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *key_id* (byte):Slot ID of Master Key.
        - *versus* (Enums.Versus): 
            - 0x01 = DES/3DES ECB encrypt (data key only),
            - 0x02 = DES/3DES ECB decrypt or RSA encrypt/decrypt (data key only),
            - 0x03 = DES/3DES CBC encrypt (data key only),
            - 0x04 = DES/3DES CBC decrypt (data key only),
            - 0x05 = ANSI x9.19 MAC calculation,
            - 0x06 = DUKPT1992 MAC (only with a DUKPT1992),
            - 0x07 = DES/3DES OFB encrypt (data key only),
            - 0x08 = DES/3DES MAB (Message Authentication Block) for Australia,
            - 0x09 = reserved,
            - 0x0A = DUKPT x9.19 MAC IN variant for Nordic,
            - 0x0B = DUKPT x9.19 MAC OUT variant for Nordic,
            - 0x0C = 3DES-CBC ENCRYPTION with DUKPT (with mask3 as defined in ref.[10]),
            - 0x0D = 3DES-CBC DECRYPTION with DUKPT (with mask4 as defined in ref.[10]),
            - 0x0E = x9.19 MAC using DES or 3DES DUKPT variated according to DUKPT.
        - *data* (binary string): Data buffer. In case of DUKPT1992 MAC this buffer must be filled according to ANSI x9.24 specification.
        - *mode* (byte): 
            - 0x00 = Use ``key_id`` (mandatory for DUKPT and DUKPT2002),
            - 0x01 = Use the passed WorkingKey (mandatory in SPOT M3c-2.1, SPOT M3c-Canada and SPOT M4 since fixed key PIN encryption not supported).
        - *working_key* (binary string): Key to be used by the command. Working key is ECB encrypted under ``key_id``. Encrypted DES key must be passed here by doubling its 8 bytes sequence (this since a 3DES with ``KeyLeft`` = ``KeyRight`` is the same as a DES key). If ``ode`` = 0x00 these 16 bytes are not sent. ``WorkingKey[]`` buffer is normally 16 bytes length since it contains a 3DES data key ECBencrypted under ``key_id``.
        
    """
	data_len = len(data)
	apdu = pupyspot.create_apdu(_apid, '\x40\x03')
	apdu.put_byte(keyid)
	apdu.put_byte(versus) 
	apdu.put_word(len(data)) 
	apdu.put_array(data) 
	apdu.put_byte(mode)
	if len(working_key) > 0 : apdu.put_array(working_key) 
	connection.send_apdu(apdu)

def send_application_key_downloadtr31(connection, keyid, transport_keyid, key_data):
	"""
    Sends a **Keyboard and Security - Application key download** command in TR31 format to SPOT.
    It tries to load the passed TR31 key data into the required keyId using the specified Transport key as TR31 Key Block Protection Key
    
        
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x0D      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *keyid* (byte): Slot ID of Master Key.
        - *transport_keyid* (byte): 0 = use MTK (factory loaded) or >0 previously loaded transport key to be here used as *Key Block Protection Key* (KBPK).
        - *key_data* (binary string): data buffer (TR31 format).
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x40\x0D')
	apdu.put_byte(keyid) # keyid
	apdu.put_byte(transport_keyid) # transport keyid
	apdu.put_word(len(key_data))
	apdu.put_array(key_data) # key data #TODO: need a way to compose easily the key_data block
	connection.send_apdu(apdu)

def send_key_derivation(connection, keyid, derivation_type = 1):
	"""
    Sends a **Keyboard and Security - Key derivation** command to SPOT.
    This command generates next derived session key or destroys the session key.
        
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x05      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *keyid* (byte): Slot ID of Master Key. It must refer to a DUKPT1992 or a DUKPT2002 key.
        - *derivation_type* (byte):
            - 0x00 = Reserved,
            - 0x01 = DUKPT1992 or DUKPT2002,
            - 0x02 = Reserved.
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x40\x05')
	apdu.put_byte(keyid) # keyid
	apdu.put_byte(derivation_type)	# derivation type
	connection.send_apdu(apdu)

def send_key_status(connection):
	"""
    Sends a **Keyboard and Security - Key status** command to SPOT.
    This command allows to get information about loaded keys.  
        
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x06      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
 
    """
	apdu = pupyspot.create_apdu(_apid, '\x40\x06')
	connection.send_apdu(apdu)

def send_mac_verification(connection, key_id, versus, data, mac):
	"""
    Sends a **Keyboard and Security - MAC verification** command to SPOT.
    It allows to verify if the MAC calculated on the passed data buffer matches the passed MAC.
    The referred KeyId must have ``key_use`` = ``data`` (no PIN keys or transport are allowed for this function).
        
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x05      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *keyid* (byte): Slot ID of Master Key.
        - *versus* (Enums.Versus): 
            - 0x05 = ANSI x9.19 MAC; it works with DUKPT2002 keys. DES/3DES keys require ``key_use`` = ``data`` in SPOT M3,
            - 0x06 = DUKPT1992 MAC; it works only with a DUKPT1992 keys.
        - *data* (binary string): Source data.
        - *mac* (binary string): MAC to be matched.
        
    """
	data_len = len(data)
	mac_len = len(mac)
	apdu = pupyspot.create_apdu(_apid, '\x40\x07')
	apdu.put_byte(key_id)
	apdu.put_byte(versus) 
	apdu.put_word(data_len) 
	apdu.put_array(data) 
	apdu.put_word(mac_len) 
	apdu.put_array(mac) 
	connection.send_apdu(apdu)
	
def send_get_random_byte(connection, num_byte):
	"""
    Sends a **Keyboard and Security - Get random byte** command to SPOT.
    This command asks for a block of randomly generated bytes. 
    The random number generator of the crypto-processor is used to obtain random data.
        
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x08      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *num_byte* (byte): Number of requested bytes. 
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x40\x08')
	apdu.put_word(num_byte) 
	connection.send_apdu(apdu)

def send_multiblock_encryption(connection, key_id, flag, versus, data, chaining_data, working_key_mode, working_key):
	"""
    Sends a **Keyboard and Security - Multiblock encryption** command to SPOT.
        
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x13      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *key_id* (byte): Slot ID of Master Key.
        - *flag* (Enums.Flags):  Multi-block operation.
        - *versus* (Enums.Versus): Operation required.
        - *data*  (binary string):  Data block on which the operation is performed
        - *chainingData* (binary string): Initial vector IV on fiirst block or Chaining data block as replied in the answer to the last multiblock op
        - *working_key_mode* (Enums.WKMode): Working Key format.
        - *working_key* (BinaryString): Working Key value encrypted with Master Key. 
        
    """        
	apdu = pupyspot.create_apdu(_apid, '\x40\x13')
	apdu.put_byte(key_id)
	apdu.put_byte(flag)
	apdu.put_byte(versus)
	apdu.put_word(len(data))
	apdu.put_array(data)
	apdu.put_word(len(chaining_data))
	apdu.put_array(chaining_data)
	apdu.put_byte(working_key_mode)
	apdu.put_word(len(working_key))
	apdu.put_array(working_key)
	connection.send_apdu(apdu)

def send_multiblock_encrypt(connection, KeyId=None, WorkingKeyMode=None, WorkingKey=None, Flags=None, Versus=None, Data=None, ChainingData=None):
	"""
    Sends a **Keyboard and Security - Multiblock encryption** command to SPOT.
        
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x13      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *KeyId* (byte): Slot ID of Master Key.
        - *WorkingKeyMode* (Enums.WKMode): Working Key format.
        - *WorkingKey* (BinaryString): Working Key value encrypted with Master Key.
        - *Flags* (Enums.Flags):  Multi-block operation.
        - *Versus* (Enums.Versus): Operation required.
        - *Data*  (binary string):  Data block on which the operation is performed
        - *chainingData* (binary string): Initial vector IV on fiirst block or Chaining data block as replied in the answer to the last multiblock op
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x40\x15')
	apdu.put_byte(KeyId)
	apdu.put_byte(Flags.value)
	apdu.put_byte(Versus.value)
	# Data
	if Data is None:
		apdu.put_word(0)
	else:
		apdu.put_word(len(Data))
		apdu.put_array(Data)
	# Chaining data
	if ChainingData is None:
		apdu.put_word(0)
	else:
		apdu.put_word(len(ChainingData))
		apdu.put_array(ChainingData)
	# Working key
	if WorkingKeyMode is not None: 
		apdu.put_byte(WorkingKeyMode.value)
		if WorkingKey is None:
			apdu.put_word(0)
		else:
			apdu.put_word(len(WorkingKey))
			apdu.put_array(WorkingKey)
	connection.send_apdu(apdu)

def send_multiblock_verification(connection, KeyId=None, WorkingKeyMode=None, WorkingKey=None, Flags=None, Versus=None, Data=None, ChainingData=None, MACData=None):
        """
    Sends a **Keyboard and Security - Multiblock verification** command to SPOT.
        
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x40     |      0x16      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *KeyId* (byte): Slot ID of Master Key.
        - *WorkingKeyMode* (Enums.WKMode): Working Key format.
        - *WorkingKey* (BinaryString): Working Key value encrypted with Master Key.
        - *Flags* (Enums.Flags):  Multi-block operation.
        - *Versus* (Enums.Versus): Operation required.
        - *Data*  (binary string):  Data block on which the operation is performed
        - *chainingData* (binary string): Initial vector IV on fiirst block or Chaining data block as replied in the answer to the last multiblock op
        -  MACData (binary string):  MAC Data buffer to compare.
        
    """ 
	apdu = pupyspot.create_apdu(_apid, '\x40\x16')
	apdu.put_byte(KeyId)
	apdu.put_byte(Flags.value)
	apdu.put_byte(Versus.value)
	# Data
	if Data is None:
		apdu.put_word(0)
	else:
		apdu.put_word(len(Data))
		apdu.put_array(Data)
	# MACData
	if MACData is not None:
		apdu.put_word(len(MACData))
		apdu.put_array(MACData)
	# Chaining data
	if ChainingData is None:
		apdu.put_word(0)
	else:
		apdu.put_word(len(ChainingData))
		apdu.put_array(ChainingData)
	# Working key
	if WorkingKeyMode is not None: 
		apdu.put_byte(WorkingKeyMode.value)
		if WorkingKey is None:
			apdu.put_word(0)
		else:
			apdu.put_word(len(WorkingKey))
			apdu.put_array(WorkingKey)
	connection.send_apdu(apdu)

###For VGD only:###
def send_delete_key(connection, keyid):
	apdu = pupyspot.create_apdu(_apid, '\x40\x12')
	apdu.put_byte(keyid)
	connection.send_apdu(apdu)

def send_cert(connection, certdata):
	apdu = pupyspot.create_apdu(_apid, '\x70\x01')
	apdu.put_array(certdata)
	connection.send_apdu(apdu)

def send_cert_browse(connection):
	apdu = pupyspot.create_apdu(_apid, '\x70\x02')
	connection.send_apdu(apdu)

def send_cert_block_browse(connection, blk_offset):
	apdu = pupyspot.create_apdu(_apid, '\x70\x03')
	apdu.put_dword(blk_offset)
	connection.send_apdu(apdu)

def send_retrieve_terminal_cert_request(connection):
    """
    Sends a **Retrieve Terminal Certificate** command to SPOT.
    This command retrieves a "Certificate request" in PKCS\#10 format.
    The request is prepared asynchronously in the background by the terminal, thus it may be not yet available at the moment of the reception of this command.
    The PKCS\#10 Certificate request is intended to be submitted to a certificate authority in order to become a valid certificate that will be used by the terminal to identify itself during the TLS handshake. 
    The resulting certificate is valid as the ``TerminalCertificate`` parameter of a subsequent Set TLS Certificate command. 
        
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x90     |      0x01      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        
    """
	apdu = pupyspot.create_apdu(_apid, '\x90\x01')
	connection.send_apdu(apdu)

def send_set_tls_certificates(connection, cert_mac, terminal_certificate, masterCA_certificate):
    """
    Sends a **Set TLS Certificate** command to SPOT.
    This command set into the unit 2 certificates used during TLS handshake:
        - The certificate used to identify the terminal you are sending the command to,
        - The certificate of the certification authority of the remote peer. In order to establish a connection the remote peer must send, during TLS handshake, a certificate that can be verified using this certificate.
    The 1st certificate must match the latest PKCS#10 certificate request obtained with command Retrieve.
    Terminal Certificate and the cryptographic checks based on the MAC field to be accepted.
    
    +---------------+----------------+
    | Command class | Command type   |
    +===============+================+
    |      0x90     |      0x02      |
    +---------------+----------------+
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *cert_mac* (dword):  MAC x9.19 of the SHA256 of the remaining message parameters TermCertLen (length in bytes of TerminalCertificate) big endian DER representation of the X509 Terminal Certificate parameter.
        - *terminal_certificate* (binary string): See "Spot Programmers Guide". 
        - *masterCA_certificate* (binary string): See "Spot Programmers Guide".
    """
	apdu = pupyspot.create_apdu(_apid, '\x90\x02')
	apdu.put_dword(cert_mac)
	apdu.put_word(len(terminal_certificate))
	apdu.put_array(terminal_certificate)
	apdu.put_word(len(masterCA_certificate))
	apdu.put_array(masterCA_certificate)
	connection.send_apdu(apdu)

###Recv File Management Class Command Methods Definition###

def recv_file_download_request_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **File Download Request** *(0x00,0x01)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x00\x01', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_file_block_download_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **File Block Download** *(0x00,0x02)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x00\x02', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'offset': apdu.pop_dword()
		}
	else:
		ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_file_upload_request_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **File Upload Request** *(0x00,0x05)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x00\x05', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'FileSz': apdu.pop_dword()
		}
	else:
		ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_file_delete_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **File Delete** *(0x00,0x04)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x00\x04', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params


###Recv Information Request Class Command Methods Definition###

def recv_hardware_configuration(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Information Request - Hardware Configuration** *(0x10,0x02)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x10\x02', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'NumEntries': apdu.pop_byte()
		}
		ack_params['Entry'] = {}
		for n in range(0,ack_params['NumEntries']):
			entry = {
				'TagId': apdu.pop_byte(),
				'TagLen': apdu.pop_byte()
			}
			entry['TagData'] = apdu.pop_array(entry['TagLen'])
			ack_params['Entry'][str(entry['TagId'])]= entry['TagData']
	else:
		ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_software_configuration(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Information Request - Software Configuration** *(0x10,0x03)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x10\x03', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'NumEntries': apdu.pop_byte()
		}
		ack_params['Entry'] = {}
		for n in range(0,ack_params['NumEntries']):
			entry = {
				'TagId': apdu.pop_byte(),
				'TagLen': apdu.pop_byte()
			}
			entry['TagData'] = apdu.pop_array(entry['TagLen'])
			ack_params['Entry'][str(entry['TagId'])]= entry['TagData']
	else:
		ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_system_status(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - System status** *(0x10,0x01)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x10\x01', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'NumEntries': apdu.pop_byte()
		}
		ack_params['Entry'] = {}
		for n in range(0,ack_params['NumEntries']):
			entry = {
				'DeviceCode': apdu.pop_byte(),
				'Status': apdu.pop_byte(),
				'StatusExt': apdu.pop_byte(),
				'errorcode': apdu.pop_byte()
			}
			ack_params['Entry'][entry['DeviceCode']]= entry
	else:
		ack_params = {
			'ack_code': ack_code
		}
	return ack_params

	
###Receive Card Reader AckCommand Methods Definition###

def recv_cr_enable_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Card Reader - Enable** *(0x20,0x01)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x01', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_cr_disable_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Card Reader - Disable** *(0x20,0x02)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x02', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params
            
def recv_cr_track_data_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Card Reader - Track data** *(0x20,0x03)* command.
    
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
			'num_tracks': apdu.pop_byte()
		}
		tracks_list = []
		for n_track in range(0,ack_params['num_tracks']):
			track = {
				'source': apdu.pop_byte(),
				'status': apdu.pop_byte(),
				'size': apdu.pop_word()
			}
			track['data'] = apdu.pop_array(track['size'])
			tracks_list.append(track)
		ack_params['data'] = tracks_list
	else:
		ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_cr_write_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Card Reader - Write** *(0x20,0x04)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x04', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params
            
def recv_cr_chip_reset_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Card Reader - Chip reset** *(0x20,0x07)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x07', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params
            
def recv_cr_enable_sankyo_reader_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Card Reader - Enable Sankyo Reader** *(0x20,0x08)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x08', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params
            
def recv_get_emvcard_signature_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Card Reader - Get EMV Card Signature** *(0x20,0x09)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x20\x09', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'sign_len': apdu.pop_byte()
		}
		ack_params['sign_data'] = apdu.pop_array(ack_params['sign_len'])
	else:
		ack_params = {
			'ack_code': ack_code
		}
	return ack_params


###Recv Display Class Command Methods Definition###
            
def recv_create_window_from_resource_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Display - Create Window from Resource** *(0x30,0x0A)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x30\x0A', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params


###Recv Keyboard and security Class Command Methods Definition###

def recv_customer_input_enable_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - Customer Input Enable** *(0x40,0x01)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x01', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
			'ack_code': ack_code,
			'input_size': apdu.pop_word()
		}
		if ack_params['input_size'] > 0 :
			ack_params['input_data'] = apdu.pop_array(ack_params['input_size'])
		ack_params['key_size'] = apdu.pop_word()
		if ack_params['key_size'] > 0 :
			ack_params['key_data'] = apdu.pop_array(ack_params['key_size'])
		try:
			ack_params['smidr'] = apdu.pop_array(10)
		except:
			print('WARNING: missing SMIDR field')
	else:
		ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_customer_input_abort_cmd(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - Customer Input Abort** *(0x40,0x02)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x02', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_app_key_download(connection,timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - Application key download** *(0x40,0x04)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """		
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x04', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {'ack_code': ack_code}
	try:
		key_id = apdu.pop_byte()
		ack_params['KeyId'] = key_id
	finally:
		return ack_params
	
def recv_multiblock_encryption(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - MultiBlock Encryption** *(0x40,0x13)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x13', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {'ack_code': ack_code}
	try:
		ack_params['data_len'] = apdu.pop_word()
		ack_params['data'] = apdu.pop_array(ack_params['data_len'])
		ack_params['chaining_data_len'] = apdu.pop_word()
		ack_params['chaining_data'] = apdu.pop_array(ack_params['chaining_data_len'])
	finally:
		return ack_params

def recv_multiblock_encrypt(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - MultiBlock Encrypt** *(0x40,0x15)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x15', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {'ACKcode': ack_code}
	try:
		ack_params['DataLen'] = apdu.pop_word()
		ack_params['Data'] = apdu.pop_array(ack_params['DataLen'])
		ack_params['ChainingDataLen'] = apdu.pop_word()
		ack_params['ChainingData'] = apdu.pop_array(ack_params['ChainingDataLen'])
	finally:
		return ack_params
	
def recv_multiblock_verification(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - MultiBlock Verification** *(0x40,0x16)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x16', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {'ACKcode': ack_code}
	try:
		ack_params['ChainingDataLen'] = apdu.pop_word()
		ack_params['ChainingData'] = apdu.pop_array(ack_params['ChainingDataLen'])
	finally:
		return ack_params

def recv_send_app_key_download_TR31(connection,timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - Application key download TR31** *(0x40,0x0D)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """		
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x0D', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params	

def recv_data_encryption(connection,timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - Data Encryption** *(0x40,0x03)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x03', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
				'ack_code': 0x00,
				'Datalen':apdu.pop_word()
				}
		ack_params['Data'] = apdu.pop_array(ack_params['Datalen'])
		ack_params['SMIDR'] = apdu.pop_array(10)
	else:
		ack_params = {'ack_code': ack_code}
	return ack_params

def recv_send_mac_verification(connection,timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - MAC verification** *(0x40,0x07)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x07', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	return ack_params

def recv_key_derivation(connection, timeout=0):
	"""
    Receives an APDU containing the answer produced by the SPOT on the **Keyboard and Security - Key Derivation** *(0x40,0x05)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x05', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
			'ack_code': ack_code
		}
	if ack_code == 0x00:
		ack_params ['SMIDR']= apdu.pop_array(10)
	return ack_params

###For VGD only:###
def recv_delete_key(connection, timeout = 0):
	apdu = connection.wait_apdu(0x80 + _apid, '\x40\x12', timeout)
	ack_code = apdu.pop_byte()
	ack_params = {
				'ack_code': ack_code
				}
	return ack_params

def recv_cert(connection, timeout = 0):
	apdu = connection.wait_apdu(0x80 + _apid, '\x70\x01', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
				'ack_code' : 0x00,
				#TODO: complete answer decode
			}
	else:
		ack_params = {
				'ack_code' : ack_code
			}
	return ack_params

def recv_cert_browse(connection, timeout = 0):
	apdu = connection.wait_apdu(0x80 + _apid, '\x70\x02', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
				'ack_code'  : 0x00,
				'file_size' : apdu.pop_dword()
			}
	else:
		ack_params = {
				'ack_code'  : ack_code
			}
	return ack_params
	
def recv_cert_block_browse(connection, timeout = 0):
	apdu = connection.wait_apdu(0x80 + _apid, '\x70\x03', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
				'ack_code' : 0x00,
				'Offset'   : apdu.pop_dword(),
				'BlkSize'  : apdu.pop_dword(),
				'BlkData'  : apdu.pop_array(ack_params['BlkSize']),
			}
	else:
		ack_params = {
				'ack_code' : ack_code
			}
	return ack_params

def recv_retrieve_terminal_cert_request(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Retrieve Terminal Certificate** *(0x90,0x01)* command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x90\x01', timeout)
	ack_code = apdu.pop_byte()
	if ack_code == 0x00:
		ack_params = {
				'ack_code' : 0x00,
				'cert_request': apdu.pop_array(apdu.remaining_length())
			}
	else:
		ack_params = {
				'ack_code' : ack_code
			}
	return ack_params

def revc_set_tls_certificates(connection, timeout = 0):
    """
    Receives an APDU containing the answer produced by the SPOT on the **Set TLS Certificate** *(0x90,0x02)** command.
    
    :Parameters:
        - *connection* (PupySPOT): Object managing spot connection.
        - *timeout* (integer): Receive timeout.
        
    :Returns: A dictionary containing the "answer APDU" parameters.    

    """
	apdu = connection.wait_apdu(0x80 + _apid, '\x90\x02', timeout)
	ack_params = {
			'ack_code': apdu.pop_byte()
		}
	return ack_params
