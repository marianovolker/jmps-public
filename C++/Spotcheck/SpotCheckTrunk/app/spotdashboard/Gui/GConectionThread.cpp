
#include "GConectionThread.h"
#include "SPOTDashboard.h"
#include "Common/Util/DEBUGOUT.h"

#include "GSPOTDashboardMainGUIChild.h"

const int GConectionThread::CONNECTION_STATUS_EVENT_ID 	= 100000;
const int GConectionThread::CONNECTION_STATUS_OK 		= 0;
const int GConectionThread::CONNECTION_STATUS_FAIL 		= 1;

wxThread::ExitCode GConectionThread::Entry()
{
	wxCommandEvent event(wxEVT_COMMAND_TEXT_UPDATED, CONNECTION_STATUS_EVENT_ID);

	LOG(eUnCategorized, eDebug, "Begin a new thread.\n");

	LOG(eUnCategorized, eInfo, "Reconnecting...\n");
	int iRet = SPOTDashboard_Reconnect();

	if (!iRet)
	{
		event.SetInt(CONNECTION_STATUS_OK);
		LOG(eUnCategorized, eInfo, "Reconected OK.\n");
	}
	else
	{
		event.SetInt(CONNECTION_STATUS_FAIL);
		LOG(eUnCategorized, eInfo, "Could not get data\n");
	}

	if( m_parent && !(((GSPOTDashboardMainGUIChild*)m_parent)->IsClosed()) && m_parent->GetEventHandler() )
	{
		m_parent->GetEventHandler()->AddPendingEvent(event);
	}

	LOG(eUnCategorized, eDebug, "Ending the new thread.\n");

	return 0;
}
