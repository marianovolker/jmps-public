#include"GGstLaunchMediaManager.h"

#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"

#include <QMutexLocker>
#include <QWaitCondition>
#include <string.h>
#include <iostream>
#include <QMutex>
#include <QString>
#include <QList>
#include <QByteArray>
#include <QFile>


QSharedPointer<GGstLaunchMediaManager>  GGstLaunchMediaManager::st_poGGstLaunchMediaManager	(NULL);
QMutex GGstLaunchMediaManager::st_oMutex;
QMutex GGstLaunchMediaManager::_mtxSynchMutexVideoTermination;
QWaitCondition GGstLaunchMediaManager::_cndSynchConditionalVideoTermination;

GGstLaunchMediaManager::GGstLaunchMediaManager():
GAbsMediaManager(),
m_strVideoAppCommand(""),
m_iVideoProcessid(0)
{
	LOG_LEVEL4("GGstLaunchMediaManager()");

	moveToThread(this);
}

GGstLaunchMediaManager::~GGstLaunchMediaManager()
{
	LOG_LEVEL4("~GGstLaunchMediaManager()");
}

QSharedPointer<GGstLaunchMediaManager> GGstLaunchMediaManager::GetInstance()
{
	LOG_LEVEL4("GetInstance()");

	st_oMutex.lock();

	if(st_poGGstLaunchMediaManager == NULL)
	{
		st_poGGstLaunchMediaManager = QSharedPointer<GGstLaunchMediaManager>(new GGstLaunchMediaManager());
	}

	st_oMutex.unlock();

	return st_poGGstLaunchMediaManager;
}

void GGstLaunchMediaManager::Initialize(char **env)
{

    GAbsMediaManager::Initialize(env);

    m_strVideoAppCommand = GETCONF_STR(eDisplayManagerVideoAppCommand);
    LOG_LEVEL4(QString("Video playback: Video Application Command Property is: %1").arg(m_strVideoAppCommand));

}




void GGstLaunchMediaManager::StartVideo(const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile) throw(GException)
{

	LOG_LEVEL4("StartVideo()");

	if (!QFile::exists(m_strVideoFilePath+strVideoFile)) { // first check to see if the video exists in the default applause directory
    	if(!QFile::exists(m_strVideoPlayCommandFilePath + strVideoFile)) { // if not there, see if this is the "VideoPlay" command file from the POS
    		LOG_LEVEL3(QString("Video playback: THROWING EXCEPTION: Video File (%1) to play was not found.").arg(strVideoFile));
    		throw GEXCEPTION(QString("Video playback: Video File (%1) to play was not found.").arg(strVideoFile));
    	}
    	else {
    		m_strCurrentVideoFilePath = m_strVideoPlayCommandFilePath;
    	}
    }
    else {
    	m_strCurrentVideoFilePath = m_strVideoFilePath;
    }

    LOG_LEVEL3(QString("Video playback: Video file (%1) found").arg(m_strCurrentVideoFilePath + strVideoFile));

	emit SignalStartVideo(iVideoWidth, iVideoHeight, iVideoTopLeftX, iVideoTopLeftY, strVideoFile);

}


int GGstLaunchMediaManager::GetVideoStopEndDelay()
{
	return m_iVideoStopEndSleep;
}

void GGstLaunchMediaManager::StopMedia()
{
	LOG_LEVEL4("StopMedia()");

	if (m_iVideoProcessid)
	{
		emit SignalStopMedia();
	}
	else
	{
		LOG_LEVEL4("No Video Process running currently.");
	}

	
}


void GGstLaunchMediaManager::SynchroStopMedia()
{
	LOG_LEVEL4("SynchroStopMedia()");

	if (m_iVideoProcessid)
	{

		QMutexLocker oLocker(&GGstLaunchMediaManager::_mtxSynchMutexVideoTermination);

		emit SignalSynchroStopMedia();

		GGstLaunchMediaManager::_cndSynchConditionalVideoTermination.wait(&GGstLaunchMediaManager::_mtxSynchMutexVideoTermination);

	}
	else
	{
		LOG_LEVEL4("No Video Process running currently.");
	}
	
}


// Stop any child processes created by the MediaManager here
void GGstLaunchMediaManager::StopChildProcesses()
{
	LOG_LEVEL3("StopChildProcesses()");
}



void GGstLaunchMediaManager::BuildLaunchVideoCommand(QList<QByteArray> &balVideoCmdLine, const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile)
{
	//"filesrc location=%1 ! qtdemux name=demux demux.video_00 ! queue ! %2 ! 
	//omapsink x=%3 y=%4 overlayWidth=%5 overlayHeight=%6 sync=true max-lateness=%7 rotation=%8
	//demux.audio_00 ! queue max-size-buffers=%9 max-size-time=0 max-size-bytes=0 ! %10 ! alsasink"
	
	balVideoCmdLine = m_strVideoGstPipeline.arg(m_strCurrentVideoFilePath+strVideoFile)
								.arg(m_strVideoVideoDecoderGstElement)
								.arg(iVideoTopLeftX).arg(iVideoTopLeftY)
								.arg(iVideoWidth).arg(iVideoHeight)
								.arg(m_strVideoLateness)
								.arg(m_iVideoRotation)
								.arg(m_strVideoMaxBufs)								
								.arg(m_strVideoAudioDecoderGstElement)
									  .toLocal8Bit()
									  .split(' ');

	balVideoCmdLine.prepend(m_strVideoAppCommand.toLocal8Bit());

}



void GGstLaunchMediaManager::ExecuteLaunchVideoCommand(char **cppArgsLaunchVideoCmd,  int iArgQtty)
{
	UnBlockAllSignals();

	printf("Video playback: Launch Video Command: %s", cppArgsLaunchVideoCmd[0]);

	for(int i=0;i<iArgQtty;i++)
	{

		printf("%s ", cppArgsLaunchVideoCmd[i]);
	}

	printf("\n");

	execve(cppArgsLaunchVideoCmd[0], cppArgsLaunchVideoCmd, this->m_env);

        LOG_LEVEL3(QString("Video playback: External application [launch video command] execution ended: errno: %1 errstr: %2").arg(errno).arg(strerror(errno)));
	_exit(EXIT_FAILURE);

}


void GGstLaunchMediaManager::SlotStartVideo(const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile)
{
	LOG_LEVEL4("SlotStartVideo()");

	LOG_LEVEL4(QString("Video playback: Pars: iVideoWidth: %1, iVideoHeight: %2 , iVideoTopLeftX: %3, iVideoTopLeftY: %4, strVideoFile: %5").arg(iVideoWidth).arg(iVideoHeight).arg(iVideoTopLeftX).arg(iVideoTopLeftY).arg(strVideoFile));
	LOG_LEVEL3("Video playback: Doing fork for Video Application.");

	int iPid = fork();

	QList<QByteArray> balVideoCmdLine;

	//Building Launch Video Command (all the arguments)
	BuildLaunchVideoCommand(balVideoCmdLine, iVideoWidth, iVideoHeight, iVideoTopLeftX, iVideoTopLeftY, strVideoFile);

	int iArgQtty = balVideoCmdLine.count();

	char **cppArgsLaunchVideoCmd = CreateVariableArgument(balVideoCmdLine, iArgQtty);

	if( iPid == 0 ) // child process
	{
		ExecuteLaunchVideoCommand(cppArgsLaunchVideoCmd, iArgQtty);
	}
        else if ( iPid > 0 )  // parent process
	{//Save Pid
		LOG_LEVEL3(QString("Video playback: Launch Video playback Command executed: %1").arg(QString(JoinLaunchVideoCmd(balVideoCmdLine," "))));
		m_iVideoProcessid = iPid;
		LOG_LEVEL4(QString("Video playback: Process created successfully (PID: %1)").arg(m_iVideoProcessid));
	}
	else
	{
		LOG_LEVEL2("Video playback: Fork failed - Couldn't create Video Process");
		ProcessFailedFork();
	}

	delete[] cppArgsLaunchVideoCmd;
}



void GGstLaunchMediaManager::EndVideoExecution()
{
	int iStatus;
	int iRetError;
	int iRetWait;

	LOG_LEVEL3(QString("Video playback: Sending SIGUSR2 to Video Application Process Id [%1].").arg(m_iVideoProcessid));
	iRetError = kill(m_iVideoProcessid, SIGUSR2);

	if ( iRetError == -1 )
	{
		LOG_LEVEL2(QString("Video playback: Result: Error in USR2 signal sending. Results: errno: %1 errstr: %2").arg(errno).arg(strerror(errno)));
	}
	else
	{
		LOG_LEVEL4("Video playback: Result: No errors in USR2 signal sending.");

		bool bVideoProcessTerminated = false;
		int  iWaitTime = m_iVideoStopEndSleep;

		LOG_LEVEL4(QString("Video playback: Waiting up to %1 seconds for Video Application Process Id [%2] to exit.").arg(iWaitTime).arg(m_iVideoProcessid));
		while (!bVideoProcessTerminated && (iWaitTime > 0))
		{
			iRetWait = waitpid(m_iVideoProcessid,&iStatus,WNOHANG);
			if (iRetWait == m_iVideoProcessid)
			{
				// Video playback process terminated.
				LOG_LEVEL4(QString("Video playback: Video Application Process Id [%1] exited.").arg(m_iVideoProcessid));
				bVideoProcessTerminated = true;
			}
			else if (iRetWait == 0)
			{
				// Allow time for process to exit.
				LOG_LEVEL4(QString("Video playback: Waiting for Video Application Process Id [%1] to exit (%2).").arg(m_iVideoProcessid).arg(iWaitTime));
				sleep(1);
				iWaitTime--;
			}
			else if (iRetWait == -1)
			{
				// Assume process doesn't exist.
				LOG_LEVEL2(QString("Video playback: waitpid() failed. Results: errno: %1 errstr: %2").arg(errno).arg(strerror(errno)));
				bVideoProcessTerminated = true;
			}
		}

		if (!bVideoProcessTerminated)
		{
			LOG_LEVEL3(QString("Video playback: Sending SIGTERM to Video Application Process Id [%1].").arg(m_iVideoProcessid));
			iRetError = kill(m_iVideoProcessid, SIGTERM);
			if ( iRetError == -1 )
			{
				LOG_LEVEL2(QString("Video playback: Result: Error in TERM signal sending. Results: errno: %1 errstr: %2").arg(errno).arg(strerror(errno)));
			}
			else
			{
				LOG_LEVEL4("Video playback: Result: No errors in TERM signal sending.");
			}
		}
	}
}




void GGstLaunchMediaManager::SlotStopMedia()
{
	LOG_LEVEL4("SlotStopMedia()");

	if (m_iVideoProcessid)
	{
		EndVideoExecution();
		m_iVideoProcessid = 0;
	}
}


void GGstLaunchMediaManager::SlotSynchroStopMedia()
{
	LOG_LEVEL3("SlotSynchroStopMedia()");

	if (m_iVideoProcessid)
	{
		EndVideoExecution();
		m_iVideoProcessid = 0;
		GGstLaunchMediaManager::_cndSynchConditionalVideoTermination.wakeAll();

	}
}



