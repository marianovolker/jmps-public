#include "GSignalHandler.h"

#include "GDisplayManagerDaemon.h"
#include "GProcess.h"

#include <sys/socket.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>

GSignalHandler::GSignalHandler(GDisplayManagerDaemon *pDMObserver=0):
	m_pDMObserver(pDMObserver),
	m_bContinue(true)
{
	LOG_LEVEL4("GSignalHandler()");

	// From my understanding of the blog post listed below, we should not call moveToThread()
	// here in the constructor, so I am commenting this out.
	// http://blog.qt.digia.com/blog/2010/06/17/youre-doing-it-wrong/
	//moveToThread(this);
}

GSignalHandler::~GSignalHandler()
{
	LOG_LEVEL4("~GSignalHandler()");
}

void GSignalHandler::run()
{
	LOG_LEVEL3("run()");

	if( !connect(this, SIGNAL(StopDM()), m_pDMObserver, SLOT(goAway()), Qt::QueuedConnection) )
	{
		LOG_LEVEL1("Can't connect to GDisplayManagerDaemon object.");
	}

	MainLoop();

	LOG_LEVEL3("The event loop of this thread has been finished");
}

void GSignalHandler::Stop()
{
	LOG_LEVEL3("Stop()");

	if( !disconnect(this, SIGNAL(StopDM()), m_pDMObserver, SLOT(goAway())) )
	{
		LOG_LEVEL1("Can't disconnect to GDisplayManagerDaemon object.");
	}

	quit();
}

void GSignalHandler::MainLoop()
{
	LOG_LEVEL3("MainLoop()");

	struct pollfd fds[3];

	// Close the other end of the channel.
	close(GProcess::sighupFd [0]);
	close(GProcess::sigtermFd[0]);
	close(GProcess::sigintFd [0]);

	fds[0].events = POLLIN|POLLRDHUP;
	fds[1].events = fds[0].events;
	fds[2].events = fds[1].events;

	fds[0].fd = GProcess::sighupFd [1];
	fds[1].fd = GProcess::sigtermFd[1];
	fds[2].fd = GProcess::sigintFd [1];

	while( m_bContinue )
	{
		int rc(poll(&fds[0], 3, 10));

		if( rc > 0 )
		{
			for( unsigned int i = 0; i < 3; i++ )
			{
				if( fds[i].revents&POLLRDHUP )
				{
					LOG_LEVEL4(QString("Received POLLRDHUP on [%1] socketfd. Ending application.").arg(fds[i].fd));
					handleSigTerm();
					break;
				}

				if( fds[i].revents&POLLIN )
				{
					if( fds[i].fd == GProcess::sighupFd[1] )
					{
						LOG_LEVEL4("Received data on sighupFd");
						handleSigHup();
					}
					else if( fds[i].fd == GProcess::sigtermFd[1] )
					{
						LOG_LEVEL4("Received data on sigtermFd");
						handleSigTerm();
					}
					else if( fds[i].fd == GProcess::sigintFd[1] )
					{
						LOG_LEVEL4("Received data on sigintFd");
						handleSigInt();
					}
					else
					{
						LOG_LEVEL3("Unknow socketfd. Ending application.");
						handleSigTerm();
					}
					break;
				}
			}
		}
		else if ( rc < 0 )
		{
			LOG_LEVEL3("Reding error on some socketfd. Ending application.");
			handleSigTerm();
		}
	}
}

void GSignalHandler::handleSigTerm()
{
	LOG_LEVEL3("handleSigTerm()");

	char tmp;
	::read(GProcess::sigtermFd[1], &tmp, sizeof(tmp));

	close(GProcess::sighupFd [1]);
	close(GProcess::sigtermFd[1]);
	close(GProcess::sigintFd [1]);

	emit StopDM();

	m_bContinue = false;
}

void GSignalHandler::handleSigInt()
{
	LOG_LEVEL3("handleSigInt()");

	char tmp;
	::read(GProcess::sigintFd[1], &tmp, sizeof(tmp));

	close(GProcess::sighupFd [1]);
	close(GProcess::sigtermFd[1]);
	close(GProcess::sigintFd [1]);

	emit StopDM();

	m_bContinue = false;
}

void GSignalHandler::handleSigHup()
{
	LOG_LEVEL3("handleSigHup()");

	char tmp;
	::read(GProcess::sighupFd[1], &tmp, sizeof(tmp));

	GLogger* pLogger(GLogManager::GetDefaultLogger());
	if( pLogger )
	{
		pLogger->CloseFile();
		pLogger->OpenFile();
		LOG_LEVEL3("Re-Open Object Log");
	}
}
