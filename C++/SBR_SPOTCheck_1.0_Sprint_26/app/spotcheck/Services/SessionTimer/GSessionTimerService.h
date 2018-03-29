/*!
 * © 2013 Gilbarco Inc.
 * Confidential and Proprietary
 *
 * @file GSessionTimerService.h
 * @author Mariano Volker
 * @date Aug 13 2013
 * @copyright © 2013 Gilbarco Inc. Confidential and Propietary
 */

#ifndef GSessionTimerService_h
#define GSessionTimerService_h 1

#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"


const UINT32 STS_EVENT_TIMEOUT = 180000; // 180 sec


class GSessionTimerService
{
	public:

		inline static GSessionTimerService* GetService();
		inline static GSessionTimerService* CreateInstance(GSPOTSession* pSession);
		inline static void DestroyInstance();

		bool StopTimer();
		bool StartTimer();
		bool ResetTimer();

	private:
		GSessionTimerService(GSPOTSession* pSession);
		~GSessionTimerService();

		static GSessionTimerService* mst_pSoleInstance;
		static GMutex m_createInstanceMutex;

		void ProcessTimeoutEvent();

		GSPOTSession* m_pSession;
		GMutex m_SincronizeMutex;
		GTimer* m_pTimer;
		GSimpleCommand<GSessionTimerService> m_TOutCommand; // command to process timeout event
};


inline GSessionTimerService* GSessionTimerService::GetService()
{
	GMutexLock ml(m_createInstanceMutex);

	return mst_pSoleInstance;
}

inline GSessionTimerService* GSessionTimerService::CreateInstance(GSPOTSession* pSession)
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance == NULL)
	{
		mst_pSoleInstance = new GSessionTimerService(pSession);
	}

	return mst_pSoleInstance;
}

inline void GSessionTimerService::DestroyInstance()
{
	GMutexLock ml(m_createInstanceMutex);

	if (mst_pSoleInstance != NULL)
	{
		delete mst_pSoleInstance;
		mst_pSoleInstance = NULL;
	}
}

#endif // GSessionTimerService_h
