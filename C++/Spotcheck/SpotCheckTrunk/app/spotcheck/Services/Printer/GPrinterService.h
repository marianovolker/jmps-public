/**
 * GPrinterService.h
 *
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GPrinterService.h
 * @author Guillermo Paris
 * @date Apr 19 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GPRINTERSERVICE_H_
#define GPRINTERSERVICE_H_

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GSimpleCommand.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Menus/GMenuManager.h"

const int PR_QUEDAT_CMDLEN  =    14;
const int PR_STAJOB_CMDLEN  =    10;
const int PR_CRTJOB_CMDLEN  =     6;
const int PR_EXEJOB_CMDLEN  =    10;
const int PR_CANJOB_CMDLEN  =    10;
const int PR_PRTSTA_CMDLEN  =     7;
const int PR_MAX_BLOCK_LEN  =  2048;
const int PR_MAX_BLOCK_QTY  =    32;
const int PR_STEP_WAIT      =  2000; //  2 sec
const int PR_READY_WAIT     =  7000; //  7 sec
const int PR_TOUT_CANCELJOB =  7000; //  7 sec
const int PR_TOUT_CHECKRDY  = 20000; // 20 sec

const int DATA_MSG_MAX_LEN  =  PR_MAX_BLOCK_LEN + PR_QUEDAT_CMDLEN;

class GPrinterManager;

class GPrinterService
{
public:
	enum eStatus
	{
		ePrinterReady,  // Printer device ready in IDLE state.
		eJobOpen,       // Job has been opened.
		eDataQueued,    // Data has been queued to printer driver.
		eExecutingJob,  // Job execution has been started.
		eJobError,      // Printer Service has got some kind of error during Job life.
		ePrinterError,  // Printer Device has got some kind of error.
		eStatusQty
	};

	enum eEvents
	{
		eOpenJobEvent,      // Event signaling a new job is open.
		eQueueDataEvent,    // Event signaling all print data is queued.
		eExecuteJobEvent,   // Event signaling the job is being executed.
		eReadyNowEvent,     // Event signaling the last printer ready notification.
		eJobErrorEvent,     // Event signaling a problem with the current job.
		ePrinterErrorEvent, // Event signaling a problem with the printer device.
		eTimeoutEvent,      // Event signaling a synchronizing timeout in the GCondition.
		eEventsQty
	};

	struct DataEvent
	{
		void*  pData;
		UINT   uSize;
	};


public:
	static GPrinterService* GetService();
	static void             CreateInstance(GSPOTSession* pSession);
	static void             DestroyInstance();
	static bool             isPrinterReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);
	void                    AttachManager(GPrinterManager* pMgr);
	void                    DetachManager();
	void                    NotifyPrinterStatus(const char* pcStatus);
	bool                    ProcessPrinterReply(GSPOTMessage* pMsg);
	bool                    ProcessOPTSystemStatusNotification(UINT8 u8Status,
	                                                           UINT8 u8StatusEx,
	                                                           UINT8 u8Error);
/**
 * This method prints (not immediately) the given text buffer.
 * The function returns once all the data is queued in the printer spooler
 * and the job execution has started.
 * @param u32len is the text length.
 * @param pText is the pointer to text characters.
 *
 * @return bool  true on success, fail on any error in the printing sequence.
 *
 */
	bool                    PrintText(UINT32 u32len, const char* pText);


private:
	GPrinterService(GSPOTSession* pSession);
	~GPrinterService();

	void                    ClearMsgBuffer();
	bool                    OpenJob(int& iJobID);
	bool                    SendBlocks(int iJobID, UINT32 u32len, const char* pText);
	bool                    SendBlock(int iJobID, int iSeqNo, UINT32 u32len, const char* pText);
	bool                    ExecuteJob(int iJobID);
	bool                    CancelCurrentJob();
	bool                    QueryPrinterStatus();
	bool                    WaitPrinterReady();
	bool                    ProcessOpenJobAnswer(UINT16 u16len, UINT8* pu8Data);
	bool                    ProcessSendBlockAnswer(UINT16 u16len, UINT8* pu8Data);
	bool                    ProcessExecuteJobAnswer(UINT16 u16len, UINT8* pu8Data);
	bool                    ProcessCancelJobAnswer(UINT16 u16len, UINT8* pu8Data);
	void                    LogUnexpectedEvent(eEvents ev, eLogLevel el);

/**
 * This method implements the state change and log the transition.
 * @param ev is the event code that initiates a state transition.
 * @param es is the future state machine, reached after the transition.
 * @return void
 *
 */
	void                    NextState(eEvents ev, eStatus es);

/**
 * This method implements the state machine.
 * @param e is the event code that initiates a state transition.
 * @param pEventData is the optional pointer to data needed for this state machine.
 * @return void
 *
 */
	void                    StatusTransitionRequest(eEvents e, DataEvent* data = NULL);

	void                    ProcessTimeoutEvent();

	static const UINT8      mst_aryu8EnqueueDataCmd[PR_QUEDAT_CMDLEN];
	static const UINT8      mst_aryu8EnqueueMetaDataCmd[PR_QUEDAT_CMDLEN];
	static const UINT8      mst_aryu8JobStatusCmd[PR_STAJOB_CMDLEN];
	static const UINT8      mst_aryu8CreateJobCmd[PR_CRTJOB_CMDLEN];
	static const UINT8      mst_aryu8ExecuteJobCmd[PR_EXEJOB_CMDLEN];
	static const UINT8      mst_aryu8CancelJobCmd[PR_CANJOB_CMDLEN];
	static const UINT8      mst_aryu8PRTStatusCmd[PR_PRTSTA_CMDLEN];
	static const char*      mst_arypcStatusLabels[eStatusQty];
	static const char*      mst_arypcEventLabels[eEventsQty];
	static GPrinterService* mst_pSoleInstance;
	static GMutex           mst_InstanceMutex;
	static UINT8            mst_aryu8MsgBuffer[DATA_MSG_MAX_LEN];

	volatile bool           m_blPrinterReady;
	volatile bool           m_blPrinterError;
	volatile int            m_iCurrentJobID;
	volatile int            m_iCurrentSeqNo;
	volatile int            m_iCurrentBlockQty;
	volatile eStatus        m_eStatus;
	GSPOTSession*           m_pSession;
	GPrinterManager*        m_pManager;
	GTimer*                 m_pTimer;
	GCondition              m_Condition;
	GSimpleCommand<GPrinterService> m_TOutCommand; // command to process timeout event
};


#endif /* GPRINTERSERVICE_H_ */
