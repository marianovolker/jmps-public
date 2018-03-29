/*
 * GMainUnitService.cpp
 *
 *  Created on: 30/09/2013
 *      Author: gparis
 */

#include <cstring>
#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"

#include "GMainUnitService.h"

using namespace GSpotDefs;

GMainUnitService* GMainUnitService::mst_pSoleInstance = NULL;
GMutex            GMainUnitService::mst_createInstanceMutex;

const UINT8       GMainUnitService::mst_aryu8SystemStatusRequestCmd[7] =
                  { 0x00, 0x07, 0x01, 0x00, 0x10, 0x01, 0xFF };


GMainUnitService::GMainUnitService(GSPOTSession* pSession)
	: m_pSession(pSession)
{
	LOG(eSpot, eInfo, "Main Unit Service started.\n");
}

GMainUnitService::~GMainUnitService()
{
	LOG(eSpot, eInfo, "Main Unit Service stopped.\n");
}

GMainUnitService* GMainUnitService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(mst_createInstanceMutex);

	if(mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GMainUnitService(pSession);
	}

	return mst_pSoleInstance;
}

void GMainUnitService::DestroyInstance()
{
	GMutexLock ml(mst_createInstanceMutex);

	if(mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}


bool GMainUnitService::SendStatusRequestCmdToSPOT()
{
	if(m_pSession == NULL )
	{
		return false;
	}

	const char*  pcMethod = "GMainUnitService::SendStatusRequestCmdToSPOT() - ";
	LOG(eSpot, eTrace, "%s%s", pcMethod, "Request system status message for all pinpad devices has been queued to SPOT.\n");

	GConversion::LogBytesAsASCIIHexString(mst_aryu8SystemStatusRequestCmd, sizeof(mst_aryu8SystemStatusRequestCmd));

	UINT8* pu8SysStatusCmd = const_cast<UINT8*>(mst_aryu8SystemStatusRequestCmd);
	GSPOTMessage oMsg(pu8SysStatusCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GMainUnitService::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	GMutexLock  ml(m_instanceMutex);
	eLogLevel   eLevel = eNotice;
	const char* pcStatus = NULL;
	const char* pcMethod = "GMainUnitService::ProcessSystemStatusNotification() -- ";

	LOG(eSpot, eDebug, "%s status %02X, stex %02X, error %02X \n", pcMethod, u8Status, u8StatusEx, u8Error);

	if(u8Status > 0 && u8Status < 7)
	{
		if(u8Status == 0x01)
		{
			pcStatus = "STARTUP";
		}
		else if(u8Status == 0x02)
		{
			eLevel = eError;
			pcStatus = "INOPERATIVE";
		}
		else if(u8Status == 0x03)
		{
			pcStatus = "OFFLINE";
		}
		else if(u8Status == 0x04)
		{
			pcStatus = "ONLINE";
		}
		else if(u8Status == 0x05)
		{
			pcStatus = "BUSY";
		}
		else // u8Status == 0x06
		{
			pcStatus = "SERVICE";
		}

		LOG( eSpot, eLevel, "%s Main Unit reported %s state.\n", pcMethod, pcStatus );
	}
	else // other not common errors
	{
		LOG(eCardReader, eError,
		    "%s Main Unit reported state 0x%02X and error 0x%02X.\n",
		    pcMethod, u8Status, u8Error);
	}

	if( u8Error != 0x00 )
	{
		LOG(eSpot, eWarning, "MAIN UNIT device reported one of its devices in ERROR condition. Error code 0x%02X.\n", u8Error);
	}

	m_oRulesProcessor.ProcessSystemStatusNotification(u8Status, u8StatusEx, u8Error);

	return true;
}

