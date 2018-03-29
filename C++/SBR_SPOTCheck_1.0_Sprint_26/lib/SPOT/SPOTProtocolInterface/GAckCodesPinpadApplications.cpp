/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GAckCodesPinpadApplications.h
 * @author Mariano Volker
 * @date Oct 02 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#include "GAckCodesPinpadApplications.h"

GAckCodesPinpadApplications* GAckCodesPinpadApplications::m_pInstance = NULL;
GMutex GAckCodesPinpadApplications::mst_InstanceMutex;

GAckCodesPinpadApplications::GAckCodesPinpadApplications():m_pIter(0)
{
	populateMap();
}

GAckCodesPinpadApplications::~GAckCodesPinpadApplications()
{
}

void GAckCodesPinpadApplications::populateMap()
{
	mapAckCodes.insert ( std::make_pair( 0x00, itemType("NO_ERROR"                         , "Operation completed with success"                                                             ) ) );
	mapAckCodes.insert ( std::make_pair( 0x02, itemType("DOWNLOAD_COMPLETED"               , "Download completed: resource stored"                                                          ) ) );
	mapAckCodes.insert ( std::make_pair( 0x11, itemType("NOT_ENOUGH_SPACE"                 , "Insufficient space on resource repository"                                                    ) ) );
	mapAckCodes.insert ( std::make_pair( 0x12, itemType("BLOCK_OUT_OF_SEQUENCE"            , "Resource data block out of sequence (not expected)"                                           ) ) );
	mapAckCodes.insert ( std::make_pair( 0x13, itemType("OVERRUN"                          , "Resource data block out of resource size"                                                     ) ) );
	mapAckCodes.insert ( std::make_pair( 0x14, itemType("BAD_SIGNATURE"                    , "Resource CRC32 or DIGEST mismatch"                                                            ) ) );
	mapAckCodes.insert ( std::make_pair( 0x15, itemType("BLOCK_WRITE"                      , "Error writing resource data block on repository"                                              ) ) );
	mapAckCodes.insert ( std::make_pair( 0x16, itemType("CANNOT_ADD_RESOURCE"              , "Error adding resource on repository (resource already present or store error)"                ) ) );
	mapAckCodes.insert ( std::make_pair( 0x17, itemType("CANNOT_DELETE_RESOURCE"           , "Error deleting resource on repository"                                                        ) ) );
	mapAckCodes.insert ( std::make_pair( 0x18, itemType("CANNOT_CREATE_TMP_FILE"           , "Error creating tmp structure to download resource"                                            ) ) );
	mapAckCodes.insert ( std::make_pair( 0x19, itemType("CANNOT_READ_RESOURCE"             , "Error opening resource file"                                                                  ) ) );
	mapAckCodes.insert ( std::make_pair( 0x1A, itemType("ERR_BLOCK_READ"                   , "Error reading resource data block"                                                            ) ) );
	mapAckCodes.insert ( std::make_pair( 0x20, itemType("USER_ABORT"                       , "User aborted input operation (ABORT key)"                                                     ) ) );
	mapAckCodes.insert ( std::make_pair( 0x21, itemType("USER_TIMEOUT"                     , "User did not finish input operation in time"                                                  ) ) );
	mapAckCodes.insert ( std::make_pair( 0x22, itemType("KEYBOARD_ERROR"                   , "Keyboard device failure during input operation"                                               ) ) );
	mapAckCodes.insert ( std::make_pair( 0x23, itemType("ICC_ERROR"                        , "Smart card access failure during verify operation"                                            ) ) );
	mapAckCodes.insert ( std::make_pair( 0x24, itemType("CHALLENGE_ERROR"                  , "Smart card verify failure"                                                                    ) ) );
	mapAckCodes.insert ( std::make_pair( 0x25, itemType("FORMATTING_ERROR"                 , "Keyboard message formatting error"                                                            ) ) );
	mapAckCodes.insert ( std::make_pair( 0x26, itemType("ENCRYPTION_ERROR"                 , "Keyboard message encryption error"                                                            ) ) );
	mapAckCodes.insert ( std::make_pair( 0x27, itemType("PIN_VERIFY_ERROR"                 , "Offline Pin Verify Error"                                                                     ) ) );
	mapAckCodes.insert ( std::make_pair( 0x28, itemType("WRONG_PIN_ENTERED"                , "The user entered a wrong PIN. (only Sagem/Cryptera Keypad FW R1B-3 or later on M5)"           ) ) );
	mapAckCodes.insert ( std::make_pair( 0x30, itemType("SECURITY_DEVICE_FAILURE"          , "Security device driver failure during cryptography operation"                                 ) ) );
	mapAckCodes.insert ( std::make_pair( 0x31, itemType("SECURITY_DEVICE_INTERNAL_ERROR"   , "Security device failure during cryptography operation"                                        ) ) );
	mapAckCodes.insert ( std::make_pair( 0x32, itemType("BAD_KEY_SLOT"                     , "Bad key slot selected (out of range, key not loaded)"                                         ) ) );
	mapAckCodes.insert ( std::make_pair( 0x33, itemType("BAD_KEY_TYPE"                     , "Bad key type selected for required operation or key already present (SPOT M3c-2.1, M5)"       ) ) );
	mapAckCodes.insert ( std::make_pair( 0x34, itemType("BAD_KEY_USAGE"                    , "Usage of key selected not compatible with required operation"                                 ) ) );
	mapAckCodes.insert ( std::make_pair( 0x35, itemType("BAD_KEY_CRC"                      , "Key data CRC error"                                                                           ) ) );
	mapAckCodes.insert ( std::make_pair( 0x36, itemType("DUKPT_END_COUNTER"                , "DUKPT/DUKPT2K counter end"                                                                    ) ) );
	mapAckCodes.insert ( std::make_pair( 0x37, itemType("DATA_SIZE"                        , "Key data size error"                                                                          ) ) );
	mapAckCodes.insert ( std::make_pair( 0x38, itemType("UNSUPPORTED_SERVICE"              , "Unsupported service"                                                                          ) ) );
	mapAckCodes.insert ( std::make_pair( 0x39, itemType("MACVERIFICATION_FAILED"           , "MAC verification failed"                                                                      ) ) );
	mapAckCodes.insert ( std::make_pair( 0x3A, itemType("ERR_TR31_DATA"                    , "TR31 data not corrected"                                                                      ) ) );
	mapAckCodes.insert ( std::make_pair( 0x3B, itemType("ERR_TR31_MAC"                     , "TR31 MAC verification failed"                                                                 ) ) );
	mapAckCodes.insert ( std::make_pair( 0x3C, itemType("ERR_TR31_UNSUPPORTED"             , "TR31 key attributes not supported by SPOT translation"                                        ) ) );
	mapAckCodes.insert ( std::make_pair( 0x3D, itemType("ERR_DUKPT_DERIV_NEEDED"           , "DUKPT pin input failed since derivation is needed"                                            ) ) );
	mapAckCodes.insert ( std::make_pair( 0x3E, itemType("CERTERR_NOTRSAFORMATTED"          , "Certificate RSA not well formatted or not RSA encrypted (only for M5)"                        ) ) );
	mapAckCodes.insert ( std::make_pair( 0x3F, itemType("SECURITY_GENERIC_ERROR"           , "Other error"                                                                                  ) ) );
	mapAckCodes.insert ( std::make_pair( 0x43, itemType("CR_GETSIGN_ABT_ICCRESET"          , "Get Signature procedure has been aborted due to an ICC Reset command"                         ) ) );
	mapAckCodes.insert ( std::make_pair( 0x44, itemType("CR_GETSIGN_ABT_CRDISABLE"         , "Get Signature procedure has been aborted due to a Card Reader Disable command"                ) ) );
	mapAckCodes.insert ( std::make_pair( 0x45, itemType("CR_GETSIGN_ABT_TIMEOUT"           , "Get Signature procedure has been aborted due to Timeout"                                      ) ) );
	mapAckCodes.insert ( std::make_pair( 0x46, itemType("CR_GETSIGN_CARDBLOCKED"           , "Get Signature procedure failure since Card is blocked"                                        ) ) );
	mapAckCodes.insert ( std::make_pair( 0x47, itemType("CR_GETSIGN_AIDEMPTY"              , "Get Signature procedure failure since AID list is empty"                                      ) ) );
	mapAckCodes.insert ( std::make_pair( 0x80, itemType("ERR_GENERIC"                      , ""                                                                                             ) ) );
	mapAckCodes.insert ( std::make_pair( 0x81, itemType("ERR_MESSAGE_SIZE"                 , "Wrong message size"                                                                           ) ) );
	mapAckCodes.insert ( std::make_pair( 0x82, itemType("ERR_MESSAGE_UNKNOWN"              , "Unknown message id"                                                                           ) ) );
	mapAckCodes.insert ( std::make_pair( 0x83, itemType("ERR_MESSAGE_FORMAT"               , "Bad message format"                                                                           ) ) );
	mapAckCodes.insert ( std::make_pair( 0x84, itemType("ERR_MESSAGE_OPTION"               , "Unsupported message option"                                                                   ) ) );
	mapAckCodes.insert ( std::make_pair( 0x90, itemType("ERR_INTERNAL"                     , "Application internal error"                                                                   ) ) );
	mapAckCodes.insert ( std::make_pair( 0x91, itemType("CMD_NOT_EXECUTABLE"               , "Command not executable"                                                                       ) ) );
	mapAckCodes.insert ( std::make_pair( 0x92, itemType("ERR_LOCKED"                       , "Application locked by another client"                                                         ) ) );
	mapAckCodes.insert ( std::make_pair( 0xA0, itemType("ERR_NO_CATEGORY"                  , "No application matching one selected"                                                         ) ) );
	mapAckCodes.insert ( std::make_pair( 0xA1, itemType("ERR_ALREADY_LOGGED"               , "Application already logged"                                                                   ) ) );
	mapAckCodes.insert ( std::make_pair( 0xA2, itemType("ERR_LOGIN_FAILED"                 , "Login failed"                                                                                 ) ) );
	mapAckCodes.insert ( std::make_pair( 0xA3, itemType("ERR_LOGOUT_FAILED"                , "Logout failed"                                                                                ) ) );
	mapAckCodes.insert ( std::make_pair( 0xA4, itemType("ERR_PMK_FAILURE"                  , "Bad PMK modulo length"                                                                        ) ) );
	mapAckCodes.insert ( std::make_pair( 0xA5, itemType("ERR_SSKEYS_FAILURE"               , "Bad session keys"                                                                             ) ) );
};

bool GAckCodesPinpadApplications::SetDescription(int iAckCode, const char* descrip)
{
	int key = iAckCode;
	MapType::iterator i = mapAckCodes.find(key);
	if (i != mapAckCodes.end())
	{
		(*i).second.sDescription = descrip;
		return true;
	};
	return false;
}

const char * GAckCodesPinpadApplications::GetDescription(int iAckCode)
{
	int key = iAckCode;
	MapType::iterator i = mapAckCodes.find(key);
	if (i != mapAckCodes.end())
	{
		itemType d = (*i).second;
		return d.sDescription.c_str();
	};
	return "";
}

const char * GAckCodesPinpadApplications::GetNemonic(int iAckCode)
{
	int key = iAckCode;
	MapType::iterator i = mapAckCodes.find(key);
	if (i != mapAckCodes.end())
	{
		itemType d = (*i).second;
		return d.sNemonic.c_str();
	};
	return "";
}

int GAckCodesPinpadApplications::GetNextKey(int &iKey )
{
   if(mapAckCodes.empty()) return false;

   if( !m_pIter )
   {
	   m_pIter = new MapType::const_iterator;
      (*m_pIter)=mapAckCodes.begin();
      iKey=(*m_pIter)->first;
      (*m_pIter)++;
      return true;
   }
   else
   {
      if( (*m_pIter) != mapAckCodes.end() )
      {
    	 iKey=(*m_pIter)->first;
         (*m_pIter)++;
         return true;
      }

      delete m_pIter; m_pIter=0;
   }

   return false;
}
