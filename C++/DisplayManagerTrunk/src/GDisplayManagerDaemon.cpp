#include "GDisplayManagerDaemon.h"

#include <QWSServer>
#include <QMutexLocker>
#include <iostream>
#include <sys/socket.h>
#include <signal.h>

#include "utils/base/src/GException.h"
#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"
#include "utils/image/src/GImageConverter.h"
#include "view/src/utils/requestsmanager/GPageLoadRequestManager.h"
#include "utils/image/src/GImageHelper.h"
#include "utils/image/src/GImageMappingsImporter.h"
#include "utils/image/src/GImageMappings.h"
#include "utils/media/src/GAbsMediaManager.h"
#include "utils/media/src/GMediaManagerFactory.h"
#include "view/src/utils/GPersonalityView.h"
#include "GControlChannelMgr.h"
#include "config/GDisplayManagerConfig.h"
#include "GSignalHandler.h"
#include "model/src/infoscreenmodel/GISModelSingleton.h"


GDisplayManagerDaemon::GDisplayManagerDaemon(QApplication* pApp, char **env)
	:	QObject(0),
		m_pApp(pApp),
		m_env(env)
{
}

GDisplayManagerDaemon::~GDisplayManagerDaemon()
{
}

void GDisplayManagerDaemon::Init()
{
	QTInit();
	LoggerInit();
	ShowEnvironment();
	ShowCurrentConfiguration();
	PageLoadRequestManagerInit();
	ImageConverterInit();
	ImageMappingsInit();
	PersonalityViewInit();
	VideoManagerInit();
        // Moving getting the model and parsing xml file to startup of DM for NGP-110 "white screen" issue
        //  Sometimes parsing the xml file took more than the 10 second timeout the CRIND uses for commands,
        //  and would cause the screen to be white instead of displaying the last screen after a restart
        //  by the GRestartManager
        GISModelSingleton::GetInstance()->GetModel();
	ControlChannelMgrInit();
	SignalHandlerInit();
}

void GDisplayManagerDaemon::QTInit()
{
	m_pApp->setApplicationName("Gilbarco Display Manager");

	QWSServer::setCursorVisible(false);
}

void GDisplayManagerDaemon::LoggerInit()
{
	// Configuring the logging of the implementation.
	GLogConfig::eLevel eLogLevel(static_cast<GLogConfig::eLevel>(GETCONF_NUM(eDisplayManagerLogLevel)));
	QString strLogFileName(GETCONF_STR(eDisplayManagerLogFileName));
	GLogConfig * pConfig(new GLogConfig(GLogConfig::eFile, eLogLevel, strLogFileName));
	GLogger * pLogger(new GLogger(pConfig));
	GLogManager::AddLogger(GETCONF_STR(eDisplayManagerLogFileName), pLogger, true);

	LOG_LEVEL3("Starting Up Application.");
}

void GDisplayManagerDaemon::ShowEnvironment()
{
	LOG_LEVEL4("ShowEnvironment()");

		// Show the environment of the process.
	if( GETCONF_BOOL(eDisplayManagerShowEnvironment) )
	{
		QString strTemp;
		for( unsigned int n = 0 ; (m_env[n] != NULL); n++ )
		{
			strTemp += QString().sprintf("%s; ", m_env[n]);
		}

		LOG_LEVEL3( "Process Environment:" );
		LOG_LEVEL3( "----------------------------------------------------------------------" );
		LOG_LEVEL3( strTemp );
		LOG_LEVEL3( "----------------------------------------------------------------------" );
	}
}

void GDisplayManagerDaemon::ShowCurrentConfiguration()
{
	LOG_LEVEL4("ShowCurrentConfiguration()");

	// Show the configuration in the log file.
	SHOW_CURRENT_CONFIGURATION();
}

void GDisplayManagerDaemon::PageLoadRequestManagerInit()
{
	LOG_LEVEL4("PageLoadRequestManagerInit()");

	// GPageLoadRequestManager Object needs to be in the main thread for QT requirements.
	QSharedPointer<GPageLoadRequestManager> pGPageLoadRequestManager = GPageLoadRequestManager::GetInstance();
	if( pGPageLoadRequestManager.isNull() )
	{
		throw(GException("main() obtained a GPageLoadRequestManager NULL pointer."));
	}
}

void GDisplayManagerDaemon::ImageConverterInit()
{
	LOG_LEVEL4("ImageConverterInit()");

	// GPageLoadRequestManager Object needs to be in the main thread for QT requirements.
	QSharedPointer<GPageLoadRequestManager> pGPageLoadRequestManager = GPageLoadRequestManager::GetInstance();

	// ImageConverter Object needs to be in the main thread for QT requirements.
	QSharedPointer<GImageConverter> pImageConverter = GImageConverter::GetInstance();
	pImageConverter->Initialize(pGPageLoadRequestManager->GetScaleXFactor(), pGPageLoadRequestManager->GetScaleYFactor());
	pImageConverter->start();
	pImageConverter->ConvertImagesIfNotConverted(GImageHelper::RawImagesPath(), GImageHelper::CurrentResolutionMonochromeImagesPath());
}

void GDisplayManagerDaemon::ImageMappingsInit()
{
	LOG_LEVEL4("ImageMappingsInit()");

	// ImageMappings Object needs to be in the main thread for QT requirements.
	QSharedPointer<GImageMappings> poImageMappings = GImageMappings::GetInstance();
    QMutexLocker mtxImageMappingsScope(poImageMappings->GetMutex());

	if( QFile::exists(GImageHelper::ImagesMappingsFile()) )
	{
		poImageMappings = GImageMappingsImporter::Import(GImageHelper::ImagesMappingsFile(), poImageMappings);
	}
}



void GDisplayManagerDaemon::VideoManagerInit()
{
	LOG_LEVEL4("VideoManagerInit()");

        // Use factory to create the correct instance of the MediaManager
        GMediaManagerFactory mediaManagerFactory;
	QSharedPointer<GAbsMediaManager> pMediaManager = mediaManagerFactory.CreateMediaManager();
	pMediaManager->Initialize(m_env);
	pMediaManager->start();

}


void GDisplayManagerDaemon::PersonalityViewInit()
{
	LOG_LEVEL4("PersonalityViewInit()");

	QSharedPointer<GPersonalityView> pGPersonalityView = GPersonalityView::GetInstance();
	if( pGPersonalityView.isNull() )
	{
		throw(GException("main() obtained a GPersonalityView NULL pointer."));
	}

	pGPersonalityView->RefreshPersonality();
}

void GDisplayManagerDaemon::ControlChannelMgrInit()
{
	LOG_LEVEL4("ControlChannelMgrInit()");

	// Starting the main loop.
	int iTcpPort(GETCONF_NUM(eDisplayManagerTcpPort));

	m_pControlChMgr = QSharedPointer<GControlChannelMgr>(new GControlChannelMgr(iTcpPort));
	m_pControlChMgr->start();
}

void GDisplayManagerDaemon::SignalHandlerInit()
{
	LOG_LEVEL4("SignalHandlerInit()");

	m_pSignalHandler = QSharedPointer<GSignalHandler>(new GSignalHandler(this));

	m_pSignalHandler->start();
}

void GDisplayManagerDaemon::Stop()
{
	LOG_LEVEL3("Stop()");

	SignalHandlerStop();
	ControlChannelMgrStop();
	ImageConverterStop();
	PageLoadRequestManagerStop();
	LoggerStop();
}

void GDisplayManagerDaemon::ImageConverterStop()
{
	LOG_LEVEL4("ImageConverterStop()");

	QSharedPointer<GImageConverter> pImageConverter = GImageConverter::GetInstance();
	pImageConverter->Stop();
	// The wait calls for these threads were causing the DM process not to exit when the
	// dispmgrd_signal_handler process received SIGTERM when the DM process was busy (NGP-833)
	//pImageConverter->wait();
}

void GDisplayManagerDaemon::PageLoadRequestManagerStop()
{
	LOG_LEVEL4("PageLoadRequestManagerStop()");

	QSharedPointer<GPageLoadRequestManager> pGPageLoadRequestManager = GPageLoadRequestManager::GetInstance();
	pGPageLoadRequestManager->Stop();
}

void GDisplayManagerDaemon::ControlChannelMgrStop()
{
	LOG_LEVEL4("ControlChannelMgrStop()");

	m_pControlChMgr->Stop();
	// The wait calls for these threads were causing the DM process not to exit when the
	// dispmgrd_signal_handler process received SIGTERM when the DM process was busy (NGP-833)
	//m_pControlChMgr->wait();
}

void GDisplayManagerDaemon::SignalHandlerStop()
{
	LOG_LEVEL4("SignalHandlerStop()");

	m_pSignalHandler->Stop();
	// The wait calls for these threads were causing the DM process not to exit when the
	// dispmgrd_signal_handler process received SIGTERM when the DM process was busy (NGP-833)
	//m_pSignalHandler->wait();
}

void GDisplayManagerDaemon::LoggerStop()
{
	LOG_LEVEL3("LoggerStop()");

	QString strLogFileName(GETCONF_STR(eDisplayManagerLogFileName));

	LOG_LEVEL3("Shutting Down Application.");

	GLogManager::DelLogger(strLogFileName);
}

int GDisplayManagerDaemon::Run()
{
	LOG_LEVEL3("Run()");

	// Starting QT main loop.
	return m_pApp->exec();
}

void GDisplayManagerDaemon::goAway()
{
	LOG_LEVEL3("goAway()");

        // Use factory to create the correct instance of the MediaManager
        GMediaManagerFactory mediaManagerFactory;
	QSharedPointer<GAbsMediaManager> pMediaManager = mediaManagerFactory.CreateMediaManager();

	// Synchronous call to stop video playback if it is playing
	pMediaManager->SynchroStopMedia();
	// Shut down any child processes that the DM may have started (like gstd)
	pMediaManager->StopChildProcesses();

	LOG_LEVEL3("instance()->quit()");

	QCoreApplication::instance()->quit();
}
