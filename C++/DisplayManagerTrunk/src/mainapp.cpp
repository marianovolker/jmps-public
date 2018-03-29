#include "GProcess.h"

#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[], char *env[])
{
	GProcess::BlockAllSignalsInit();
	GProcess::SignalsSocketNotifierInit();

	int iChildStatus = 0;

	int iPid = fork();
	if( iPid > 0 )
	{
                waitpid(-1, &iChildStatus, WNOHANG);	//waits for any children,WNOHANG makes it non-blocking

		strcat(argv[0],"_signal_handler ");
		int rc = GProcess::StartSignalHandlerProcess();

		exit(rc);
	}
	else if( iPid == 0 )

	{
		GProcess::UnBlockChildSignal();

		int rc = GProcess::StartDisplayManagerProcess(argc, argv, env);

		exit(rc);
	}
	else
	{
		std::cout << "Couldn't create main QT process\n";
		exit(1);
	}

	return 0;
}
