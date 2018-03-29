//==============================================================================
//
// File: SPOTDashboard.cpp
//
// Created: 26/08/13
//
// Author: Volker Mariano
//
// Description:
//
//	This file is the main driver used to build the executible image for 
//	the SPOTDashboard utility, which is an embedded diagnostic tool to be used
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
#include "Services/MainUnit/GMainUnitService.h"
#include "Services/SecurityModule/GSecurityModuleService.h"
#include "Services/CardReader/GCardReaderService.h"
#include "Services/Keypad/GKeypadService.h"

#include "Services/ContactlessReader/GContactlessReaderService.h"
#include "Services/Printer/GPrinterService.h"

#include "Messaging/GMessageManager.h"
#include "Config/SPOTDashboardIni.h"

// String functions
#include <string>

#include "Version/version.h"
#include "Version/buildinfo.h"
#include "Version/product.h"

GSPOTSession* CreateSPOTSession(const char* spot_ip)
{
	const UINT8 CMaxAttempts(3);

	GSPOTTransportLayer::CreateInstance(spot_ip);
	GSPOTTransportLayer* pSPOTTransportLayer = GSPOTTransportLayer::GetSoleInstance();

	GSPOTSession::CreateInstance(pSPOTTransportLayer);
	GSPOTSession *pSPOTSession = GSPOTSession::GetSoleInstance();
	GSPOTSession::SetSessionID(0x83);

	pSPOTSession->Connect(false);
	for (UINT8 ui8Attempts = 1; !pSPOTSession->IsConnected() && ui8Attempts <= CMaxAttempts; ui8Attempts++)
	{
		GSystem::Sleep(100);
		pSPOTSession->Connect(false);
	}

	//if session is not yet connected it will keep trying to connect every two seconds
	if (!pSPOTSession->IsConnected())
	{
		pSPOTSession->Connect(true);
	}

	return pSPOTSession;
}

int SPOTDashboard_init()
{
	const char *szMethodName = "SPOTDashboard_init()";
	const char* pcInitError = "SPOTDashboard - FATAL ERROR in Initialization phase. Could not complete ";
	const char* pcIPAddr = "127.0.0.1";

	GSPOTSession *pSession(NULL);
	GMessageManager *pMessageMgr(NULL);

	// Prevent multiple startups.
	SPOTDashboardIni::CreateInstance();
	if (!SPOTDashboardIni::GetSoleInstance()->Initialize())
	{
		return 1;
	}

	// Instantiate the logger
	GLoggerConfig* pLogConfig = new GLoggerConfig(SPOTDashboardIni::GetSoleInstance()->GetINIFilename());
	pLogConfig->Initialize();
	GLogger::CreateInstance(pLogConfig);

	//get configured IP address.
	pcIPAddr = SPOTDashboardIni::GetSoleInstance()->GetSPOTIpAddress();

	LOG(eUnCategorized, eInfo, "------------------------------------------------\n");
	LOG(eUnCategorized, eInfo, "Info: SPOTDashboard version [%s]\n", gszVersion);
	LOG(eUnCategorized, eInfo, "------------------------------------------------\n");
	LOG(eUnCategorized, eInfo, "Info: Connecting to [%s]\n", pcIPAddr);
	LOG(eUnCategorized, eInfo, "------------------------------------------------\n");

	// Create the Transport Layer / Session objects
	pSession = CreateSPOTSession(pcIPAddr);

	// create global information object
	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::CreateInstance(pSession);
	pSysInfo->SetIPAddress(pcIPAddr);

	// create main unit service
	GMainUnitService* pMainUnitSvc;
	pMainUnitSvc = GMainUnitService::CreateInstance(pSession);

	// create security module service
	GSecurityModuleService::CreateInstance(pSession);

	// create card reader service
	GCardReaderService::CreateInstance(pSession);

	// create keypad service
	GKeypadService::CreateInstance(pSession);

	// create printer service (not needed in this release)
	//GPrinterService::CreateInstance(pSession);

	// create cless (contactless) reader service (not needed in this release)
	//GContactlessReaderService::CreateInstance(pSession);

	pMessageMgr = GMessageManager::GetSoleInstance();

	GSystem::Sleep(100);

	pMainUnitSvc->SendStatusRequestCmdToSPOT();

	// Once the main menu is displayed, the global system information is requested and
	// the menu manager catches this command reply to the global object.
	if (!pSysInfo->SendSyncHwrInfoCommandToSPOT())
	{
		LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Hardware Info request.");
		return 1;
	}

	if (!pSysInfo->SendSyncSwrInfoCommandToSPOT())
	{
		LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Software Info request.");
		return 1;
	}

	LOG(eUnCategorized, eInfo, "Info: Connecting to SPOT serial number: [%s]\n", pSysInfo->GetSPOTInfo().m_pcSerialNumber);
	LOG(eUnCategorized, eInfo, "Info: Connecting to SPOT display model: [%s]\n", pSysInfo->GetAllHardwareInfo().m_DisplayInfo.m_pcDevModel);

	if (pSysInfo->isM3Unit())
	{
		LOG(eUnCategorized, eInfo, "Info: Connecting to SPOT type:  SPOT M3\n");

	}
	else if(pSysInfo->isM5Unit())
	{
		LOG(eUnCategorized, eInfo, "Info: Connecting to SPOT type:  SPOT M5\n");
	}

	else if(pSysInfo->isVGDUnit())
	{
		LOG(eUnCategorized, eInfo, "Info: Connecting to SPOT type:  SPOT M7\n");
	}

	return 0;
}

int SPOTDashboard_exit(GSPOTSession *pSession)
{
	GSPOTTransportLayer* pSPOTTransportLayer;
	GSPOTSession *pSPOTSession;

	GKeypadService::DestroyInstance();
	GCardReaderService::DestroyInstance();
	GMainUnitService::DestroyInstance();
	GSecurityModuleService::DestroyInstance();
	GGlobalSystemInformation::DestroyInstance();
	GMessageManager::DestroySoleInstance();

	pSPOTTransportLayer = GSPOTTransportLayer::GetSoleInstance();
	pSPOTSession = GSPOTSession::GetSoleInstance();
	pSPOTSession->Disconnect(true);
	pSPOTTransportLayer->CloseConnection();
	pSPOTTransportLayer->Stop();
	pSPOTTransportLayer->Join();

	GSystem::Sleep(200);

	GSPOTSession::Destroy();
	GSPOTTransportLayer::DestroySoleInstance();

	GLoggerConfig* pCfg = GLogger::GetInstance()->GetConfig();
	GLogger::DestroyInstance();
	delete pCfg;

	return 0;
}

int SPOTDashboard_Reconnect()
{
	GGlobalSystemInformation* pSysInfo;
	GMainUnitService* pMainUnitSvc;
	GSPOTTransportLayer* pSPOTTransportLayer;
	GSPOTSession *pSPOTSession;

	GKeypadService::DestroyInstance();
	GCardReaderService::DestroyInstance();
	GMainUnitService::DestroyInstance();
	GSecurityModuleService::DestroyInstance();
	GGlobalSystemInformation::DestroyInstance();
	GMessageManager::DestroySoleInstance();

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

	GSystem::Sleep(200);

	GSPOTTransportLayer::CreateInstance(SPOTDashboardIni::GetSoleInstance()->GetSPOTIpAddress());
	pSPOTTransportLayer = GSPOTTransportLayer::GetSoleInstance();

	GSPOTSession::CreateInstance(pSPOTTransportLayer);
	pSPOTSession = GSPOTSession::GetSoleInstance();
	GSPOTSession::SetSessionID(0x83);

	if( !pSPOTTransportLayer || !pSPOTSession )
	{
		return 1;
	}

	pSPOTSession->Connect(false);
	if (!pSPOTSession->IsConnected())
	{
		return 1;
	}
	GSystem::Sleep(100);

	pSysInfo = GGlobalSystemInformation::CreateInstance(pSPOTSession);
	pSysInfo->SetIPAddress(SPOTDashboardIni::GetSoleInstance()->GetSPOTIpAddress());
	pMainUnitSvc = GMainUnitService::CreateInstance(pSPOTSession);
	GSecurityModuleService::CreateInstance(pSPOTSession);
	GCardReaderService::CreateInstance(pSPOTSession);
	GKeypadService::CreateInstance(pSPOTSession);
	GMessageManager::GetSoleInstance();

	GSystem::Sleep(100);

	pMainUnitSvc->SendStatusRequestCmdToSPOT();

	GSystem::Sleep(100);

	if (!pSysInfo->SendSyncHwrInfoCommandToSPOT())
	{
		LOG(eUnCategorized, eFatal, "%s\n", "Hardware Info request.");
		return 1;
	}

	if (!pSysInfo->SendSyncSwrInfoCommandToSPOT())
	{
		LOG(eUnCategorized, eFatal, "%s\n", "Software Info request.");
		return 1;
	}

	return 0;
}
