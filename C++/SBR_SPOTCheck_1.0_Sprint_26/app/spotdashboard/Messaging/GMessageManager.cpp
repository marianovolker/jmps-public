
#include "GMessageManager.h"

#include <cstring>

#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "SPOT/SPOTProtocolInterface/GSPOTTransportLayer.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"

#include "Services/MainUnit/GMainUnitService.h"
#include "Services/SecurityModule/GSecurityModuleService.h"
#include "Services/CardReader/GCardReaderService.h"
#include "Services/Keypad/GKeypadService.h"

// These are not needed in this release
//#include "Services/ContactlessReader/GContactlessReaderService.h"
//#include "Services/Printer/GPrinterService.h"

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"

using namespace GSpotDefs;

GMessageManager* GMessageManager::mst_pSoleInstance(NULL);
volatile bool 	 GMessageManager::mst_blExitApplication = false;
GMutex GMessageManager::m_createInstanceMutex;
GMutex GMessageManager::m_InstanceMutex;


GMessageManager::GMessageManager()
{
	m_pSession = GSPOTSession::GetSoleInstance();
	m_pSession->AddSessionListener(this);
}

GMessageManager::~GMessageManager()
{
}

GMessageManager* GMessageManager::GetSoleInstance(void)
{
	GMutexLock ml(m_createInstanceMutex);

	if(!mst_pSoleInstance)
	{
		mst_pSoleInstance = new GMessageManager();
	}

	return (mst_pSoleInstance);
}

void GMessageManager::DestroySoleInstance(void)
{
	GMutexLock ml(m_createInstanceMutex);

	if(mst_pSoleInstance)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

bool GMessageManager::isExitApplicationRequested()
{
	GMutexLock ml(m_InstanceMutex);
	return mst_blExitApplication;
}

void GMessageManager::RequestExitApplication()
{
	GMutexLock ml(m_InstanceMutex);
	mst_blExitApplication = true;

	GSPOTTransportLayer* pSPOTTransportLayer = GSPOTTransportLayer::GetSoleInstance();
	if( pSPOTTransportLayer )
	{
		pSPOTTransportLayer->Stop();
	}
}

bool GMessageManager::SendCommandToSPOT(GSPOTMessage* pMsg)
{
   return m_pSession->SendSPOTCommand(pMsg);
}


/*
 *
 * NOTIFICATION CLASIFICATIONS METHODS
 *
 */

bool GMessageManager::isPinpadSystemStatusNotification(GSPOTMessage* pMessage)
{
	if(!pMessage)
	{
		return false; // no message is supplied
	}

	if( pMessage->GetAppId() != eSPOT_PP_ANSWER )
	{
		return false; // unexpected application ID
	}

	if( pMessage->GetCommandCode() != eSPOT_PP_SYSTEM )
	{
		return false; // unexpected command category
	}

	if( pMessage->GetSubCommandCode() != eSPOT_PP_SYS_STATUS_INFO )
	{
		return false; // non device status notification
	}

	return true;
}

/* This is not needed in this release.
bool GMessageManager::isDiagnosticInfoNotification(GSPOTMessage* pMessage)
{
	if(!pMessage)
	{
		return false; // no message is supplied
	}

	if( pMessage->GetAppId() != eSPOT_PP_ANSWER )
	{
		return false; // unexpected application ID
	}

	if( pMessage->GetCommandCode() != eSPOT_PP_SYSTEM )
	{
		return false; // unexpected command category
	}

	if( pMessage->GetSubCommandCode() != eSPOT_PP_SYS_DIAG_INFO )
	{
		return false; // non device status notification
	}

	return true;
}
*/

bool GMessageManager::isServiceMessage(GSPOTMessage* pMessage)
{
	if(!pMessage)
	{
		return false; // no message is supplied
	}

	if( pMessage->GetAppId() == eSPOT_PP_ANSWER )
	{
		if( pMessage->GetCommandCode() == eSPOT_PP_RESOURCES )
		{
		    return true;
		}

		if( pMessage->GetCommandCode() == eSPOT_PP_CARDREADER )
		{
			return true;
		}

		if( pMessage->GetCommandCode() == eSPOT_PP_KEYBOARD )
		{
			return true;
		}

	}
	/* This is not needed in this release.
	else if( pMessage->GetAppId() == eSPOT_OPT_ANSWER )
	{
		if( pMessage->GetCommandCode() == eSPOT_OPT_PRINTER )
		{
			return true;
		}

		if( pMessage->GetCommandCode() == eSPOT_OPT_CLESS )
		{
			return true;
		}
	}
	*/

	return false;
}


/*
 *
 * SPOT REPLY (notification) MESSAGE DISPATCHER
 *
 */

void GMessageManager::ProcessMessage(GSPOTMessage* pMessage)
{
	if( isExitApplicationRequested() )
	{
		return;
	}

	GMutexLock ml(m_InstanceMutex);

	if( GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(pMessage) )
	{
		GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();

		if( GGlobalSystemInformation::isHardwareInfoMessageReply(pMessage) )
		{
			pSysInfo->ParseHrwInfoReply(pMessage);
		}

		else if( GGlobalSystemInformation::isSoftwareInfoMessageReply(pMessage) )
		{
			pSysInfo->ParseSwfInfoReply(pMessage);
		}

		return;
	}

/*  These are not needed in this release.
	if( GGlobalSystemInformation::isOPTHardwareConfigMessageReply(pMessage) )
	{
		ProcessOPTHardwareConfigMessageReply(pMessage);
		return;
	}

	if (GGlobalSystemInformation::isOPTStatusInfoRequestMessageReply(pMessage))
	{
		ProcessOPTStatusInfoRequestMessageReply(pMessage);
		return;
	}

	if( isDiagnosticInfoNotification(pMessage) )
	{
		ProcessDiagnosticInfoNotification(pMessage);
		return;
	}
*/

	// service notification and messages are always sent to its consumers
	// because services have application life cycle
	if( isPinpadSystemStatusNotification(pMessage) )
	{
		ProcessSystemStatusNotification(pMessage);
		return;
	}

	if( isServiceMessage(pMessage) )
	{
		ProcessServiceMessage(pMessage);
		return;
	}
}


/*
 *
 * SPOT NOTIFICATION HANDLER METHODS
 *
 */

/* These are not needed in this release.
void GMessageManager::ProcessOPTHardwareConfigMessageReply(GSPOTMessage* pMessage)
{
	UINT8 u8aryBuffer[1024];
	memset(u8aryBuffer, 0, sizeof(u8aryBuffer));

	UINT16 u16Len = 0;

	u16Len = pMessage->GetTransmitBuffer(u8aryBuffer);
	if(u16Len > 0 )
	{
		char pcPrintBuffer[1024];

		GConversion::BytesToASCIIHexString(pcPrintBuffer, u8aryBuffer, u16Len);

		LOG(eMenuManager, eDebug, "******* Response to OPT Hardware Config: %s\n", pcPrintBuffer);
	}

	GGlobalSystemInformation::GetInstance()->ParseHardwareConfigReply(pMessage);

	LOG(eMenuManager, eDebug, "******* Printer serial number: \n");
	GConversion::LogBytesAsASCIIHexString((UINT8*)(GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_pcSerialNumber), OPT_SERIALNO_LEN);

	LOG(eMenuManager, eDebug, "******* Printer device type  : %02x \n",GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_u8DeviceType );

	LOG(eMenuManager, eDebug, "******* Printer dev functions: %02x %02x %02x %02x \n", GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_aryu8DevFunctions[0],
		GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_aryu8DevFunctions[1],
		GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_aryu8DevFunctions[2],
		GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_aryu8DevFunctions[3]);

	LOG(eMenuManager, eDebug, "******* Printer device model : \n");
	if(GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_pcDevModel != NULL)
	{
		GConversion::LogBytesAsASCIIHexString(reinterpret_cast<UINT8*>(GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_pcDevModel), OPT_SERIALNO_LEN);
		LOG(eMenuManager, eDebug, "******* Printer device model (ASCII) : %s\n",GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_pcDevModel );
	}

	return;
}

void GMessageManager::ProcessOPTStatusInfoRequestMessageReply(GSPOTMessage* pMessage)
{
	UINT8 u8aryBuffer[1024];
	memset(u8aryBuffer, 0, sizeof(u8aryBuffer));
	UINT16 u16Len = 0;

	u16Len = pMessage->GetTransmitBuffer(u8aryBuffer);
	if (u16Len > 0)
	{
		char pcPrintBuffer[1024];
		GConversion::BytesToASCIIHexString(pcPrintBuffer, u8aryBuffer, u16Len);
		LOG(eMenuManager, eDebug, "******* Response to OPT System OTI Request: %s\n", pcPrintBuffer);
	}

	GGlobalSystemInformation::GetInstance()->ParseOPTStatusInfoRequestReply(pMessage);

	ProcessSystemStatusNotification(pMessage);

	return;
}
*/

bool GMessageManager::ProcessSystemStatusNotification(GSPOTMessage* pMsg)
{
	bool   blret = false;
	UINT8  aryu8NotificationData[32];
	UINT16 u16NotificationLength = pMsg->GetDataLen();
	int    nItems = 0;

	if( u16NotificationLength < 6)
		return false; // not enough data for one device status report

	pMsg->GetData(aryu8NotificationData, 32);

	nItems = aryu8NotificationData[1];

	if( nItems == 0 )
		return false; // no information items

	int iItemNo = 0;
	UINT8* pb = &aryu8NotificationData[2]; // byte pointer to scan items
	while( iItemNo < nItems && (pb - aryu8NotificationData) < u16NotificationLength )
	{
		bool bl = false;
		int iDeviceCode;
		UINT8 u8Status, u8StatusEx, u8Error;

		iDeviceCode = *pb++;
		u8Status    = *pb++;
		u8StatusEx  = *pb++;
		u8Error     = *pb++;

		if( pMsg->GetAppId() == eSPOT_PP_ANSWER )
		{
			switch( iDeviceCode )
			{
				case eSPOT_SYS_MAIN:
					bl = GMainUnitService::GetService()->ProcessSystemStatusNotification(u8Status, u8StatusEx, u8Error);
					blret = blret || bl; // one service successful is enough
					break;

				case eSPOT_SYS_SECURITYMODULE:
					bl = GSecurityModuleService::GetService()->ProcessSystemStatusNotification(u8Status, u8StatusEx, u8Error);
					blret = blret || bl; // one service successful is enough
					break;

				case eSPOT_SYS_CARDREADER:
					bl = GCardReaderService::GetService()->ProcessSystemStatusNotification(u8Status, u8StatusEx, u8Error);
					blret = blret || bl; // one service successful is enough
				break;

				case eSPOT_SYS_KEYBOARD:
					bl = GKeypadService::GetService()->ProcessSystemStatusNotification(u8Status, u8StatusEx, u8Error);
					blret = blret || bl; // one service successful is enough
				break;

				default:
				break;
			}
		}
/*  These are not needed in this release.
		else if( pMsg->GetAppId() == eSPOT_OPT_ANSWER )
		{
			switch( iDeviceCode )
			{
				case eSPOT_SYS_PRINTER:
					bl = GPrinterService::GetService()->ProcessOPTSystemStatusNotification(u8Status, u8StatusEx, u8Error);
					blret = blret || bl; // one service successful is enough
				break;

				case eSPOT_SYS_CLESS:
					bl = GContactlessReaderService::GetService()->ProcessSystemStatusNotification(u8Status, u8StatusEx, u8Error);
					blret = blret || bl; // one service successful is enough
					break;

				default:
				break;
			}
		}
*/

		iItemNo++;
	}

	return blret;
}

/* This is not needed in this release.
bool GMessageManager::ProcessDiagnosticInfoNotification(GSPOTMessage* pMsg)
{
	int  nret = 0;

	// The purpose of the next block of code is for printing and debugging.
	UINT16 u16DiagnosticInfoLength = pMsg->GetDataLen();
	UINT8* paryu8DiagnosticInfoData = new UINT8[u16DiagnosticInfoLength];
	pMsg->GetData(paryu8DiagnosticInfoData, u16DiagnosticInfoLength);
	GConversion::LogBytesAsASCIIHexString((const UINT8*)paryu8DiagnosticInfoData, u16DiagnosticInfoLength);
	delete[] paryu8DiagnosticInfoData;

	nret += GCardReaderService::GetService()->ParseDiagnosticInfoReply(pMsg);
	nret += GKeypadService::GetService()->ParseDiagnosticInfoReply(pMsg);

	return (nret>0);
}
*/

/*
 *
 * SPOT APLICATION REPLY HANDLER METHODS
 *
 */

bool GMessageManager::ProcessServiceMessage(GSPOTMessage* pMsg)
{
	bool blret = false;
	int iAppId = pMsg->GetAppId();
	int iDevice = pMsg->GetCommandCode();

	if( iAppId == eSPOT_PP_ANSWER ) // services talking with Pinpad application
	{
		switch (iDevice)
		{
			case eSPOT_PP_CARDREADER:
				if (GCardReaderService::isCardReaderReply(pMsg, true))
				{
				    blret = GCardReaderService::GetService()->ProcessCardReaderReply(pMsg);
				}
			break;

			case eSPOT_PP_KEYBOARD:
				if (GKeypadService::isKeypadReply(pMsg, true))
				{
				    blret = GKeypadService::GetService()->ProcessKeypadReply(pMsg);
				}
			break;

			default:
			break;
		}
	}
/*  These are not needed in this release.
	else if( iAppId == eSPOT_OPT_ANSWER )
	{
		switch (iDevice)
		{
			case eSPOT_OPT_PRINTER:
				if (GPrinterService::isPrinterReply(pMsg, true))
				{
					blret = GPrinterService::GetService()->ProcessPrinterReply(pMsg);
				}
			break;

			case eSPOT_OPT_CLESS:
				if (GContactlessReaderService::isContactlessReaderReply(pMsg, true))
				{
					blret = GContactlessReaderService::GetService()->ProcessContactlessReaderReply(pMsg);
				}
			break;

			default:
			break;
		}
	}
*/
	return blret;
}

void GMessageManager::NotifyError(GSpotDefs::eSPOTTransportLayerError eSessionError)
{
	LOG(eCommon, eError,
	    "GMessageManager::NotifyError() - Session reported error: %s %d ./n"
	    "eSessionError =", eSessionError);
}
