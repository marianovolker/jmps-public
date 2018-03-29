/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GContactlessReaderService.h
 * @author Mariano Volker
 * @date Jul 01 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GContactlessReaderService_h
#define GContactlessReaderService_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Menus/GMenuManager.h"
#include "GContactlessReaderServiceStatistics.h"



const UINT8  CLR_TRACK_OK 				= 0;  // error code for a normal encoded track
const UINT8  CLR_ERROR_MAXATTEMPTS 		= 10; // max severe error to count before blocking CLR
const int    CLR_ENABLE_CMD_LEN 		= 8;
const int    CLR_DISABLE_CMD_LEN 		= 6;
const int    CLR_TRACK_SIZE 			= 128;
const UINT   CLR_TRACK1_MASK 			= 1;
const UINT   CLR_TRACK2_MASK 			= 2;
const UINT   CLR_TRACK3_MASK 			= 4;
const UINT32 CLR_EVENT_TIMEOUT 			= 20000; // 20 sec


class GContactlessReaderManager;

class GContactlessReaderService
{
	public:

		enum eTrackNo
		{
			eTrack1 = 0,
			eTrack2,
			eTrack3,
			eTotalTracks=eTrack3
		};

		typedef struct _clr_track_info
		{
			UINT16 m_u16Length;
			UINT8 m_aryu8Data[CLR_TRACK_SIZE];

		} CLR_TRACK_INFO;

		enum eStatus
		{
			eStatusInitial,				// Waiting for enable event.
			eStatusWaitingRFCardData,   // Waiting RF card data to come
			eStatusDataArrived,			// The data has arrived.
			eStatusError,				// Card reader device in permanent error state

			eLastStatus = (eStatusError+1)
		};

		enum eEvents
		{
			eEventEnableDevice,
			eEventDisableDevice,
			eEventContactlessReaderEnable,
			eEventContactlessReaderDisable,
			eEventRFCardData,
			eEventTimeout,
			eEventError,

			eLastEvent = (eEventError+1)
		};

		struct sDataEvent
		{
			void* data;
			UINT16 size;
		};

		inline static GContactlessReaderService* GetService();
		static GContactlessReaderService* CreateInstance(GSPOTSession* pSession);
		static void DestroyInstance();

		static const char* GetErrorDescription(UINT8 u8Error);
		static bool isContactlessReaderReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);

		void AttachManager(GContactlessReaderManager* pMgr);
		void DetachManager();
		void EnableDevice();
		void DisableDevice();

		bool ProcessContactlessReaderReply(GSPOTMessage* pMsg);
		bool ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);

		inline eStatus GetStatus() const {return m_eStatus;};
		inline GContactlessReaderService::CLR_TRACK_INFO GetTrackInfo(eTrackNo e) const {return mst_aryCLRTracks[e];};
		inline const GContactlessReaderServiceStatistics & GetSessionStatistics() const {return m_oCLRSessionStatistics;};


	private:
		GContactlessReaderService(GSPOTSession* pSession);
		~GContactlessReaderService();

		void ProcessEventReceived(eEvents e, sDataEvent* data = NULL);
		void NotifyRFCardDataArrived( bool bError = false );
		bool StopTimer();
		bool StartTimer();
		void ClearTrackInfo();
		void CheckIfContactlessReaderIsInPermanentError();
		UINT16 ClipPartialLength(UINT16 u16Length, int nTrack);
		bool EnableRequest();
		bool DisableRequest();
		bool SendEnableCmdToSPOT();
		bool SendDisableCmdToSPOT();
		void ProcessTimeoutEvent();
		bool ProcessDataEvent(UINT8* pMsgData, UINT16 u16DataSize);
		UINT8 ProcessRFCardInfo(UINT8* pMsgdata, UINT16 u16DataSize);
		void NextState(eEvents e, eStatus s);

		static GContactlessReaderService* mst_pSoleInstance;
		static GMutex m_createInstanceMutex;
		static const UINT8 mst_aryu8EnableCLRCmd							[CLR_ENABLE_CMD_LEN		];
		static const UINT8 mst_aryu8DisableCLRCmd							[CLR_DISABLE_CMD_LEN	];
		static GContactlessReaderService::CLR_TRACK_INFO mst_aryCLRTracks	[eTotalTracks			];
		static const char * mst_arypcStatusLabel							[eLastStatus			];
		static const char * mst_arypcEventLabel								[eLastEvent				];

		enum eStatus m_eStatus;
		UINT8 m_u8AttemptsInError; // consecutive severe error count
		GMutex m_CardStateMutex; // mutex to protect status changes
		GMutex m_SincronizeMutex; // mutex to sincronize a response to diagnostic info.
		GCondition m_Condition;
		GTimer* m_pTimer;
		GSPOTSession* m_pSession;
		GContactlessReaderManager* m_pManager;
		GSimpleCommand<GContactlessReaderService> m_TOutCommand; // command to process timeout event
		GContactlessReaderServiceStatistics m_oCLRSessionStatistics;
};


inline GContactlessReaderService* GContactlessReaderService::GetService()
{
	GMutexLock ml(m_createInstanceMutex);

	return mst_pSoleInstance;
}

inline GContactlessReaderService* GContactlessReaderService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GContactlessReaderService(pSession);
	}

	return mst_pSoleInstance;
}

inline void GContactlessReaderService::DestroyInstance()
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

#endif // GContactlessReaderService_h
