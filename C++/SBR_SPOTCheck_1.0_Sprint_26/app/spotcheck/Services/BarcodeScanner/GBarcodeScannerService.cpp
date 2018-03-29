#include <unistd.h>
#include <cstring>

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "GBarcodeScannerService.h"

#include "Menus/BarcodeScanner/GBarcodeScannerMenu.h"
#include "Menus/BarcodeScanner/GBarcodeScannerManager.h"

#include <GSystem.h>

using namespace GSpotDefs;

GBarcodeScannerService* GBarcodeScannerService::mst_pSoleInstance = NULL;
GMutex GBarcodeScannerService::m_createInstanceMutex;

const UINT8  GBarcodeScannerService::mst_aryu8EnableBarcodeScannerCmd	[BCS_ENABLE_CMD_LEN]	= {0x00, 0x08, 0x04, 0x00, 0x50, 0x01, 0x00, 0x00};
const UINT8  GBarcodeScannerService::mst_aryu8DisableBarcodeScannerCmd	[BCS_DISABLE_CMD_LEN]	= {0x00, 0x06, 0x04, 0x00, 0x50, 0x05};
const char * GBarcodeScannerService::mst_arypcStatusLabel				[]=
{
	"eStatusInitial",
	"eStatusWaitingBarcodeData",
	"eStatusDataArrived",
	"eStatusError"
};
const char * GBarcodeScannerService::mst_arypcEventLabel[]=
{
	"eEventEnableDevice",
	"eEventDisableDevice",
	"eEventBarcodeScannerEnable",
	"eEventBarcodeScannerDisable",
	"eEventBarcodeData",
	"eEventTimeout",
	"eEventError"
};



//##########################################################################################
//##########################################################################################
// Public methods
//##########################################################################################
//##########################################################################################

const char* GBarcodeScannerService::GetErrorDescription(UINT8 u8Error)
{
	switch( u8Error )
	{
		case 0:
			return "good";

		case 1:
			return "not encoded";

		case 2:
			return "SS ERROR";

		case 4:
			return "ES ERROR";

		case 8:
			return "LRC ERROR";

		case 0x10:
			return "URC ERROR";

		case 0x20:
			return "no data";

		case 0x40:
			return "jitter ERROR";

		case 0x80:
			return "generic ERROR";

		default:
			return "unknown ERROR";
	}
}

bool GBarcodeScannerService::isBarcodeScannerReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
{
	if(!pMsg)
	{
		return false; // no message is supplied
	}

	return false; // unexpected command code
}

void GBarcodeScannerService::AttachManager(GBarcodeScannerManager* pMgr)
{
	if( !pMgr )
	{
		return;
	}

	GMutexLock ml(m_CardStateMutex);
	m_pManager = pMgr;
}

void GBarcodeScannerService::DetachManager()
{
	GMutexLock ml(m_CardStateMutex);
	m_pManager = NULL;
}

void GBarcodeScannerService::EnableDevice()
{
	GMutexLock ml(m_CardStateMutex);

	ProcessEventReceived(eEventEnableDevice);

	return;
}

void GBarcodeScannerService::DisableDevice()
{
	GMutexLock ml(m_CardStateMutex);

	ProcessEventReceived(eEventDisableDevice);

	return;
}

bool GBarcodeScannerService::ProcessBarcodeScannerReply(GSPOTMessage* pMsg)
{
	bool blret = false;

	return blret;
}

bool GBarcodeScannerService::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	LOG(eBarcode, eDebug, "[st %02X, stex %02X, error %02X]\n", int(u8Status), int(u8StatusEx), int(u8Error) );

	GMutexLock ml(m_CardStateMutex);

	if( u8Status == 0x04 && u8StatusEx == 0x00 && u8Error == 0x00 )
	{
		ProcessEventReceived(eEventBarcodeScannerEnable);

		return true;
	}

	if( u8Status == 0x03 && u8StatusEx == 0x00 && u8Error == 0x00 )
	{
		ProcessEventReceived(eEventBarcodeScannerDisable);

		return true;
	}

	if( u8Error != 0x00 )
	{
		ProcessEventReceived(eEventError);

		return true;
	}


	return false;
}




//##########################################################################################
//##########################################################################################
// Private methods
//##########################################################################################
//##########################################################################################

GBarcodeScannerService::GBarcodeScannerService(GSPOTSession* pSession)
	: m_pSession(pSession), m_pTimer(NULL), m_pManager(NULL),
		m_eStatus(eStatusInitial),
		m_u8AttemptsInError(0),
		m_Condition(&m_SincronizeMutex),
		m_TOutCommand(this, &GBarcodeScannerService::ProcessTimeoutEvent)
{
	m_pTimer = new GTimer(&m_TOutCommand, BCS_EVENT_TIMEOUT, false, false);
	LOG(eBarcode, eInfo, "Barcode Service started.\n");
}

GBarcodeScannerService::~GBarcodeScannerService()
{
	LOG(eBarcode, eInfo, "Barcode Service stopped.\n");
}

bool GBarcodeScannerService::EnableRequest()
{
	const char* pcMethod = "GBarcodeScannerService::EnableRequest() - ";

	if( ! SendEnableCmdToSPOT() )
	{
		LOG(eBarcode, eError, "%s%s", pcMethod, "Could not send asynchronous Enable message to SPOT.\n");
		return false;
	}

	LOG(eBarcode, eTrace, "%s%s", pcMethod, "SPOT barcode scanner Enable message has been queued to SPOT.\n");
	return true;
}

bool GBarcodeScannerService::DisableRequest()
{
	const char*  pcMethod = "GBarcodeScannerService::DisableRequest() - ";

	if( ! SendDisableCmdToSPOT() )
	{
		LOG(eBarcode, eError, "%s%s", pcMethod, "Could not send asynchronous Disable message to SPOT.\n");
		return false;
	}

	LOG(eBarcode, eTrace, "%s%s", pcMethod, "SPOT barcode scanner Disable message has been queued to SPOT.\n");
	return true;
}

bool GBarcodeScannerService::SendEnableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	UINT8* pu8EnableCRCmd = const_cast<UINT8*>(mst_aryu8EnableBarcodeScannerCmd);
	GSPOTMessage oMsg(pu8EnableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GBarcodeScannerService::SendDisableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	UINT8* pu8DisableCRCmd = const_cast<UINT8*>(mst_aryu8DisableBarcodeScannerCmd);
	GSPOTMessage oMsg(pu8DisableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

void GBarcodeScannerService::ProcessEventReceived(eEvents e, sDataEvent* pEventData)
{
	switch(m_eStatus)
	{
		//======================================================================================================
		case eStatusInitial:
		{
			switch(e)
			{
				case eEventEnableDevice:
					NextState(eEventEnableDevice, eStatusWaitingBarcodeData);
					EnableRequest();
				break;

				case eEventBarcodeScannerDisable:
					NextState(eEventBarcodeScannerDisable, eStatusInitial);
				break;

				case eEventBarcodeScannerEnable:
					NextState(eEventBarcodeScannerEnable, eStatusInitial);
				break;

				case eEventError:
					NextState(eEventError, eStatusWaitingBarcodeData);
					CheckIfBarcodeScannerIsInPermanentError();
				break;

				default:
					LOG(eBarcode, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusWaitingBarcodeData:
		{
			switch(e)
			{
				case eEventBarcodeData:
					NextState(eEventBarcodeData, eStatusDataArrived);
					if(pEventData != NULL)
					{
						NotifyBarcodeDataArrived();
					}
				break;

				case eEventEnableDevice:
					NextState(eEventEnableDevice, eStatusWaitingBarcodeData);
				break;

				case eEventError:
					NextState(eEventError, eStatusInitial);
					CheckIfBarcodeScannerIsInPermanentError();
				break;

				case eEventDisableDevice:
					NextState(eEventDisableDevice, eStatusInitial);
					DisableRequest();
				break;

				case eEventBarcodeScannerEnable:
					NextState(eEventBarcodeScannerEnable, eStatusWaitingBarcodeData);
				break;

				case eEventBarcodeScannerDisable:
					NextState(eEventBarcodeScannerDisable, eStatusWaitingBarcodeData);
					LOG(eBarcode, eWarning, "Data never received!!\n");
					NotifyBarcodeDataArrived(true /*error*/);
					EnableRequest();
				break;

				default:
					LOG(eBarcode, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusDataArrived:
		{
			switch(e)
			{
				case eEventBarcodeScannerDisable:
					NextState(eEventBarcodeScannerDisable, eStatusWaitingBarcodeData);
					EnableRequest();
				break;

				case eEventError:
					NextState(eEventError, eStatusInitial);
					CheckIfBarcodeScannerIsInPermanentError();
				break;

				case eEventDisableDevice:
					NextState(eEventDisableDevice, eStatusInitial);
					DisableRequest();
				break;

				case eEventBarcodeScannerEnable:
					NextState(eEventBarcodeScannerEnable, eStatusDataArrived);
				break;

				default:
					LOG(eBarcode, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusError:
		{
			switch(e)
			{
				default:
					LOG(eBarcode, eError, "ERROR: event [%s] received in eStatusError. The Contactless Reader device is in unrecoverable state.\n", mst_arypcEventLabel[e]);
					NextState(e, eStatusError);
				break;
			}
		}
		break;
		//======================================================================================================
	}

	if( pEventData != NULL )
	{
		delete pEventData;
	}

}

void GBarcodeScannerService::NotifyBarcodeDataArrived(bool bError)
{
	(bError)? (m_oBarcodeScannerSessionStatistics.IncrementStatistics(GBarcodeScannerServiceStatistics::eFail)):
			  (m_oBarcodeScannerSessionStatistics.IncrementStatistics(GBarcodeScannerServiceStatistics::ePass));

	m_oBarcodeScannerSessionStatistics.IncrementTotalReadings();

	LOG( eBarcode, eDebug, "[TotalReadings [%d]]\n"   			, m_oBarcodeScannerSessionStatistics.GetTotalReadings());
	LOG( eBarcode, eDebug, "[Readings with fail status [%d]]\n"	, m_oBarcodeScannerSessionStatistics.GetStatistics(GBarcodeScannerServiceStatistics::eFail) );
	LOG( eBarcode, eDebug, "[Readings with pass status [%d]]\n"	, m_oBarcodeScannerSessionStatistics.GetStatistics(GBarcodeScannerServiceStatistics::ePass) );

	if(m_pManager)
	{
		GBarcodeScannerServiceStatistics::eTypeStatistics eResult;
		(bError)? eResult = GBarcodeScannerServiceStatistics::eFail: eResult = GBarcodeScannerServiceStatistics::ePass;

		//Notify to manager
		//m_pManager->NotifyCardDataResult(eResult);
	}
}

void GBarcodeScannerService::CheckIfBarcodeScannerIsInPermanentError()
{
	m_u8AttemptsInError++;

	if( m_u8AttemptsInError < BCS_ERROR_MAXATTEMPTS )
	{
		DisableRequest();
		GSystem::Sleep(900);
	}
	else
	{
		if(m_pManager)
		{
			//Notify to manager
			//m_pManager->NotifyBarcodeScannerInPermanentError();
		}
	}
}

bool GBarcodeScannerService::StopTimer()
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Stop();
}

bool GBarcodeScannerService::StartTimer()
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Start();
}

void GBarcodeScannerService::ProcessTimeoutEvent()
{
	GMutexLock ml(m_CardStateMutex);

	ProcessEventReceived(eEventTimeout);
}

bool GBarcodeScannerService::ProcessDataEvent(UINT8* pMsgData, UINT16 u16DataSize)
{
	GMutexLock ml(m_CardStateMutex);

	sDataEvent* pData = new sDataEvent;
	pData->data = (void *)pMsgData;
	pData->size = u16DataSize;
	ProcessEventReceived(eEventBarcodeData, pData);

	return true;
}

void GBarcodeScannerService::NextState(eEvents e, eStatus s)
{
	LOG( eBarcode, eDebug, "[Service][curr_status[%s] ==>> event[%s] ==>> nxt_status[%s]]\n", mst_arypcStatusLabel[m_eStatus], mst_arypcEventLabel[e], mst_arypcStatusLabel[s] );
	m_eStatus = s;
}

