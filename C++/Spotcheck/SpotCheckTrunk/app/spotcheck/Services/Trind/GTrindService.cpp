/*
 * GTrindService.cpp
 *
 *  Created on: 10/02/2015
 *      Author: Sayyed Mohammad
 */
#include <string.h>
#include "GTrindService.h"
#include "Menus/Trind/GTrindMenu.h"
#include "Menus/Trind/GTrindManager.h"
#include "Common/Util/GConversion.h"

using namespace GSpotDefs;

const UINT8  GTrindService::mst_aryu8EnableCmd[TR_ENABLE_CMDLEN] =
             {0x00, 0x06, 0x04, 0x00, 0xA0, 0x01};

const UINT8  GTrindService::mst_aryu8DisableCmd[TR_DISABLE_CMDLEN] =
             {0x00, 0x06, 0x04, 0x00, 0xA0, 0x02};

const UINT8  GTrindService::mst_aryu8GetStatusCmd[TR_GETSTATUS_CMDLEN] =
             {0x00, 0x06, 0x04, 0x00, 0xA0, 0x03};

const UINT8  GTrindService::mst_aryu8SetIndicatorONCmd[TR_SETINDICATOR_CMDLEN] =
             {0x00, 0x07, 0x04, 0x00, 0xA0, 0x04, 0x01};

const UINT8  GTrindService::mst_aryu8SetIndicatorOFFCmd[TR_SETINDICATOR_CMDLEN] =
             {0x00, 0x07, 0x04, 0x00, 0xA0, 0x04, 0x00};

const char * GTrindService::mst_arypcStatusLabels[] =
{
	"eStatusInitial",
	"eStatusTagDataArrived",
	"eStatusTempError",
	"eStatusError"
};
const char * GTrindService::mst_arypcEventLabels[]=
{
	"eEventEnableService",
	"eEventDisableService",
	"eEventTrindIsEnabled",
	"eEventTrindIsDisabled",
	"eEventTagDataArrived",
	"eEventTimeout",
	"eEventError"
};



GTrindService* GTrindService::mst_pSoleInstance = NULL;
GMutex         GTrindService::mst_InstanceMutex;


GTrindService::GTrindService(GSPOTSession* pSession)
	: m_blTREnabled(false),
	  m_eStatus(),
	  m_Condition(&mst_InstanceMutex),
	  m_pSession(pSession), m_pManager(NULL),
	  m_TOutCommand(this, &GTrindService::ProcessTimeoutEvent)
{
	m_pTimer = new GTimer(&m_TOutCommand,TR_TOUT_TRIND,false, false);
	LOG(eTrind, eInfo, "Trind Service Started.\n");
}

GTrindService::~GTrindService()
{
	LOG(eTrind, eInfo, "Trind Service Stopped.\n");
}

GTrindService* GTrindService::GetService()
{
	GMutexLock oLock(mst_InstanceMutex);

	return mst_pSoleInstance;
}

void GTrindService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock oLock(mst_InstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GTrindService(pSession);
	}
}

void GTrindService::DestroyInstance()
{
	GMutexLock oLock(mst_InstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

void GTrindService::AttachManager(GTrindManager* pMgr)
{
	if( !pMgr )
	{
		return;
	}

	GMutexLock oLock(mst_InstanceMutex);
	m_pManager = pMgr;
}

void GTrindService::DetachManager()
{
	GMutexLock oLock(mst_InstanceMutex);
	m_pManager = NULL;
}


bool GTrindService::isTrindReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
{
	if(!pMsg)
	{
		return false; // no message is supplied
	}

	if( ! blOnlySubCmd ) // this flag avoids to analyze the following, if already tested true
	{
		if( pMsg->GetAppId() != eSPOT_OPT_ANSWER )
		{
			return false; // unexpected application ID
		}

		if( pMsg->GetCommandCode() != eSPOT_OPT_TRIND )
		{
			return false; // unexpected command category
		}
	}

	int iSubCmd = pMsg->GetSubCommandCode();
	if( iSubCmd >= eSPOT_OPT_TRIND_ENABLE  &&  iSubCmd <= eSPOT_OPT_TRIND_TAGREAD )
	{
		return true;
	}
	else
	{
		return false; // unexpected message type
	}
}

bool GTrindService::ProcessOPTSystemStatusNotification(UINT8 u8Status,
                                                         UINT8 u8StatusEx, UINT8 u8Error)
{
	const char* pcMethod = "GTrindService::ProcessOPTSystemStatusNotification() - ";
	if(u8Status == eSPOT_TR_NOT_CONFIGURED )
	{
		//To be used in future;
	}

	return true;
}

bool GTrindService::ProcessTrindReply(GSPOTMessage* pMsg)
{
	bool blret = false;
	int iSubCmd = pMsg->GetSubCommandCode();
	const char* pcMethod = "GTrindService::ProcessTrindReply() - ";

	if(iSubCmd >= eSPOT_OPT_TRIND_ENABLE && iSubCmd <= eSPOT_OPT_TRIND_TAGREAD)
	{
		UINT16 u16MsgLen = pMsg->GetDataLen();
		if(u16MsgLen < 1)
		{
			LOG(eTrind, eError, "Message length too short !!!!!\n");
			return false;
		}

		UINT8* pu8Data = pMsg->GetData();
		if(pu8Data == NULL)
		{
			LOG(eTrind, eError, "Corrupt message with no data !!!!!\n");
			return false;
		}

		UINT8 u8Buffer[40];
		char  acBuffer[128];
		UINT  ulen = pMsg->GetTransmitBuffer(u8Buffer);
		ulen = (ulen > 40) ? 40 : ulen;
		GConversion::BytesToASCIIHexString(acBuffer, u8Buffer, ulen);
		LOG(eTrind, eDebug, "%s Message Received:\n%s\n", pcMethod, acBuffer);

		switch( iSubCmd )
		{
			case eSPOT_OPT_TRIND_ENABLE:
				if(pu8Data[0] ==0x00)
				{
					StatusTransitionRequest(eEventTrindIsEnabled,NULL);
				}
				else
				{
					StatusTransitionRequest(eEventError,NULL);
				}
				break;

			case eSPOT_OPT_TRIND_DISABLE:
				if(pu8Data[0] ==0x00)
				{
					StatusTransitionRequest(eEventTrindIsDisabled,NULL);
				}
				break;

			case eSPOT_OPT_TRIND_GETSTATUS:
			case eSPOT_OPT_TRIND_SETINDICATOR:
				break;


			case eSPOT_OPT_TRIND_TAGREAD:
			{
				if(pu8Data[0]==0x00)
				{
					StatusTransitionRequest(eEventTagDataArrived,NULL);
				}
				else
				{
					IncrementTagCount(0);
					UpdateTagStatistics(GTrindServiceStatistics::eTrFail);
					StatusTransitionRequest(eEventError,NULL);
				}

				break;
			}
		}
	}
	else
	{
		LOG(eTrind, eTrace,
		    "Trind Reply Subcommand %d ,"
		    " message processing not implemented yet.\n", iSubCmd);
	}

	return blret;
}

void GTrindService::Enable()
{
	GMutexLock ml(m_TrindStateMutex);

	StatusTransitionRequest(eEventEnableService);

	return;
}

void GTrindService::Disable()
{
	GMutexLock ml(m_TrindStateMutex);

	StatusTransitionRequest(eEventDisableService);

	return;
}

bool GTrindService::SendEnableCmdToSPOT()
{
	// TODO: correct GSPOTMessage class (several methods) and
	//       its other clients to accept const UINT8* as parameter
	LOG(eTrind, eDebug, "GTrindService::SendEnableCmdToSPOT()\n");
	if( m_pSession == NULL )
	{
		return false;
	}

	UINT8* pu8EnableTRCmd = const_cast<UINT8*>(mst_aryu8EnableCmd);
	GSPOTMessage oMsg(pu8EnableTRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GTrindService::SendDisableCmdToSPOT()
{
	// TODO: correct GSPOTMessage class (several methods) and
	//       its other clients to accept const UINT8* as parameter

	if( m_pSession == NULL )
	{
		return false;
	}

	UINT8* pu8DisableTRCmd = const_cast<UINT8*>(mst_aryu8DisableCmd);
	GSPOTMessage oMsg(pu8DisableTRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GTrindService::SendLEDONCmdToSPOT()
{
	// TODO: correct GSPOTMessage class (several methods) and
	//       its other clients to accept const UINT8* as parameter
	LOG(eTrind, eDebug, "GTrindService::SendEnableCmdToSPOT()\n");
	if( m_pSession == NULL )
	{
		return false;
	}
	UINT8* pu8LEDONTRCmd = const_cast<UINT8*>(mst_aryu8SetIndicatorONCmd);
	GSPOTMessage oMsg(pu8LEDONTRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GTrindService::SendLEDOFFCmdToSPOT()
{
	// TODO: correct GSPOTMessage class (several methods) and
	//       its other clients to accept const UINT8* as parameter
	LOG(eTrind, eDebug, "GTrindService::SendEnableCmdToSPOT()\n");
	if( m_pSession == NULL )
	{
		return false;
	}
	UINT8* pu8LEDOFFTRCmd = const_cast<UINT8*>(mst_aryu8SetIndicatorOFFCmd);
	GSPOTMessage oMsg(pu8LEDOFFTRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GTrindService::EnableRequest(bool blCheckStatus /* = false */)
{
	LOG(eTrind, eDebug, "GTrindService::EnableRequest\n");
	if(blCheckStatus && m_blTREnabled)
	{
		return true; // Nothing to do:Trind is already enabled.
	}

	const char* pcMethod = "GTrindService::EnableRequest() - ";

	if( ! SendEnableCmdToSPOT() )
	{
		LOG(eTrind, eError, "%s%s", pcMethod, "Could not send asynchronous Enable message to SPOT.\n");
		return false;
	}

	LOG(eTrind, eTrace, "%s%s", pcMethod, "SPOT card reader Enable message has been queued to SPOT.\n");
	return true;
}

bool GTrindService::DisableRequest(bool blCheckStatus /* = false */)
{
	if(blCheckStatus && !m_blTREnabled)
	{
		return true; // Nothing to do: Trind is already disabled.
	}

	const char*  pcMethod = "GTrindService::DisableRequest() - ";

	if( ! SendDisableCmdToSPOT() )
	{
		LOG(eTrind, eError, "%s%s", pcMethod, "Could not send asynchronous Disable message to SPOT.\n");
		return false;
	}

	LOG(eTrind, eTrace, "%s%s", pcMethod, "SPOT card reader Disable message has been queued to SPOT.\n");
	return true;
}

bool GTrindService::LEDRequestOn()
{
	LOG(eTrind, eDebug, "GTrindService::LEDRequestOn\n");

	const char* pcMethod = "GTrindService::LEDRequestOn() - ";

	if( ! SendLEDONCmdToSPOT() )
	{
		LOG(eTrind, eError, "%s%s", pcMethod, "Could not send asynchronous Enable message to SPOT.\n");
		return false;
	}

	LOG(eTrind, eTrace, "%s%s", pcMethod, "SPOT card reader Enable message has been queued to SPOT.\n");
	return true;
}

bool GTrindService::LEDRequestOff()
{
	LOG(eTrind, eDebug, "GTrindService::LEDRequestOFF\n");

	const char* pcMethod = "GTrindService::LEDRequestOFF() - ";

	if( ! SendLEDOFFCmdToSPOT() )
	{
		LOG(eTrind, eError, "%s%s", pcMethod, "Could not send asynchronous Enable message to SPOT.\n");
		return false;
	}

	LOG(eTrind, eTrace, "%s%s", pcMethod, "SPOT card reader Enable message has been queued to SPOT.\n");
	return true;
}

void GTrindService::ProcessTimeoutEvent()
{
	GMutexLock oLock(mst_InstanceMutex);
}

/**
 * This method implements the state change and log the transition.
 * @param ev is the event code that initiates a state transition.
 * @param es is the future state machine, reached after the transition.
 * @return void
 *
 */
void GTrindService::NextState(eEvents ev, eStatus es)
{
	LOG( eTrind, eDebug, "[ curr_status: %s ==>> event: %s ==>> next_status: %s ]\n",
	     mst_arypcStatusLabels[m_eStatus], mst_arypcEventLabels[ev], mst_arypcStatusLabels[es] );
	m_eStatus = es;
}

void GTrindService::ShowTagRead()
{
	if(m_pManager)
		m_pManager->ShowTagRead();
}

void GTrindService::ShowStatsScreen()
{
	if(m_pManager)
		m_pManager->ShowStatsScreen();
}

void GTrindService::IncrementTagCount(int result)
{
	if(m_pManager)
	{
			m_pManager->IncrementTagCount(result);

	}
}

void GTrindService::UpdateTagStatistics(GTrindServiceStatistics::eTagResult t)
{
	m_oTRSessionStatistics.IncrementTagCount(t);
}

void GTrindService::UpdateLEDStatistics(GTrindServiceStatistics::eTRLEDResult t)
{
	m_oTRSessionStatistics.UpdateLEDStatistics(t);
}
/**
 * This method implements the state machine.
 * @param e is the event code that initiates a state transition.
 * @param pEventData is the optional pointer to data needed for this state machine.
 * @return void
 *
 */
void GTrindService::StatusTransitionRequest(eEvents e, DataEvent* pEventData /* = NULL */)
{
	switch(m_eStatus)
	{
		//======================= current STATUS : Status Initial =========================
		case eStatusInitial:
			switch(e)
			{
				case eEventEnableService:
				{
					NextState(e, eStatusInitial);
					EnableRequest();
					break;
				}
				case eEventDisableService:
				{
					NextState(e, eStatusInitial);
					DisableRequest();
					break;
				}
				case eEventTrindIsEnabled:
				{
					NextState(e, eStatusWaitTag);
					this->m_blTREnabled =true;
					break;
				}
				case eEventTrindIsDisabled:
				{
					NextState(e, eStatusWaitTag);
					this->m_blTREnabled =true;
					break;
				}
				case eEventTagDataArrived:
				{
					NextState(e, eStatusInitial);
					DisableRequest();
					m_blTREnabled =false;
					break;

				}
				case eEventTimeout:
				{
					break;

				}
				case eEventError:
				{
					NextState(e, eStatusError);
					break;
				}

				default:
					break;

			}
			break;
	//------------------------------------------------------------------------
		case eStatusWaitTag:

			switch(e)
			{

				case eEventEnableService:
				case eEventTrindIsEnabled:
				{
					NextState(e,eStatusWaitTag);
					break;
				}

				case eEventDisableService:
				{
					NextState(e,eStatusWaitTag);
					DisableRequest();
					break;
				}
				case eEventTrindIsDisabled:
				{
					NextState(e, eStatusInitial);
					m_blTREnabled=false;
					break;
				}
				case eEventTagDataArrived:
				{
					LOG(eTrind,eDebug,"Tag Data Arrived")
					NextState(e, eStatusWaitTag);
					if(m_pManager)
					ShowTagRead();
					IncrementTagCount(1);
					UpdateTagStatistics(GTrindServiceStatistics::eTrPass);
					GSystem::Sleep(2000);
					if(m_pManager)
					ShowStatsScreen();
					break;
				}

				case eEventTimeout:
				case eEventError:
				default:
					break;

			}
			break;

		default:
		break;
	}


}

