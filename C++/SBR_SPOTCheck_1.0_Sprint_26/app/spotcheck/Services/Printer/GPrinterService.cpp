/*
 * GPrinterService.cpp
 *
 *  Created on: 18/04/2013
 *      Author: Guillermo Paris
 */
#include <string.h>
#include "GPrinterService.h"
#include "Menus/Printer/GPrinterManager.h"
#include "Common/Util/GConversion.h"

using namespace GSpotDefs;

const UINT8  GPrinterService::mst_aryu8EnqueueDataCmd[PR_QUEDAT_CMDLEN] =
             {0x00, 0x0E, 0x04, 0x00, 0x20, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const UINT8  GPrinterService::mst_aryu8EnqueueMetaDataCmd[PR_QUEDAT_CMDLEN] =
             {0x00, 0x0E, 0x04, 0x00, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

const UINT8  GPrinterService::mst_aryu8JobStatusCmd[PR_STAJOB_CMDLEN] =
             {0x00, 0x0A, 0x04, 0x00, 0x20, 0x03, 0x00, 0x00, 0x00, 0x00};

const UINT8  GPrinterService::mst_aryu8CreateJobCmd[PR_CRTJOB_CMDLEN] =
             {0x00, 0x06, 0x04, 0x00, 0x20, 0x04};

const UINT8  GPrinterService::mst_aryu8ExecuteJobCmd[PR_EXEJOB_CMDLEN] =
             {0x00, 0x0A, 0x04, 0x00, 0x20, 0x05, 0x00, 0x00, 0x00, 0x00};

const UINT8  GPrinterService::mst_aryu8CancelJobCmd[PR_CANJOB_CMDLEN] =
             {0x00, 0x0A, 0x04, 0x00, 0x20, 0x06, 0x00, 0x00, 0x00, 0x00};

const UINT8  GPrinterService::mst_aryu8PRTStatusCmd[PR_PRTSTA_CMDLEN] =
             {0x00, 0x07, 0x04, 0x00, 0x10, 0x01, 0x01};

const char * GPrinterService::mst_arypcStatusLabels[eStatusQty] =
{
	"eReady",
	"eJobOpen",
	"eDataQueued",
	"eExecutingJob",
	"eJobError",
	"ePrinterError"
};

const char * GPrinterService::mst_arypcEventLabels[eEventsQty] =
{
	"eOpenJobEvent",
	"eQueueDataEvent",
	"eExecuteJobEvent",
	"ePrinterReadyEvent",
	"eJobErrorEvent",
	"ePrinterErrorEvent",
	"eTimeoutEvent"
};

GPrinterService* GPrinterService::mst_pSoleInstance = NULL;
GMutex           GPrinterService::mst_InstanceMutex;
UINT8            GPrinterService::mst_aryu8MsgBuffer[DATA_MSG_MAX_LEN];


GPrinterService::GPrinterService(GSPOTSession* pSession)
	: m_blPrinterError(false), m_blPrinterReady(true),
	  m_eStatus(ePrinterReady), m_iCurrentJobID(-1),
	  m_iCurrentSeqNo(-1), m_iCurrentBlockQty(-1),
	  m_Condition(&mst_InstanceMutex),
	  m_pSession(pSession), m_pManager(NULL),
	  m_TOutCommand(this, &GPrinterService::ProcessTimeoutEvent)
{
	m_pTimer = new GTimer(&m_TOutCommand, PR_TOUT_CANCELJOB, false, false);
	LOG(ePrinter, eInfo, "Printer Service Started.\n");
}

GPrinterService::~GPrinterService()
{
	LOG(ePrinter, eInfo, "Printer Service Stopped.\n");
}

GPrinterService* GPrinterService::GetService()
{
	GMutexLock oLock(mst_InstanceMutex);

	return mst_pSoleInstance;
}

void GPrinterService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock oLock(mst_InstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GPrinterService(pSession);
	}
}

void GPrinterService::DestroyInstance()
{
	GMutexLock oLock(mst_InstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

void GPrinterService::AttachManager(GPrinterManager* pMgr)
{
	if( !pMgr )
	{
		return;
	}

	GMutexLock oLock(mst_InstanceMutex);
	m_pManager = pMgr;
}

void GPrinterService::DetachManager()
{
	GMutexLock oLock(mst_InstanceMutex);
	m_pManager = NULL;
}

void GPrinterService::NotifyPrinterStatus(const char* pcStatus)
{
	if( pcStatus && m_pManager )
		m_pManager->NotifyPrinterStatus(pcStatus);
}

void GPrinterService::ClearMsgBuffer()
{
	memset( mst_aryu8MsgBuffer, 0, sizeof(mst_aryu8MsgBuffer) );
}

bool GPrinterService::isPrinterReply(GSPOTMessage* pMsg, bool blOnlySubCmd /* = false */)
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

		if( pMsg->GetCommandCode() != eSPOT_OPT_PRINTER )
		{
			return false; // unexpected command category
		}
	}

	int iSubCmd = pMsg->GetSubCommandCode();
	if( iSubCmd >= eSPOT_OPT_PRN_TEXTDATA  &&  iSubCmd <= eSPOT_OPT_PRN_JOBCANCEL )
	{
		return true;
	}
	else
	{
		return false; // unexpected message type
	}
}

bool GPrinterService::ProcessOPTSystemStatusNotification(UINT8 u8Status,
                                                         UINT8 u8StatusEx, UINT8 u8Error)
{
	const char* pcMethod = "GPrinterService::ProcessOPTSystemStatusNotification() - ";
	if(u8Status == eSPOT_PR_ERROR_STATE || u8Error > 0)
	{
		LOG(ePrinter, eError, "%sstatus = %u, ERROR = %u .\n", pcMethod, u8Status, u8Error);
		GMutexLock oLock(mst_InstanceMutex);
		m_blPrinterError = true;
		m_blPrinterReady = false;
		StatusTransitionRequest(ePrinterErrorEvent);
		return false;
	}
	  // not ready but transitory situation
	else if(u8Status == eSPOT_PR_BUSY_STATE || u8Status == eSPOT_PR_STARTUP_STATE)
	{
		const char* pcState = "busy";
		if(u8Status == eSPOT_PR_STARTUP_STATE)
			pcState = "starting up";

		LOG(ePrinter, eNotice, "%sPrinter %s, extended status %u .\n", pcMethod, pcState, u8StatusEx);
		m_blPrinterReady = false;
		m_blPrinterError = false;
		return true;
	}
	else if(u8Status == eSPOT_PR_READY_STATE )
	{
		bool blWakeUp = false;
		m_blPrinterReady = true;
		m_blPrinterError = false;

		if( m_eStatus == ePrinterReady )
			return true; // nothing to do

		if( m_eStatus == eExecutingJob )
		{
			blWakeUp = true; // remember to wake up after the status transition.
		}

		LOG(ePrinter, eDebug, "%sPrinter is back to READY state now.\n", pcMethod);
		GMutexLock oLock(mst_InstanceMutex);
		StatusTransitionRequest(eReadyNowEvent);
		if( blWakeUp )
			m_Condition.Signal(); // wake up the print text thread.
	}

	return true;
}

bool GPrinterService::ProcessPrinterReply(GSPOTMessage* pMsg)
{
	bool blret = false;
	int iSubCmd = pMsg->GetSubCommandCode();
	const char* pcMethod = "GPrinterService::ProcessPrinterReply() - ";

	if(iSubCmd >= eSPOT_OPT_PRN_TEXTDATA && iSubCmd <= eSPOT_OPT_PRN_JOBCANCEL && iSubCmd != eSPOT_OPT_PRN_JOBSTATUS)
	{
		UINT16 u16MsgLen = pMsg->GetDataLen();
		if(u16MsgLen < 5)
		{
			LOG(ePrinter, eError, "Message length too short !!!!!\n");
			return false;
		}

		UINT8* pu8Data = pMsg->GetData();
		if(pu8Data == NULL)
		{
			LOG(ePrinter, eError, "Corrupt message with no data !!!!!\n");
			return false;
		}

		UINT8 u8Buffer[40];
		char  acBuffer[128];
		UINT  ulen = pMsg->GetTransmitBuffer(u8Buffer);
		GConversion::BytesToASCIIHexString(acBuffer, u8Buffer, ulen);
		LOG(ePrinter, eDebug, "%s Message Received:\n%s\n", pcMethod, acBuffer);

		switch( iSubCmd )
		{
			case eSPOT_OPT_PRN_TEXTDATA:
			case eSPOT_OPT_PRN_TEMPLATEDATA:
				blret = ProcessSendBlockAnswer(u16MsgLen, pu8Data);
				break;

			case eSPOT_OPT_PRN_JOBCREATE:
				blret = ProcessOpenJobAnswer(u16MsgLen, pu8Data);
				break;

			case eSPOT_OPT_PRN_JOBPRINT:
				blret = ProcessExecuteJobAnswer(u16MsgLen, pu8Data);
				break;

			case eSPOT_OPT_PRN_JOBCANCEL:
				blret = ProcessCancelJobAnswer(u16MsgLen, pu8Data);
				break;
		}
	}
	else
	{
		LOG(ePrinter, eTrace,
		    "Printer Reply Subcommand %d ,"
		    " message processing not implemented yet.\n", iSubCmd);
	}

	return blret;
}

bool GPrinterService::ProcessOpenJobAnswer(UINT16 u16Length, UINT8* pu8Data)
{
	bool blret = false;
	int iJobID = -1;

	if(pu8Data[0] == 0)
	{
		iJobID = 16777216 * pu8Data[1] + 65536 * pu8Data[2] + 256 * pu8Data[3] + pu8Data[4];
		LOG(ePrinter, eDebug, "Job# %d created.\n", iJobID);
		blret = true;
	}
	else
	{
		LOG(ePrinter, eError, "Job not created, error: %02X\n", unsigned(pu8Data[0]));
	}

	mst_InstanceMutex.Lock();
	m_iCurrentJobID = iJobID;
	StatusTransitionRequest(blret ? eOpenJobEvent : eJobErrorEvent);
	m_Condition.Signal(); // wake up the print text thread.
	mst_InstanceMutex.Unlock();
	return blret;
}

bool GPrinterService::ProcessSendBlockAnswer(UINT16 u16Length, UINT8* pu8Data)
{
	bool blret = false;
	int iJobID = -1, iSeqNo = -1;

	if(u16Length >= 5)
	{
		iJobID = 16777216 * pu8Data[1] + 65536 * pu8Data[2] + 256 * pu8Data[3] + pu8Data[4];
	}

	if(u16Length >= 7)
	{
		iSeqNo = 256 * pu8Data[5] + pu8Data[6];
	}

	if(pu8Data[0] == 0)
	{
		LOG(ePrinter, eDebug, "Block# %u ack OK in job# %u .\n",
		    UINT(iSeqNo), UINT(iJobID));
		blret = true;
	}
	else
	{
		LOG(ePrinter, eError, "Block ack error: %02X in sequence# %u, job# %u .\n",
		    UINT(pu8Data[0]), UINT(iSeqNo), UINT(iJobID));
	}

	GMutexLock oLock(mst_InstanceMutex);

	if(blret) // good ack
		m_iCurrentSeqNo = 1 + iSeqNo; // sequence number for the next iteration
	else
		StatusTransitionRequest(eJobErrorEvent);

	m_Condition.Signal(); // wake up the print text thread.
	return blret;
}

bool GPrinterService::ProcessExecuteJobAnswer(UINT16 u16Length, UINT8* pu8Data)
{
	bool blret = false;
	int iJobID = -1;

	if(u16Length >= 5)
	{
		iJobID = 16777216 * pu8Data[1] + 65536 * pu8Data[2] + 256 * pu8Data[3] + pu8Data[4];
	}

	if(pu8Data[0] == 0)
	{
		LOG(ePrinter, eDebug, "Job# %u started OK.\n",
		    UINT(iJobID));
		blret = true;
	}
	else
	{
		LOG(ePrinter, eError, "ACK Error %02X executing job# %u .\n",
		    UINT(pu8Data[0]), UINT(iJobID));
	}

	GMutexLock oLock(mst_InstanceMutex);
	StatusTransitionRequest(blret ? eExecuteJobEvent : eJobErrorEvent);
	m_Condition.Signal(); // wake up the print text thread.
	return blret;
}

bool GPrinterService::ProcessCancelJobAnswer(UINT16 u16Length, UINT8* pu8Data)
{
	bool blret = false;
	int iJobID = -1;

	if(u16Length >= 5)
	{
		iJobID = 16777216 * pu8Data[1] + 65536 * pu8Data[2] + 256 * pu8Data[3] + pu8Data[4];
	}

	if(m_iCurrentJobID == iJobID)
		m_iCurrentJobID = -1; // mark the job canceled

	if(pu8Data[0] == 0)
	{
		blret = true;
	}
	else
	{
		eLogLevel el = eDebug;
		if(m_eStatus == eJobError || m_eStatus == ePrinterError)
			el = eError;

		LOG(ePrinter, el, "ACK Error %02X canceling job# %u .\n",
		    UINT(pu8Data[0]), UINT(iJobID));
	}

	if(m_eStatus != eJobError)
		return blret;

	GMutexLock oLock(mst_InstanceMutex);
	StatusTransitionRequest(blret ? eReadyNowEvent : ePrinterErrorEvent);
	return blret;
}

void GPrinterService::ProcessTimeoutEvent()
{
	GMutexLock oLock(mst_InstanceMutex);

	switch( m_eStatus )
	{
		case ePrinterReady:
			LOG(ePrinter, eNotice, "Timeout in ready state. Ignored.\n");
			m_pTimer->Stop();
			m_pTimer->SetInterval(PR_TOUT_CANCELJOB);
			break;

		case eJobOpen:
		case eDataQueued:
		case eExecutingJob:
			LOG(ePrinter, eWarning,
			    "Timer triggered in status = %s . WEIRD.\n",
			    mst_arypcStatusLabels[m_eStatus]);
			m_pTimer->Stop();
			m_pTimer->SetInterval(PR_TOUT_CANCELJOB);
			break;

		case eJobError:
			LOG(ePrinter, eNotice, "Timeout for cancel job command.\n");
			m_pTimer->SetInterval(PR_TOUT_CHECKRDY);
			StatusTransitionRequest(ePrinterErrorEvent);
			break;

		case ePrinterError:
			if(m_blPrinterError) // still in error, must wait more time
			{
				LOG(ePrinter, eNotice,
				    "Timeout while printer is still in error. Waiting ...\n");
				QueryPrinterStatus(); // ask fore one more OPT sys status (printer)
			}
			else if(m_blPrinterReady)// recovered from error situation
			{
				LOG(ePrinter, eNotice,
				    "Timeout while printer is recovered from error situation. Ready now.\n");
				m_pTimer->Stop();
				m_pTimer->SetInterval(PR_TOUT_CANCELJOB);
				StatusTransitionRequest(eReadyNowEvent);
			}
			break;

		default:
			LOG(ePrinter, eWarning,
			    "Timeout received while UNKNOWN status = %d .\n", m_eStatus);
			break;
	}
}

/**
 * This method implements the state change and log the transition.
 * @param ev is the event code that initiates a state transition.
 * @param es is the future state machine, reached after the transition.
 * @return void
 *
 */
void GPrinterService::NextState(eEvents ev, eStatus es)
{
	LOG( ePrinter, eDebug, "[ curr_status: %s ==>> event: %s ==>> next_status: %s ]\n",
	     mst_arypcStatusLabels[m_eStatus], mst_arypcEventLabels[ev], mst_arypcStatusLabels[es] );
	m_eStatus = es;
}

/**
 * This method implements the state machine.
 * @param e is the event code that initiates a state transition.
 * @param pEventData is the optional pointer to data needed for this state machine.
 * @return void
 *
 */
void GPrinterService::StatusTransitionRequest(eEvents e, DataEvent* pEventData /* = NULL */)
{
	switch(m_eStatus)
	{
		//======================= current STATUS : PrinterReady =========================
		case ePrinterReady:
			switch(e)
			{
				case eOpenJobEvent:
					NextState(eOpenJobEvent, eJobOpen);
					break;

				case eJobErrorEvent:
					LOG(ePrinter, eError, "Error in job creation. Finished.\n");
					break;

				case ePrinterErrorEvent:
					CancelCurrentJob();
					NotifyPrinterStatus("PRINTER ERROR");
					m_pTimer->SetInterval(PR_TOUT_CHECKRDY);
					m_pTimer->Start();
					NextState(ePrinterErrorEvent, ePrinterError);
					break;

				case eTimeoutEvent:
					LOG(ePrinter, eError, "Timeout in job creation. Finished.\n");
					break;

				default:
					LogUnexpectedEvent(e, eWarning);
					break;
			}
			break;

		//======================= current STATUS : JobOpen ==============================
		case eJobOpen:
			switch(e)
			{
				case eQueueDataEvent:
					NextState(eQueueDataEvent, eDataQueued);
					break;

				case eJobErrorEvent:
					CancelCurrentJob();
					m_pTimer->Start();
					NextState(eJobErrorEvent, eJobError);
					break;

				case ePrinterErrorEvent:
					CancelCurrentJob();
					NotifyPrinterStatus("PRINTER ERROR");
					m_pTimer->SetInterval(PR_TOUT_CHECKRDY);
					m_pTimer->Start();
					NextState(ePrinterErrorEvent, ePrinterError);
					break;

				case eTimeoutEvent:
					LOG(ePrinter, eError, "Timeout queuing data. Canceling Job.\n");
					CancelCurrentJob();
					m_pTimer->Start();
					NextState(eTimeoutEvent, eJobError);
					break;

				default:
					LogUnexpectedEvent(e, eWarning);
					break;
			}
			break;

		//======================= current STATUS : DataQueued ===========================
		case eDataQueued:
			switch(e)
			{
				case eExecuteJobEvent:
					NextState(eExecuteJobEvent, eExecutingJob);
					break;

				case eJobErrorEvent:
					LOG(ePrinter, eError, "Error in job execution command reply. Canceling Job.\n");
					CancelCurrentJob();
					NextState(eJobErrorEvent, eJobError);
					break;

				case ePrinterErrorEvent:
					CancelCurrentJob();
					m_pTimer->SetInterval(PR_TOUT_CHECKRDY);
					m_pTimer->Start();
					NextState(ePrinterErrorEvent, ePrinterError);
					break;

				case eTimeoutEvent:
					LOG(ePrinter, eError, "Timeout in job execution command reply. Canceling Job.\n");
					CancelCurrentJob();
					NextState(eTimeoutEvent, eJobError);
					break;

				default:
					LogUnexpectedEvent(e, eWarning);
					break;
			}
			break;

		//======================= current STATUS : ExecutingJob =========================
		case eExecutingJob:
			switch(e)
			{
				case eReadyNowEvent:
					NextState(eReadyNowEvent, ePrinterReady);
					break;

				case eJobErrorEvent:
					LOG(ePrinter, eError, "Error executing job. Canceling Job.\n");
					CancelCurrentJob();
					m_pTimer->Start();
					NextState(eJobErrorEvent, eJobError);
					break;

				case ePrinterErrorEvent:
					CancelCurrentJob();
					NotifyPrinterStatus("PRINTER ERROR");
					m_pTimer->SetInterval(PR_TOUT_CHECKRDY);
					m_pTimer->Start();
					NextState(ePrinterErrorEvent, ePrinterError);
					break;

				case eTimeoutEvent:
					LOG(ePrinter, eError, "Timeout executing job. Canceling Job.\n");
					CancelCurrentJob();
					m_pTimer->Start();
					NextState(eTimeoutEvent, eJobError);
					break;

				default:
					LogUnexpectedEvent(e, eWarning);
					break;
			}
			break;

		//======================= current STATUS : JobError ===========================
		case eJobError:
			switch(e)
			{
				case eReadyNowEvent:
					m_pTimer->Stop();
					NextState(eReadyNowEvent, ePrinterReady);
					break;

				case eJobErrorEvent:
					LOG(ePrinter, eError,
					    "Job error event received in state JobErrorEvent. "
					    "Remaining in the same state.\n");
					break;

				case ePrinterErrorEvent:
					NotifyPrinterStatus("PRINTER ERROR");
					m_pTimer->SetInterval(PR_TOUT_CHECKRDY);
					NextState(ePrinterErrorEvent, ePrinterError);
					break;

				case eTimeoutEvent:
					LOG(ePrinter, eError, "Timeout waiting job cancellation. Going to printer error.\n");
					NotifyPrinterStatus("PRINTER ERROR");
					m_pTimer->SetInterval(PR_TOUT_CHECKRDY);
					NextState(eTimeoutEvent, ePrinterError);
					break;

				default:
					LogUnexpectedEvent(e, eWarning);
					break;
			}
			break;

		//======================= current STATUS : PrinterError ===========================
		case ePrinterError:
			switch(e)
			{
				case eReadyNowEvent:
					LOG(ePrinter, eNotice, "Printer is ready now!\n")
					m_pTimer->Stop();
					m_pTimer->SetInterval(PR_TOUT_CANCELJOB);
					NotifyPrinterStatus(" ");
					NextState(eReadyNowEvent, ePrinterReady);
					break;

				case eJobErrorEvent:
					LOG(ePrinter, eTrace,
						"Job error event received in state PrinterErrorEvent. "
						"Remaining in the same state.\n");
					break;

				case ePrinterErrorEvent:
					LOG(ePrinter, eTrace,
						"Printer error event received in state PrinterErrorEvent. "
						"Remaining in the same state.\n");
					break;

				case eTimeoutEvent:
					LOG(ePrinter, eError,
					    "Timeout event shouldn't be routed in "
					    "state PrinterErrorEvent.\n");
					break;

				default:
					LogUnexpectedEvent(e, eWarning);
					break;
			}
			break;

		default:
			LOG(ePrinter, eError,
			    "Event# %d received in UNKNOWN printer status %d !!!!!!\n", e, m_eStatus);
			break;
	}
}


/**
 * @brief This method prints (not immediately) the given text buffer.
 * The function returns once all the data is queued in the printer spooler
 * and the job execution has started, or when the first error is generated.
 * @param u32len is the text length.
 * @param pText is the pointer to text characters.
 *
 * @return bool  true on success, fail on any error in the printing sequence.
 *
 * @note This method MUST NOT BE CALLED from a SPOT ANSWER queue thread, like
 * soft-key notification routine in menus, due this itself waits several times for
 * SPOT answers (like status changes in the print job or the printer device).
 */
bool GPrinterService::PrintText(UINT32 u32len, const char* pText)
{
	if(u32len == 0 || pText == NULL)
		return false; // bad parameters

	if(m_eStatus != ePrinterReady)
		return false; // printer not ready to accept new job

	int  iJobID = 0;
	const char* pcMethod = "GPrinterService::PrintText() - ";

	LOG(ePrinter, eDebug, "%sPhase ONE.\n", pcMethod);
	if( !OpenJob(iJobID) || iJobID == -1 )
	{
		return false; // could not open a valid printer job
	}

	LOG(ePrinter, eDebug, "%sPhase TWO.\n", pcMethod);
	if( !SendBlocks(iJobID, u32len, pText) )
	{
		return false; // error queuing data into printer job
	}

	LOG(ePrinter, eDebug, "%sPhase THREE.\n", pcMethod);
	if( !ExecuteJob(iJobID) )
	{
		return false; // could not start executing the printer job
	}

	LOG(ePrinter, eDebug, "%sPhase FOUR.\n", pcMethod);
	return WaitPrinterReady();
}

bool GPrinterService::OpenJob(int& iJobID)
{
	UINT8* pu8CreateJobPRCmd = const_cast<UINT8*>(mst_aryu8CreateJobCmd);
	GSPOTMessage oMsg(pu8CreateJobPRCmd);
	int iret = m_pSession->SendSPOTCommand( &oMsg );
	if(!iret)
		return false; // couldn't send message to SPOT

	mst_InstanceMutex.Lock();

	iret = 1;
	while( iret > 0 && m_eStatus == ePrinterReady )
		iret = m_Condition.Wait(PR_STEP_WAIT); // wait SPOT notification

	iJobID = m_iCurrentJobID;

	if( m_eStatus == eJobOpen )
	{
		iret = 1; // Job successfully created.
	}
	else
	{
		LOG(ePrinter, eDebug,
		    "GPrinterService::OpenJob() - Status: %s\n", mst_arypcStatusLabels[m_eStatus]);
		iret = 0; // Couldn't create job
	}

	mst_InstanceMutex.Unlock();

	return (iret ? true : false);
}

bool GPrinterService::SendBlocks(int iJobID, UINT32 u32len, const char* pText)
{
	const char* pcMethod = "GPrinterService::SendBlocks() - ";
	UINT32 u32BlockLength = PR_MAX_BLOCK_LEN;
	UINT32 u32LastBlklen  = ( u32len % u32BlockLength );
	int    iBlockQty      = ( u32len / u32BlockLength );

	if( u32LastBlklen != 0)
		iBlockQty++; // last smallest block accounted

	if( iBlockQty > PR_MAX_BLOCK_QTY)
	{
		LOG(ePrinter, eError, "%s%s%d chars.\n", pcMethod, "Text too big: ", u32len);
		return false; // text too big !!
	}

	LOG(ePrinter, eDebug, "%sTotal Blocks: %d .\n", pcMethod, iBlockQty);
	mst_InstanceMutex.Lock();
	m_iCurrentBlockQty = iBlockQty;
	m_iCurrentSeqNo = 0;
	mst_InstanceMutex.Unlock();

	int iBlockNo = 0;
	while( iBlockNo < iBlockQty )
	{
		if( iBlockNo == (iBlockQty - 1) && u32LastBlklen != 0 ) // last smallest block ?
			u32BlockLength = u32LastBlklen;

		const char* pcBlock = pText + iBlockNo * u32BlockLength;
		if( ! SendBlock(iJobID, iBlockNo, u32BlockLength, pcBlock) )
			return false; // block handshake failure

		iBlockNo = m_iCurrentSeqNo; // incremented by notification thread
	}

	mst_InstanceMutex.Lock();
	m_iCurrentSeqNo = 0;
	StatusTransitionRequest(eQueueDataEvent); // data successfully queued.
	mst_InstanceMutex.Unlock();
	return true;
}

bool GPrinterService::SendBlock(int iJobID, int iSeqNo, UINT32 u32len, const char* pText)
{
	UINT32 u32MsgLen = u32len + PR_QUEDAT_CMDLEN;

	memcpy( mst_aryu8MsgBuffer, mst_aryu8EnqueueDataCmd, PR_QUEDAT_CMDLEN);
	mst_aryu8MsgBuffer[0] = UINT8(u32MsgLen / 256); // high byte
	mst_aryu8MsgBuffer[1] = UINT8(u32MsgLen % 256); // low  byte

	mst_aryu8MsgBuffer[6] = UINT8( iJobID / 16777216 );
	mst_aryu8MsgBuffer[7] = UINT8( (iJobID / 65536) & 0x000000FF);
	mst_aryu8MsgBuffer[8] = UINT8( (iJobID % 65536) / 256);
	mst_aryu8MsgBuffer[9] = UINT8( iJobID % 256 );

	mst_aryu8MsgBuffer[10] = UINT8(iSeqNo / 256);
	mst_aryu8MsgBuffer[11] = UINT8(iSeqNo % 256);

	mst_aryu8MsgBuffer[12] = UINT8(u32len / 256);
	mst_aryu8MsgBuffer[13] = UINT8(u32len % 256);
	memcpy( mst_aryu8MsgBuffer + PR_QUEDAT_CMDLEN, pText, u32len);


	GSPOTMessage oMsg(mst_aryu8MsgBuffer);
	int iret = m_pSession->SendSPOTCommand( &oMsg );
	if(!iret)
		return false; // couldn't send message to SPOT

	mst_InstanceMutex.Lock();

	while( iret > 0 && m_eStatus == eJobOpen && iSeqNo == m_iCurrentSeqNo )
		iret = m_Condition.Wait(PR_STEP_WAIT); // wait SPOT for block acknowledge

	if( m_eStatus != eJobOpen || iret == 0 || iSeqNo == m_iCurrentSeqNo )
	{
		// if entered here then there is an Error or Timeout problem

		if( m_eStatus == eJobOpen ) // timeout situation
			StatusTransitionRequest(eTimeoutEvent);

		iret = 0; // return error
	}
	else
		iret = 1;

	mst_InstanceMutex.Unlock();

	return (iret == 1 ? true : false);
}

bool GPrinterService::ExecuteJob(int iJobID)
{
	memcpy( mst_aryu8MsgBuffer, mst_aryu8ExecuteJobCmd, PR_EXEJOB_CMDLEN);

	mst_aryu8MsgBuffer[6] = UINT8( iJobID / 16777216 );
	mst_aryu8MsgBuffer[7] = UINT8( (iJobID / 65536) & 0x000000FF);
	mst_aryu8MsgBuffer[8] = UINT8( (iJobID % 65536) / 256);
	mst_aryu8MsgBuffer[9] = UINT8( iJobID % 256 );

	GSPOTMessage oMsg(mst_aryu8MsgBuffer);
	int iret = m_pSession->SendSPOTCommand( &oMsg );
	if(!iret)
		return false; // couldn't send message to SPOT

	mst_InstanceMutex.Lock();

	while( iret > 0 && m_eStatus == eDataQueued )
		iret = m_Condition.Wait(PR_STEP_WAIT); // wait SPOT notification

	iret = 0;
	if( m_eStatus == eDataQueued )
		StatusTransitionRequest(eTimeoutEvent); // timeout situation

	else if( m_eStatus == eExecutingJob ) // job started, detected by notification th
		iret = 1; // Successful return

	mst_InstanceMutex.Unlock();

	return (iret == 1 ? true : false);
}

bool GPrinterService::CancelCurrentJob()
{
	if( m_iCurrentJobID == -1)
		return false; // job already finished/canceled

	memcpy( mst_aryu8MsgBuffer, mst_aryu8CancelJobCmd, PR_CANJOB_CMDLEN);

	mst_aryu8MsgBuffer[6] = UINT8( m_iCurrentJobID / 16777216 );
	mst_aryu8MsgBuffer[7] = UINT8( (m_iCurrentJobID / 65536) & 0x000000FF);
	mst_aryu8MsgBuffer[8] = UINT8( (m_iCurrentJobID % 65536) / 256);
	mst_aryu8MsgBuffer[9] = UINT8( m_iCurrentJobID % 256 );

	GSPOTMessage oMsg(mst_aryu8MsgBuffer);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GPrinterService::QueryPrinterStatus()
{
	UINT8* pu8PRTStatusCmd = const_cast<UINT8*>(mst_aryu8PRTStatusCmd);

	GSPOTMessage oMsg(pu8PRTStatusCmd);
	return m_pSession->SendSPOTCommand( &oMsg );
}

bool GPrinterService::WaitPrinterReady()
{
	int iret = 0;
	const char* pcMethod = "GPrinterService::WaitPrinterReady() :";
	mst_InstanceMutex.Lock();

	int iretries = 3;
	while( iretries > 0 && m_eStatus == eExecutingJob )
	{
		iret = m_Condition.Wait(PR_READY_WAIT); // wait SPOT notification: printer ready
		if(iret == 1)
			break;
		else
			--iretries;
	}

	if( m_eStatus != ePrinterReady )
	{
		LOG(ePrinter, eDebug, "%s finished in wrong status [%s].\n",
		    pcMethod, mst_arypcStatusLabels[m_eStatus]);

		if(m_eStatus == eExecutingJob)
		{
			LOG(ePrinter, eError, "%s TIMEOUT waiting for printer ready.\n", pcMethod);
		}

		iret = 0; // error condition
	}

	m_iCurrentJobID = -1; // mark the job finished

	mst_InstanceMutex.Unlock();

	return (iret == 1 ? true : false);
}

void GPrinterService::LogUnexpectedEvent(eEvents ev, eLogLevel el)
{
	LOG(ePrinter, el, "UNEXPECTED EVENT %s[%d] while status is %s[%d] .\n",
	    mst_arypcEventLabels[ev], ev, mst_arypcStatusLabels[m_eStatus], m_eStatus);
}


