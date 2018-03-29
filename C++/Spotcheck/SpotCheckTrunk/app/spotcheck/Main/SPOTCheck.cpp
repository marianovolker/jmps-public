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

#include "SPOTCheck.h"

// GLogger
#include "Common/Util/DEBUGOUT.h"

// GHardware
#include "GHardware.h"
// SPOT Protocol interface
#include "SPOT/SPOTProtocolInterface/GSPOTTransportLayer.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"

#include "Services/Printer/GPrinterService.h"
#include "Services/Trind/GTrindService.h"
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
#include "Menus/FeatureConnector/GFeatureConnectorStatistics.h"
#include "Config/GSPOTCheckIni.h"

// String functions
#include <string>

// Input Stream functions
#include <iostream>

#include "Version/version.h"
#include "Version/buildinfo.h"
#include "Version/product.h"

#include <unistd.h>
#include <fstream>
#include <string.h>
#include <dirent.h>

GSPOTSession* CreateSPOTSession(const char* spot_ip)
{
	const UINT8 CMaxAttempts(100);		//Changed no. of attempts for VGD.

	GSPOTTransportLayer::CreateInstance(spot_ip);
	GSPOTTransportLayer* pSPOTTransportLayer = GSPOTTransportLayer::GetSoleInstance();

	GSPOTSession::CreateInstance(pSPOTTransportLayer);
	GSPOTSession *pSPOTSession = GSPOTSession::GetSoleInstance();
	GSPOTSession::SetSessionID(0x82);

	pSPOTSession->Connect(false);
	for (UINT8 ui8Attempts = 1; !pSPOTSession->IsConnected() && ui8Attempts <= CMaxAttempts; ui8Attempts++)
	{
		GSystem::Sleep(3000);

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
	pLogConfig->SetRunningMode(pSPOTCheckIni->GetRunningMode());
	GLogger::CreateInstance(pLogConfig);

	if(pSPOTCheckIni->GetRunningMode() == eInjected)
	{
		GSystem::Sleep(30000);
	}

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



	// create cless (contactless) reader service
	GContactlessReaderService::CreateInstance(pSession);

	// create bcs (barcode scanner) service
	GBarcodeScannerService::CreateInstance(pSession);

	// create keypad service
	GKeypadService::CreateInstance(pSession);

	// create display service
	GDisplayService* pDisplayService = GDisplayService::CreateInstance(pSession);

	GTrindService::CreateInstance(pSession);

	GFeatureConnectorStatistics::GetInstance();

	// create session timer service
	GSessionTimerService::CreateInstance(pSession);

	GSystem::Sleep(100);

	// create menu manager
	pMenuMgr = GMenuManager::GetSoleInstance();

	GSystem::Sleep(100);


	//bool blEmbedded(IS_EMBEDDED_APP);
	bool blExternal(IS_EXTERNAL_APP);
	//if( ! blEmbedded )
	if(blExternal)
	{
		int iWindows = pDisplayService->UpdateTotalWindowsOnSPOT();
		if(iWindows < 0)
		{
			LOG(eUnCategorized, eWarning, "Timeout waiting for the number of windows used!\n", iWindows);
		}
		else if(iWindows > 0)
		{
			LOG(eUnCategorized, eError, "%s %d\n\n%s\n%s\n\n%s\n\n",
				"Another external application is using the SPOT. Total open windows:", iWindows,
				"Please re-run SPOTCheck as the ONLY external application",
				"(STOP CRIND or other external applications if necessary).",
				"Press a key followed by enter to exit the application.");

			std::cin.clear();
			cin.ignore(2);

			SPOTCheck_exit(pSession, 1);
			return 2;
		}
	}

	// The global system information is requested and
	// the menu manager catches this command reply to the global object.
	if( !pSysInfo->SendSyncHwrInfoCommandToSPOT() )
	{
		LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Hardware Info request.");
		return 1;
	}

	if( GGlobalSystemInformation::GetInstance()->isVGDUnit())
	{
		if(!pSPOTCheckIni->ReadSecureAudioSCRId())
		{
			return 1; //Could not read resource Id for Secure Audio
		}

		if( !pSysInfo->SendSyncCRFwrCommandToSPOT() )
		{
			LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "CardReader Firmware Version Request.");
			return 1;
		}
		if( !pSysInfo->SendSyncUPMAppVerCommandToSPOT() )
		{
			LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "UPM App Version Request.");
			return 1;
		}

		if( !pSysInfo->SendSyncUPMBuildVerCommandToSPOT() )
		{
			LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "UPM Build Version Request.");
			return 1;
		}
	}
	if( GGlobalSystemInformation::GetInstance()->isVGDUnit())
	{
		if( !pSysInfo->SendSyncKeypadFWRCommandToSPOT() )
		{
				LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Keypad firmware version request.");
				return 1;
		}
	}
	else
	{
		if( !pSysInfo->SendSyncSwrInfoCommandToSPOT() )
		{
			LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Software Info request.");
			return 1;
		}
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

	if( GGlobalSystemInformation::GetInstance()->isVGDUnit())
	{
		if( !pSysInfo->SendSyncPkgInfoCommandToSPOT() )
		{
			LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Package info request.");
			return 1;
		}
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
	// create card reader service
	GCardReaderService::CreateInstance(pSession);

	GMainMenu* pMainmenu = pMenuMgr->Initialize();

	GResourcesService::CreateInstance(pSession);

	LOG(eUnCategorized, eDebug, "******************* Initializing resource service ************************\n");

	GResourcesService* pResourceService = GResourcesService::GetService();

	if( pResourceService->Initialize() && pResourceService->UploadKeypadResourcesToSPOT() && pResourceService->isLastUploadProcessOk() )
	{
		LOG(eUnCategorized, eInfo, "SPOTCheck - Completed resource upload!\n");
		pMainmenu->UnLockForNormalUse();
	}
	else
	{
		LOG(eUnCategorized, eFatal, "%s%s\n", pcInitError, "Resource Upload!");
		pMainmenu->ResourceFatalErrorLock();
	}

	// Panasonic CLess readers need this extra disable to remain disabled.
	GContactlessReaderService::GetService()->DisableDevice();
	return 0;
}


int DeleteFolder(const char *dirname)
{

	DIR *dir;
	struct dirent *entry;
	char path[1024] = {'\0'};


	dir = opendir(dirname);
	if (dir == NULL)
	{
		LOG(eUnCategorized, eDebug,"DeleteFolder dir NULL\n");
		return -1;
	}

	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
		{
			snprintf(path, (size_t) 1024, "%s/%s", dirname, entry->d_name);
			LOG(eUnCategorized, eDebug,"Folder entry : %s\n",entry->d_name);
//			if (entry->d_type == DT_DIR)
//			{
//				LOG(eUnCategorized, eDebug,"DeleteFolder recursive dir NULL\n");
//				DeleteFolder(path);
//
//			}
//			else
			{
				unlink(path);
				LOG(eUnCategorized, eDebug,"DeleteFolder unlink :%s\n",path);
			}
		}

	}
	closedir(dir);

	return rmdir(dirname);
}



/**SelfDelete : This function is called in the Injected Mode to delete/modify all the SPOTCheck related files .
 * @param IRomfsNumber:  Romfs slot number where SpotCheck Romfs package is stored.
 * @param pcRomfsDir:  Path of the Directory where the Romfs package is stored. (For M3/M5)
 * @param pLogFilePath : Path of the Spotcheck Log file for m3/m5
 * @return bool : returns true
 */

bool SelfDelete(int iRomfsNumber, const char* pcRomfsDir, const char* pcLogFilePath, const char* pcSpotCheckfsPath)
{
	char        pcSysmanKey[20]       = {"\0"};
	char        pcVersionFilePath[100]= {'\0'};
	char        pcRomfsFilePath[100]  = {'\0'};
	char        pcRomfsSysmanPath[100]= {'\0'};
	char        pcSPCIniPath[100]     = {'\0'};

	snprintf(pcSysmanKey, sizeof(pcSysmanKey), pcSysmanKeyholder,iRomfsNumber);
	snprintf(pcVersionFilePath, sizeof(pcVersionFilePath), "%sversion-ROMfs%d", pcConfigPathM3,iRomfsNumber);

	snprintf(pcRomfsFilePath, sizeof(pcRomfsFilePath), "%sROMfs%d", pcRomfsDir,iRomfsNumber);

	snprintf(pcRomfsSysmanPath, sizeof(pcRomfsSysmanPath), "%ssysman-ROMfs%d.ini", pcConfigPathM3,iRomfsNumber);
	snprintf(pcSPCIniPath, sizeof(pcSPCIniPath), "%s%s",pcConfigPathM3,INI_SPOTCHECK_CONFIG_DEFAULT_FILE);


	int iret = remove(pcVersionFilePath);
	LOG(eUnCategorized, eDebug,"Removing file |%s| answer: %d\n",pcVersionFilePath,iret);

	iret = remove(pcRomfsFilePath);
	LOG(eUnCategorized, eDebug,"Removing file |%s| answer: %d\n",pcRomfsFilePath,iret);

	iret = remove(pcRomfsSysmanPath);
	LOG(eUnCategorized, eDebug,"Removing file |%s| answer: %d\n",pcRomfsSysmanPath,iret);


	//Remove  SPOTCheck.ini
	iret = remove(INI_SPOTCHECK_CONFIG_DEFAULT_FILE);
	LOG(eUnCategorized, eDebug,"Removing file |%s| answer: %d\n",INI_SPOTCHECK_CONFIG_DEFAULT_FILE,iret);

	//For deleting the log file
	iret =remove(pcLogFilePath);
	LOG(eUnCategorized, eDebug,"Removing file |%s| answer: %d\n",pcLogFilePath,iret);

	//Deleting spotcheckfs
	iret =DeleteFolder(pcSpotCheckfsPath);
	LOG(eUnCategorized, eDebug,"Removing Folder |%s| answer: %d\n",pcSpotCheckfsPath,iret);

	/* For making SPOTCheck untraceable more we might have to delete more files in future.
	 Reboot/PowerFailure :As of now, if during a test session, there is a Power failure or reboot command
	 is sent or for any reason spotcheck gets killed before complete execution of this function,
	 all or some of these files would remain on SPOT.
	 */
	return true;
}

/**
 * @param pSession:  Pointer to current session object.
 * @param iShortWay: Default value 0. If set not zero, then some exit stages will not
 *                   be performed. The higher this parameter is, the more exit stages will
 *                   be discarded. Currently the only stage discarded is the resource
 *                   deletion phase, which happens when this value is non zero.
 *
 * @return integer : Zero if exit phase was performed OK, non zero if some error happens.
 */


int SPOTCheck_exit(GSPOTSession *pSession, int iShortWay /* = 0 */)
{
	if(iShortWay == 0)
	{
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

		GResourcesService::DestroyInstance();
	}

	GSPOTSession *pSPOTSession;
	pSPOTSession = GSPOTSession::GetSoleInstance();

	GGlobalSystemInformation* pSysInfo = GGlobalSystemInformation::CreateInstance(pSession);
	GSPOTCheckIni* pSPOTCheckIni = GSPOTCheckIni::GetSoleInstance();

	int iRunningMode = pSPOTCheckIni->GetRunningMode();

	LOG(eUnCategorized, eDebug, "******************* Spotcheck_Exit: Running Mode-> %d *********************\n",iRunningMode);
	if((iRunningMode == eInjected || iRunningMode == eDummyInjected ))
	{
		int iRomfsNumber = pSPOTCheckIni->GetRomfsNumber();
		const char* pcLogFilePath = GLogger::GetInstance()->GetConfig()->GetLogFilePath();

		LOG(eUnCategorized, eDebug, "******************* Spotcheck_Exit: Injected Mode ************************\n");
		if(pSysInfo->isM3Unit())
		{
			LOG(eUnCategorized, eDebug, "Spotcheck_exit: M3 platform \n");
			SelfDelete(iRomfsNumber, pcRomfsDirM3, pcLogFilePath, pcSpotCheckfsPathM3);
		}
		else if(pSysInfo->isM5Unit())
		{
			LOG(eUnCategorized, eDebug, "Spotcheck_exit: M5 platform \n");
			SelfDelete(iRomfsNumber, pcRomfsDirM5, pcLogFilePath, pcSpotCheckfsPathM5);
		}
		else if(pSysInfo->isVGDUnit())                 
		{
			LOG(eUnCategorized, eDebug, "Spotcheck_exit: M7 platform \n");
		}

		free(const_cast<char*>(pcLogFilePath));

			const UINT8 aryu8ResetCmd[7] = {0x00, 0x07, 0x00, 0x00, 0x06, 0x00, 0x00};
			if( pSession != NULL )
			{
				UINT8* pu8ResetCmd = const_cast<UINT8*>(aryu8ResetCmd);
				GSPOTMessage oMsg(pu8ResetCmd);
				pSession->SendSPOTCommand( &oMsg );
			}
	}
	string strIPAddr( GGlobalSystemInformation::GetInstance()->GetIPAddress() );
	GSPOTTransportLayer* pSPOTTransportLayer;
	//GSPOTSession *pSPOTSession;
	GMenuManager::DestroySoleInstance();
 	GKeypadService::DestroyInstance();
	GCardReaderService::DestroyInstance();
	GTrindService::DestroyInstance();
	GFeatureConnectorStatistics::DestroyInstance();
	GContactlessReaderService::DestroyInstance();
	GPrinterService::DestroyInstance();
	GDisplayService::DestroyInstance();
	//GGlobalSystemInformation::DestroyInstance();
	GSessionTimerService::DestroyInstance();

	pSPOTTransportLayer = GSPOTTransportLayer::GetSoleInstance();
	//pSPOTSession = GSPOTSession::GetSoleInstance();

	bool bIsVGD = GGlobalSystemInformation::GetInstance()->isVGDUnit();

	if(bIsVGD)
	{
		UINT8 aryu8ResetCmd[7] = {0x00, 0x07, 0x00, 0x00, 0x06, 0x00, 0x00};
		if( pSession != NULL )
		{
			GSPOTMessage oMsg(aryu8ResetCmd);
			pSession->SendSPOTCommand( &oMsg );

		}
	}

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


	if(!bIsVGD)
	{
		GGlobalSystemInformation::DestroyInstance();
	}
	GSystem::Sleep(200);
	GSPOTSession::Destroy();
	GSPOTTransportLayer::DestroySoleInstance();
	GSPOTCheckIni::Destroy();

	GLoggerConfig* pCfg = GLogger::GetInstance()->GetConfig();
	GLogger::DestroyInstance();
	delete pCfg;
	
	if(bIsVGD)
	{
		while(true)
		GSystem::Sleep(5000);
	}

	return 0;
}
