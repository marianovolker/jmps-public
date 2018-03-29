/**
 * GTrindService.h
 *
 * © 2015 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GTrindService.h
 * @author Sayyed Mohammad
 * @date Feb 10 2015
 * @copyright © 2015 Gilbarco Inc. Confidential and Proprietary
 */

#ifndef GTRINDSERVICE_H_
#define GTRINDSERVICE_H_

#include "Common/Util/DEBUGOUT.h"
#include "Common/Util/GSimpleCommand.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Menus/GMenuManager.h"
#include "GTrindServiceStatistics.h"

const int TR_ENABLE_CMDLEN        = 6;
const int TR_DISABLE_CMDLEN       = 6;
const int TR_GETSTATUS_CMDLEN     = 6;
const int TR_SETINDICATOR_CMDLEN  = 7;

const int TR_TOUT_TRIND = 5000;

class GTrindManager;

class GTrindService
{
public:

	enum eStatus
	{
		eStatusInitial,
		eStatusWaitTag,
		eStatusTagDataArrived,
		eStatusTempError,
		eStatusError
	};


	enum eEvents
	{
		eEventEnableService,
		eEventDisableService,
		eEventTrindIsEnabled,
		eEventTrindIsDisabled,
		eEventTagDataArrived,
		eEventTimeout,
		eEventError,
	};

	struct DataEvent
	{
		void*  pData;
		UINT   uSize;
	};


public:
	static GTrindService*   GetService();
	static void             CreateInstance(GSPOTSession* pSession);
	static void             DestroyInstance();
	static bool             isTrindReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);
	void                    AttachManager(GTrindManager* pMgr);
	void                    DetachManager();
	bool                    ProcessTrindReply(GSPOTMessage* pMsg);
	bool                    ProcessOPTSystemStatusNotification(UINT8 u8Status,
	                                                           UINT8 u8StatusEx,
	                                                           UINT8 u8Error);

	void Enable();
	void Disable();
	void ShowTagRead();
	void ShowStatsScreen();

	bool EnableRequest(bool blCheckStatus = false);
	bool DisableRequest(bool blCheckStatus = false);
	bool LEDRequestOn();
	bool LEDRequestOff();

	void UpdateTagStatistics(GTrindServiceStatistics::eTagResult t);
	void UpdateLEDStatistics(GTrindServiceStatistics::eTRLEDResult t);
	inline GTrindServiceStatistics & GetSessionStatistics()  {return m_oTRSessionStatistics;};


private:
	GTrindService(GSPOTSession* pSession);
	~GTrindService();

	bool SendEnableCmdToSPOT();
	bool SendDisableCmdToSPOT();
	bool SendLEDONCmdToSPOT();
	bool SendLEDOFFCmdToSPOT();



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
	void                    IncrementTagCount(int);

	void                    ProcessTimeoutEvent();

	static const UINT8      mst_aryu8EnableCmd[TR_ENABLE_CMDLEN];
	static const UINT8      mst_aryu8DisableCmd[TR_DISABLE_CMDLEN];
	static const UINT8      mst_aryu8GetStatusCmd[TR_GETSTATUS_CMDLEN];
	static const UINT8      mst_aryu8SetIndicatorONCmd[TR_SETINDICATOR_CMDLEN];
	static const UINT8      mst_aryu8SetIndicatorOFFCmd[TR_SETINDICATOR_CMDLEN];

	static const char*      mst_arypcStatusLabels[eStatusError+1];
	static const char*      mst_arypcEventLabels[eEventError+1];
	static GTrindService*   mst_pSoleInstance;
	static GMutex           mst_InstanceMutex;
	GMutex                  m_TrindStateMutex;


	volatile eStatus        m_eStatus;
	GSPOTSession*           m_pSession;
	GTrindManager*          m_pManager;
	GTimer*                 m_pTimer;
	GCondition              m_Condition;

	bool                    m_blTREnabled;
	GTrindServiceStatistics m_oTRSessionStatistics;

	GSimpleCommand<GTrindService> m_TOutCommand; // command to process timeout event
};


#endif /* GTRINDSERVICE_H_ */
