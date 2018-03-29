/* 
 * File:   GAbsMediaManager.cpp
 * Author: sorgius
 * 
 * Created on March 25, 2011, 11:41 AM
 */

#include "GAbsMediaManager.h"
#include <fstream>




GAbsMediaManager::GAbsMediaManager():
m_bSystemRestartNeeded(FALSE),
m_strVideoMuteCommand(""),
m_strVideoFilePath(""),
m_strVideoMaxBufs(""),
m_strVideoLateness(""),
m_strVideoGstPipeline(""),
m_strVideoVideoDecoderGstElement(""),
m_strVideoAudioDecoderGstElement(""),
m_strVideoPlayCommandFilePath(""),
m_strCurrentVideoFilePath(""),
m_iVideoRotation(0),
m_iVideoStopEndSleep(0)
{
}

GAbsMediaManager::~GAbsMediaManager() {
}


void GAbsMediaManager::Initialize(char **env)
{

    m_env = env;

    m_strVideoFilePath = GETCONF_STR(eDisplayManagerPathForVideoFiles);
    LOG_LEVEL4(QString("Video playback: Video File Path Property is: %1").arg(m_strVideoFilePath));

    m_strVideoMaxBufs = GETCONF_STR(eDisplayManagerVideoMaxBufs);
    LOG_LEVEL4(QString("Video playback: Max Buffers Property is: %1").arg(m_strVideoMaxBufs));

    m_strVideoLateness = GETCONF_STR(eDisplayManagerVideoLateness);
    LOG_LEVEL4(QString("Video playback: Lateness Property is: %1").arg(m_strVideoLateness));

    m_strVideoGstPipeline = GETCONF_STR(eDisplayManagerVideoGstPipeline);
    LOG_LEVEL4(QString("Video playback: GStreamer Pipeline Property is: %1").arg(m_strVideoGstPipeline));

    m_strVideoVideoDecoderGstElement = GETCONF_STR(eDisplayManagerVideoVideoDecoderGstElement);
    LOG_LEVEL4(QString("Video playback: Video Decoder GStreamer Element Property is: %1").arg(m_strVideoVideoDecoderGstElement));

    m_strVideoAudioDecoderGstElement = GETCONF_STR(eDisplayManagerVideoAudioDecoderGstElement);
    LOG_LEVEL4(QString("Video playback: Audio Decoder GStreamer Element Property is: %1").arg(m_strVideoAudioDecoderGstElement));

    m_iVideoRotation = GETCONF_NUM(eDisplayManagerVideoRotation);
    LOG_LEVEL4(QString("Video playback: Rotation Property is: %1").arg(m_iVideoRotation));

    m_strVideoMuteCommand = GETCONF_STR(eDisplayManagerVideoMuteCommand);
    LOG_LEVEL4(QString("Video playback: Mute Command Property is: %1").arg(m_strVideoMuteCommand));

    m_iVideoStopEndSleep = GETCONF_NUM(eDisplayManagerVideoStopEndSleep);
    LOG_LEVEL4(QString("Video playback: Time between Stop and End of Video Process: %1").arg(m_iVideoStopEndSleep));

    m_strVideoPlayCommandFilePath = GETCONF_STR(eDisplayManagerVideoPlayCommandPathForVideoFile);
    LOG_LEVEL4(QString("Video playback: Video Play Command video file path Property is: %1").arg(m_strVideoPlayCommandFilePath));

}


void GAbsMediaManager::run()
{
    LOG_LEVEL3("Connecting Signals...");

    if (!connect(this, SIGNAL(SignalStartVideo(const int, const int, const int, const int, const QString &)), SLOT(SlotStartVideo(const int, const int, const int, const int, const QString &)))) {
        LOG_LEVEL1("Can't connect Start Video Signal.");
    }

    if (!connect(this, SIGNAL(SignalStopMedia()), SLOT(SlotStopMedia()))) {
        LOG_LEVEL1("Can't connect Stop Media Signal.");
    }

    if (!connect(this, SIGNAL(SignalSynchroStopMedia()), SLOT(SlotSynchroStopMedia()))) {
        LOG_LEVEL1("Can't connect Synchro Stop Media Signal.");
    }
    if (!connect(this, SIGNAL(SignalMuteVideo()), SLOT(SlotMuteVideo()))) {
        LOG_LEVEL1("Can't connect Mute Media Signal.");
    }

    if (!connect(this, SIGNAL(SignalUnMuteVideo()), SLOT(SlotUnMuteVideo()))) {
        LOG_LEVEL1("Can't connect UnMute Media Signal.");
    }

    exec();

    LOG_LEVEL4("The event loop of this thread has been finished");

}


void GAbsMediaManager::Stop()
{
    LOG_LEVEL3("Disconnecting Signals...");

    if (!disconnect(this, SIGNAL(SignalStartVideo(const int, const int, const int, const int, const QString &)), this, SLOT(SlotStartVideo(const int, const int, const int, const int, const QString &)))) {
        LOG_LEVEL1("Can't disconnect Start Video Signal.");
    }

    if (!disconnect(this, SIGNAL(SignalStopMedia()), this, SLOT(SlotStopMedia()))) {
        LOG_LEVEL1("Can't disconnect Stop Media Signal.");
    }

    if (!disconnect(this, SIGNAL(SignalSynchroStopMedia()), this, SLOT(SlotSynchroStopMedia()))) {
        LOG_LEVEL1("Can't connect Synchro Stop Media Signal.");
    }
    if (!disconnect(this, SIGNAL(SignalMuteVideo()), this, SLOT(SlotMuteVideo()))) {
        LOG_LEVEL1("Can't disconnect Mute Media Signal.");
    }

    if (!disconnect(this, SIGNAL(SignalUnMuteVideo()), this, SLOT(SlotUnMuteVideo()))) {
        LOG_LEVEL1("Can't disconnect UnMute Media Signal.");
    }

    quit();

}



void GAbsMediaManager::MuteVideo()
{
	LOG_LEVEL3("MuteVideo()");

	emit SignalMuteVideo();

}


void GAbsMediaManager::UnMuteVideo()
{
	LOG_LEVEL3("UnMuteVideo()");

	emit SignalUnMuteVideo();

}


void GAbsMediaManager::ExecuteMuteUnMuteVideoCommand(const QString &strMuteParameter)
{
	int iRetError = execle(m_strVideoMuteCommand.toStdString().c_str(),m_strVideoMuteCommand.toStdString().c_str(),
		   "Master",
			strMuteParameter.toStdString().c_str(),(char *) NULL, this->m_env);

	if ( iRetError == -1 )
	{
		printf("Video playback: Mute/UnMute Video external application execution: errno: %d errstr: %s \n",errno,strerror(errno));
		_exit(EXIT_FAILURE);
	}
	else
	{
		printf("Video playback: Mute/UnMute Video external application execution: no errors\n");
		_exit(EXIT_SUCCESS);
	}
}


void GAbsMediaManager::VideoMuteUnMute(const QString &strMuteParameter)
{
	int status;
	pid_t pid;

	LOG_LEVEL3("VideoMuteUnMute()");
	LOG_LEVEL4(QString("Video playback: Doing fork for Video Mute Application. Mute Parameter: %1").arg(strMuteParameter));

	pid	= fork();

	if( pid == 0 )  // child process
	{
		ExecuteMuteUnMuteVideoCommand(strMuteParameter);
	}
	else if ( pid > 0 )  // parent process will wait for child's return status
	{
		LOG_LEVEL4(QString("Video playback: MuteUnMute Video Process created"));
		::waitpid(pid, &status, 0);
		LOG_LEVEL4(QString("Video playback: MuteUnMute Video Process - Done"));
		if (!WIFEXITED(status) || !WEXITSTATUS(status) == 0)  // mute/unmute command failed
		{
			LOG_LEVEL3(QString("Video playback: ERROR - Could not mute/unmute!").append(" status[%1] WIFEXITED(status)=[%2] WEXITSTATUS(status)=[%3]").arg(status).arg(WIFEXITED(status)).arg(WEXITSTATUS(status)));
		}
	}
	else  // failed to fork
	{
		LOG_LEVEL2("Video playback: Couldn't create MuteUnMute Video Process");
		ProcessFailedFork();
	}

}


void GAbsMediaManager::UnBlockAllSignals()
{
	sigset_t senales;
	sigemptyset(&senales);

	sigfillset(&senales);

	if( sigprocmask(SIG_UNBLOCK, &senales, 0) != 0 )
	{

		std::cout << "Video playback: Couldn't unblocking all signals\n";
		exit(1);
	}
}


void GAbsMediaManager::SlotMuteVideo()
{
	LOG_LEVEL4("SlotMuteVideo()");

	VideoMuteUnMute("0");

}


void GAbsMediaManager::SlotUnMuteVideo()
{
	LOG_LEVEL4("SlotUnMuteVideo()");

		VideoMuteUnMute("2");

}

// JoinLaunchVideoCmd takes the command that was created as a QList and the separator used
// and returns it as a QByteArray so that the logging function can log it
QByteArray GAbsMediaManager::JoinLaunchVideoCmd(const QList<QByteArray> &balVideoCmdLine, const QByteArray &baSep) const
{
    QByteArray baJoinResult;
    int i;

    for (i = 0; i < balVideoCmdLine.count(); i++) {
        baJoinResult += balVideoCmdLine[i] + baSep;
    }

    return baJoinResult;
}

char **GAbsMediaManager::CreateVariableArgument(QList<QByteArray> &balVideoCmdLine, int iArgQtty) const
{
    //Passing QList<QByteArray> to char ** for execve command
    int i;

    char **cppArgsLaunchVideoCmd = new char*[iArgQtty + 1]; //one more to save the last as NULL (execv requirement)

    for (i = 0; i < iArgQtty; ++i) {
        cppArgsLaunchVideoCmd[i] = balVideoCmdLine[i].data();
    }

    cppArgsLaunchVideoCmd[i] = NULL;

    return cppArgsLaunchVideoCmd;
}

// If enabled through diagnostics, we will mark the need for a system restart
// because a fork call has failed (Therefore we are either out of processes or
// out of memory and need a restart to clean the system up)
void GAbsMediaManager::ProcessFailedFork()
{
	if (m_bSystemRestartNeeded)
	{
		// We already know we need to restart the system
		LOG_LEVEL3(QString("Video playback: System Restart already flagged. Exit ProcessFailedFork()"));
		return;
	}

	const char *CEnabled = "Enabled";

	// Read from the config file to see if Fork Failed Restart is enabled
    QString forkFailedRestart = GETCONF_STR_FROM_FILESYSTEM(eDisplayManagerMediaManagerForkFailedRestart);

    if (forkFailedRestart == QString(CEnabled))
	{
		LOG_LEVEL3(QString("Video playback: Marking system for reboot during next idle period with /tmp/systemrestart!!!"));
		std::string file = "/tmp/systemrestart";
		std::ofstream(file.c_str(), std::ios::app) ;

		m_bSystemRestartNeeded = true;
	}
}
