/*
 * GCardReaderService.cpp
 *
 *  Created on: Feb 05 2013
 *      Author: gparis
 */

#include <unistd.h>
#include <cstring>

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GConversion.h"
#include "SPOT/SystemInformation/GGlobalSystemInformation.h"
#include "GCardReaderService.h"
#include "Menus/CardReader/GCardReaderMenu.h"
#include "Menus/CardReader/GCardReaderManager.h"


using namespace GSpotDefs;

GCardReaderService* GCardReaderService::mst_pSoleInstance = NULL;
GMutex GCardReaderService::m_createInstanceMutex;

const UINT8  GCardReaderService::mst_aryu8EnableCRCmd			[CR_ENABLE_CMD_LEN]		= {0x00, 0x06, 0x01, 0x00, 0x20, 0x01};
const UINT8  GCardReaderService::mst_aryu8DisableCRCmd			[CR_DISABLE_CMD_LEN]	= {0x00, 0x06, 0x01, 0x00, 0x20, 0x02};
const UINT8  GCardReaderService::mst_aryu8ChipResetCRCmd		[CR_CHIP_RESET_CMD_LEN]	= {0x00, 0x06, 0x01, 0x00, 0x20, 0x07};
const UINT8  GCardReaderService::mst_aryu8DiagnosticInfoCRCmd	[CR_DIAGNOSTIC_CMD_LEN]	= {0X00, 0X08, 0X01, 0X00, 0X10, 0X04, 0X01, 0x10};
TRACK_INFO   GCardReaderService::mst_aryTracks					[eTotalTracks];
const char * GCardReaderService::mst_arypcStatusLabel			[] =
{
	"eStatusInitial",
	"eStatusCardOut",
	"eStatusCardInserted",
	"eStatusChipCardInserted",
	"eStatusATRDataArrived",
	"eStatusDataArrived",
	"eStatusTempError",
	"eStatusError"
};
const char * GCardReaderService::mst_arypcEventLabel[]=
{
	"eEventEnableService",
	"eEventDisableService",
	"eEventCardReaderIsEnabled",
	"eEventCardReaderIsDisabled",
	"eEventCardInsert",
	"eEventCardRemove",
	"eEventCardData",
	"eEventChipCardInsert",
	"eEventCardReadingError",
	"eEventTimeout",
	"eEventError"
};



//##########################################################################################
//##########################################################################################
// Public methods
//##########################################################################################
//##########################################################################################

const char* GCardReaderService::GetErrorDescription(UINT8 u8Error)
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

bool GCardReaderService::isCardReaderReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
{
	if(!pMsg)
	{
		return false; // no message is supplied
	}

	if( ! blOnlySubCmd ) // this flag avoids to analyze the following, if already tested true
	{
		if( pMsg->GetAppId() != eSPOT_PP_ANSWER )
		{
			return false; // unexpected application ID
		}

		if( pMsg->GetCommandCode() != eSPOT_PP_CARDREADER )
		{
			return false; // unexpected command category
		}
	}

	int iSubCmd = pMsg->GetSubCommandCode();
	if( iSubCmd == eSPOT_PP_CR_ENABLE  ||  iSubCmd == eSPOT_PP_CR_DISABLE || iSubCmd == eSPOT_PP_CR_READ )
	{
		return true;
	}
	else
	{
		return false; // unexpected command code
	}
}

void GCardReaderService::AttachManager(GCardReaderManager* pMgr)
{
	if( !pMgr )
	{
		return;
	}

	GMutexLock ml(m_CardStateMutex);
	m_pManager = pMgr;
}

void GCardReaderService::DetachManager()
{
	GMutexLock ml(m_CardStateMutex);
	m_pManager = NULL;
}

void GCardReaderService::Enable()
{
	GMutexLock ml(m_CardStateMutex);

	StatusTransitionRequest(eEventEnableService);

	return;
}

void GCardReaderService::Disable()
{
	GMutexLock ml(m_CardStateMutex);

	StatusTransitionRequest(eEventDisableService);

	return;
}

bool GCardReaderService::IsMotorized()
{
	return m_blIsMotorized;
}

bool GCardReaderService::ProcessCardReaderReply(GSPOTMessage* pMsg)
{
	bool        blret = false;
	UINT8*      pu8MsgData = NULL;
	UINT16      u16DataLen = 0;
	int         iCmdCode = pMsg->GetSubCommandCode();
	UINT8       u8Ack = ( pMsg->GetData() == NULL ? 0xFF : *(pMsg->GetData()) );
	char        arycError[16];

	if(u8Ack == 0)
	{
		strcpy(arycError, "OK");
	}
	else if(u8Ack == 0xFF)
	{
		strcpy(arycError, "ERROR: No data");
	}
	else
	{
		snprintf(arycError, sizeof(arycError), "ERROR: 0x%02X", UINT(u8Ack));
	}

	switch( iCmdCode )
	{
		case eSPOT_PP_CR_ENABLE:
			LOG(eCardReader, u8Ack == 0 ? eDebug : eError,
			    "Card Reader enable reply %s .\n", arycError);
			blret = true;
		break;

		case eSPOT_PP_CR_DISABLE:
			LOG(eCardReader, u8Ack == 0 ? eDebug : eError,
			    "Card Reader disable reply %s .\n", arycError);
			blret = true;
		break;

		case eSPOT_PP_CR_READ:
			u16DataLen = pMsg->GetDataLen();
			if(u16DataLen == 0)
			{
				break;
			}

			pu8MsgData = pMsg->GetData();
			if(pu8MsgData == NULL)
			{
				break;
			}

			blret = ProcessDataEvent(pu8MsgData, u16DataLen);
		break;

		default:
		break;
	}

	return blret;
}

/**
 *
 *  Parse message : Diagnostic Information 0x10, 0x04
 *
 * Similar to above case but now each device has different parameters set. This is managed by a variable length
 * sequence of tagged value. That is a sequence of 1byte for parameter tag and n bytes for parameter value
 * (where depends on the parameter type).

 * ACKCode[1]
 * (see: 6.1.1).
 * NumEntries[1]
 * Entries[NumEntries]
 * (see 6.1.5 for Tag coding and parameters meaning)
 * ItemId[1]
 * ItemSize[1]
 * ItemData[ItemSize]
 * @param GSPOTMessage the message answer to parse
 * */
int GCardReaderService::ParseDiagnosticInfoReply(GSPOTMessage* pMsg)
{
	GMutexLock ml(m_SincronizeMutex);

	int len = pMsg->GetDataLen();
	if(len < 8)
		return 0;

	UINT8 *pu8Data = new UINT8[len];
	memset(pu8Data, 0, len);
	pMsg->GetData(pu8Data, len);
	if(pu8Data[0] != 0x00)
	{
		delete[] pu8Data;
		return 0;
	}

	int nItems = pu8Data[1];
	if(!nItems)
	{
		delete[] pu8Data;
		return 0;
	}

	int iItemNo = 0, iItemID = 0, iItemLen = 0, iProcessedItems = 0;
	UINT8* pb = pu8Data + 2;
	char*  pc = NULL;
	UINT32 value = 0;

	while( iItemNo < nItems && (pb - pu8Data) < len)
	{
		iItemID =  *pb++;
		iItemLen = *pb++;

		if(iItemID >= eSPOT_CR_CARDINSERTIONCNT && iItemID <= eSPOT_CR_READICCKO)
		{
			iProcessedItems ++;
			value = 0;
			memcpy(&value, pb, sizeof(UINT32));
			value = GConversion::BigEndianToHost(value);
			LOG(eCardReader, eDebug, "[0x%x]=[%ld]\n", iItemID, value);

			if( iItemID == eSPOT_CR_READISO1OK )      m_oCRSPOTStatistics.SetStatistics( GCardReaderServiceStatistics::ePass,
							                                                         GCardReaderServiceStatistics::eTrack1,
							                                                         value );

			else if( iItemID == eSPOT_CR_READISO2OK ) m_oCRSPOTStatistics.SetStatistics( GCardReaderServiceStatistics::ePass,
                                                                                     GCardReaderServiceStatistics::eTrack2,
                                                                                     value );

			else if( iItemID == eSPOT_CR_READISO3OK ) m_oCRSPOTStatistics.SetStatistics( GCardReaderServiceStatistics::ePass,
                                                                                     GCardReaderServiceStatistics::eTrack3,
                                                                                     value );

			else if( iItemID == eSPOT_CR_READICCOK  ) m_oCRSPOTStatistics.SetStatistics( GCardReaderServiceStatistics::ePass,
                                                                                     GCardReaderServiceStatistics::eChip,
                                                                                     value );

			else if( iItemID == eSPOT_CR_READISO1KO ) m_oCRSPOTStatistics.SetStatistics( GCardReaderServiceStatistics::eFail,
                                                                                     GCardReaderServiceStatistics::eTrack1,
                                                                                     value );

			else if( iItemID == eSPOT_CR_READISO2KO ) m_oCRSPOTStatistics.SetStatistics( GCardReaderServiceStatistics::eFail,
                                                                                     GCardReaderServiceStatistics::eTrack2,
                                                                                     value );

			else if( iItemID == eSPOT_CR_READISO3KO ) m_oCRSPOTStatistics.SetStatistics( GCardReaderServiceStatistics::eFail,
                                                                                     GCardReaderServiceStatistics::eTrack3,
                                                                                     value );

			else if( iItemID == eSPOT_CR_READICCKO  ) m_oCRSPOTStatistics.SetStatistics( GCardReaderServiceStatistics::eFail,
                                                                                     GCardReaderServiceStatistics::eChip,
                                                                                     value );

			else if( iItemID == eSPOT_CR_CARDINSERTIONCNT  ) m_oCRSPOTStatistics.SetTotalSwipes(value);

			else
			{
				LOG(eCardReader, eWarning, "Ignoring item id [0x%x] with value [0x%d] for diagnostic info in CardReader.\n", iItemID, value);

				iProcessedItems --;
			}
		}

		pb += iItemLen;
		iItemNo++;
	}

	delete[] pu8Data;

	m_Condition.Signal();

	return iProcessedItems;
}

bool GCardReaderService::ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error)
{
	LOG(eCardReader, eDebug, "[st %02X, stex %02X, error %02X]\n", int(u8Status), int(u8StatusEx), int(u8Error) );

	GMutexLock ml(m_CardStateMutex);

	// errors and busy analysis have the highest priority
	if(u8Status >= eSPOT_CR_ERROR_STATE || u8Error >= eSPOT_CR_READERERROR)
	{
		if(!m_blIsMotorized || (u8Error != eSPOT_CR_GENERICERROR)) //Ignoring generic error
		{
			StatusTransitionRequest(eEventCardReadingError);

			return false;
		}
	}

	m_u8AttemptsInError = 0;
	bool blProcessed = false;

	if(u8Status == eSPOT_CR_BUSY_STATE)
	{
		LOG(eCardReader, eNotice, "Card reader reporting a BUSY state, Event ignored.\n");
		return true;
	}

	if(u8Status == eSPOT_CR_ENABLED_STATE && m_blCREnabled == false)
	{
		blProcessed = true;
		m_blCREnabled = true; // anti bounce flag (do not process CR enabled multiple times)
		m_blDataEventPending=true; // Flag to wait for Data Event before Remove Card Event
		StatusTransitionRequest(eEventCardReaderIsEnabled);
	}

	if(u8Status == eSPOT_CR_DISABLED_STATE && m_blCREnabled == true)
	{
		blProcessed = true;
		m_blCREnabled = false; // anti bounce flag (do not process CR disabled multiple times)
		StatusTransitionRequest(eEventCardReaderIsDisabled);
	}

	if( u8StatusEx == 0x81 && ((u8Error == 0x00) || (m_blIsMotorized && (u8Error = eSPOT_CR_GENERICERROR))) && m_blCardInserted == false )
	{
		m_blCardInserted = true; // anti bounce flag (do not process card insert multiple times)
		StatusTransitionRequest(eEventCardInsert);
		return true;
	}

	if( u8StatusEx == 0x80 && ((u8Error == 0x00) || (m_blIsMotorized && (u8Error = eSPOT_CR_GENERICERROR))) && m_blCardInserted == true )
	{
		if(u8Status != eSPOT_CR_ENABLED_STATE  || m_blIsMotorized)
		{
			m_blCardInserted = false; // anti bounce flag (do not process card remove multiple times)
			StatusTransitionRequest(eEventCardRemove);
		}
		return true;
	}

	if( u8StatusEx == 0x83 && u8Error == 0x00 )
	{
		// the event sequence "card insert, chip card insert" is present in some CR models
		if( m_blCardInserted == false || m_eStatus == eStatusCardInserted )
		{
			m_blCardInserted = true; // anti bounce flag (do not process card insert multiple times)
			StatusTransitionRequest(eEventChipCardInsert);
			return true;
		}
	}

	if( blProcessed == false )
	{
		LOG(eCardReader, eWarning, "[st %02X, stex %02X, error %02X][IGNORED!.]\n", int(u8Status), int(u8StatusEx), int(u8Error) );
		return false;
	}

	return true;
}

/**
 * @brief This method must not be called from a SPOT answer thread.
 *
 */
bool GCardReaderService::RequestStatisticsToSpot()
{
	GMutexLock ml(m_SincronizeMutex);

	if( !SendDiagnosticInfoCmdToSPOT() )
	{
		return false;
	}

	int iretries = 3, iret = 0;
	while( iretries > 0 )
	{
		iret = m_Condition.Wait(CR_WAIT_DIAGNOSTIC_INFO);

		if(iret == 1)
		{
           break;
		}
		else
		{
           --iretries;
		}
	}

	if( iret != 1 )
	{
		return false;
	}

	return true;
}



//##########################################################################################
//##########################################################################################
// Private methods
//##########################################################################################
//##########################################################################################

GCardReaderService::GCardReaderService(GSPOTSession* pSession)
	:	m_pSession(pSession), m_pTimer(NULL), m_pManager(NULL),
		m_blCREnabled(false), m_blCardInserted(false),m_blDataEventPending(true),m_blIsMotorized(false),
		m_blTimedOut(false),m_blM3Unit(false), m_blM5Unit(false), m_blATRError(false),
		m_u8AttemptsInError(0),
		m_eStatus(eStatusInitial),
		m_Condition(&m_SincronizeMutex),
		m_TOutCommand(this, &GCardReaderService::ProcessTimeoutEvent)
{
	ClearTrackInfo();

	m_blM3Unit = GGlobalSystemInformation::GetInstance()->isM3Unit();
	if( ! m_blM3Unit )
	{
		m_blM5Unit = GGlobalSystemInformation::GetInstance()->isM5Unit();
	}
	LOG(eCardReader, eInfo, "Reading Card Reader Info.\n");
	CR_HW_INFO& rCRInfo = GGlobalSystemInformation::GetInstance()->GetCardReaderInfo();
	LOG(eCardReader, eInfo, "Card Reader Type -> %02X \n",rCRInfo.m_u8DevType);

	if(rCRInfo.m_u8DevType == 0x84)
	{
		m_blIsMotorized =true;
	}

	// One shot, non autostart timer is created.
	m_pTimer = new GTimer(&m_TOutCommand, CR_EVENT_TIMEOUT, true, false);
	LOG(eCardReader, eInfo, "Card Reader Service started.\n");
}

GCardReaderService::~GCardReaderService()
{
	LOG(eCardReader, eInfo, "Card Reader Service stopped.\n");
}

void GCardReaderService::ClearTrackInfo()
{
	memset( mst_aryTracks, 0, sizeof(mst_aryTracks) );
}

void GCardReaderService::Set3TracksBad()
{
	mst_aryTracks[0].m_u8Present = 0xFF;
	mst_aryTracks[0].m_u16Length = 0;
	mst_aryTracks[0].m_u8ErrorCode = 0x80;
	mst_aryTracks[1].m_u8Present = 0xFF;
	mst_aryTracks[1].m_u16Length = 0;
	mst_aryTracks[1].m_u8ErrorCode = 0x80;
	mst_aryTracks[2].m_u8Present = 0xFF;
	mst_aryTracks[2].m_u16Length = 0;
	mst_aryTracks[2].m_u8ErrorCode = 0x80;
}

/*This function is used to mark both Magnetic tracks and Chip Data
 as FAIL when Card data is lost/not read, or Card Insert is bad
 Currently used for Motorized only */
void GCardReaderService::Set4TracksBad()  //4th track is ICC data
{
	mst_aryTracks[0].m_u8Present = 0xFF;
	mst_aryTracks[0].m_u16Length = 0;
	mst_aryTracks[0].m_u8ErrorCode = 0x80;
	mst_aryTracks[1].m_u8Present = 0xFF;
	mst_aryTracks[1].m_u16Length = 0;
	mst_aryTracks[1].m_u8ErrorCode = 0x80;
	mst_aryTracks[2].m_u8Present = 0xFF;
	mst_aryTracks[2].m_u16Length = 0;
	mst_aryTracks[2].m_u8ErrorCode = 0x80;
	mst_aryTracks[3].m_u8Present = 0xFF;
	mst_aryTracks[3].m_u16Length = 0;
	mst_aryTracks[3].m_u8ErrorCode = 0x80;
}

UINT16 GCardReaderService::ClipPartialLength(UINT16 u16Length, int nTrack)
{
	if(nTrack < eTrack1 || nTrack > eTrack4) return 0; // parameter error protection

	const char* pcMethod = "GCardReaderService::ClipPartialLength() - ";
	UINT16 u16MAXLen = CR_TRACK_SIZE - 1;

	if(u16Length > u16MAXLen)
	{
		char cmsg[96];

		LOG(eCardReader, eWarning,
		    "%s Track #%u, Data Length: %u, clipped to %u bytes limit.\n",
		    pcMethod, 1 + nTrack, u16Length, u16MAXLen);

		return u16MAXLen; // it gives the available room and no track buffer overflow
	}
	else
	{
		return u16Length;
	}
}

void GCardReaderService::ResetEnable(UINT32 u32delay /* = 0 */)
{
	DisableRequest();

	if(u32delay)
	{
		usleep(u32delay); // u32delay in usec
	}
	else
	{
		usleep(100000); // 0.1sec default delay
	}

	EnableRequest();
}

bool GCardReaderService::EnableRequest(bool blCheckStatus /* = false */)
{
	if(blCheckStatus && m_blCREnabled)
	{
		return true; // Nothing to do: Card Reader device is already enabled.
	}

	const char* pcMethod = "GCardReaderService::EnableRequest() - ";

	if( ! SendEnableCmdToSPOT() )
	{
		LOG(eCardReader, eError, "%s%s", pcMethod, "Could not send asynchronous Enable message to SPOT.\n");
		return false;
	}

	LOG(eCardReader, eTrace, "%s%s", pcMethod, "SPOT card reader Enable message has been queued to SPOT.\n");
	return true;
}

bool GCardReaderService::DisableRequest(bool blCheckStatus /* = false */)
{
	if(blCheckStatus && !m_blCREnabled)
	{
		return true; // Nothing to do: Card Reader device is already disabled.
	}

	const char*  pcMethod = "GCardReaderService::DisableRequest() - ";

	if( ! SendDisableCmdToSPOT() )
	{
		LOG(eCardReader, eError, "%s%s", pcMethod, "Could not send asynchronous Disable message to SPOT.\n");
		return false;
	}

	LOG(eCardReader, eTrace, "%s%s", pcMethod, "SPOT card reader Disable message has been queued to SPOT.\n");
	return true;
}

bool GCardReaderService::ChipReset_ATRRequest()
{
	const char*  pcMethod = "GCardReaderService::ChipReset_ATRRequest() - ";

	if( m_pSession == NULL )
	{
		return false;
	}

	LOG(eCardReader, eTrace, "%s%s", pcMethod, "SPOT card reader chip reset message has been queued to SPOT.\n");

	UINT8* pu8ChipResetCRCmd = const_cast<UINT8*>(mst_aryu8ChipResetCRCmd);
	GSPOTMessage oMsg(pu8ChipResetCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GCardReaderService::SendEnableCmdToSPOT()
{
	// TODO: correct GSPOTMessage class (several methods) and
	//       its other clients to accept const UINT8* as parameter

	if( m_pSession == NULL )
	{
		return false;
	}

	UINT8* pu8EnableCRCmd = const_cast<UINT8*>(mst_aryu8EnableCRCmd);
	GSPOTMessage oMsg(pu8EnableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GCardReaderService::SendDisableCmdToSPOT()
{
	// TODO: correct GSPOTMessage class (several methods) and
	//       its other clients to accept const UINT8* as parameter

	if( m_pSession == NULL )
	{
		return false;
	}

	UINT8* pu8DisableCRCmd = const_cast<UINT8*>(mst_aryu8DisableCRCmd);
	GSPOTMessage oMsg(pu8DisableCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GCardReaderService::SendDiagnosticInfoCmdToSPOT()
{
	// TODO: correct GSPOTMessage class (several methods) and
	//       its other clients to accept const UINT8* as parameter

	if( m_pSession == NULL )
	{
		return false;
	}

	UINT8* pu8DiagnosticInfoCRCmd = const_cast<UINT8*>(mst_aryu8DiagnosticInfoCRCmd);

	GConversion::LogBytesAsASCIIHexString((const UINT8*)mst_aryu8DiagnosticInfoCRCmd, sizeof(mst_aryu8DiagnosticInfoCRCmd));

	GSPOTMessage oMsg(pu8DiagnosticInfoCRCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

/**
 * This method implements the state machine.
 * @param e is the event code that initiates a state transition.
 * @param pEventData is the optional pointer to data optionally needed for the current state.
 * @return void
 *
 */
void GCardReaderService::StatusTransitionRequest(eEvents e, sDataEvent* pEventData /* = NULL */)
{
	const char* pcInvalidEventError = "INVALID EVENT [%s] received in [%s].\n";

	//==========================================================================================================
	if( e == eEventCardReadingError )
	{
		MarkCardReaderAsError();
		return;
	}
	//==========================================================================================================


	switch(m_eStatus)
	{
		//======================================================================================================
		case eStatusInitial:
		{
			switch(e)
			{
				case eEventEnableService:
					NextState(eEventEnableService, eStatusCardOut);
					EnableRequest();
				break;

				default:
				break; // This service must be enabled in order to process further messages.
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusCardOut:
		{
			switch(e)
			{
				case eEventDisableService:
					NextState(eEventDisableService, eStatusInitial);
					DisableRequest();
				break;

				case eEventCardInsert:
					NextState(eEventCardInsert, eStatusCardInserted);
					if(m_blIsMotorized)
					{
						NotifyProcessingCard();
					}
					else
					{
						NotifyCardInserted();
					}
					m_blTimedOut=false;
					StartTimer();
				break;

				case eEventChipCardInsert:
					NextState(eEventChipCardInsert, eStatusChipCardInserted);
					ChipReset_ATRRequest();
					if(m_blM5Unit) { GSystem::Sleep(500); }// make sure proper card lock
					NotifyChipCardInserted();
					StartTimer();
				break;

				case eEventCardRemove:
					LOG(eCardReader, eNotice, "Late card remove arrival while card is out.\n");
					NextState(eEventCardRemove, eStatusCardOut);
					EnableRequest(); // protection against card inserted during "lunch"
				break;

				case eEventCardReaderIsEnabled: // no status change, just logging.
					NextState(eEventCardReaderIsEnabled, eStatusCardOut);
				break;

				case eEventCardReaderIsDisabled: // no status change, but enabling the CR.
					NextState(eEventCardReaderIsDisabled, eStatusCardOut);
					EnableRequest(); // don't keep the CR disabled
				break;

				case eEventCardData: // logging alert for some kind of desynchronization.
					LOG(eCardReader, eWarning, "WARNING: Data Arrival before Card Insert event !! - Discarded.\n");
					NextState(eEventCardData, eStatusCardOut);
				break;

				case eEventTimeout: // no status change, just logging.
					LOG(eCardReader, eNotice, "Late timeout arrival while card is out.\n");
					NextState(eEventTimeout, eStatusCardOut);
				break;

				default:
					LOG(eCardReader, eError, pcInvalidEventError, mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusCardInserted:
		{
			switch(e)
			{
				case eEventDisableService:
					NextState(eEventDisableService, eStatusInitial);
					DisableRequest();
				break;

				case eEventCardData:
					NextState(eEventCardData, eStatusDataArrived);
					if(m_blIsMotorized)
					{
						StopTimer();
						m_blTimedOut=false;
						StartTimer();
					}

					if(pEventData != NULL)
					{
						if( ProcessCardInfo((UINT8*)pEventData->data, pEventData->size) != 0 )
						{
							LOG(eCardReader, eError, "Could not process card data.!!!\n");
						}
					}
					m_blDataEventPending =false;
					LOG(eCardReader, eDebug, "[eStatusCardInserted]-> EventCardData :ProcessDataEvent :%d\n",m_blDataEventPending);
					if(m_blIsMotorized)
					{
						DisableRequest();
						GSystem::Sleep(50);
					}
				break;

				case eEventCardReaderIsEnabled: // no status change, just logging.
				case eEventCardReaderIsDisabled:
					NextState(e, eStatusCardInserted);
				break;

				case eEventCardRemove:
					if(m_blIsMotorized)
					{
						LOG(eCardReader, eWarning, "CardRemove event, Data never came down,lost or Bad Card Insert !!!\n ");
						NextState(eEventCardRemove, eStatusCardOut);
						if(!m_blTimedOut)
						{
							Set4TracksBad();
							ExecuteCardRemoveSteps(true, true);
						}
						else
						{
							EnableRequest();
						}
						break;
					}

					if(m_blDataEventPending == false)
					{
						NextState(eEventCardRemove, eStatusCardOut);
						ExecuteCardRemoveSteps(true, true);
					}
					else
					{
						NextState(eEventCardRemove, eStatusCardInserted);
					}
				break;

				case eEventTimeout:
					m_blTimedOut = true;
					LOG(eCardReader, eWarning, "Timeout detected while card is still inserted.\n");
					if(m_blIsMotorized)
					{
						NextState(eEventTimeout, eStatusCardInserted);
						if(m_blCREnabled)
						{
							DisableRequest();
							GSystem::Sleep(50);
						}
						Set4TracksBad();
						NotifyCardInserted();
						ExecuteCardRemoveSteps(false,false);
						break;
					}
					NextState(eEventTimeout, eStatusCardOut);

					//	Card has not been removed in a reasonable amount of time.
					//	Assume "card out" state and report all bad tracks.
					Set3TracksBad();
					//ExecuteCardRemoveSteps(false, false);// just notify card removed.
					ExecuteCardRemoveSteps(true, true);// just notify card removed.

				break;

				default:
					LOG(eCardReader, eError, pcInvalidEventError, mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
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
				case eEventDisableService:
					NextState(eEventDisableService, eStatusInitial);
					DisableRequest();
				break;

				case eEventCardData:
					NextState(eEventCardData, eStatusDataArrived);
					LOG(eCardReader, eWarning, "WARNING: Card Data AGAIN -- Ignored!!!\n");
				break;

				case eEventCardRemove:
					LOG(eCardReader, eNotice, "CARD REMOVE event.\n");
					NextState(eEventCardRemove, eStatusCardOut);
					ExecuteCardRemoveSteps(true, true);// STOP the timer, notify, and ENABLE CR.
				break;

				case eEventCardReaderIsEnabled:
					if(m_blDataEventPending ==false && !m_blIsMotorized)
					{
						NextState(eEventCardRemove, eStatusCardOut);
						LOG(eCardReader, eNotice, "magnetic CARD REMOVE event.\n");
						ExecuteCardRemoveSteps(true, true);// STOP the timer, notify, and ENABLE CR.
					}
					else
					{
						NextState(e, eStatusDataArrived); // just logging
					}
				break;

				case eEventCardReaderIsDisabled:
					if(m_blIsMotorized)
					{
						LOG(eCardReader, eNotice, "Motorized CARD READER Disable event.\n");
						NextState(e, eStatusDataArrived);
						NotifyCardInserted();
						//ExecuteCardRemoveSteps(true, true);// STOP the timer, notify, and ENABLE CR.
					}

					else if( m_blPrematureCardRemove )
					{
						LOG(eCardReader, eNotice, "CARD REMOVE event.\n");
						NextState(eEventCardRemove, eStatusCardOut);
						ExecuteCardRemoveSteps(true, true);// STOP the timer, notify, and ENABLE CR.
					}
					else if(m_blDataEventPending ==false)
					{

						NextState(eEventCardRemove, eStatusCardOut);
						LOG(eCardReader, eNotice, "magnetic CARD REMOVE event.\n");
						ExecuteCardRemoveSteps(true, true);// STOP the timer, notify, and ENABLE CR.

					}
					else
					{
						NextState(e, eStatusDataArrived); // just logging
					}
				break;

				case eEventTimeout:
					m_blTimedOut =true;
					LOG(eCardReader, eNotice, "Timeout event after data arrived, waiting for card remove.\n");

					if(m_blIsMotorized)
					{
						NextState(e,eStatusCardOut);
						if(m_blCREnabled)
						{
							DisableRequest();
							GSystem::Sleep(50);
						}

						ExecuteCardRemoveSteps(false, false);
					}
					else
					{
						NextState(eEventCardRemove, eStatusCardOut);
						ExecuteCardRemoveSteps(false, true);// just notify card removed.
					}
				break;

				default:
					LOG(eCardReader, eError, pcInvalidEventError, mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusChipCardInserted:
		{
			switch(e)
			{
				case eEventDisableService:
					NextState(eEventDisableService, eStatusInitial);
					DisableRequest(); // unlock the card if chip reset has just been sent.
				break;

				case eEventCardData:
					NextState(eEventCardData, eStatusATRDataArrived);
					if(pEventData != NULL)
					{
						if( ProcessChipCardInfo((UINT8*)pEventData->data, pEventData->size) != 0 )
						{
							LOG(eCardReader, eError, "Could not process chip card data.!!!\n");
							m_blATRError = true;
						}
					}

					EnableRequest(); // unlock the card unleashing the mag stripe data
					if(m_blM5Unit) { GSystem::Sleep(750); }
					StartTimer();
					NotifyCardInserted();
				break;

				case eEventCardRemove:
					StopTimer();
					NextState(eEventCardRemove, eStatusCardOut);
					LOG(eCardReader, eWarning, "Chip Data never came down, lost!!!\n");

					// There is not card data since it was cleaned up in the initial state(s)
					// So at this point the card event should report non present tracks.
					ExecuteCardRemoveSteps(false, false);// just notify card removed.
				break;

				case eEventTimeout:
					LOG(eCardReader, eWarning, "Timeout while chip card is inserted!!\n");
					DisableRequest();
					GSystem::Sleep(50);
					NextState(eEventTimeout, eStatusChipCardInserted);
					//	Card has not reported ATR in a reasonable amount of time.
					//  Keep disabling the CR until the card is released by mechanism.
					StartTimer();
				break;

				default:
					LOG(eCardReader, eError, pcInvalidEventError, mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusATRDataArrived:
		{
			switch(e)
			{
				case eEventDisableService:
					NextState(eEventDisableService, eStatusInitial);
					DisableRequest(); // unlock the card
				break;

				case eEventCardData:
					//StopTimer(); NOT ANYMORE
					NextState(eEventCardData, eStatusDataArrived);
					if(pEventData != NULL)
					{
						if( ProcessCardInfo((UINT8*)pEventData->data, pEventData->size) != 0 )
						{
							LOG(eCardReader, eError, "Could not process card data.!!!\n");
						}
					}
				break;

				case eEventCardRemove:
					// this event is temporarily ignored until the card reader disables itself.
					LOG(eCardReader, eDebug, "card reported outside while ATRDataArrived.\n");
					m_blPrematureCardRemove = true;
					StartTimer(); // card remove before mag card data protection
				break;

				case eEventCardInsert:
				case eEventChipCardInsert:
					NextState(e, eStatusATRDataArrived); // just logging
				break;

				case eEventCardReaderIsDisabled:
				case eEventCardReaderIsEnabled: // no status change, just logging.
					NextState(e, eStatusATRDataArrived);
				break;

				case eEventTimeout:
					NextState(eEventTimeout, eStatusCardOut);
					LOG(eCardReader, eWarning, "ATR received but timeout detected while card is still inserted.\n");

					//	Card has not been removed in a reasonable amount of time.
					//	Assume "card out" state and report all magnetic tracks as bad.
					Set3TracksBad();
					ExecuteCardRemoveSteps(false, true);// just notify card removed.
				break;

				default:
					LOG(eCardReader, eError, pcInvalidEventError, mst_arypcEventLabel[e], mst_arypcStatusLabel[m_eStatus]);
				break;
			}
		}
		break;
		//======================================================================================================

		//======================================================================================================
		case eStatusTempError:
		{
			const char* pcCROk = "Card Reader recovered from error condition.";
			const char* pcCRKo = "ERROR: event [%s] received in eStatusTempError. Card Reader device is temporary KO. Attempts: %u\n";
			switch(e)
			{
				case eEventDisableService:
					NextState(eEventDisableService, eStatusInitial);
					DisableRequest();
				break;

				case eEventCardInsert:
					LOG( eCardReader, eNotice, pcCROk );
					NextState(eEventCardInsert, eStatusCardInserted);
				break;

				case eEventChipCardInsert:
					LOG( eCardReader, eNotice, pcCROk );
					NextState(eEventChipCardInsert, eStatusChipCardInserted);
				break;

				case eEventCardRemove:
					LOG( eCardReader, eNotice, pcCROk );
					NextState(eEventCardRemove, eStatusCardOut);
				break;

				default:
					LOG( eCardReader, eError, pcCRKo,
					     mst_arypcEventLabel[e], int(m_u8AttemptsInError) );
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
				// the only transition allowed in this state.
				case eEventDisableService:
					NextState(eEventDisableService, eStatusInitial);
					DisableRequest();
				break;

				default:
					LOG(eCardReader, eError, "ERROR: event [%s] received in eStatusError. The Card Reader device is in unrecoverable state.\n", mst_arypcEventLabel[e]);
				break;
			}
		}
		break;
		//======================================================================================================
	}
}

void GCardReaderService::MarkCardReaderAsError()
{
	const char* pcMethod = "GCardReaderService::MarkCardReaderAsError() -- ";

	if((m_u8AttemptsInError++) < CR_ERROR_MAXATTEMPTS)
	{
		LogErrorEvent();
		ResetEnable(300000); // 0.3 sec for recovery time
		ClearTrackInfo();
		StopTimer();
		NextState(eEventError, eStatusTempError);
	}
	else if(m_eStatus != eStatusError) // go to final permanent error state, if it is not there.
	{
		NextState(eEventError, eStatusError);
		LOG(eCardReader, eFatal, "Consecutive errors reached the maximum count. Card Reader shall be PERMANENTLY DISABLED !!!\n");
	}
	// if it is already in permanent error ( eStatusError ), do nothing
}

void GCardReaderService::ExecuteCardRemoveSteps(bool blStopTimer, bool blEnableCR)
{
	if( blStopTimer )
	{
		StopTimer();
	}

	NotifyCardRemoved();
	ClearTrackInfo();
	m_blATRError = false;
	m_blPrematureCardRemove = false;

	if( blEnableCR )
	{
		LOG(eCardReader, eDebug, "must re-enable the CR...\n");
		EnableRequest();
	}
}

void GCardReaderService::UpdateSessionStatistics(TRACK_INFO* pTrack, GCardReaderServiceStatistics::eTrack track)
{
	if (pTrack->m_u8Present == 0)
	{
		//this means that the track was not present at all, or the hardware didn't supported that type of track
		//for example it doesn't have a chip reader.
		m_oCRSessionStatistics.IncrementStatistics(GCardReaderServiceStatistics::eNotPresent, track);
	}
	else
	{
		switch (pTrack->m_u8ErrorCode)
		{
			case 0:
				m_oCRSessionStatistics.IncrementStatistics(GCardReaderServiceStatistics::ePass, track);
			return;

			case 1:
			case 0x20:
				m_oCRSessionStatistics.IncrementStatistics(GCardReaderServiceStatistics::eNotPresent, track);
			return;

			case 2:
			case 4:
			case 8:
			case 0x10:
			case 0x40:
			case 0x80:
			default:
				m_oCRSessionStatistics.IncrementStatistics(GCardReaderServiceStatistics::eFail, track);
			return;
		}
	}
}

void GCardReaderService::NotifyCardRemoved()
{
	UpdateSessionStatistics(&mst_aryTracks[eTrack1], GCardReaderServiceStatistics::eTrack1);
	UpdateSessionStatistics(&mst_aryTracks[eTrack2], GCardReaderServiceStatistics::eTrack2);
	UpdateSessionStatistics(&mst_aryTracks[eTrack3], GCardReaderServiceStatistics::eTrack3);
	UpdateSessionStatistics(&mst_aryTracks[eChip  ], GCardReaderServiceStatistics::eChip  );

	m_oCRSessionStatistics.IncrementTotalSwipes();

	if(m_pManager)
	{
		m_pManager->NotifyCardRemoved( mst_aryTracks );
	}
}

void GCardReaderService::NotifyCardInserted()
{
	if(m_pManager)
	{
		m_pManager->NotifyCardInserted();
	}
}

void GCardReaderService::NotifyProcessingCard()
{
	if(m_pManager)
	{
		m_pManager->NotifyProcessingCard();
	}
}

void GCardReaderService::NotifyChipCardInserted()
{
	if(m_pManager)
	{
		m_pManager->NotifyChipCardInserted();
	}
}

bool GCardReaderService::StopTimer()
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Stop();
}

bool GCardReaderService::StartTimer()
{
	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Start();
}

void GCardReaderService::ProcessTimeoutEvent()
{
	const char* pcMethod = "GSPOTCardReader::ProcessTimeoutEvent() -- ";

	GMutexLock ml(m_CardStateMutex);

	StatusTransitionRequest(eEventTimeout);
}

bool GCardReaderService::ProcessDataEvent(UINT8* pMsgData, UINT16 u16DataSize)
{
	const char* pcMethod = "GCardReaderService::ProcessDataEvent() -- ";

	GMutexLock ml(m_CardStateMutex);

	sDataEvent oDataEvent;
	oDataEvent.data = (void *) pMsgData;
	oDataEvent.size = u16DataSize;

	m_blDataEventPending = true;
	LOG(eCardReader, eDebug, " ProcessDataEvent : %d\n",m_blDataEventPending);
	StatusTransitionRequest(eEventCardData, &oDataEvent);

	return true;
}

void GCardReaderService::NextState(eEvents e, eStatus s)
{
	LOG( eCardReader, eDebug, "[curr_status[%s] ==>> event[%s] ==>> nxt_status[%s]]\n", mst_arypcStatusLabel[m_eStatus], mst_arypcEventLabel[e], mst_arypcStatusLabel[s] );
	m_eStatus = s;
}

UINT8 GCardReaderService::ProcessChipCardInfo(UINT8* pMsgData, UINT16 u16DataSize)
{
	UINT8*      pu8data = NULL; // auxiliary data pointer
	UINT16      u16len, u16lenclp;
	int         ierror, nTracks;
	const char* pcMethod = "GCardReaderService::ProcessChipCardInfo - ";

	pu8data = pMsgData;
	ierror = *pu8data++; // overall error code
	if(ierror != 0)
	{
		LOG(eCardReader, eWarning, "%sSPOT returned the overall error %02X\n", pcMethod, ierror);
		return ierror;// the general error code
	}

	nTracks = *pu8data++; // number of tracks
	if(nTracks > 4)
	{
		nTracks = 4; // protective
	}

	// Uncomment just for debugging purposes ONLY.
	// GConversion::LogBytesAsASCIIHexString(pMsgData, u16DataSize);

	// iteration over SPOT message fields
	for(int n = 0, idtrack, ntrack ; n < nTracks && (pu8data - pMsgData) < u16DataSize; n++)
	{
		idtrack = *pu8data++; //track source

		if(idtrack & CR_TRACK1_MASK)
		{
			ntrack = eTrack1;
		}
		else if(idtrack & CR_TRACK2_MASK)
		{
			ntrack = eTrack2;
		}
		else if(idtrack & CR_TRACK3_MASK)
		{
			ntrack = eTrack3;
		}
		else if(idtrack & CR_TRACK4_MASK)
		{
			ntrack = eTrack4;
		}
		else // extremely odd situation
		{
			LOG(eCardReader, eWarning, "%s WARNING: WRONG field ID bitmap: %02X\n", pcMethod, idtrack);

			ntrack = -1; // bad value, no track #
		}

		UINT8 bh = 0, bl = 0, u8err = 0;

		u8err  =  *pu8data++; // track error code
		bh     =  *pu8data++; // length MSB , SPOT protocol: always big endian
		bl     =  *pu8data++; // length LSB , SPOT protocol: always big endian

		u16len  = 256 * bh + bl; // track data length
		u16lenclp = ClipPartialLength(u16len, ntrack); // track buffer should be large enough !!

		if(ntrack == eTrack4) // it means a valid Track# reported by SPOT
		{
			mst_aryTracks[ntrack].m_u8Present 	= 0xFF; // valid track handled by the device
			mst_aryTracks[ntrack].m_u8ErrorCode = u8err;
			mst_aryTracks[ntrack].m_u16Length 	= u16lenclp;

			if(u16lenclp > 0)
			{
				memcpy(mst_aryTracks[ntrack].m_aryu8Data, pu8data, u16lenclp);
				mst_aryTracks[ntrack].m_aryu8Data[u16lenclp] = 0; // safety reason
			}
			else
			{
				memset(mst_aryTracks[ntrack].m_aryu8Data, 0, CR_TRACK_SIZE);
			}

			if(u8err == 0)
			{
				LOG(eCardReader, eInfo, "ATR chip card read valid, length: %d bytes .\n", u16lenclp);
				if(  mst_aryTracks[ntrack].m_u16Length > 0 )
				{
					GConversion::LogBytesAsASCIIHexString(mst_aryTracks[ntrack].m_aryu8Data, mst_aryTracks[ntrack].m_u16Length);
					LOG(eCardReader, eDebug, "ProcessChipCardInfo: Track #%d valid,  error code: %02X,  Length: %d bytes .\n", (1 + ntrack), u8err, u16lenclp);
				}
				ierror = 0;
			}
			else
			{
				LOG(eCardReader, eError, "ERROR: reading ATR chip card, length: %d bytes, error: %d.\n", u16lenclp, u8err);
				ierror = u8err;
			}
		}

		pu8data += u16len; // for the next iteration
	}

	return ierror;
}

UINT8 GCardReaderService::ProcessCardInfo(UINT8* pMsgData, UINT16 u16DataSize)
{
	UINT8*      pu8data = NULL; // auxiliary data pointer
	UINT16      u16len, u16lenclp;
	int         ierror, nTracks;
	int         pTrack = eTrack3; // Indicates upper limit for number of tracks to process depending on cardreader type,  eTrack4 for Motorized


	if(m_blIsMotorized)
	{
		pTrack = eTrack4;
	}
	const char* pcMethod = "GCardReaderService::ProcessCardInfo - ";

	pu8data = pMsgData;
	ierror = *pu8data++; // overall error code
	if(ierror != 0)
	{
		LOG(eCardReader, eError, "%sSPOT returned the overall error %02X\n", pcMethod, ierror);
		return ierror;// the general error code
	}

	nTracks = *pu8data++; // number of tracks
	if(nTracks > 4)
	{
		nTracks = 4; // protective
	}

	// Uncomment just for debugging purposes ONLY.
	//GConversion::LogBytesAsASCIIHexString(pMsgData, u16DataSize);

	// iteration over SPOT message fields
	for(int n = 0, idtrack, ntrack ; n < nTracks && (pu8data - pMsgData) < u16DataSize; n++)
	{
		idtrack = *pu8data++; //track source

		if(idtrack & CR_TRACK1_MASK)
		{
			ntrack = eTrack1;
		}
		else if(idtrack & CR_TRACK2_MASK)
		{
			ntrack = eTrack2;
		}
		else if(idtrack & CR_TRACK3_MASK)
		{
			ntrack = eTrack3;
		}
		else if(idtrack & CR_TRACK4_MASK)
		{
			ntrack = eTrack4;
		}
		else // extremely odd situation
		{
			LOG(eCardReader, eWarning, "%s WARNING: WRONG field ID bitmap: %02X\n", pcMethod, idtrack);

			ntrack = -1; // bad value, no track #
		}

		UINT8 bh = 0, bl = 0, u8err = 0;

		u8err  =  *pu8data++; // track error code
		bh     =  *pu8data++; // length MSB , SPOT protocol: always big endian
		bl     =  *pu8data++; // length LSB , SPOT protocol: always big endian

		u16len  = 256 * bh + bl; // track data length
		u16lenclp = ClipPartialLength(u16len, ntrack); // track buffer should be large enough !!

		if(ntrack >= 0 && ntrack <= pTrack) // it means a valid Track# reported by SPOT
		{
			mst_aryTracks[ntrack].m_u8Present 	= 0xFF; // valid track handled by the device
			mst_aryTracks[ntrack].m_u8ErrorCode = u8err;
			mst_aryTracks[ntrack].m_u16Length 	= u16lenclp;

			if(u16lenclp > 0)
			{
				memcpy(mst_aryTracks[ntrack].m_aryu8Data, pu8data, u16lenclp);
				mst_aryTracks[ntrack].m_aryu8Data[u16lenclp] = 0; // safety reason
			}
			else  // protective: clear the track data
			{
				memset(mst_aryTracks[ntrack].m_aryu8Data, 0, CR_TRACK_SIZE);
			}

			LOG(eCardReader, eDebug, "ProcessCardInfo: Track #%d valid,  error code: %02X,  Length: %d bytes .\n", (1 + ntrack), u8err, u16lenclp);
		}

		pu8data += u16len; // for the next iteration
	}

	return ierror;
}

void GCardReaderService::LogErrorEvent()
{
	LOG(eCardReader, eError, "GSPOTCardReader::LogErrorEvent() - Severe error detected. Whole current cycle card reading discarded !!\n");
}
