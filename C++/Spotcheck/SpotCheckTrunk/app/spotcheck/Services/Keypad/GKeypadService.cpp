/*
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GKeypadService.h
 * @author Mariano Volker
 * @date Mach 12 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#include <unistd.h>
#include <cstring>

#include "GKeypadService.h"

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "Menus/Keypad/GAbsKeyboardMenu.h"
#include "Menus/Keypad/GAbsKeyboardManager.h"
#include "Menus/SecureAudio/GSecureAudioManager.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"

using namespace GSpotDefs;

GKeypadService* GKeypadService::mst_pSoleInstance = NULL;
GMutex GKeypadService::mst_createInstanceMutex;

const UINT8  GKeypadService::mst_aryu8EnableKBCmd [KB_ENABLE_CMD_LEN] =
             { 0x00, 0x14, 0x01, 0x00, 0x40, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x90, 0x01, 0x01, 0x00, 0x00, 0x00, 0x3D, 0x00, 0x00 };

const UINT8  GKeypadService::mst_aryu8EnableKBCmdVGD [KB_ENABLE_CMD_LEN] =
             { 0x00, 0x14, 0x01, 0x00, 0x40, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x90, 0x01, 0x01, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00 };

const UINT8  GKeypadService::mst_aryu8DisableKBCmd[KB_DISABLE_CMD_LEN] =
             { 0x00, 0x06, 0x01, 0x00, 0x40, 0x02 };

const char*  GKeypadService::mst_arypcKeysLabel[] =
{
	  "0",  "1",   "2",  "3",   "4",  "5",  "6",  "7",  "8",  "9", // decimals 00 - 09
	"Hlp", "Up", "Can", "OK", "Clr", "No", "L1", "L2", "L3", "L4", // decimals 10 - 19
	 "R1", "R2",  "R3", "R4",  "P1", "P2", "P3", "P4", "P5", "P6", // decimals 20 - 19
	 "P7", "P8",  "P9", "P10", "Yes"                               // decimals 30 - 34
};

const char* GKeypadService::m_aryu8SAResultStrings[] =
{
  "No Test",
  "Could not test\nNo Audio in NonSecure Mode",
  "Disabled",
  "Enabled"
};


const char* GKeypadService::GetSAResults()
{
	return m_aryu8SAResultStrings[m_eSAResult];
}


void GKeypadService::SetSAResults(eSAResult value)
{
	 m_eSAResult = value;
}

void GKeypadService::SetNonSecStatusResults(eSATestStatus value)
{
	m_NonSecureStatus = value;
}

void GKeypadService::SetSecStatusResults(eSATestStatus value)
{
	m_SecureStatus = value;
}

GKeypadService::GKeypadService(GSPOTSession* pSession):
                m_pSession(pSession), m_pManager(NULL)
{
	LOG(eKeypad, eInfo, "Keypad Service started.\n");

	ClearKeysStatistics();
}

GKeypadService::~GKeypadService()
{
	LOG(eKeypad, eInfo, "Keypad Service stopped.\n");
}

bool GKeypadService::isKeypadReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
{
	if (!pMsg)
	{
		return false; // no message is supplied
	}

	if (!blOnlySubCmd) // this flag avoids to analyze the following, if already tested true
	{
		if (pMsg->GetAppId() != eSPOT_PP_ANSWER)
			return false; // unexpected application ID

		if (pMsg->GetCommandCode() != eSPOT_PP_KEYBOARD)
			return false; // unexpected command category
	}

	int iSubCmd = pMsg->GetSubCommandCode();
	if (iSubCmd == eSPOT_PP_KB_ENABLE || iSubCmd == eSPOT_PP_KB_DISABLE)
	{
		return true;
	}
	else
	{
		return false; // unexpected command code
	}
}

const char * GKeypadService::GetKeyLabel(UINT8 u8Key)
{
	if( u8Key < (eKeyYes +1) )
	{
		return GKeypadService::mst_arypcKeysLabel[u8Key];
	}
	else
	{
		return "ERR";
	}
}

UINT8 GKeypadService::GetKeyStatistics(UINT8 u8Key)
{
	if( u8Key < eMaxKeys )
	{
		LOG(eKeypad, eDebug,
		    "GKeypadService::GetKeyStatistics - Key: [0x%02X]. Frecuency %d.\n",
		    u8Key, int(mst_aryu8KeysStatistics[u8Key]));
		return mst_aryu8KeysStatistics[u8Key];
	}
	else
	{
		return 0xFF;
	}
}

bool GKeypadService::ProcessKeypadReply(GSPOTMessage* pMsg)
{
	bool blret = false;
	const char* pcMethod = "GKeypadService::ProcessKeypadReply() -- ";
	UINT8* pu8MsgData = NULL;
	UINT16 u16DataLen = 0;
	int iCmdCode = pMsg->GetSubCommandCode();

	switch (iCmdCode)
	{
		case eSPOT_PP_KB_DISABLE:
			LOG(eKeypad, eDebug, "%s%s\n", pcMethod, "Reply to a disable keypad command has just been received.\n" );
			// Uncomment just for debugging purposes ONLY.
			// GConversion::LogBytesAsASCIIHexString(pMsg->GetData(), pMsg->GetDataLen());
			blret = true;
		break;

		case eSPOT_PP_KB_ENABLE:
		{
			LOG(eKeypad, eDebug, "%s%s\n", pcMethod, "Keypad Input data received.");
			u16DataLen = pMsg->GetDataLen();
			if (u16DataLen == 0)
			{
				LOG(eKeypad, eWarning, "%s%s\n", pcMethod, "Message length is NULL!");
				break;
			}

			pu8MsgData = new UINT8[u16DataLen];
			if (pu8MsgData == NULL)
			{
				LOG(eKeypad, eError, "%s%s\n", pcMethod, "Data buffer denied!");
				break;
			}

			if (!pMsg->GetData(pu8MsgData, u16DataLen))
			{
				LOG(eKeypad, eWarning, "%s%s\n", pcMethod, "Message data is NULL!");
				delete[] pu8MsgData;
				break;
			}

			// Uncomment just for debugging purposes ONLY.
			 GConversion::LogBytesAsASCIIHexString(pMsg->GetData(), pMsg->GetDataLen());

			blret = ProcessDataEvent(pu8MsgData, u16DataLen);
			delete[] pu8MsgData;
		}
		break;

		default:
		break;
	}

	return blret;
}

/**
 *
 *  Parse message : Diagnostic Information 0x10, 0x04
 *
 * Similar to above case but now each device has different parameters set. This is managed by a variable length
 * sequence of tagged value. That is a sequence of 1byte for parameter tag and n bytes for parameter value
 * (where depends on the parameter type).

 * ACKCode[1]
 * (see: 6.1.1).
 * NumEntries[1]
 * Entries[NumEntries]
 * (see 6.1.5 for Tag coding and parameters meaning)
 * ItemId[1]
 * ItemSize[1]
 * ItemData[ItemSize]
 * @param GSPOTMessage the message answer to parse
 * */
int GKeypadService::ParseDiagnosticInfoReply(GSPOTMessage* pMsg)
{
	int len = pMsg->GetDataLen();
	if(len < 8)
		return 0;

	UINT8 *pu8Data = new UINT8[len];
	memset(pu8Data, 0, len);
	pMsg->GetData(pu8Data, len);
	if(pu8Data[0] != 0x00)
	{
		delete[] pu8Data;
		return 0;
	}

	int nItems = pu8Data[1];
	if(!nItems)
	{
		delete[] pu8Data;
		return 0;
	}

	int iItemNo = 0, iItemID = 0, iItemLen = 0, iProcessedItems = 0;
	UINT8* pb = pu8Data + 2;
	char*  pc = NULL;
	UINT32 value = 0;

	while( iItemNo < nItems && (pb - pu8Data) < len)
	{
		iItemID =  *pb++;
		iItemLen = *pb++;

		value = 0;
		memcpy(&value, pb, sizeof(UINT32));
		value = GConversion::BigEndianToHost(value);

		if(iItemID == eSPOT_KB_KEYSTROKECNT)
		{
			LOG(eKeypad, eInfo, "[eSPOT_KB_KEYSTROKECNT]=[%ld]\n", value);
			iProcessedItems ++;
		}

		pb += iItemLen;
		iItemNo++;
	}

	delete[] pu8Data;
	return iProcessedItems;
}

bool GKeypadService::ProcessDataEvent(UINT8* pMsgData, UINT16 u16DataSize)
{
	GMutexLock ml(m_instanceMutex);

	const char* pcMethod = "GKeypadService::ProcessDataEvent() -- ";

	//GSecureAudioManager* pSAManager = dynamic_cast<GSecureAudioManager*>(m_pManager);

	UINT8*     pu8data = NULL; // auxiliary data pointer
	UINT16     u16len;
	eKeyValue  eNormalizeKey;

	pu8data = pMsgData;

	UINT8 bh = 0, bl = 0, u8err = 0;

	u8err  =  *pu8data++;       // keypad error code
	bh     =  *pu8data++;       // length MSB , SPOT protocol: always big endian
	bl     =  *pu8data++;       // length LSB , SPOT protocol: always big endian
	u16len =  256 * bh + bl;    // keypad reply data length

	// First error condition: fatal error.
	if( u8err != 0 )
	{
		// Keep keypad enabled between key strokes.
		if(m_pManager && m_pManager->IsSecureAudioTest())
		{
			EnableRequestForSecAud();
		}
		else
		{
			EnableRequest();
		}

		return false;
	}

	// Second error condition: wrong length.
	else if( u16len != 1 )
	{
		// Keep keypad enabled between key strokes.
		if(m_pManager && m_pManager->IsSecureAudioTest())
		{
			EnableRequestForSecAud();
		}
		else
		{
			EnableRequest();
		}

		return false;
	}

	// Normal key pressed.
	else
	{
		if( !KeyMapping( (*pu8data), eNormalizeKey ) )
		{
			// Keep keypad enabled between key strokes.
			if(m_pManager && m_pManager->IsSecureAudioTest())
			{
				EnableRequestForSecAud();
			}
			else
			{
				EnableRequest();
			}

			return false;
		}
	}

	if( m_pManager != NULL )
	{
		m_pManager->NotifyCustomerInputKey( (UINT8)eNormalizeKey );
	}

	// Keep keypad enabled between key strokes.
	if(m_pManager && m_pManager->IsSecureAudioTest())
	{
		EnableRequestForSecAud();
	}
	else
	{
		EnableRequest();
	}

	return true;
}

bool GKeypadService::KeyMapping(UINT8 u8KeyPressed, eKeyValue& eNormalizeKey)
{
	UINT8 u8Key = 0x00;
	const char* pcMethod = "GKeypadService::KeyMapping() -- ";

	if( u8KeyPressed == 0 || (u8KeyPressed > eKeyR4 && u8KeyPressed < '0') || u8KeyPressed > '9')
	{
		// Wrong unknown keycode.
		LOG(eKeypad, eError, "%s%s. [0x%X].\n", pcMethod, "ERROR, key pressed not recognized.", u8KeyPressed);
		return false;
	}
	else if( u8KeyPressed >= 0x01 && u8KeyPressed <= 0x0A )
	{
		// If the key pressed is an auxiliary (pump) key.

		u8Key = u8KeyPressed + 0x17; // normalize to [24..33] range
	}
	else if( u8KeyPressed >= '0' && u8KeyPressed <= '9' )
	{
		// If the key pressed is a numeric digit.

		u8Key = u8KeyPressed - 0x30; // normalize to [0..9] range
	}
	else // some other few keycodes that need shifting to fit in array
	{
		switch(u8KeyPressed)
		{
			case 0x14:
				u8Key = eKeyHelp;
			break;

			case 0x15:
				u8Key = eKeyNo;
			break;

			case 0x16:
				u8Key = eKeyYes;
			break;

			default:
				u8Key = u8KeyPressed; // all other codes remain unchanged.
		}
	}

	mst_aryu8KeysStatistics[u8Key]++;

	LOG(eKeypad, eDebug, "%s%s[0x%X].  Frecuency: %d.\n", pcMethod,
		"Key pressed: ", u8Key, int(mst_aryu8KeysStatistics[u8Key]));

	eNormalizeKey = (eKeyValue) u8Key;

	return true;
}

void GKeypadService::ClearKeysStatistics()
{
	GMutexLock ml(m_instanceMutex);

	memset(mst_aryu8KeysStatistics, 0x00, sizeof(mst_aryu8KeysStatistics));
}

void GKeypadService::NotifySoftKey(UINT8 u8KeyCode)
{
	GMutexLock ml(m_instanceMutex);

	const char* pcMethod = "GKeypadService::NotifySoftKey() -- ";
	const char* pcPrefix = "Soft";

	if( u8KeyCode >= eKeyL1 && u8KeyCode <= eKeyP10 )
	{
		mst_aryu8KeysStatistics[u8KeyCode]++;

		if(u8KeyCode >= eKeyP1)
			pcPrefix = "Aux";

		LOG(eKeypad, eDebug, "%s%s%s [0x%X].\n", pcMethod, pcPrefix, "-key is pressed.", u8KeyCode);
	}
	else if( u8KeyCode >= eKeyAL1 && u8KeyCode <= eKeyAR4 )
	{
		mst_aryu8KeysStatistics[u8KeyCode]++;

		if(u8KeyCode >= eKeyAL1)
			pcPrefix = "ADASoft";

		LOG(eKeypad, eDebug, "%s%s%s [0x%X].\n", pcMethod, pcPrefix, "-key is pressed.", u8KeyCode);
	}
	else
	{
		LOG(eKeypad, eError, "%s%s [0x%X].\n", pcMethod, "ERROR, soft-key pressed not recognized.", u8KeyCode);
	}
}

void GKeypadService::AttachManager(GAbsKeyboardManager* pMgr)
{
	if( !pMgr )
		return;

	GMutexLock ml(m_instanceMutex);

	m_pManager = pMgr;
}

void GKeypadService::DetachManager()
{
	GMutexLock ml(m_instanceMutex);

	m_pManager = NULL;
}

bool GKeypadService::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	GMutexLock ml(m_instanceMutex);

	const char* pcMethod = "GKeypadService::ProcessSystemStatusNotification() -- ";
	LOG(eKeypad, eDebug, "%s st %02X, stex %02X, error %02X \n", pcMethod, int(u8Status), int(u8StatusEx), int(u8Error));

	if( u8Error != 0x00 )
	{
		LOG(eKeypad, eError, "%s Keypad Status: st %02X, stex %02X, ERROR %02X \n", pcMethod, int(u8Status), int(u8StatusEx), int(u8Error));

		m_eStatus = eKPError;

		return false;
	}
	else
	{
		LOG(eKeypad, eDebug, "%s Keypad Status: %02X, Extended: %02X \n", pcMethod, int(u8Status), int(u8StatusEx));
	}

	if( u8Status < 0x02 )
	{
		m_eStatus = (u8Status == 0x00 ? eDisabled : eEnabled);

		LOG(eKeypad, eNotice, "KEYPAD reported %sABLED.\n", u8Status == 0x00 ? "DIS" : "EN");

		return true;
	}

	if( u8Status == 0x02 )
	{
		m_eStatus = eBusy;

		LOG(eKeypad, eNotice, "KEYPAD reported BUSY.\n");

		return true;
	}
	else
	{
		m_eStatus = eKPError;

		LOG(eKeypad, eError, "%s KEYPAD ERROR: st %02X, stex %02X, error %02X \n", pcMethod, int(u8Status), int(u8StatusEx), int(u8Error));

		return false;
	}
}

bool GKeypadService::EnableRequest()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GKeypadService::EnableRequest() - ";

	if( !SendEnableCmdToSPOT() )
	{
		LOG(eKeypad, eError, "%s%s", pcMethod, "Could not send asynchronous Enable message to SPOT.\n");
		return false;
	}

	LOG(eKeypad, eTrace, "%s%s", pcMethod, "SPOT keypad enable message has been queued to SPOT.\n");
	return true;
}

bool GKeypadService::EnableRequestForSecAud()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GKeypadService::EnableRequestForSecAud() - ";

	if( !SendEnableCmdForSecAudToSPOT() )
	{
		LOG(eKeypad, eError, "%s%s", pcMethod, "Could not send asynchronous Enable message to SPOT.\n");
		return false;
	}

	LOG(eKeypad, eTrace, "%s%s", pcMethod, "SPOT keypad enable message has been queued to SPOT.\n");
	return true;
}


bool GKeypadService::DisableRequest()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GKeypadService::DisableRequest() - ";

	if( !SendDisableCmdToSPOT() )
	{
		LOG(eKeypad, eError, "%s%s", pcMethod, "Could not send asynchronous Disable message to SPOT.\n");
		return false;
	}

	LOG(eKeypad, eTrace, "%s%s", pcMethod, "SPOT keypad Disable message has been queued to SPOT.\n");
	return true;
}

bool GKeypadService::SendEnableCmdToSPOT()
{
	UINT8 aryu8EnableKBCmd[KB_ENABLE_CMD_LEN];

	if( GGlobalSystemInformation::GetInstance()->isVGDUnit())
		memcpy(aryu8EnableKBCmd, mst_aryu8EnableKBCmdVGD, KB_ENABLE_CMD_LEN);
	else
		memcpy(aryu8EnableKBCmd, mst_aryu8EnableKBCmd, KB_ENABLE_CMD_LEN);
	aryu8EnableKBCmd[KB_ENABLE_WIN_ID_IDX] = eWinId_KeypadMatrix;
	aryu8EnableKBCmd[KB_ENABLE_TAG_ID_IDX] = KB_DATAENTRY_TAG_ID;

	GSPOTMessage oMsg(aryu8EnableKBCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

//This function is kept in case of future use

/*bool GKeypadService::SendNonSecureEnableCmdToSPOT()
{
	UINT8 aryu8EnableKBCmd[KB_ENABLE_CMD_LEN];

	if( GGlobalSystemInformation::GetInstance()->isVGDUnit())
		memcpy(aryu8EnableKBCmd, mst_aryu8EnableKBCmdVGD, KB_ENABLE_CMD_LEN);
	else
		memcpy(aryu8EnableKBCmd, mst_aryu8EnableKBCmd, KB_ENABLE_CMD_LEN);
	aryu8EnableKBCmd[KB_ENABLE_AU_ENTER_IDX] = 0x00;
	aryu8EnableKBCmd[KB_ENABLE_WIN_ID_IDX] = eWinId_KeypadMatrix;
	aryu8EnableKBCmd[KB_ENABLE_TAG_ID_IDX] = KB_DATAENTRY_TAG_ID;
	aryu8EnableKBCmd[17] =0x01;

	GSPOTMessage oMsg(aryu8EnableKBCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}*/

bool GKeypadService::SendEnableCmdForSecAudToSPOT()
{
	UINT8 aryu8EnableKBCmd[KB_ENABLE_CMD_LEN];

	if( GGlobalSystemInformation::GetInstance()->isVGDUnit())
		memcpy(aryu8EnableKBCmd, mst_aryu8EnableKBCmdVGD, KB_ENABLE_CMD_LEN);
	else
		memcpy(aryu8EnableKBCmd, mst_aryu8EnableKBCmd, KB_ENABLE_CMD_LEN);
	aryu8EnableKBCmd[KB_ENABLE_WIN_ID_IDX] = eWinId_SecureAudio;
	aryu8EnableKBCmd[KB_ENABLE_TAG_ID_IDX] = KB_DATAENTRY_TAG_ID;

	GSPOTMessage oMsg(aryu8EnableKBCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}


bool GKeypadService::SendDisableCmdToSPOT()
{
	UINT8* pu8DisableKBCmd = const_cast<UINT8*>(mst_aryu8DisableKBCmd);
	GSPOTMessage oMsg(pu8DisableKBCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}
