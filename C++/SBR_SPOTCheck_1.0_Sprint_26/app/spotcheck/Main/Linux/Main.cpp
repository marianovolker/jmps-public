//============================================================================
// Name        : main.cpp
// Author      :
// Version     :
//============================================================================

#include "GSystem.h"
#include "SPOTCheck.h"
#include "Menus/GMenuManager.h"


int main()
{
	int iret = SPOTCheck_init();
	if( iret != 0 )
	{
		return iret; // some fatal error during initialization
	}

	while (! GMenuManager::isExitApplicationRequested() )
	{
		GSystem::Sleep(2000);
	}

	GSPOTSession* pSession = GSPOTSession::GetSoleInstance();

	return SPOTCheck_exit(pSession);
}


