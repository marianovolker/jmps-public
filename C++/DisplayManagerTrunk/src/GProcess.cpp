#include "GProcess.h"

#include "GDisplayManagerDaemon.h"

#include <sys/socket.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

#include <QApplication>
#include "GDMQtApplication.h"

int  GProcess::sighupFd  [2];
int  GProcess::sigtermFd [2];
int  GProcess::sigintFd  [2];
bool GProcess::bContinue = true;

int GProcess::StartDisplayManagerProcess(int argc, char *argv[], char *env[])
{
	int iRet(0);

	std::cout << "Process for handling QT is [" << getpid() << "]\n";

	GDMQtApplication app (argc, argv);

	try
	{
		GDisplayManagerDaemon gApp(&app, env);

		gApp.Init();

		iRet = gApp.Run();

		gApp.Stop();
	}
	catch(GException &e)
	{
		std::cout << "ERROR: [" << e.what() << "]\n";
		iRet = -1;
	}

	std::cout << "Process for handling QT is dying [" << getpid() << "], iRet [" << iRet << "]\n";

	return (iRet);
}

int GProcess::StartSignalHandlerProcess()
{
	int iRet(0);

	std::cout << "Process for handling signals is [" << getpid() << "]\n";

	GProcess::SignalsInit();

	GProcess::UnBlockUsefulSignalsInit();

	GProcess::MainLoop();

	std::cout << "Process for handling signals is dying [" << getpid() << "], iRet [" << iRet << "]\n";

	return (iRet);
}

void GProcess::BlockAllSignalsInit()
{
	sigset_t senales;
	sigemptyset(&senales);
	sigfillset(&senales);

	if( sigprocmask(SIG_SETMASK, &senales, 0) != 0 )
	{
		std::cout << "Couldn't blocking all signals\n";
		exit(1);
	}
}

void GProcess::UnBlockUsefulSignalsInit()
{
	sigset_t senales;
	sigemptyset(&senales);

	if( sigaddset(&senales, SIGHUP) != 0 )
	{
		std::cout << "Failed to install SIGHUP handler\n";
		exit(1);
	}

	if( sigaddset(&senales, SIGTERM) != 0 )
	{
		std::cout << "Failed to install SIGTERM handler\n";
		exit(1);
	}

	if( sigaddset(&senales, SIGINT) != 0 )
	{
		std::cout << "Failed to install SIGINT handler\n";
		exit(1);
	}

	if( sigprocmask(SIG_UNBLOCK, &senales, 0) != 0 )
	{
		std::cout << "Couldn't unblocking SIGHUP|SIGTERM|SIGINT signals\n";
		exit(1);
	}
}


void GProcess::UnBlockChildSignal()
{
	sigset_t senales;
	sigemptyset(&senales);

    std::cout << "Unblocking SIGCHLD signal\n";

	if( sigaddset(&senales, SIGCHLD) != 0 )
	{
		std::cout << "Failed to install SIGCHLD handler\n";
		exit(1);
	}

	if( sigprocmask(SIG_UNBLOCK, &senales, 0) != 0 )
	{
		std::cout << "Couldn't unblocking SIGCHLD signal\n";
		exit(1);
	}
}



void GProcess::AddSigChildHandler()
{
	struct sigaction sigchld;

	std::cout << "AddSigChildHandler()\n";

	sigchld.sa_handler = GProcess::chldSignalHandler;
	//sigchld.sa_handler = SIG_DFL;
	sigemptyset(&sigchld.sa_mask);
	sigchld.sa_flags = SA_NOCLDSTOP;
	//sigchld.sa_flags = SA_NOCLDWAIT;
	if( sigaction(SIGCHLD, &sigchld, 0) != 0 )
	{
		std::cout << "Failed to install SIGCHLD handler\n";
		exit(1);
	}
}


void GProcess::SetNoChildWait()
{
	struct sigaction sigchld;

	sigchld.sa_handler = SIG_DFL;
	sigemptyset(&sigchld.sa_mask);
	sigchld.sa_flags = SA_NOCLDWAIT;
	if( sigaction(SIGCHLD, &sigchld, 0) != 0 )
	{
		std::cout << "Failed to Set No Child Wait\n";
		exit(1);
	}
}






void GProcess::SignalsInit()
{
	struct sigaction sighup, sigterm, sigint;

	sighup.sa_handler = GProcess::hupSignalHandler;
	sigemptyset(&sighup.sa_mask);
	sigfillset(&sighup.sa_mask);
	sighup.sa_flags = 0;
	if( sigaction(SIGHUP, &sighup, 0) != 0 )
	{
		std::cout << "Failed to install SIGHUP handler\n";
		exit(1);
	}

	sigterm.sa_handler = GProcess::termSignalHandler;
	sigemptyset(&sigterm.sa_mask);
	sigfillset(&sigterm.sa_mask);
	sigterm.sa_flags = 0;
	if( sigaction(SIGTERM, &sigterm, 0) != 0 )
	{
		std::cout << "Failed to install SIGTERM handler\n";
		exit(1);
	}

	sigint.sa_handler = GProcess::intSignalHandler;
	sigemptyset(&sigint.sa_mask);
	sigfillset(&sigint.sa_mask);
	sigint.sa_flags = 0;
	if( sigaction(SIGINT, &sigint, 0) != 0 )
	{
		std::cout << "Failed to install SIGINT handler\n";
		exit(1);
	}
}

void GProcess::MainLoop()
{
	struct pollfd fds[3];

	// Close the other end of the channel.
	close(GProcess::sighupFd [1]);
	close(GProcess::sigtermFd[1]);
	close(GProcess::sigintFd [1]);

	fds[0].events = POLLRDHUP;
	fds[1].events = fds[0].events;
	fds[2].events = fds[1].events;

	fds[0].fd = GProcess::sighupFd [0];
	fds[1].fd = GProcess::sigtermFd[0];
	fds[2].fd = GProcess::sigintFd [0];

	while( GProcess::bContinue )
	{
		int rc(poll(&fds[0], 3, 10));

		if( rc > 0 )
		{
			for( unsigned int i = 0; i < 3; i++ )
			{
				if( fds[i].revents&POLLRDHUP )
				{
					std::cout << "Ending application\n";

					GProcess::bContinue = false;
					break;
				}
			}
		}
	}
}

void GProcess::SignalsSocketNotifierInit()
{
	if( ::socketpair(AF_UNIX, SOCK_STREAM, 0, GProcess::sighupFd) )
	{
		std::cout << "Couldn't create HUP socketpair\n";
		exit(1);
	}

	if( ::socketpair(AF_UNIX, SOCK_STREAM, 0, GProcess::sigtermFd) )
	{
		std::cout << "Couldn't create HUP socketpair\n";
		exit(1);
	}

	if( ::socketpair(AF_UNIX, SOCK_STREAM, 0, GProcess::sigintFd) )
	{
		std::cout << "Couldn't create HUP socketpair\n";
		exit(1);
	}
}

void GProcess::hupSignalHandler(int)
{
    char a = 1;
    ::write(GProcess::sighupFd[0], &a, sizeof(a));
}

void GProcess::termSignalHandler(int)
{
    char a = 1;
    ::write(GProcess::sigtermFd[0], &a, sizeof(a));
    bContinue = false;
}

void GProcess::intSignalHandler(int)
{
    char a = 1;
    ::write(GProcess::sigintFd[0], &a, sizeof(a));
    bContinue = false;
}


void GProcess::chldSignalHandler(int)
{
    int status, child_val;

	//std::cout << "chldSignalHandler(int): Sig child received\n";

    /* Wait for any child without blocking */
    if (waitpid(-1, &status, WNOHANG) < 0)
    {
        //fprintf(stderr, "waitpid failed\n");
        return;
    }

	//std::cout << "chldSignalHandler(int): Done waitpid\n";

    /*
     * We now have the info in 'status' and can manipulate it using
     * the macros in wait.h.
     */
    if (WIFEXITED(status))                /* did child exit normally? */
    {
        child_val = WEXITSTATUS(status); /* get child's exit status */
        //printf("child's exited normally with status %d\n", child_val);
    }

}

