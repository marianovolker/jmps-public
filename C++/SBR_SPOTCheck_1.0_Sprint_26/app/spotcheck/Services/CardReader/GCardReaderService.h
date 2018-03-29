/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GCardReaderService.h
 * @author Guillermo Paris
 * @date Feb 05 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GCardReaderService_h
#define GCardReaderService_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Menus/GMenuManager.h"
#include "GCardReaderServiceStatistics.h"

const UINT8 CR_TRACK_OK 			= 0; // error code for a normal encoded track
const UINT8 CR_ERROR_MAXATTEMPTS 	= 20; // max severe error to count before blocking CR
const UINT32 CR_EVENT_TIMEOUT 		= 10000; // 10 sec
const int CR_WAIT_DIAGNOSTIC_INFO   = 7000; //  7 sec
const UINT CR_TRACK1_MASK 			= 1;
const UINT CR_TRACK2_MASK 			= 2;
const UINT CR_TRACK3_MASK 			= 4;
const UINT CR_TRACK4_MASK 			= 8;
const UINT CR_ATR_MASK 				= 8;
const int CR_ENABLE_CMD_LEN 		= 6;
const int CR_DISABLE_CMD_LEN 		= 6;
const int CR_CHIP_RESET_CMD_LEN		= 6;
const int CR_DIAGNOSTIC_CMD_LEN		= 8;
const int CR_MSG_HDR_LEN 			= 6;
const int CR_TRACK_SIZE 			= 128;

class GCardReaderManager;

enum eTrackNo
{
	eTrack1 = 0,
	eTrack2,
	eTrack3,
	eTrack4,
	eChip=eTrack4,
	eTotalTracks
};

typedef struct _track_info
{
		UINT8 m_u8ErrorCode;
		UINT8 m_u8Present;
		UINT16 m_u16Length;
		UINT8 m_aryu8Data[CR_TRACK_SIZE];

} TRACK_INFO;


class GCardReaderService
{
	public:

		/*!
		  	Card Insert Event:    		eStatusCardOut 				-> eCardInserted
			Card Data Event:      		eStatusCardInserted 		-> eDataArrived
			Card Remove Event:	  		eStatusDataArrived 			-> eCardOut

			Chip Card Insert Event:  	eStatusCardOut 				-> eChipDetected  ,  or the rare case eCardInserted -> eChipDetected
			Card Data Event (ATR):  	eStatusChipDetected			-> eATRReceived ó eATRerror
			Card Remove Event:      	eATRReceived ó eATRerror	-> eCardOut
		 */

		enum eStatus
		{
			eStatusInitial,				// Waiting for enable event.
			eStatusCardOut,				// Waiting card insert (initial state)
			eStatusCardInserted,   		// Waiting data burst
			eStatusChipCardInserted,	// Waiting ATR
			eStatusATRDataArrived,		// Waiting card remove
			eStatusDataArrived,			// Waiting card remove
			eStatusError,				// Card reader device in permanent error state

			eLastStatus = (eStatusError+1)
		};

		enum eEvents
		{
			eEventEnableDevice,
			eEventDisableDevice,
			eEventCardReaderEnable,
			eEventCardReaderDisable,
			eEventCardInsert,
			eEventCardRemove,
			eEventCardData,
			eEventChipCardInsert,
			eEventCardReaderError,
			eEventTimeout,
			eEventError,

			eLastEvent = (eEventError+1)
		};

		struct sDataEvent
		{
			void* data;
			UINT16 size;
		};

		inline static GCardReaderService* GetService();
		static GCardReaderService* CreateInstance(GSPOTSession* pSession);
		static void DestroyInstance();

		static const char* GetErrorDescription(UINT8 u8Error);
		static bool isCardReaderReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);

		void AttachManager(GCardReaderManager* pMgr);
		void DetachManager();
		void EnableDevice();
		void DisableDevice();
		bool RequestStatisticsToSpot();

		bool ProcessCardReaderReply(GSPOTMessage* pMsg);
		int  ParseDiagnosticInfoReply(GSPOTMessage* pMsg);
		bool ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);

		inline eStatus GetStatus() const {return m_eStatus;};
		inline TRACK_INFO GetTrackInfo(eTrackNo e) const {return mst_aryTracks[e];};
		inline const GCardReaderServiceStatistics & GetSPOTStatistics() const {return m_oCRSPOTStatistics;};
		inline const GCardReaderServiceStatistics & GetSessionStatistics() const {return m_oCRSessionStatistics;};


	private:
		GCardReaderService(GSPOTSession* pSession);
		~GCardReaderService();

		inline bool isThereSomeTrackOK();
		void ProcessEventReceived(eEvents e, sDataEvent* data = NULL);
		void MarkCardReaderAsError();
		void UpdateSessionStatistics(TRACK_INFO* pTrack, GCardReaderServiceStatistics::eTrack track);
		void NotifyCardRemoved();
		void NotifyCardInserted();
		void NotifyChipCardInserted();
		bool StopTimer();
		bool StartTimer();
		void ClearTrackInfo();
		void Set3TracksBad();
		UINT16 ClipPartialLength(UINT16 u16Length, int nTrack);
		void ResetEnable(UINT32 u32delay = 0); // uSec to sleep
		bool EnableRequest(bool blCheckStatus = false);
		bool DisableRequest(bool blCheckStatus = false);
		bool ChipReset_ATRRequest();
		bool SendEnableCmdToSPOT();
		bool SendDisableCmdToSPOT();
		bool SendDiagnosticInfoCmdToSPOT();
		void ProcessTimeoutEvent();
		bool ProcessDataEvent(UINT8* pMsgData, UINT16 u16DataSize);
		UINT8 ProcessCardInfo(UINT8* pMsgdata, UINT16 u16DataSize);
		UINT8 ProcessChipCardInfo(UINT8* pMsgData, UINT16 u16DataSize);
		void LogErrorEvent();
		void NextState(eEvents e, eStatus s);

		static GCardReaderService* mst_pSoleInstance;
		static GMutex m_createInstanceMutex;
		static const UINT8 mst_aryu8EnableCRCmd			[CR_ENABLE_CMD_LEN		];
		static const UINT8 mst_aryu8DisableCRCmd		[CR_DISABLE_CMD_LEN		];
		static const UINT8 mst_aryu8ChipResetCRCmd		[CR_CHIP_RESET_CMD_LEN	];
		static const UINT8 mst_aryu8DiagnosticInfoCRCmd	[CR_DIAGNOSTIC_CMD_LEN	];
		static TRACK_INFO mst_aryTracks					[eTotalTracks			];
		static const char * mst_arypcStatusLabel		[eLastStatus			];
		static const char * mst_arypcEventLabel			[eLastEvent				];

		enum eStatus m_eStatus;
		bool m_blCREnabled; // CR enabled/disabled device status flag
		bool m_blCardInserted; // card inside/outside device status flag
		bool m_blM5Unit; // Always true in M5 units, false otherwise.
		bool m_blATRError; // Temporarily true in a card cycle where ATR couldn't be read,
		UINT8 m_u8AttemptsInError; // consecutive severe error count
		GMutex m_CardStateMutex; // mutex to protect status changes
		GMutex m_SincronizeMutex; // mutex to sincronize a response to diagnostic info.
		GCondition m_Condition;
		GTimer* m_pTimer;
		GSPOTSession* m_pSession;
		GCardReaderManager* m_pManager;
		GSimpleCommand<GCardReaderService> m_TOutCommand; // command to process timeout event

		GCardReaderServiceStatistics m_oCRSPOTStatistics;
		GCardReaderServiceStatistics m_oCRSessionStatistics;
};


typedef GCardReaderService::eStatus STATUS;


inline GCardReaderService* GCardReaderService::GetService()
{
	GMutexLock ml(m_createInstanceMutex);

	return mst_pSoleInstance;
}

inline GCardReaderService* GCardReaderService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GCardReaderService(pSession);
	}

	return mst_pSoleInstance;
}

inline void GCardReaderService::DestroyInstance()
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

inline bool GCardReaderService::isThereSomeTrackOK()
{
	return ( mst_aryTracks[eTrack1].m_u8ErrorCode == CR_TRACK_OK ||
			 mst_aryTracks[eTrack2].m_u8ErrorCode == CR_TRACK_OK ||
			 mst_aryTracks[eTrack3].m_u8ErrorCode == CR_TRACK_OK ||
			 mst_aryTracks[eTrack4].m_u8ErrorCode == CR_TRACK_OK  );
};

#endif // GCardReaderService_h
