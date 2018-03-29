#include <unistd.h>
#include <cstring>

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "GContactlessReaderService.h"

#include "Menus/ContactlessReader/GContactlessReaderMenu.h"
#include "Menus/ContactlessReader/GContactlessReaderManager.h"

#include <GSystem.h>

using namespace GSpotDefs;

GContactlessReaderService* GContactlessReaderService::mst_pSoleInstance = NULL;
GMutex GContactlessReaderService::m_createInstanceMutex;

const UINT8  								GContactlessReaderService::mst_aryu8EnableCLRCmd	[CLR_ENABLE_CMD_LEN]	= {0x00, 0x08, 0x04, 0x00, 0x50, 0x01, 0x00, 0x00};
const UINT8  								GContactlessReaderService::mst_aryu8DisableCLRCmd	[CLR_DISABLE_CMD_LEN]	= {0x00, 0x06, 0x04, 0x00, 0x50, 0x05};
GContactlessReaderService::CLR_TRACK_INFO   GContactlessReaderService::mst_aryCLRTracks			[eTotalTracks];
const char * GContactlessReaderService::mst_arypcStatusLabel[]=
{
	"eStatusInitial",
	"eStatusWaitingRFCardData",
	"eStatusDataArrived",
	"eStatusError"
};
const char * GContactlessReaderService::mst_arypcEventLabel[]=
{
	"eEventEnableDevice",
	"eEventDisableDevice",
	"eEventContactlessReaderEnable",
	"eEventContactlessReaderDisable",
	"eEventRFCardData",
	"eEventTimeout",
	"eEventError"
};



//##########################################################################################
//##########################################################################################
// Public methods
//##########################################################################################
//##########################################################################################

const char* GContactlessReaderService::GetErrorDescription(UINT8 u8Error)
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

bool GContactlessReaderService::isContactlessReaderReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
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

		if( pMsg->GetCommandCode() != eSPOT_OPT_CLESS )
		{
			return false; // unexpected command category
		}
	}

	int iSubCmd = pMsg->GetSubCommandCode();
	if( iSubCmd == eSPOT_OPT_CLR_ENABLE  ||  iSubCmd == eSPOT_OPT_CLR_DISABLE || iSubCmd == eSPOT_OPT_CLR_CARD_DATA )
	{
		return true;
	}
	else
	{
		return false; // unexpected command code
	}
}

void GContactlessReaderService::AttachManager(GContactlessReaderManager* pMgr)
{
	if( !pMgr )
	{
		return;
	}

	GMutexLock ml(m_CardStateMutex);
	m_pManager = pMgr;
}

void GContactlessReaderService::DetachManager()
{
	GMutexLock ml(m_CardStateMutex);
	m_pManager = NULL;
}

void GContactlessReaderService::EnableDevice()
{
	GMutexLock ml(m_CardStateMutex);

	ProcessEventReceived(eEventEnableDevice);

	return;
}

void GContactlessReaderService::DisableDevice()
{
	GMutexLock ml(m_CardStateMutex);

	ProcessEventReceived(eEventDisableDevice);

	return;
}

bool GContactlessReaderService::ProcessContactlessReaderReply(GSPOTMessage* pMsg)
{
	bool blret = false;
	UINT8*      pu8MsgData = NULL;
	UINT16      u16DataLen = 0;
	int         iCmdCode = pMsg->GetSubCommandCode();

	switch( iCmdCode )
	{
		case eSPOT_OPT_CLR_ENABLE:
			LOG( eContactlessReader, eDebug, "[Contactless Reader device reported ENABLED]\n" );
			blret = true;
		break;

		case eSPOT_OPT_CLR_DISABLE:
			LOG( eContactlessReader, eDebug, "[Contactless Reader device reported DISABLED]\n" );
			blret = true;
		break;

		case eSPOT_OPT_CLR_CARD_DATA:
			u16DataLen = pMsg->GetDataLen();
			if(u16DataLen == 0)
			{
				break;
			}
			pu8MsgData = new UINT8[u16DataLen + 4];
			if(pu8MsgData == NULL)
			{
				break;
			}
			if( ! pMsg->GetData(pu8MsgData, u16DataLen) )
			{
				break;
			}
			blret = ProcessDataEvent(pu8MsgData, u16DataLen);
			delete[] pu8MsgData;
		break;

		default:
		break;
	}

	return blret;
}

bool GContactlessReaderService::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	LOG(eContactlessReader, eDebug, "[st %02X, stex %02X, error %02X]\n", int(u8Status), int(u8StatusEx), int(u8Error) );

	GMutexLock ml(m_CardStateMutex);

	if( u8Status == 0x04 && u8StatusEx == 0x00 && u8Error == 0x00 )
	{
		ProcessEventReceived(eEventContactlessReaderEnable);

		return true;
	}

	if( u8Status == 0x03 && u8StatusEx == 0x00 && u8Error == 0x00 )
	{
		ProcessEventReceived(eEventContactlessReaderDisable);

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

GContactlessReaderService::GContactlessReaderService(GSPOTSession* pSession)
	: m_pSession(pSession), m_pTimer(NULL), m_pManager(NULL),
		m_eStatus(eStatusInitial),
		m_u8AttemptsInError(0),
		m_Condition(&m_SincronizeMutex),
		m_TOutCommand(this, &GContactlessReaderService::ProcessTimeoutEvent)
{
	ClearTrackInfo();
	m_pTimer = new GTimer(&m_TOutCommand, CLR_EVENT_TIMEOUT, false, false);
	LOG(eContactlessReader, eInfo, "Contactless Reader Service started.\n");
}

GContactlessReaderService::~GContactlessReaderService()
{
	LOG(eContactlessReader, eInfo, "Contactless Reader Service stopped.\n");
}

void GContactlessReaderService::ClearTrackInfo()
{
	memset( mst_aryCLRTracks, 0, sizeof(mst_aryCLRTracks) );
}

UINT16 GContactlessReaderService::ClipPartialLength(UINT16 u16Length, int nTrack)
{
	if(nTrack < eTrack1 || nTrack > eTrack3) return 0; // parameter error protection

	const char* pcMethod = "GContactlessReaderService::ClipPartialLength() - ";
	UINT16 u16MAXLen = CLR_TRACK_SIZE - 1;

	if(u16Length > u16MAXLen)
	{
		char cmsg[96];

		LOG(eContactlessReader, eWarning, "%s Track #%u, Data Length: %u, clipped to %u bytes limit.\n",
						        pcMethod, 1 + nTrack, u16Length, u16MAXLen);

		return u16MAXLen; // it gives the available room and no track buffer overflow
	}
	else
	{
		return u16Length;
	}
}

bool GContactlessReaderService::EnableRequest()
{
	const char* pcMethod = "GContactlessReaderService::EnableRequest() - ";

	if( ! SendEnableCmdToSPOT() )
	{
		LOG(eContactlessReader, eError, "%s%s", pcMethod, "Could not send asynchronous Enable message to SPOT.\n");
		return false;
	}

	LOG(eContactlessReader, eTrace, "%s%s", pcMethod, "SPOT contactless reader Enable message has been queued to SPOT.\n");
	return true;
}

bool GContactlessReaderService::DisableRequest()
{
	const char*  pcMethod = "GContactlessReaderService::DisableRequest() - ";

	if( ! SendDisableCmdToSPOT() )
	{
		LOG(eContactlessReader, eError, "%s%s", pcMethod, "Could not send asynchronous Disable message to SPOT.\n");
		return false;
	}

	LOG(eContactlessReader, eTrace, "%s%s", pcMethod, "SPOT contactless reader Disable message has been queued to SPOT.\n");
	return true;
}

bool GContactlessReaderService::SendEnableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	UINT8* pu8EnableCRCmd = const_cast<UINT8*>(mst_aryu8EnableCLRCmd);
	GSPOTMessage oMsg(pu8EnableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GContactlessReaderService::SendDisableCmdToSPOT()
{
	if( m_pSession == NULL )
	{
		return false;
	}

	UINT8* pu8DisableCRCmd = const_cast<UINT8*>(mst_aryu8DisableCLRCmd);
	GSPOTMessage oMsg(pu8DisableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

void GContactlessReaderService::ProcessEventReceived(eEvents e, sDataEvent* pEventData)
{
	switch(m_eStatus)
	{
		//======================================================================================================
		case eStatusInitial:
		{
			switch(e)
			{
				case eEventEnableDevice:
					NextState(eEventEnableDevice, eStatusWaitingRFCardData);
					EnableRequest();
				break;

				case eEventDisableDevice:
					NextState(eEventDisableDevice, eStatusInitial);
					DisableRequest();
				break;

				case eEventContactlessReaderEnable:
					NextState(eEventContactlessReaderEnable, eStatusInitial);
				break;

				case eEventContactlessReaderDisable:
					NextState(eEventContactlessReaderDisable, eStatusInitial);
				break;

				case eEventError:
					NextState(eEventError, eStatusWaitingRFCardData);
					CheckIfContactlessReaderIsInPermanentError();
				break;

				default:
					LOG(eContactlessReader, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusWaitingRFCardData:
		{
			switch(e)
			{
				case eEventRFCardData:
					NextState(eEventRFCardData, eStatusDataArrived);
					if(pEventData != NULL)
					{
						if( ProcessRFCardInfo((UINT8*)pEventData->data, pEventData->size) != 0 )
						{
							LOG(eContactlessReader, eError, "Could not process card data.!!!\n");
							NotifyRFCardDataArrived(true /*error*/);
						}
						else
						{
							NotifyRFCardDataArrived();
						}
					}
				break;

				case eEventEnableDevice:
					NextState(eEventEnableDevice, eStatusWaitingRFCardData);
				break;

				case eEventError:
					NextState(eEventError, eStatusInitial);
					CheckIfContactlessReaderIsInPermanentError();
				break;

				case eEventDisableDevice:
					NextState(eEventDisableDevice, eStatusInitial);
					DisableRequest();
				break;

				case eEventContactlessReaderEnable:
					NextState(eEventContactlessReaderEnable, eStatusWaitingRFCardData);
				break;

				case eEventContactlessReaderDisable:
					NextState(eEventContactlessReaderDisable, eStatusWaitingRFCardData);
					LOG(eContactlessReader, eWarning, "Data never received!!\n");
					NotifyRFCardDataArrived(true /*error*/);
					EnableRequest();
				break;

				default:
					LOG(eContactlessReader, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
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
				case eEventContactlessReaderDisable:
					NextState(eEventContactlessReaderDisable, eStatusWaitingRFCardData);
					EnableRequest();
				break;

				case eEventError:
					NextState(eEventError, eStatusInitial);
					CheckIfContactlessReaderIsInPermanentError();
				break;

				case eEventDisableDevice:
					NextState(eEventDisableDevice, eStatusInitial);
					DisableRequest();
				break;

				case eEventContactlessReaderEnable:
					NextState(eEventContactlessReaderEnable, eStatusDataArrived);
				break;

				default:
					LOG(eContactlessReader, eError, "ERROR: INVALID EVENT [%s] received in [%s].\n", mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
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
					LOG(eContactlessReader, eError, "ERROR: event [%s] received in eStatusError. The Contactless Reader device is in unrecoverable state.\n", mst_arypcEventLabel[e]);
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

void GContactlessReaderService::NotifyRFCardDataArrived(bool bError)
{
	(bError)? (m_oCLRSessionStatistics.IncrementStatistics(GContactlessReaderServiceStatistics::eFail)):
			  (m_oCLRSessionStatistics.IncrementStatistics(GContactlessReaderServiceStatistics::ePass));

	m_oCLRSessionStatistics.IncrementTotalWaves();

	LOG( eContactlessReader, eDebug, "[TotalWaves [%d]]\n"   			, m_oCLRSessionStatistics.GetTotalWaves() );
	LOG( eContactlessReader, eDebug, "[Waves with fail status [%d]]\n"	, m_oCLRSessionStatistics.GetStatistics(GContactlessReaderServiceStatistics::eFail) );
	LOG( eContactlessReader, eDebug, "[Waves with pass status [%d]]\n"	, m_oCLRSessionStatistics.GetStatistics(GContactlessReaderServiceStatistics::ePass) );

	if(m_pManager)
	{
		GContactlessReaderServiceStatistics::eTypeStatistics eResult;
		(bError)? eResult = GContactlessReaderServiceStatistics::eFail: eResult = GContactlessReaderServiceStatistics::ePass;

		//Notify to manager
		m_pManager->NotifyCardDataResult(eResult);
	}
}

void GContactlessReaderService::CheckIfContactlessReaderIsInPermanentError()
{
	m_u8AttemptsInError++;

	if( m_u8AttemptsInError < CLR_ERROR_MAXATTEMPTS )
	{
		DisableRequest();
		GSystem::Sleep(900);
	}
	else
	{
		if(m_pManager)
		{
			//Notify to manager
			m_pManager->NotifyContactlessReaderInPermanentError();
		}
	}
}

bool GContactlessReaderService::StopTimer()
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Stop();
}

bool GContactlessReaderService::StartTimer()
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Start();
}

void GContactlessReaderService::ProcessTimeoutEvent()
{
	GMutexLock ml(m_CardStateMutex);

	ProcessEventReceived(eEventTimeout);
}

bool GContactlessReaderService::ProcessDataEvent(UINT8* pMsgData, UINT16 u16DataSize)
{
	GMutexLock ml(m_CardStateMutex);

	sDataEvent* pData = new sDataEvent;
	pData->data = (void *)pMsgData;
	pData->size = u16DataSize;
	ProcessEventReceived(eEventRFCardData, pData);

	return true;
}

void GContactlessReaderService::NextState(eEvents e, eStatus s)
{
	LOG( eContactlessReader, eDebug, "[Service][curr_status[%s] ==>> event[%s] ==>> nxt_status[%s]]\n", mst_arypcStatusLabel[m_eStatus], mst_arypcEventLabel[e], mst_arypcStatusLabel[s] );
	m_eStatus = s;
}

UINT8 GContactlessReaderService::ProcessRFCardInfo(UINT8* pMsgData, UINT16 u16DataSize)
{
	UINT8*      pu8data = NULL; // auxiliary data pointer
	int         ierror, nTracks, nTran;
	const char* pcMethod = "GContactlessReaderService::ProcessRFCardInfo - ";

	pu8data = pMsgData;
	ierror = *pu8data++; // overall error code
	if(ierror != 0)
	{
		LOG(eContactlessReader, eError, "%sSPOT returned the overall error %02X\n", pcMethod, ierror);
		return ierror;// the general error code
	}

	nTran = *pu8data++; // transaction number
	LOG(eContactlessReader, eTrace, "%sSPOT returned a number of transaction %02X\n", pcMethod, nTran);

	nTracks = *pu8data++; // number of tracks
	if(nTracks > 4)
	{
		nTracks = 4; // protective
	}

	LOG(eContactlessReader, eTrace, "%sSPOT returned a number of tracks %02X\n", pcMethod, nTracks);

	for(int n = 0, idtrack, ntrack ; n < nTracks && (pu8data - pMsgData) < u16DataSize; n++)
	{
		idtrack = *pu8data++; //track source

		if(idtrack & CLR_TRACK1_MASK)
		{
			ntrack = eTrack1;
		}
		else if(idtrack & CLR_TRACK2_MASK)
		{
			ntrack = eTrack2;
		}
		else if(idtrack & CLR_TRACK3_MASK)
		{
			ntrack = eTrack3;
		}
		else
		{
			LOG(eContactlessReader, eWarning, "%s WARNING: WRONG field ID bitmap: %02X\n", pcMethod, idtrack);
			ntrack = -1;
		}

		UINT8 bh = 0, bl = 0;
		UINT16 u16len, u16lenclp;

		bh =  *pu8data++; // length MSB , SPOT protocol: always big endian
		bl =  *pu8data++; // length LSB , SPOT protocol: always big endian

		u16len  = 256 * bh + bl; // track data length
		u16lenclp = ClipPartialLength(u16len, ntrack); // track buffer should be large enough !!

		if(ntrack >= 0 && ntrack <= eTrack3) // it means a valid Track# reported by SPOT
		{
			mst_aryCLRTracks[ntrack].m_u16Length = u16lenclp;

			if(u16lenclp > 0)
			{
				memcpy(mst_aryCLRTracks[ntrack].m_aryu8Data, pu8data, u16lenclp);
				mst_aryCLRTracks[ntrack].m_aryu8Data[u16lenclp] = 0;
			}
			else
			{
				memset(mst_aryCLRTracks[ntrack].m_aryu8Data, 0, CLR_TRACK_SIZE);
			}

			LOG(eContactlessReader, eDebug, "ProcessRFCardInfo: Track #%d valid,  Length: %d bytes .\n", (1 + ntrack), u16lenclp);
		}

		pu8data += u16len;
	}

	return ierror;
}
