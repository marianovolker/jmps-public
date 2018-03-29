/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GBarcodeScannerService.h
 * @author Mariano Volker
 * @date Jul 16 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GBarcodeScannerService_h
#define GBarcodeScannerService_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Menus/GMenuManager.h"
#include "GBarcodeScannerServiceStatistics.h"


const int    BCS_ENABLE_CMD_LEN 		= 8;
const int    BCS_DISABLE_CMD_LEN 		= 6;
const UINT32 BCS_EVENT_TIMEOUT 			= 20000; // 20 sec
const UINT8  BCS_ERROR_MAXATTEMPTS 		= 10; 	 // max severe error to count before blocking BCS

class GBarcodeScannerManager;

class GBarcodeScannerService
{
	public:

		enum eStatus
		{
			eStatusInitial,				// Waiting for enable event.
			eStatusWaitingBarcodeData,  // Waiting barcode data to come
			eStatusDataArrived,			// The data has arrived.
			eStatusError,				// Barcode device in permanent error state

			eLastStatus = (eStatusError+1)
		};

		enum eEvents
		{
			eEventEnableDevice,
			eEventDisableDevice,
			eEventBarcodeScannerEnable,
			eEventBarcodeScannerDisable,
			eEventBarcodeData,
			eEventTimeout,
			eEventError,

			eLastEvent = (eEventError+1)
		};

		struct sDataEvent
		{
			void* data;
			UINT16 size;
		};

		inline static GBarcodeScannerService* GetService();
		static GBarcodeScannerService* CreateInstance(GSPOTSession* pSession);
		static void DestroyInstance();

		static const char* GetErrorDescription(UINT8 u8Error);
		static bool isBarcodeScannerReply(GSPOTMessage* pMsg, bool blOnlySubCmd = false);

		void AttachManager(GBarcodeScannerManager* pMgr);
		void DetachManager();
		void EnableDevice();
		void DisableDevice();

		bool ProcessBarcodeScannerReply(GSPOTMessage* pMsg);
		bool ProcessSystemStatusNotification(UINT8 u8Status, UINT8 u8StatusEx, UINT8 u8Error);

		inline eStatus GetStatus() const {return m_eStatus;};
		inline const GBarcodeScannerServiceStatistics & GetSessionStatistics() const {return m_oBarcodeScannerSessionStatistics;};


	private:
		GBarcodeScannerService(GSPOTSession* pSession);
		~GBarcodeScannerService();

		void ProcessEventReceived(eEvents e, sDataEvent* data = NULL);
		void NotifyBarcodeDataArrived( bool bError = false );
		bool StopTimer();
		bool StartTimer();
		void CheckIfBarcodeScannerIsInPermanentError();
		bool EnableRequest();
		bool DisableRequest();
		bool SendEnableCmdToSPOT();
		bool SendDisableCmdToSPOT();
		void ProcessTimeoutEvent();
		bool ProcessDataEvent(UINT8* pMsgData, UINT16 u16DataSize);
		void NextState(eEvents e, eStatus s);

		static GBarcodeScannerService* mst_pSoleInstance;
		static GMutex m_createInstanceMutex;
		static const UINT8 mst_aryu8EnableBarcodeScannerCmd					[BCS_ENABLE_CMD_LEN		];
		static const UINT8 mst_aryu8DisableBarcodeScannerCmd				[BCS_DISABLE_CMD_LEN	];
		static const char * mst_arypcStatusLabel							[eLastStatus			];
		static const char * mst_arypcEventLabel								[eLastEvent				];

		enum eStatus m_eStatus;
		UINT8 m_u8AttemptsInError; // consecutive severe error count
		GMutex m_CardStateMutex; // mutex to protect status changes
		GMutex m_SincronizeMutex; // mutex to sincronize a response to diagnostic info.
		GCondition m_Condition;
		GTimer* m_pTimer;
		GSPOTSession* m_pSession;
		GBarcodeScannerManager* m_pManager;
		GSimpleCommand<GBarcodeScannerService> m_TOutCommand; // command to process timeout event
		GBarcodeScannerServiceStatistics m_oBarcodeScannerSessionStatistics;
};


inline GBarcodeScannerService* GBarcodeScannerService::GetService()
{
	GMutexLock ml(m_createInstanceMutex);

	return mst_pSoleInstance;
}

inline GBarcodeScannerService* GBarcodeScannerService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GBarcodeScannerService(pSession);
	}

	return mst_pSoleInstance;
}

inline void GBarcodeScannerService::DestroyInstance()
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

#endif // GBarcodeScannerService_h
