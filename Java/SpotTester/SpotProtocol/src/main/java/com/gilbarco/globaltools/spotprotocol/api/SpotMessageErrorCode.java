
package com.gilbarco.globaltools.spotprotocol.api;

public enum SpotMessageErrorCode
{
	// ******************************************************************
	// PUBLIC FIELDS.
	// ******************************************************************
	NO_ERROR((byte) 0x00), // Operation completed with success
	// Resource management commands
	DOWNLOAD_COMPLETED((byte) 0x02), // Download completed: resource stored
	NOT_ENOUGH_SPACE((byte) 0x11), // Insufficient space on resource repository
	BLOCK_OUT_OF_SEQUENCE((byte) 0x12), // Resource data block out of sequence (not expected)
	OVERRUN((byte) 0x13), // Resource data block out of resource size
	BAD_SIGNATURE((byte) 0x14), // Resource CRC32 or DIGEST mismatch
	BLOCK_WRITE((byte) 0x15), // Error writing resource data block on repository
	CANNOT_ADD_RESOURCE((byte) 0x16), // Error adding resource on repository (resource already present or store error)
	CANNOT_DELETE_RESOURCE((byte) 0x17), // Error deleting resource on repository
	CANNOT_CREATE_TMP_FILE((byte) 0x18), // Error creating tmp structure to download resource
	CANNOT_READ_RESOURCE((byte) 0x19), // Error opening resource file
	ERR_BLOCK_READ((byte) 0x1A), // Error reading resource data block
	// Keyboard management commands
	USER_ABORT((byte) 0x20), // User aborted input operation (ABORT key)
	USER_TIMEOUT((byte) 0x21), // User did not finish input operation in time
	KEYBOARD_ERROR((byte) 0x22), // Keyboard device failure during input operation
	ICC_ERROR((byte) 0x23), // Smart card access failure during verify operation
	CHALLENGE_ERROR((byte) 0x24), // Smart card verify failure
	FORMATTING_ERROR((byte) 0x25), // Keyboard message formatting error
	ENCRYPTION_ERROR((byte) 0x26), // Keyboard message encryption error
	PIN_VERIFY_ERROR((byte) 0x27), // Offline Pin Verify Error
	// Cryptography management commands
	SECURITY_DEVICE_FAILURE((byte) 0x30), // Security device driver failure during cryptography operation
	SECURITY_DEVICE_INTERNAL_ERROR((byte) 0x31), // Security device failure during cryptography operation
	BAD_KEY_SLOT((byte) 0x32), // Bad key slot selected (out of range, key not loaded)
	BAD_KEY_TYPE((byte) 0x33), // Bad key type selected for required operation
	BAD_KEY_USAGE((byte) 0x34), // Usage of key selected not compatible with required operation
	BAD_KEY_CRC((byte) 0x35), // Key data CRC error
	DUKPT_END_COUNTER((byte) 0x36), // DUKPT/DUKPT2K counter end
	DATA_SIZE((byte) 0x37), // Key data size error
	UNSUPPORTED_SERVICE((byte) 0x38), // Unsupported service
	MACVERIFICATION_FAILED((byte) 0x39), // MAC verification failed
	ERR_TR31_DATA((byte) 0x3A), // TR31 data not corrected
	ERR_TR31_MAC((byte) 0x3B), // TR31 MAC verification failed
	ERR_TR31_UNSUPPORTED((byte) 0x3C), // TR31 key attributes not supported by SPOT translation
	ERR_DUKPT_DERIV_NEEDED((byte) 0x3D), // DUKPT pin input failed since derivation is needed
	CERTERR_NOTRSAFORMATTED((byte) 0x3E), // Certificate RSA not well formatted or not RSA encrypted (only for M5)
	SECURITY_GENERIC_ERROR((byte) 0x3F), // Other error	
	//Certificate Management
	SPOT_ERR_ROOTCA_NOT_DOWNLOADABLE((byte)0x70), //ROOT_CA certificate may not be download
	SPOT_ERR_CERTMAN_ROOTCA_NOT_FOUND((byte)0x71), //Certificate may not be downloaded since ROOT_CA Certificate was not found
	SPOT_ERR_CERTMAN_ROOTCA_IS_EXPIRED((byte)0x72), //Certificate may not be downloaded since ROOT_CA  is expired	   
	SPOT_ERR_CERTMAN_ROOTCA_IS_AHEADOFTIME((byte)0x73), //Certificate may not be downloaded since ROOT_CA is ahead of time	   
	SPOT_ERR_CERTMAN_ROOTCA_INTERNAL_ERROR((byte)0x74), //Certificate may not be downloaded due to an internal error getting ROOT_CA Certificate	   
	SPOT_ERR_CERTMAN_CERTIFICATE_IS_EXPIRED((byte)0x75), //Certificate may not be downloaded since it is expired	   
	SPOT_ERR_CERTMAN_CERTIFICATE_IS_AHEADOFTIME((byte)0x76), //Certificate may not be downloaded since it is ahead of time	   
	SPOT_ERR_CERTMAN_CERTIFICATE_SIGNATURE_FAILED((byte)0x77), //Certificate may not be downloaded since certificate signature verification was failed	   
	SPOT_ERR_CERTMAN_CERTIFICATE_MALFORMED((byte)0x78), //Certificate may not be downloaded since it is malformed	   
	SPOT_ERR_CERTMAN_INTERNAL_ERROR((byte)0x79), //Certificate may not be downloaded  due to an internal error
	// Protocol generic
	ERR_GENERIC((byte) 0x80),
	ERR_MESSAGE_SIZE((byte) 0x81), // Wrong message size
	ERR_MESSAGE_UNKNOWN((byte) 0x82), // Unknown message id
	ERR_MESSAGE_FORMAT((byte) 0x83), // Bad message format
	ERR_MESSAGE_OPTION((byte) 0x84), // Unsupported message option
	// Application generic
	ERR_INTERNAL((byte) 0x90), // Application internal error
	CMD_NOT_EXECUTABLE((byte) 0x91), // Command not executable
	ERR_LOCKED((byte) 0x92), // Application locked by another client
	// Transport protocol generic
	ERR_NO_CATEGORY((byte) 0xA0), // No application matching one selected
	ERR_ALREADY_LOGGED((byte) 0xA1), // Application already logged
	ERR_LOGIN_FAILED((byte) 0xA2), // Login failed
	ERR_LOGOUT_FAILED((byte) 0xA3), // Logout failed
	ERR_PMK_FAILURE((byte) 0xA4), // Bad PMK modulo length
	ERR_SSKEYS_FAILURE((byte) 0xA5), // Bad session keys
	// SAM interface-module specific (returned only in answers to commands of 0x50 class)
	ERR_GEM_COM_REJECTED((byte) 0x01), // The command to be forwarded to SAM slot is not valid and thus rejected.
	ERR_GEM_DEVICE_LINK((byte) 0x02)// The serial interface-module doesn’t respond correctly (may be cable disconnected)
	;

	// ******************************************************************
	// PRIVATE FIELDS.
	// ******************************************************************
	private byte	errCode;

	// ******************************************************************
	// CONSTRUCTOR.
	// ******************************************************************
	SpotMessageErrorCode(byte code)
	{
		errCode = code;
	}

	public byte getCode()
	{
		return errCode;
	}

	public static SpotMessageErrorCode getByName(byte code)
	{
		switch(code)
		{
			case (byte) 0x00 :
				return NO_ERROR;
			// Resource management commands
			case (byte) 0x02 :
				return DOWNLOAD_COMPLETED;
			case (byte) 0x11 :
				return NOT_ENOUGH_SPACE;
			case (byte) 0x12 :
				return BLOCK_OUT_OF_SEQUENCE;
			case (byte) 0x13 :
				return OVERRUN;
			case (byte) 0x14 :
				return BAD_SIGNATURE;
			case (byte) 0x15 :
				return BLOCK_WRITE;
			case (byte) 0x16 :
				return CANNOT_ADD_RESOURCE;
			case (byte) 0x17 :
				return CANNOT_DELETE_RESOURCE;
			case (byte) 0x18 :
				return CANNOT_CREATE_TMP_FILE;
			case (byte) 0x19 :
				return CANNOT_READ_RESOURCE;
			case (byte) 0x1A :
				return ERR_BLOCK_READ;
			// Keyboard management commands
			case (byte) 0x20 :
				return USER_ABORT;
			case (byte) 0x21 :
				return USER_TIMEOUT;
			case (byte) 0x22 :
				return KEYBOARD_ERROR;
			case (byte) 0x23 :
				return ICC_ERROR;
			case (byte) 0x24 :
				return CHALLENGE_ERROR;
			case (byte) 0x25 :
				return FORMATTING_ERROR;
			case (byte) 0x26 :
				return ENCRYPTION_ERROR;
			case (byte) 0x27 :
				return PIN_VERIFY_ERROR;
			// Cryptography management commands
			case (byte) 0x30 :
				return SECURITY_DEVICE_FAILURE;
			case (byte) 0x31 :
				return SECURITY_DEVICE_INTERNAL_ERROR;
			case (byte) 0x32 :
				return BAD_KEY_SLOT;
			case (byte) 0x33 :
				return BAD_KEY_TYPE;
			case (byte) 0x34 :
				return BAD_KEY_USAGE;
			case (byte) 0x35 :
				return BAD_KEY_CRC;
			case (byte) 0x36 :
				return DUKPT_END_COUNTER;
			case (byte) 0x37 :
				return DATA_SIZE;
			case (byte) 0x38 :
				return UNSUPPORTED_SERVICE;
			case (byte) 0x39 :
				return MACVERIFICATION_FAILED;
			case (byte) 0x3A :
				return ERR_TR31_DATA;
			case (byte) 0x3B :
				return ERR_TR31_MAC;
			case (byte) 0x3C :
				return ERR_TR31_UNSUPPORTED;
			case (byte) 0x3D :
				return ERR_DUKPT_DERIV_NEEDED;
			case (byte) 0x3E :
				return CERTERR_NOTRSAFORMATTED;
			case (byte) 0x3F :
				return SECURITY_GENERIC_ERROR;
			//Certificate Management
			case (byte) 0x70 :
				return SPOT_ERR_ROOTCA_NOT_DOWNLOADABLE;
			case (byte) 0x71 :
				return SPOT_ERR_CERTMAN_ROOTCA_NOT_FOUND;
			case (byte) 0x72 :
				return SPOT_ERR_CERTMAN_ROOTCA_IS_EXPIRED;
			case (byte) 0x73 :
				return SPOT_ERR_CERTMAN_ROOTCA_IS_AHEADOFTIME;
			case (byte) 0x74 :
				return SPOT_ERR_CERTMAN_ROOTCA_INTERNAL_ERROR;
			case (byte) 0x75 :
				return SPOT_ERR_CERTMAN_CERTIFICATE_IS_EXPIRED;
			case (byte) 0x76 :
				return SPOT_ERR_CERTMAN_CERTIFICATE_IS_AHEADOFTIME;
			case (byte) 0x77 :
				return SPOT_ERR_CERTMAN_CERTIFICATE_SIGNATURE_FAILED;
			case (byte) 0x78 :
				return SPOT_ERR_CERTMAN_CERTIFICATE_MALFORMED;
			case (byte) 0x79 :
				return SPOT_ERR_CERTMAN_INTERNAL_ERROR;
			// Protocol generic
			case (byte) 0x80 :
				return ERR_GENERIC;
			case (byte) 0x81 :
				return ERR_MESSAGE_SIZE;
			case (byte) 0x82 :
				return ERR_MESSAGE_UNKNOWN;
			case (byte) 0x83 :
				return ERR_MESSAGE_FORMAT;
			case (byte) 0x84 :
				return ERR_MESSAGE_OPTION;
			// Application generic
			case (byte) 0x90 :
				return ERR_INTERNAL;
			case (byte) 0x91 :
				return CMD_NOT_EXECUTABLE;
			case (byte) 0x92 :
				return ERR_LOCKED;
				// Transport protocol generic
			case (byte) 0xA0 :
				return ERR_NO_CATEGORY;
			case (byte) 0xA1 :
				return ERR_ALREADY_LOGGED;
			case (byte) 0xA2 :
				return ERR_LOGIN_FAILED;
			case (byte) 0xA3 :
				return ERR_LOGOUT_FAILED;
			case (byte) 0xA4 :
				return ERR_PMK_FAILURE;
			case (byte) 0xA5 :
				return ERR_SSKEYS_FAILURE;
			default :
				return null;
		}
	}

	public static String getDescription(byte ackCode)
	{
		switch(ackCode)
		{
			case (byte) 0x00 :
				return "Operation completed with success";
				// Resource management commands
			case (byte) 0x02 :
				return "Download completed: resource stored";
			case (byte) 0x11 :
				return "Insufficient space on resource repository";
			case (byte) 0x12 :
				return "Resource data block out of sequence (not expected)";
			case (byte) 0x13 :
				return "Resource data block out of resource size";
			case (byte) 0x14 :
				return "Resource CRC32 or DIGEST mismatch";
			case (byte) 0x15 :
				return "Error writing resource data block on repository";
			case (byte) 0x16 :
				return "Error adding resource on repository (resource already present or store error)";
			case (byte) 0x17 :
				return "Error deleting resource on repository";
			case (byte) 0x18 :
				return "Error creating tmp structure to download resource";
			case (byte) 0x19 :
				return "Error opening resource file";
			case (byte) 0x1A :
				return "Error reading resource data block";
				// Keyboard management commands
			case (byte) 0x20 :
				return "User aborted input operation (ABORT key)";
			case (byte) 0x21 :
				return "User did not finish input operation in time";
			case (byte) 0x22 :
				return "Keyboard device failure during input operation";
			case (byte) 0x23 :
				return "Smart card access failure during verify operation";
			case (byte) 0x24 :
				return "Smart card verify failure";
			case (byte) 0x25 :
				return "Keyboard message formatting error";
			case (byte) 0x26 :
				return "Keyboard message encryption error";
			case (byte) 0x27 :
				return "Offline Pin Verify Error";
				// Cryptography management commands
			case (byte) 0x30 :
				return "Security device driver failure during cryptography operation";
			case (byte) 0x31 :
				return "Security device failure during cryptography operation";
			case (byte) 0x32 :
				return "Bad key slot selected (out of range, key not loaded)";
			case (byte) 0x33 :
				return "Bad key type selected for required operation";
			case (byte) 0x34 :
				return "Usage of key selected not compatible with required operation";
			case (byte) 0x35 :
				return "Key data CRC error";
			case (byte) 0x36 :
				return "DUKPT/DUKPT2K counter end";
			case (byte) 0x37 :
				return "Key data size error";
			case (byte) 0x38 :
				return "Unsupported service";
			case (byte) 0x39 :
				return "MAC verification failed";
			case (byte) 0x3A :
				return "TR31 data not corrected";
			case (byte) 0x3B :
				return "TR31 MAC verification failed";
			case (byte) 0x3C :
				return "TR31 key attributes not supported by SPOT translation";
			case (byte) 0x3D :
				return "DUKPT pin input failed since derivation is needed";
			case (byte) 0x3E :
				return "Certificate RSA not well formatted or not RSA encrypted (only for M5)";
			case (byte) 0x3F :
				return "Other error";
				// Certificate Management
			case (byte) 0x70 :
				return "ROOT_CA certificate may not be download";
			case (byte) 0x71 :
				return "Certificate may not be downloaded since ROOT_CA Certificate was not found";
			case (byte) 0x72 :
				return "Certificate may not be downloaded since ROOT_CA  is expired";
			case (byte) 0x73 :
				return "Certificate may not be downloaded since ROOT_CA is ahead of time";
			case (byte) 0x74 :
				return "Certificate may not be downloaded due to an internal error getting ROOT_CA Certificate";
			case (byte) 0x75 :
				return "Certificate may not be downloaded since it is expired";
			case (byte) 0x76 :
				return "Certificate may not be downloaded since it is ahead of time";
			case (byte) 0x77 :
				return "Certificate may not be downloaded since certificate signature verification was failed";
			case (byte) 0x78 :
				return "Certificate may not be downloaded since it is malformed";
			case (byte) 0x79 :
				return "Certificate may not be downloaded  due to an internal error";
				// Protocol generic
			case (byte) 0x80 :
				return "Generic Error";
			case (byte) 0x81 :
				return "Wrong message size";
			case (byte) 0x82 :
				return "Unknown message id";
			case (byte) 0x83 :
				return "Bad message format";
			case (byte) 0x84 :
				return "Unsupported message option";
				// Application generic
			case (byte) 0x90 :
				return "Application internal error";
			case (byte) 0x91 :
				return "Command not executable";
			case (byte) 0x92 :
				return "Application locked by another client";
				// Transport protocol generic
			case (byte) 0xA0 :
				return "No application matching one selected";
			case (byte) 0xA1 :
				return "Application already logged";
			case (byte) 0xA2 :
				return "Login failed";
			case (byte) 0xA3 :
				return "Logout failed";
			case (byte) 0xA4 :
				return "Bad PMK modulo length";
			case (byte) 0xA5 :
				return "Bad session keys";
			default :
				return "Unknown result code";
		}
	}

}
