//==============================================================================
//
// File: SPOTCheck.cpp
//
// Created: 10/11/12
//
// Author: Vance Tate
//
// Description:
//
//	This file is the main driver used to build the executible image for 
//	the SPOTCheck utility, which is an embedded diagnostic tool to be used
//	on various SPOT platforms where CRIND Diagnostics don't/can't provide
//	equivalent functionality.
//
//==============================================================================

// GLogger
#include "Common/Util/DEBUGOUT.h"

// GHardware
#include "GHardware.h"
// SPOT Protocol interface
#include "SPOT/SPOTProtocolInterface/GSPOTTransportLayer.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"

#include "Services/Printer/GPrinterService.h"
#include "Services/CardReader/GCardReaderService.h"
#include "Services/ContactlessReader/GContactlessReaderService.h"
#include "Services/BarcodeScanner/GBarcodeScannerService.h"
#include "Services/Keypad/GKeypadService.h"
#include "Services/Display/GDisplayService.h"
#include "Services/Resource/GResourcesService.h"
#include "Services/SessionTimer/GSessionTimerService.h"

// Menu functions
#include "Menus/GMenuManager.h"
#include "Menus/MainMenus/GMainMenu.h"
#include "Config/GSPOTCheckIni.h"

// String functions
#include <string>

#include "Version/version.h"
#include "Version/buildinfo.h"
#include "Version/product.h"


GSPOTSession* CreateSPOTSession(const char* spot_ip)
{
	const UINT8 CMaxAttempts(10);

	GSPOTTransportLayer::CreateInstance(spot_ip);
	GSPOTTransportLayer* pSPOTTransportLayer = GSPOTTransportLayer::GetSoleInstance();

	GSPOTSession::CreateInstance(pSPOTTransportLayer);
	GSPOTSession *pSPOTSession = GSPOTSession::GetSoleInstance();
	GSPOTSession::SetSessionID(0x82);

	pSPOTSession->Connect(false);
	for (UINT8 ui8Attempts = 1; !pSPOTSession->IsConnected() && ui8Attempts <= CMaxAttempts; ui8Attempts++)
	{
		GSystem::Sleep(1500);
		pSPOTSession->Connect(false);
	}

	//if session is not yet connected it will keep trying to connect every two seconds
	if (!pSPOTSession->IsConnected())
	{
		pSPOTSession->Connect(true);
	}

	return pSPOTSession;
}

int SPOTCheck_init()
{
	const char *szMethodName = "SPOTCheck_init()";
	const char* pcInitError  = "SPOTCheck - FATAL ERROR in Initialization phase. Could not complete ";
	const char* pcIPAddr 	 = LOOPBACK_SPOT_IP;

	GSPOTSession *pSession = NULL;
	GMenuManager *pMenuMgr = NULL;

	// Prevent multiple startups.
	static bool FirstRun = true;
	if (!FirstRun)
	{
		return -1;
	}
	FirstRun = false;

	GSPOTCheckIni::CreateInstance();
	GSPOTCheckIni* pSPOTCheckIni = GSPOTCheckIni::GetSoleInstance();
	if( !pSPOTCheckIni->Initialize() )
	{
		return 1; // fatal errors reading configuration file
	}

	// Instantiate the logger
	GLoggerConfig* pLogConfig = new GLoggerConfig(pSPOTCheckIni->GetINIFilename());
	pLogConfig->Initialize();
	GLogger::CreateInstance(pLogConfig);


	//get configured IP address.
	pcIPAddr = pSPOTCheckIni->GetSPOTIpAddress();

	LOG(eUnCategorized, eInfo, "------------------------------------------------\n");
	LOG(eUnCategorized, eInfo, "Info: SPOTCheck version [%s]\n", gszVersion);
	LOG(eUnCategorized, eInfo, "------------------------------------------------\n");
	LOG(eUnCategorized, eInfo, "Info: Connecting to [%s]\n", pcIPAddr);
	LOG(eUnCategorized, eInfo, "------------------------------------------------\n");

	// Create the Transport Layer / Session objects
	pSession = CreateSPOTSession(pcIPAddr);

	// create global information object (ONLY HERE)
	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::CreateInstance(pSession);
	pSysInfo->SetIPAddress(pcIPAddr);

	// create printer service
	GPrinterService::CreateInstance(pSession);

	// create card reader service
	GCardReaderService::CreateInstance(pSession);

	// create cless (contactless) reader service
	GContactlessReaderService::CreateInstance(pSession);

	// create bcs (barcode scanner) service
	GBarcodeScannerService::CreateInstance(pSession);

	// create keypad service
	GKeypadService::CreateInstance(pSession);

	// create display service
	GDisplayService::CreateInstance(pSession);

	// create session timer service
	GSessionTimerService::CreateInstance(pSession);

	// create menu manager
	pMenuMgr = GMenuManager::GetSoleInstance();

	GSystem::Sleep(100);

	// Once the main menu is displayed, the global system information is requested and
	// the menu manager catches this command reply to the global object.
	if( !pSysInfo->SendSyncHwrInfoCommandToSPOT() )
	{
		LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Hardware Info request.");
		return 1;
	}


	if( !pSysInfo->SendSyncSwrInfoCommandToSPOT() )
	{
		LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Software Info request.");
		return 1;
	}

	if( !pSysInfo->SendSyncOPTHwrConfigCommandToSPOT() )
	{
		LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "OPT Hardware Info request.");
		return 1;
	}

	if( !pSysInfo->SendSyncOPTSystemInfoCommandToSPOT() )
	{
		LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "OPT System status request.");
		return 1;
	}

	LOG(eUnCategorized, eInfo, "Info: Connecting to SPOT serial number: [%s]\n", pSysInfo->GetSPOTInfo().m_pcSerialNumber);
	LOG(eUnCategorized, eInfo, "Info: Connecting to SPOT display model: [%s]\n", pSysInfo->GetAllHardwareInfo().m_DisplayInfo.m_pcDevModel);

	if (pSysInfo->isM3Unit())
	{
		LOG(eUnCategorized, eInfo, "Info: Connecting to SPOT type:  SPOT M3\n");
	}
	else
	{
		LOG(eUnCategorized, eInfo, "Info: Connecting to SPOT type:  SPOT M5\n");
	}

	GMainMenu* pMainmenu = pMenuMgr->Initialize();

	GResourcesService::CreateInstance(pSession);

	LOG(eUnCategorized, eDebug, "******************* Initializing resource service ************************\n");

	if (GResourcesService::GetService()->Initialize() && GResourcesService::GetService()->UploadKeypadResourcesToSPOT() && GResourcesService::GetService()->isLastUploadProcessOk())
	{
		LOG(eUnCategorized, eInfo, "SPOTCheck - Completed resource upload!\n");
		pMainmenu->UnLockForNormalUse();
	}
	else
	{
		LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Resource Upload!");
		pMainmenu->FatalErrorLock();
	}

	// Panasonic CLess readers need this extra disable to remain disabled.
	GContactlessReaderService::GetService()->DisableDevice();
	return 0;
}

int SPOTCheck_exit(GSPOTSession *pSession)
{
	string strIPAddr( GGlobalSystemInformation::GetInstance()->GetIPAddress() );

	if (GResourcesService::GetService()->DeleteKeypadResourcesToSPOT())
	{

		if (GResourcesService::GetService()->isLastUploadProcessOk())
		{
			LOG(eUnCategorized, eInfo, "!**** Completed resource deletion!\n");

		}
		else
		{
			//there was an error
			LOG(eUnCategorized, eFatal, "!**** Fatal Error in Resource Deletion phase. Could not complete resource deletion!\n");

		}
	}
	else
	{
		LOG(eUnCategorized, eInfo, "!**** Resource deletion not done or not needed. \n");

	}

	GSPOTTransportLayer* pSPOTTransportLayer;
	GSPOTSession *pSPOTSession;

	GResourcesService::DestroyInstance();
	GKeypadService::DestroyInstance();
	GCardReaderService::DestroyInstance();
	GContactlessReaderService::DestroyInstance();
	GPrinterService::DestroyInstance();
	GDisplayService::DestroyInstance();
	GGlobalSystemInformation::DestroyInstance();
	GSessionTimerService::DestroyInstance();
	GMenuManager::DestroySoleInstance();

	pSPOTTransportLayer = GSPOTTransportLayer::GetSoleInstance();
	pSPOTSession = GSPOTSession::GetSoleInstance();

	if( pSPOTSession )
	{
		pSPOTSession->Disconnect(true);
	}

	if( pSPOTTransportLayer )
	{
		pSPOTTransportLayer->CloseConnection();
		pSPOTTransportLayer->Stop();
		pSPOTTransportLayer->Join();
	}

	GSystem::Sleep(200);

	GSPOTSession::Destroy();
	GSPOTTransportLayer::DestroySoleInstance();

	GLoggerConfig* pCfg = GLogger::GetInstance()->GetConfig();
	GLogger::DestroyInstance();
	delete pCfg;

	return 0;
}
