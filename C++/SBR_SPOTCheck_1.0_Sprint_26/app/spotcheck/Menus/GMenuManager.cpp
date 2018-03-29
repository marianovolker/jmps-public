#include "GMenuManager.h"

#include <cstring>

#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "Menus/Templates/GSoftKeysWindowTemplate.h"
#include "Menus/MainMenus/GMainMenu.h"


#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"

#include "SPOT/SPOTProtocolInterface/GSPOTDefs.h"
#include "SPOT/SPOTProtocolInterface/GSPOTTransportLayer.h"

#include "Services/CardReader/GCardReaderService.h"
#include "Services/ContactlessReader/GContactlessReaderService.h"
#include "Services/Keypad/GKeypadService.h"
#include "Services/Printer/GPrinterService.h"
#include "Services/Resource/GResourcesService.h"
#include "Services/SessionTimer/GSessionTimerService.h"

#include "Main/SPOTCheck.h"

using namespace GSpotDefs;

GMenuManager* GMenuManager::mst_pSoleInstance(NULL);
volatile bool GMenuManager::mst_blExitApplication = false;
GMutex GMenuManager::m_createInstanceMutex;

GMenuManager::GMenuManager()
  :  m_bShouldPopMenu(false)
{
	m_pSession = GSPOTSession::GetSoleInstance();
	m_pSession->AddSessionListener(this);
}

GMenuManager::~GMenuManager()
{
	// make sure to kill all objects on the stack
	while (!m_MenuStack.empty())
	{
		GAbsMenu* ptr = m_MenuStack.top();
		m_MenuStack.pop();
		delete ptr;
	}

	// Main menu window destruction.
	GSoftKeysWindowTemplate* pTemplate = GSoftKeysWindowTemplate::GetSoleInstance();
	GSPOTMessage* pMsg = pTemplate->GetDestroyMessage();
	SendCommandToSPOT(pMsg);

	// Exit window destruction
	pMsg = pTemplate->GetDestroyMessage(UINT8(eWinId_ExitScreen));
	SendCommandToSPOT(pMsg);

	// SoftKeysWindowTemplate destruction before exit.
	GSoftKeysWindowTemplate::DestroySoleInstance();
}

GMenuManager* GMenuManager::GetSoleInstance(void)
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GMenuManager();
	}

	return mst_pSoleInstance;
}

void GMenuManager::DestroySoleInstance(void)
{
	GMutexLock ml(m_createInstanceMutex);

	if(mst_pSoleInstance)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

bool GMenuManager::isExitApplicationRequested()
{
	GMutexLock ml(m_createInstanceMutex);
	return mst_blExitApplication;
}

void GMenuManager::RequestExitApplication()
{
	GMutexLock ml(m_createInstanceMutex);
	mst_blExitApplication = true;
}

/**
 * @note This method creates the main screen, holding all the menus, and the main menu.
 */
GMainMenu*  GMenuManager::Initialize()
{
	// the menus will expect a screen to already exist so draw the default
	InitializeScreen();
	GMainMenu* pMainMenu = new GMainMenu();
	PushMenu(pMainMenu);
	return pMainMenu;
}

void GMenuManager::InitializeScreen()
{
   // Instantiate and get reference to the main window/template
   GSoftKeysWindowTemplate* tmp = GSoftKeysWindowTemplate::GetSoleInstance();

   // create the spot message for creating the base window
   GSPOTMessage* msg = tmp->GetCreateMessage();

   SendCommandToSPOT(msg);
   delete msg;

   // create the spot message for showing the base window
   msg = tmp->GetShowMessage();

   SendCommandToSPOT(msg);
   delete msg;
}


/*
 *
 * MENU HANDLING METHODS
 *
 */

void GMenuManager::QueueMenuPop()
{
	GMutexLock lock(m_PopMutex);
	GSoftKeysWindowTemplate* pTemplate = GSoftKeysWindowTemplate::GetSoleInstance();

	// create the spot message for cleaning ONLY the used text tags
	GSPOTMessage* pMsg = pTemplate->GetUpdateTextClearedMessage();

	SendCommandToSPOT(pMsg);
	delete pMsg;

	m_bShouldPopMenu = true;
}

void GMenuManager::PopMenu()
{
	if (m_MenuStack.size() > 1)
	{
		// pop the menu ptr and delete the object when finished
		GAbsMenu* old = m_MenuStack.top();
		m_MenuStack.pop();
		delete old;

		// make sure we have a connection
		while (!m_pSession->IsConnected())
		{
			GSystem::Sleep(100);
		}
		// draw the next screen, now on top of the stack
		m_MenuStack.top()->DrawMenu();
	}
	else // size = 1 => only the main menu
	{
		// if asked to pop the final menu then we just exit the program
		mst_blExitApplication = true;
	}
}

void GMenuManager::PushMenu(GAbsMenu* pMenu)
{
	if (pMenu)
	{
		GSoftKeysWindowTemplate* pTemplate = GSoftKeysWindowTemplate::GetSoleInstance();

		// create the spot message for cleaning ONLY the used text tags
		GSPOTMessage* pMsg = pTemplate->GetUpdateTextClearedMessage();

		SendCommandToSPOT(pMsg);
		delete pMsg;

		// store the new menu pointer as the current
		m_MenuStack.push(pMenu);

		while (!m_pSession->IsConnected())
		{
			GSystem::Sleep(100);
		}

		pMenu->DrawMenu();
	}
}

bool GMenuManager::SendCommandToSPOT(GSPOTMessage* pMsg)
{
   return m_pSession->SendSPOTCommand(pMsg);
}


/*
 *
 * NOTIFICATION CLASIFICATIONS METHODS
 *
 */

bool GMenuManager::isPinpadSystemStatusNotification(GSPOTMessage* pMessage)
{
	if(!pMessage)
		return false; // no message is supplied

	if( pMessage->GetAppId() != eSPOT_PP_ANSWER )
		return false; // unexpected application ID

	if( pMessage->GetCommandCode() != eSPOT_PP_SYSTEM )
		return false; // unexpected command category

	if( pMessage->GetSubCommandCode() != eSPOT_PP_SYS_STATUS_INFO )
		return false; // non device status notification

	return true;
}

bool GMenuManager::isDiagnosticInfoNotification(GSPOTMessage* pMessage)
{
	if(!pMessage)
		return false; // no message is supplied

	if( pMessage->GetAppId() != eSPOT_PP_ANSWER )
		return false; // unexpected application ID

	if( pMessage->GetCommandCode() != eSPOT_PP_SYSTEM )
		return false; // unexpected command category

	if( pMessage->GetSubCommandCode() != eSPOT_PP_SYS_DIAG_INFO )
		return false; // non device status notification

	return true;
}

bool GMenuManager::isServiceMessage(GSPOTMessage* pMessage)
{
	if(!pMessage)
		return false; // no message is supplied

	if( pMessage->GetAppId() == eSPOT_PP_ANSWER )
	{
		if( pMessage->GetCommandCode() == eSPOT_PP_RESOURCES )
		    return true;

		if( pMessage->GetCommandCode() == eSPOT_PP_CARDREADER )
			return true;

		if( pMessage->GetCommandCode() == eSPOT_PP_KEYBOARD )
			return true;

	}
	else if( pMessage->GetAppId() == eSPOT_OPT_ANSWER )
	{
		if( pMessage->GetCommandCode() == eSPOT_OPT_PRINTER )
			return true;

		if( pMessage->GetCommandCode() == eSPOT_OPT_CLESS )
			return true;
	}

	return false;
}


/*
 *
 * SPOT REPLY (notification) MESSAGE DISPATCHER
 *
 */

void GMenuManager::ProcessMessage(GSPOTMessage* pMessage)
{
	GMutexLock ml(m_InstanceMutex);

	if( GGlobalSystemInformation::isPinpadSysInfoMessageGroupReply(pMessage) )
	{
		GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::GetInstance();

		if( GGlobalSystemInformation::isHardwareInfoMessageReply(pMessage) )
			pSysInfo->ParseHrwInfoReply(pMessage);

		else if( GGlobalSystemInformation::isSoftwareInfoMessageReply(pMessage) )
			pSysInfo->ParseSwfInfoReply(pMessage);

		return;
	}

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

	// service notification and messages are always sent to its consumers
	// because services have application life cycle
	if( isPinpadSystemStatusNotification(pMessage) )
	{
		ProcessSystemStatusNotification(pMessage);
		return;
	}

	if( isDiagnosticInfoNotification(pMessage) )
	{
		ProcessDiagnosticInfoNotification(pMessage);
		return;
	}

	if( isServiceMessage(pMessage) )
	{
		ProcessServiceMessage(pMessage);
		return;
	}

	// if the menu stack isn't empty send message to top menu object (displayed)
	if( !m_MenuStack.empty() )
	{
		// if the message is a softkey, then pass it in through the menu's soft key event handler
		if( (pMessage->GetAppId() == eSPOT_OPT_ANSWER)
		    && (pMessage->GetCommandCode() == eSPOT_OPT_TERMIO)
		    && (pMessage->GetSubCommandCode() == eSPOT_OPT_IO_KEYENABLE) )
		{
			UINT16 data_len = pMessage->GetDataLen();
			UINT8* data = new UINT8[data_len];

			pMessage->GetData(data, data_len);

			if (data[0] == 0x00 /* ack code ok */)
			{
				GSessionTimerService::GetService()->ResetTimer();
				m_MenuStack.top()->HandleSoftKeyEvent(data[1]/*keycode*/);
			}
			else
			{
				LOG(eMenuManager, eError, "GMenuManager::ProcessMessage - Soft Key ACK code error\n");
			}

			delete[] data;
		}
		else // very weird dispatching device messages here
		{    // (only BNA having no service yet)

			// if not soft key event, send the message to the current menu
			m_MenuStack.top()->HandleSPOTMessage(pMessage);
		}
	}

	// since we don't know if the request to pop the stack was prior to receiving
	//  the spot message, just let the menu decide how to handle the message,
	//  then execute the pop.  The menu should ignore if it's already asked to be
	//  popped off the queue
	m_PopMutex.Lock();

	if (m_bShouldPopMenu)
	{
		m_bShouldPopMenu = false;
		m_PopMutex.Unlock();
		PopMenu();
	}
	else
	{
		m_PopMutex.Unlock();
	}
}


/*
 *
 * SPOT NOTIFICATION HANDLER METHODS
 *
 */

void GMenuManager::ProcessOPTHardwareConfigMessageReply(GSPOTMessage* pMessage)
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
	GConversion::LogBytesAsASCIIHexString(reinterpret_cast<UINT8*>(GGlobalSystemInformation::GetInstance()->GetPrinterInfo().m_pcSerialNumber), OPT_SERIALNO_LEN);

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

void GMenuManager::ProcessOPTStatusInfoRequestMessageReply(GSPOTMessage* pMessage)
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

bool GMenuManager::ProcessSystemStatusNotification(GSPOTMessage* pMsg)
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

		iItemNo++;
	}

	return blret;
}

bool GMenuManager::ProcessDiagnosticInfoNotification(GSPOTMessage* pMsg)
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


/*
 *
 * SPOT APLICATION REPLY HANDLER METHODS
 *
 */

bool GMenuManager::ProcessServiceMessage(GSPOTMessage* pMsg)
{
	bool blret = false;
	int iAppId = pMsg->GetAppId();
	int iDevice = pMsg->GetCommandCode();

	if( iAppId == eSPOT_PP_ANSWER ) // services talking with Pinpad application
	{
		switch (iDevice)
		{
			case eSPOT_PP_RESOURCES:
				if (GResourcesService::isResourcesReply(pMsg, true))
				{
					blret = GResourcesService::GetService()->ProcessResourcesReply(pMsg);
				}
			break;

			case eSPOT_PP_CARDREADER:
				if (GCardReaderService::isCardReaderReply(pMsg, true))
				{
				    blret = GCardReaderService::GetService()->ProcessCardReaderReply(pMsg);
				}
			break;

			case eSPOT_PP_KEYBOARD:
				if (GKeypadService::isKeypadReply(pMsg, true))
				{
					GSessionTimerService::GetService()->ResetTimer();
				    blret = GKeypadService::GetService()->ProcessKeypadReply(pMsg);
				}
			break;

			default:
			break;
		}
	}
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

	return blret;
}

void GMenuManager::ProcessTimeoutEvent()
{
   // make sure we aren't waiting to transition to parent menu
   m_PopMutex.Lock();

   if (m_bShouldPopMenu)
   {
      m_bShouldPopMenu = false;
      m_PopMutex.Unlock();
      PopMenu();
   }

   m_PopMutex.Unlock();
}

void GMenuManager::NotifyError(GSpotDefs::eSPOTTransportLayerError eSessionError)
{
	LOG(eMenuManager, eError, "GMenuManager::NotifyError() - Session reported error: [eSessionError = [%d]] .\n", eSessionError);
}


