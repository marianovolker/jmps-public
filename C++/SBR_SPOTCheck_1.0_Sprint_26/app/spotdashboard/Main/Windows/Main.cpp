//============================================================================
// Name        : main.cpp
// Author      : 
// Version     :
//============================================================================

#include "GSystem.h"
#include "SPOTDashboard.h"
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Messaging/GMessageManager.h"
#include "Gui/GSPOTDashboardController.h"


void startWxApp(int ac, char **av)
{

	GSPOTDashboardController* pController = new GSPOTDashboardController();

	pController->startWxApp(ac,av);
}

int main(int ac, char **av)
{
	int iret = SPOTDashboard_init();

	if( iret != 0 )
	{
		return iret;
	}

	startWxApp( ac, av);

	GSPOTSession* pSession = GSPOTSession::GetSoleInstance();

	return SPOTDashboard_exit(pSession);
}


