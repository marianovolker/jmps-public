//============================================================================
// Name        : main.cpp
// Author      :
// Version     :
//============================================================================

#include "GSystem.h"
#include "SPOTDashboard.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Messaging/GMessageManager.h"


int main(int ac, char **av)
{
	int iret = SPOTDashboard_init();

	if (iret != 0)
	{
		return iret;
	}

	while (!GMessageManager::isExitApplicationRequested())
	{
		GSystem::Sleep(2000);
	}

	GSPOTSession* pSession = GSPOTSession::GetSoleInstance();

	return SPOTDashboard_exit(pSession);
}


