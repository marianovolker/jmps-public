/*
 * GSecurityModuleService.cpp
 *
 *  Created on: 30/09/2013
 *      Author: gparis
 */

#include <cstring>
#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"

#include "GSecurityModuleService.h"

using namespace GSpotDefs;

GSecurityModuleService* GSecurityModuleService::mst_pSoleInstance = NULL;
GMutex                  GSecurityModuleService::mst_createInstanceMutex;

GSecurityModuleService::GSecurityModuleService(GSPOTSession* pSession)
	:  m_pSession(pSession)
{
	LOG(eSpot, eInfo, "Security Module started.\n");
}

GSecurityModuleService::~GSecurityModuleService()
{
	LOG(eSpot, eInfo, "Security Module Service stopped.\n");
}

GSecurityModuleService* GSecurityModuleService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(mst_createInstanceMutex);

	if(mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GSecurityModuleService(pSession);
	}

	return mst_pSoleInstance;
}

void GSecurityModuleService::DestroyInstance()
{
	GMutexLock ml(mst_createInstanceMutex);

	if(mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}


bool GSecurityModuleService::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	GMutexLock  ml(m_instanceMutex);
	eLogLevel   eLevel = eNotice;
	const char* pcStatus = NULL;
	const char* pcMethod = "GSecurityModuleService::ProcessSystemStatusNotification() -- ";

	LOG(eSpot, eDebug, "%s status %02X, stex %02X, error %02X \n", pcMethod, u8Status, u8StatusEx, u8Error);

	if(u8Status < 6)
	{
		if(u8Status == 0x00)
		{
			pcStatus = "IDLE";
		}
		else if(u8Status == 0x01)
		{
			pcStatus = "BUSY";
		}
		else if(u8Status == 0x02)
		{
			pcStatus = "recoverable ERROR";
			eLevel = eWarning;
		}
		else if(u8Status == 0x03)
		{
			pcStatus = "INOPERATIVE";
			eLevel = eError;
		}
		else if(u8Status == 0x04)
		{
			pcStatus = "STARTUP";
		}
		else // u8Status == 0x05
		{
			pcStatus = "firmware DOWNLOAD";
		}

		LOG( eSpot, eLevel, "%s Security Module reported %s state.\n", pcMethod, pcStatus );
	}
	else // other not common errors
	{
		LOG(eSpot, eError,
		    "%s Security Module reported state 0x%02X and error 0x%02X.\n",
		    pcMethod, u8Status, u8Error);
	}

	if( u8Error != 0x00 )
	{
		LOG(eSpot, eWarning, "SECURITY MODULE device reported ERROR code %d.\n", u8Error);
	}

	m_oRulesProcessor.ProcessSystemStatusNotification(u8Status, u8StatusEx, u8Error);

	return true;
}

