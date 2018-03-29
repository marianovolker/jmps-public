#include"GGstDaemonMediaManager.h"

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


QSharedPointer<GGstDaemonMediaManager>  GGstDaemonMediaManager::st_poGGstDaemonMediaManager	(NULL);
QMutex GGstDaemonMediaManager::st_oMutex;
QMutex GGstDaemonMediaManager::_mtxSynchMutexVideoTermination;
QWaitCondition GGstDaemonMediaManager::_cndSynchConditionalVideoTermination;

GGstDaemonMediaManager::GGstDaemonMediaManager():
GAbsMediaManager(),
m_bIsDisplayHidden(FALSE),
m_eCurrentVideoState(null),
m_strVideoDaemonCommand(""),
m_strVideoDaemonClientCommand(""),
m_strVideoDaemonClientCreatePipelineCommand(""),
m_strVideoDaemonClientFilenameCommand(""),
m_strVideoDaemonClientShowCommand(""),
m_strVideoDaemonClientHideCommand(""),
m_strVideoDaemonClientSetStateCommand(""),
m_strVideoDaemonProcessName("gstd"),
m_iVideoDaemonClientCommandTimeout(0),
m_iDaemonProcessID(0)
{
	LOG_LEVEL4("GGstDaemonMediaManager()");

	//moveToThread(this);
}

GGstDaemonMediaManager::~GGstDaemonMediaManager()
{
	LOG_LEVEL4("~GGstDaemonMediaManager()");
}

QSharedPointer<GGstDaemonMediaManager> GGstDaemonMediaManager::GetInstance()
{
	LOG_LEVEL4("GetInstance()");

	st_oMutex.lock();

	if(st_poGGstDaemonMediaManager == NULL)
	{
		st_poGGstDaemonMediaManager = QSharedPointer<GGstDaemonMediaManager>(new GGstDaemonMediaManager());
	}

	st_oMutex.unlock();

	return st_poGGstDaemonMediaManager;
}


void GGstDaemonMediaManager::Initialize(char **env)
{

    GAbsMediaManager::Initialize(env);

    m_strVideoDaemonCommand = GETCONF_STR(eDisplayManagerVideoDaemonCommand);
    LOG_LEVEL3(QString("Video playback: Video Daemon Command Property is: %1").arg(m_strVideoDaemonCommand));

    m_strVideoDaemonClientCommand = GETCONF_STR(eDisplayManagerVideoDaemonClientCommand);
    LOG_LEVEL4(QString("Video playback: Video Daemon Client Command Property is: %1").arg(m_strVideoDaemonClientCommand));

    m_strVideoDaemonClientCreatePipelineCommand = GETCONF_STR(eDisplayManagerVideoDaemonClientCreatePipelineCommand);
    LOG_LEVEL4(QString("Video playback: Video Daemon Client Create Pipeline Command Property is: %1").arg(m_strVideoDaemonClientCreatePipelineCommand));

    m_strVideoDaemonClientFilenameCommand = GETCONF_STR(eDisplayManagerVideoDaemonClientFilenameCommand);
    LOG_LEVEL4(QString("Video playback: Video Daemon Client Filename Command Property is: %1").arg(m_strVideoDaemonClientFilenameCommand));

    m_strVideoDaemonClientShowCommand = GETCONF_STR(eDisplayManagerVideoDaemonClientShowCommand);
    LOG_LEVEL4(QString("Video playback: Video Daemon Client Show Command Property is: %1").arg(m_strVideoDaemonClientShowCommand));

    m_strVideoDaemonClientHideCommand = GETCONF_STR(eDisplayManagerVideoDaemonClientHideCommand);
    LOG_LEVEL4(QString("Video playback: Video Daemon Client Hide Command Property is: %1").arg(m_strVideoDaemonClientHideCommand));

    m_strVideoDaemonClientSetStateCommand = GETCONF_STR(eDisplayManagerVideoDaemonClientSetStateCommand);
    LOG_LEVEL4(QString("Video playback: Video Daemon Client Set State Command Property is: %1").arg(m_strVideoDaemonClientSetStateCommand));

    m_strVideoFilePath = GETCONF_STR(eDisplayManagerPathForVideoFiles);
    LOG_LEVEL4(QString("Video playback: Video File Path Property is: %1").arg(m_strVideoFilePath));

    m_iVideoDaemonClientCommandTimeout = GETCONF_NUM(eDisplayManagerVideoDaemonClientCommandTimeout);
    LOG_LEVEL4(QString("Video playback: Time to wait for Gstreamer Daemon Client command is: %1").arg(m_iVideoDaemonClientCommandTimeout));

    //Start the GStreamer Daemon. Attempt to stop it first in case the DM was restarted and gstd was not
    GGstDaemonMediaManager::RestartGstDaemon();

}

void GGstDaemonMediaManager::StartVideo(const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile) throw (GException)
{

    LOG_LEVEL3("StartVideo()");

    if (!QFile::exists(m_strVideoFilePath + strVideoFile))  // first check to see if the video exists in the default applause directory
    {
    	if(!QFile::exists(m_strVideoPlayCommandFilePath + strVideoFile))  // if not there, see if this is the "VideoPlay" command file from the POS
    	{
    		LOG_LEVEL3(QString("Video playback: THROWING EXCEPTION: Video File (%1) to play was not found.").arg(strVideoFile));
    		throw GEXCEPTION(QString("Video playback: Video File (%1) to play was not found.").arg(strVideoFile));
    	}
    	else
    	{
    		m_strCurrentVideoFilePath = m_strVideoPlayCommandFilePath;
    	}
    }
    else
    {
    	m_strCurrentVideoFilePath = m_strVideoFilePath;
    }

    LOG_LEVEL3(QString("Video playback: Video file (%1) found").arg(m_strCurrentVideoFilePath + strVideoFile));

    emit SignalStartVideo(iVideoWidth, iVideoHeight, iVideoTopLeftX, iVideoTopLeftY, strVideoFile);

}


void GGstDaemonMediaManager::StopMedia()
{
	LOG_LEVEL3("StopMedia()");

	if (m_eCurrentVideoState == play)
	{
		emit SignalStopMedia();
	}
	else
	{
		LOG_LEVEL4("No Video Process running currently.");
	}

}


void GGstDaemonMediaManager::SynchroStopMedia()
{
	LOG_LEVEL3("SynchroStopMedia()");

	if (m_eCurrentVideoState == play)
	{

		QMutexLocker oLocker(&GGstDaemonMediaManager::_mtxSynchMutexVideoTermination);

		emit SignalSynchroStopMedia();

		GGstDaemonMediaManager::_cndSynchConditionalVideoTermination.wait(&GGstDaemonMediaManager::_mtxSynchMutexVideoTermination);

	}
	else
	{
		LOG_LEVEL4("No Video Process running currently.");
	}
	
}





void GGstDaemonMediaManager::BuildCreatePipelineCommand(QList<QByteArray> &balVideoCmdLine, const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile)
{
	//"filesrc location=%1 name=fsrc ! qtdemux name=demux demux.video_00 ! queue ! %2 !
	//omapsink x=%3 y=%4 overlayWidth=%5 overlayHeight=%6 sync=true max-lateness=%7 rotation=%8 name=vsink
	//demux.audio_00 ! queue max-size-buffers=%9 max-size-time=0 max-size-bytes=0 ! %10 ! alsasink"
	
	balVideoCmdLine = m_strVideoDaemonClientCreatePipelineCommand.toLocal8Bit().split(' ');
        // Do not split the pipeline command into separate parameters!  'create' needs to have one parameter following it or it will not work.
        balVideoCmdLine.append((m_strVideoGstPipeline).arg(m_strCurrentVideoFilePath+strVideoFile)
								.arg(m_strVideoVideoDecoderGstElement)
								.arg(iVideoTopLeftX).arg(iVideoTopLeftY)
								.arg(iVideoWidth).arg(iVideoHeight)
								.arg(m_strVideoLateness)
								.arg(m_iVideoRotation)
								.arg(m_strVideoMaxBufs)								
								.arg(m_strVideoAudioDecoderGstElement)
									  .toLocal8Bit());

	balVideoCmdLine.prepend(m_strVideoDaemonClientCommand.toLocal8Bit());

}


// NOTE: IMPORTANT!  Calling LOG_LEVEL4() statements from a child process has been
// thought to cause lockups, as all memory (including MUTEXES) are copied when we fork.
// So if another thread has the mutex locked, the child's process would think it is
// locked at wait forever.  Not good.
void GGstDaemonMediaManager::ExecuteSetStateCommand(QString newState)
{
    QList<QByteArray> balSetStateCmdLine;

    balSetStateCmdLine = m_strVideoDaemonClientSetStateCommand.arg(newState)
								  .toLocal8Bit()
								  .split(' ');
    balSetStateCmdLine.prepend(m_strVideoDaemonClientCommand.toLocal8Bit());

    int iArgQtty = balSetStateCmdLine.count();
    char **cppArgsSetStateCmd = CreateVariableArgument(balSetStateCmdLine, iArgQtty);


    // execve does not return on success, only on failure
    execve(cppArgsSetStateCmd[0], cppArgsSetStateCmd, this->m_env);

    _exit(EXIT_FAILURE);
}

void GGstDaemonMediaManager::ExecuteCreatePipelineCommand(char **cppArgsCreatePipelineCmd, int iArgQtty)
{

    printf("Video playback: Create Pipeline Command: %s", cppArgsCreatePipelineCmd[0]);

    for (int i = 0; i < iArgQtty; i++)
    {
        printf("%s ", cppArgsCreatePipelineCmd[i]);
    }
    printf("\n");

    // execve does not return on success, only on failure
    execve(cppArgsCreatePipelineCmd[0], cppArgsCreatePipelineCmd, this->m_env);

    _exit(EXIT_FAILURE);

}


void GGstDaemonMediaManager::ExecuteSetVideoFilenameCommand(const QString& strVideoFile)
{

    QList<QByteArray> balSetFilenameCmdLine;

    balSetFilenameCmdLine = m_strVideoDaemonClientFilenameCommand.arg(m_strCurrentVideoFilePath+strVideoFile)
								  .toLocal8Bit()
								  .split(' ');

    balSetFilenameCmdLine.prepend(m_strVideoDaemonClientCommand.toLocal8Bit());

    int iArgQtty = balSetFilenameCmdLine.count();
    char **cppArgsSetVideoFilenameCmd = CreateVariableArgument(balSetFilenameCmdLine, iArgQtty);

    // execve does not return on success, only on failure
    execve(cppArgsSetVideoFilenameCmd[0], cppArgsSetVideoFilenameCmd, this->m_env);

    _exit(EXIT_FAILURE);

}


void GGstDaemonMediaManager::ExecuteShowDisplayCommand()
{

    QList<QByteArray> balShowDisplayCmdLine;
    
    balShowDisplayCmdLine = m_strVideoDaemonClientShowCommand.toLocal8Bit().split(' ');
    balShowDisplayCmdLine.prepend(m_strVideoDaemonClientCommand.toLocal8Bit());

    int iArgQtty = balShowDisplayCmdLine.count();
    char **cppArgsShowDisplayCmd = CreateVariableArgument(balShowDisplayCmdLine, iArgQtty);

    // execve does not return on success, only on failure
    execve(cppArgsShowDisplayCmd[0], cppArgsShowDisplayCmd, this->m_env);

 //   LOG_LEVEL4(QString("Video playback: External application [show display] execution ended: errno: %1 errstr: %2").arg(errno).arg(strerror(errno)));
    _exit(EXIT_FAILURE);

}


void GGstDaemonMediaManager::ExecuteHideDisplayCommand()
{

    int iError;

    QList<QByteArray> balHideDisplayCmdLine;

    balHideDisplayCmdLine = m_strVideoDaemonClientHideCommand.toLocal8Bit().split(' ');
    balHideDisplayCmdLine.prepend(m_strVideoDaemonClientCommand.toLocal8Bit());

    int iArgQtty = balHideDisplayCmdLine.count();
    char **cppArgsHideDisplayCmd = CreateVariableArgument(balHideDisplayCmdLine, iArgQtty);

    fprintf(stderr, "Hide - about to call execve [%s]\n", cppArgsHideDisplayCmd[1]);
    // execve does not return on success, only on failure
    iError = execve(cppArgsHideDisplayCmd[0], cppArgsHideDisplayCmd, this->m_env);
    fprintf(stderr, "Hide - ERROR - returned from call [%d]\n", iError);

 //   LOG_LEVEL4(QString("Video playback: External application [hide display] execution ended: errno: %1 errstr: %2").arg(errno).arg(strerror(errno)));
    _exit(EXIT_FAILURE);

}



// NOTE: IMPORTANT!  We use waitpid() instead of wait() while sending gstd client commands
// This is because the gstd is a child process of the dispmgrd process, so if the child we
// fork to execute the client command returns before we get to wait(), it would not know about
// it and wait for ANY child to return, leaving it waiting for the gstd, which will never return.
void GGstDaemonMediaManager::SlotStartVideo(const int iVideoWidth, const int iVideoHeight, const int iVideoTopLeftX, const int iVideoTopLeftY, const QString &strVideoFile)
{
    LOG_LEVEL3("SlotStartVideo()");

    LOG_LEVEL4(QString("Video playback: Pars: iVideoWidth: %1, iVideoHeight: %2 , iVideoTopLeftX: %3, iVideoTopLeftY: %4, strVideoFile: %5").arg(iVideoWidth).arg(iVideoHeight).arg(iVideoTopLeftX).arg(iVideoTopLeftY).arg(strVideoFile));
    LOG_LEVEL4("Video playback: Doing fork for Video Application.");

    int status;
    pid_t pid;

    // We create a new pipeline for each video that is played (starting with SBR_NGPM_02.1.06)
    // Historical note: We used to create one pipeline for each video resolution and re-use it,
    //                  but functionality was broken by the DVSDK and GStreamer update for the
    //                  video framerate improvement (NPG-1178)

	pid = fork();

	QList < QByteArray > balPipelineCmdLine;

	BuildCreatePipelineCommand(balPipelineCmdLine, iVideoWidth, iVideoHeight, iVideoTopLeftX, iVideoTopLeftY, strVideoFile);

	int iArgQtty = balPipelineCmdLine.count();

	char **cppArgsCreatePipelineCmd = CreateVariableArgument(balPipelineCmdLine, iArgQtty);

	if (pid == 0)  // child process
	{
		ExecuteCreatePipelineCommand(cppArgsCreatePipelineCmd, iArgQtty);
	}
	else if (pid > 0)  // parent process
	{
		LOG_LEVEL3(QString("Video playback: Create Video Pipeline Command executed: %1").arg(QString(JoinLaunchVideoCmd(balPipelineCmdLine, " "))));
		::waitpid(pid, &status, 0);
		if (!WIFEXITED(status) || !WEXITSTATUS(status) == 0)  // create pipeline command failed
		{
			LOG_LEVEL3(QString("Video playback: ERROR - Could not create video pipeline!").append(" status[%1] WIFEXITED(status)=[%2] WEXITSTATUS(status)=[%3]").arg(status).arg(WIFEXITED(status)).arg(WEXITSTATUS(status)));
			RestartGstDaemon();
			delete[] cppArgsCreatePipelineCmd;
			return;
		}
	}
	else  // failed to fork
	{
		LOG_LEVEL2("Video playback: ERROR - Couldn't fork to Create Pipeline");
		ProcessFailedFork();
	}

	delete[] cppArgsCreatePipelineCmd;


    // We need to be in the ready state before we go to the play state
    if (m_eCurrentVideoState != ready)
    {
        pid = fork();
        if (pid == 0)  // child process
        {
            ExecuteSetStateCommand(QString("ready"));
        }
        else if (pid > 0)  // parent process will wait for child's return status
        {
            LOG_LEVEL3(QString("Video playback: Setting state to Ready"));
            ::waitpid(pid, &status, 0);
            LOG_LEVEL4(QString("Video playback: Setting state to Ready - Done"));
            if (!WIFEXITED(status) || !WEXITSTATUS(status)==0)  // set state command failed
            {
                LOG_LEVEL3(QString("Video playback: ERROR - Could not set state to ready!").append(" status[%1] WIFEXITED(status)=[%2] WEXITSTATUS(status)=[%3]").arg(status).arg(WIFEXITED(status)).arg(WEXITSTATUS(status)));
                RestartGstDaemon();
                return;
            }
            else
            {
                m_eCurrentVideoState = ready;
            }
        }
        else   // failed to fork
        {
            LOG_LEVEL2("Video playback: ERROR - Couldn't fork to Set State Ready Process");
            ProcessFailedFork();
        }
    }


    // Next we will put the video in the play state
    pid = fork();
    if (pid == 0)  // child process
    {
        ExecuteSetStateCommand(QString("play"));
    }
    else if (pid > 0)  // parent process will wait for child's return status
    {
        LOG_LEVEL3(QString("Video playback: Setting state to Play"));
        ::waitpid(pid, &status, 0);
        LOG_LEVEL4(QString("Video playback: Setting state to Play - Done"));
        if (!WIFEXITED(status) || !WEXITSTATUS(status)==0)  // set state command failed
        {
            LOG_LEVEL3(QString("Video playback: ERROR - Could not set state to play!").append(" status[%1] WIFEXITED(status)=[%2] WEXITSTATUS(status)=[%3]").arg(status).arg(WIFEXITED(status)).arg(WEXITSTATUS(status)));
            RestartGstDaemon();
            return;
        }
        else
        {
            m_eCurrentVideoState = play;
        }
    }
    else   // failed to fork
    {
        LOG_LEVEL2("Video playback: ERROR - Couldn't fork to Set State Play Process");
        ProcessFailedFork();
    }

}

// Clears the vector then populates it with anything from the ps list matching the name
// of the GstDaemon process (excluding grep itself and any defunct processes)
void GGstDaemonMediaManager::GetGstDaemonProcessID (std::vector<std::string>& vProcessID) const
{
	enum { eMaxBufferSize = 2048 };
	char cBuffer[eMaxBufferSize];

	vProcessID.clear();
	// execute command line (ignore the grep command itself, and any defunct processes)
	const std::string sCommandLine("ps ax | grep " + m_strVideoDaemonProcessName.toLocal8Bit() + " | grep -v -e \"grep\" -e \"defunct\" | awk '{ print $1 }'");
	FILE* pIDStream( popen(sCommandLine.c_str(), "r") );

	while ( fgets(cBuffer, eMaxBufferSize, pIDStream) )
		vProcessID.push_back(cBuffer);

	pclose(pIDStream);

}


// In case we detect an error with video, we can restart the GStreamer Daemon
void GGstDaemonMediaManager::RestartGstDaemon()
{
    LOG_LEVEL4(QString("Video playback: Restart Gst Daemon"));

    // There might be a daemon running we need to kill first
    GGstDaemonMediaManager::StopGstDaemon();
    // Now start the daemon
    GGstDaemonMediaManager::StartGstDaemon();

}



void GGstDaemonMediaManager::StartGstDaemon()
{
    pid_t pid;

    LOG_LEVEL3(QString("Video playback: Start Gst Daemon"));

    pid = fork();
    if (pid == 0)  // child process
    {
        execle(m_strVideoDaemonCommand.toLocal8Bit(), m_strVideoDaemonProcessName.toLocal8Bit(), "--signals=1000", "--system", NULL, this->m_env);
        _exit(EXIT_FAILURE);
    }
    else if (pid > 0)  // parent process will wait for child's return status
    {
        LOG_LEVEL4(QString("Video playback: Starting Gst Daemon"));
        m_iDaemonProcessID = pid;
        // Give the daemon a second to start up before we try to send it commands
        sleep(1);
        //::wait(&status);  we won't wait as we're starting a Daemon and it's not going to return :)
        LOG_LEVEL4(QString("Video playback: Started Gst Daemon"));
    }
    else  // failed to fork
    {
        LOG_LEVEL2(QString("Video playback: ERROR - Failed to fork (start daemon)"));
        ProcessFailedFork();
    }

}



void GGstDaemonMediaManager::StopGstDaemon()
{
    int iRetError;
    std::vector<std::string>vGstdProcessID;

    LOG_LEVEL3(QString("Video playback: Stop Gst Daemon"));

    if (m_bSystemRestartNeeded)
    {
    	// We already know we need to restart the system
    	LOG_LEVEL3(QString("Video playback: System Restart already flagged. Exit StopGstDaemon()"));
    	return;
    }

    // There might not be a daemon running
    if (!m_iDaemonProcessID)
    {
        // There might be one (or more, if something went very wrong)
        // running from a previous GGstDaemonMediaManager (like before a DM restart),
        // so we will check to see if there is one running on the system
        GetGstDaemonProcessID(vGstdProcessID);

        // if we found a PID, convert it from a string to an int and set our member variable
        if (vGstdProcessID.size() > 0)
        {
            m_iDaemonProcessID = atoi(vGstdProcessID.at(0).c_str());
            LOG_LEVEL2(QString("Video playback: Process ID for gstd found [%1]").arg(m_iDaemonProcessID));
        }
        else
        {
            LOG_LEVEL2(QString("Video playback: No Gst Daemon running.  Returning."));
            return;
        }
    }

    iRetError = KillGstDaemon(SIGTERM);

    if (iRetError == false)
    {
    	// Let's make sure it really failed and wasn't a false failure from the process already dying
        GetGstDaemonProcessID(vGstdProcessID);
        if (vGstdProcessID.size() > 0)
        {
        	LOG_LEVEL2(QString("Video playback: Sending SIGKILL to Daemon Application Process Id [%1].").arg(m_iDaemonProcessID));
        	iRetError = KillGstDaemon(SIGKILL);
        	if (iRetError == false)
        	{
                LOG_LEVEL2(QString("Video playback: Result: Error in KILL signal sending."));
                LOG_LEVEL3(QString("Video playback: Marking system for reboot during next idle period with /tmp/systemrestart!!! *AMS*"));
                system("touch /tmp/systemrestart");
                m_bSystemRestartNeeded = true;
                return;
        	}
        }
    }

    // Call StopGstDaemon() recursively until all instances are gone.
    // There should never be more than one, but we'll be cautious.
    // clear the vector and check again
    // If we already know we need to restart because we can't kill gstd,
    // then don't try again.
    GetGstDaemonProcessID(vGstdProcessID);
    if ( (vGstdProcessID.size() != 0) && !m_bSystemRestartNeeded )
    {
    	m_iDaemonProcessID = atoi(vGstdProcessID.at(0).c_str());
    	LOG_LEVEL2(QString("Video playback: Process ID for gstd found [%1]").arg(m_iDaemonProcessID));
    	StopGstDaemon();
    }


    m_iDaemonProcessID = 0;
    m_eCurrentVideoState = null;
    m_bIsDisplayHidden = FALSE;

}

bool GGstDaemonMediaManager::KillGstDaemon(int signal)
{
	int iRetError, iRetWait, iStatus;
    std::vector<std::string>vGstdProcessID;

    LOG_LEVEL2(QString("Video playback: Killing gstd process [%1] with signal %2").arg(m_iDaemonProcessID).arg(signal));
    iRetError = kill(m_iDaemonProcessID, signal);

    if (iRetError == -1)
    {
        // gstd may have died externally
        LOG_LEVEL2(QString("Video playback: Result: Error sending signal. Results: errno: %1 errstr: %2").arg(errno).arg(strerror(errno)));
        return false;
    }
    else
    {
        LOG_LEVEL4("Video playback: Result: No errors in sending signal");
        int iWaitTime = m_iVideoStopEndSleep;

        LOG_LEVEL4(QString("Video playback: Waiting up to %1 seconds for Daemon Application Process Id [%2] to exit.").arg(iWaitTime).arg(m_iDaemonProcessID));
        while (iWaitTime > 0)
        {
            iRetWait = waitpid(m_iDaemonProcessID, &iStatus, WNOHANG);
            if (iRetWait == m_iDaemonProcessID)
            {
                // Video playback process terminated.
                LOG_LEVEL4(QString("Video playback: Daemon Application Process Id [%1] exited.").arg(m_iDaemonProcessID));
                return true;
            }
            else if (iRetWait == 0)
            {
                // Allow time for process to exit.
                LOG_LEVEL4(QString("Video playback: Waiting for Daemon Application Process Id [%1] to exit (%2).").arg(m_iDaemonProcessID).arg(iWaitTime));
                sleep(1);
                iWaitTime--;
            }
            else if (iRetWait == -1)
            {
                // Process doesn't exist. Already terminated.
                // (or in our case, the gstd process we just killed was not our child,
                //  so we get into this block. Let's check to make sure it's gone.)
                LOG_LEVEL2(QString("Video playback: waitpid() returned. Results: errno: %1 errstr: %2").arg(errno).arg(strerror(errno)));
                
                // check again
                sleep(1);
                GetGstDaemonProcessID(vGstdProcessID);

                if (vGstdProcessID.size() == 0)
                {
                    return true;
                }
                else
                {
                    LOG_LEVEL3(QString("Video playback: waitpid=[-1] GSTD Not our child process?"));
                    return false;
                }
            }
        }
    }
    return false;

}

void GGstDaemonMediaManager::EndVideoExecution()
{

    int status;
    pid_t pid;

    LOG_LEVEL3(QString("Video playback: End Video Execution"));

        // We set the state to NULL to free the video and audio devices.  Otherwise, when
        //  we play a static promo with audio, alsa sink will not be available and
        //  phonon will try to set up a pipeline using fakesink [and we will get no audio].
        pid = fork();
        if (pid == 0)  // child process
        {
            ExecuteSetStateCommand(QString("null"));
        }
        else if (pid > 0)  // parent process will wait for child's return status
        {
            LOG_LEVEL4(QString("Video playback: Setting state to Null"));
            ::waitpid(pid, &status, 0);
            LOG_LEVEL4(QString("Video playback: Setting state to Null - Done"));
            if (!WIFEXITED(status) || !WEXITSTATUS(status) == 0)  // set state command failed
            {
                LOG_LEVEL2(QString("Video playback: ERROR - Could not set state to null!").append(" status[%1] WIFEXITED(status)=[%2] WEXITSTATUS(status)=[%3]").arg(status).arg(WIFEXITED(status)).arg(WEXITSTATUS(status)));
                RestartGstDaemon();
            }
            m_eCurrentVideoState = null;
        }
        else  // failed to fork
        {
            LOG_LEVEL2(QString("Video playback: ERROR - Failed to fork to End Video Execution"));
            ProcessFailedFork();
        }


    LOG_LEVEL3(QString("Video playback: Destroy Pipeline"));

    // Destroy the pipeline so it will be recreated the next time a video is started
    pid = fork();
    if (pid == 0)  // child process
    {
        ExecuteSetStateCommand(QString("destroy"));
    }
    else if (pid > 0)  // parent process will wait for child's return status
    {
        LOG_LEVEL3(QString("Video playback: Destroy video pipeline"));
        ::waitpid(pid, &status, 0);
        LOG_LEVEL4(QString("Video playback: Destroy video pipeline - Done"));
        if (!WIFEXITED(status) || !WEXITSTATUS(status) == 0)  // set state command failed
        {
            LOG_LEVEL3(QString("Video playback: ERROR - Could not destroy video pipeline!").append(" status[%1] WIFEXITED(status)=[%2] WEXITSTATUS(status)=[%3]").arg(status).arg(WIFEXITED(status)).arg(WEXITSTATUS(status)));
            RestartGstDaemon();
        }
        m_eCurrentVideoState = null;
    }
    else  // failed to fork
    {
        LOG_LEVEL2(QString("Video playback: ERROR - Failed to fork to Destroy Pipeline"));
        ProcessFailedFork();
    }
}



void GGstDaemonMediaManager::SlotStopMedia()
{
	LOG_LEVEL4("SlotStopMedia()");

	if (m_eCurrentVideoState == play)
	{
		EndVideoExecution();
	}
}


void GGstDaemonMediaManager::SlotSynchroStopMedia()
{
	LOG_LEVEL4("SlotSynchroStopMedia()");

	if (m_eCurrentVideoState == play)
	{
		EndVideoExecution();
		GGstDaemonMediaManager::_cndSynchConditionalVideoTermination.wakeAll();

	}
}


void GGstDaemonMediaManager::StopChildProcesses()
{
	LOG_LEVEL3("StopChildProcesses()");

	// Stop any child processes that the DM has started below
	StopGstDaemon();
}
