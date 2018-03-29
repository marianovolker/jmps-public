#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "SPOT/SPOTProtocolInterface/GSPOTSession.h"
#include "Common/Util/DEBUGOUT.h"

const char*  gC_pcIniPath = "lib/Common/ConfigFile/UnitTest/ini/SPOTCheck_UT.ini";


bool CreateLogForUT()
{
	char arycFullPath[512], *pcFound;

	memset(arycFullPath, 0, 512);
	getcwd(arycFullPath, 508);
	printf("Current Directory: %s\n", arycFullPath);
	pcFound = strstr(arycFullPath, ".build_linux");

	if(pcFound == NULL)
	{
		printf("Base directory not found. Test suite ABORTED.\n");
		return false;
	}

	*pcFound = '\0'; // truncate current directory to base directory
	printf("Base Directory: %s\n", arycFullPath);
	strcat(arycFullPath, gC_pcIniPath);

	// Instantiate the logger for the unit tests
	GLoggerConfig*  pLogConfig = new GLoggerConfig(arycFullPath);
	pLogConfig->Initialize();
	GLogger::CreateInstance(pLogConfig);

	return true;
}

void DestroyLogForUT()
{
	GLoggerConfig* pCfg = GLogger::GetInstance()->GetConfig();
	GLogger::DestroyInstance();
	delete pCfg;
}


int main(int argc, char **argv)
{
	if( ! CreateLogForUT() )
		return 1;

	// return code to shell script
	int iret = 0;

	// Initialize the testing framework
	testing::InitGoogleTest(&argc, argv);

	// Run the tests
	iret =  RUN_ALL_TESTS();

	// Destroy the log
	DestroyLogForUT();

	return iret;
}



