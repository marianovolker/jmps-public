#include <unistd.h>
#include <cstring>

#include "Common/Util/DEBUGOUT.h"
#include "GSessionTimerService.h"
#include "Menus/GMenuManager.h"

#include <GMutex.h>


GSessionTimerService* GSessionTimerService::mst_pSoleInstance = NULL;
GMutex GSessionTimerService::m_createInstanceMutex;



//##########################################################################################
//##########################################################################################
// Public methods
//##########################################################################################
//##########################################################################################

bool GSessionTimerService::StopTimer()
{
	GMutexLock ml(m_SincronizeMutex);

	LOG(eCommon, eDebug, "SessionTimer stopping timer.\n");

	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Stop();
}

bool GSessionTimerService::StartTimer()
{
	GMutexLock ml(m_SincronizeMutex);
	m_InactiveTime = STS_MAX_INACTIVE;

	LOG(eCommon, eDebug, "SessionTimer starting timer.\n");

	if( m_pTimer == NULL )
	{
		return false;
	}

	return m_pTimer->Start();
}

bool GSessionTimerService::ResetTimer()
{
	return ( StopTimer() && StartTimer() );
}




//##########################################################################################
//##########################################################################################
// Private methods
//##########################################################################################
//##########################################################################################

GSessionTimerService::GSessionTimerService(GSPOTSession* pSession)
	: m_pSession(pSession),
	  m_pTimer(NULL),
	  m_InactiveTime(STS_MAX_INACTIVE),
	  m_TOutCommand(this, &GSessionTimerService::ProcessTimeoutEvent)
{
	//m_pTimer = new GTimer(&m_TOutCommand, STS_EVENT_TIMEOUT, false, false);
	m_pTimer = new GTimer(&m_TOutCommand, SINACTIVE_EVENT_TIMEOUT, false, false);
	StartTimer();

	LOG(eCommon, eInfo, "SessionTimer Service started.\n");
}

GSessionTimerService::~GSessionTimerService()
{
	LOG(eCommon, eInfo, "SessionTimer Service  stopped.\n");
}

void GSessionTimerService::ProcessTimeoutEvent()
{
	//LOG(eCommon, eDebug, "Inactive Time :%d.\n",m_InactiveTime);
	if(m_InactiveTime >= (SINACTIVE_EVENT_TIMEOUT/1000))
	{
		m_InactiveTime -= (SINACTIVE_EVENT_TIMEOUT/1000);
		if (m_InactiveTime)
		{
			GMenuManager* pManager = GMenuManager::GetSoleInstance();
			pManager->ProcessInactiveTimeout(m_InactiveTime);
			return;
		}
	}

	m_InactiveTime = 0;

	LOG(eCommon, eInfo, "SessionTimer raise a timer action.\n");

	GMenuManager::RequestExitApplication();

	LOG(eCommon, eInfo, "SessionTimer ending application.\n");
}



