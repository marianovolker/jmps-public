#include "GAbsPageLoader.h"

#include <QtWebKit/QWebPage>
#include <QtWebKit/QWebFrame>

#include "view/src/utils/loadersmanager/GAbsPageLoaderManager.h"

#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"

GAbsPageLoader::GAbsPageLoader(unsigned int uiId, GAbsPageLoaderManager *pObserver)
	: m_pObserver(pObserver),
	  m_iId(uiId),
	  m_bWaitingForLoadingPage(false),
	  m_bPageActive(false),
	  m_pPage(0),
	  m_oTimer(),
	  m_jsHelper(),
	  m_jsSignal(),
	  m_pRequest(0),
	  m_uiCurrentSessionId(0),
	  m_uiLastRequestIdProcessed(0),
	  m_oHTTPSHelper()
{
	LOG_LEVEL4(QString("GAbsPageLoader(") + QString::number(m_iId) + ").");
}

GAbsPageLoader::~GAbsPageLoader()
{
	LOG_LEVEL4(QString("~GAbsPageLoader(") + QString::number(m_iId) + ").");
}

void GAbsPageLoader::Init()
{
	LOG_LEVEL4(QString("Init(") + QString::number(m_iId) + ").");

	InitializePage();
	ConnectSignals();
	m_oHTTPSHelper.Init(m_pPage);
}

void GAbsPageLoader::Stop()
{
	LOG_LEVEL4(QString("Stop(") + QString::number(m_iId) + ").");

	DisconnectSignals();

	m_oHTTPSHelper.Stop();

	delete m_pPage;

	m_pPage = 0;
}

void GAbsPageLoader::ConnectSignals()
{
	LOG_LEVEL4(QString("ConnectSignals(") + QString::number(m_iId) + ").");

	if( GETCONF_BOOL(eDisplayManagerEnableLoggingOfLoadingPages) )
	{
		if( !connect( m_pPage, SIGNAL(loadStarted()), this, SLOT(LoadStarted()) ) )
		{
			LOG_LEVEL1("Can't connect LoadStarted() to QWebPage one object.");
		}

		if( !connect( m_pPage, SIGNAL(loadProgress(int)), this, SLOT(LoadProgress(int)) ) )
		{
			LOG_LEVEL1("Can't connect LoadProgress() to QWebPage one object.");
		}
	}

	if( !connect( m_pPage, SIGNAL(loadFinished(bool)), this, SLOT(LoadFinished(bool)) ) )
	{
		LOG_LEVEL1("Can't connect LoadFinished() to QWebPage one object.");
	}

	if( !connect( m_pPage->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(PopulateJavaScriptWindowObject()) ) )
	{
		LOG_LEVEL1("Can't connect javaScriptWindowObjectCleared() to QWebPage one object.");
	}

	if( !connect( &m_jsSignal, SIGNAL(Emited()), this, SLOT(Completed()) ) )
	{
		LOG_LEVEL1("Can't connect GJavaScriptSignal::Emit() to SwitchPage.");
	}

	if( !connect( &m_oTimer, SIGNAL(timeout()), this, SLOT(TimeoutLoadingPage()) ) )
	{
		LOG_LEVEL1("Can't connect QTimer::timeout() to TimeoutLoadingPage().");
	}
}

void GAbsPageLoader::DisconnectSignals()
{
	LOG_LEVEL4(QString("DisconnectSignals(") + QString::number(m_iId) + ").");

	m_pPage->triggerAction(QWebPage::Stop);

	if( GETCONF_BOOL(eDisplayManagerEnableLoggingOfLoadingPages) )
	{
		if( !disconnect( m_pPage, SIGNAL(loadStarted()), this, SLOT(LoadStarted()) ) )
		{
			LOG_LEVEL1("Can't connect LoadStarted() to QWebPage object.");
		}

		if( !disconnect( m_pPage, SIGNAL(loadProgress(int)), this, SLOT(LoadProgress(int)) ) )
		{
			LOG_LEVEL1("Can't connect LoadProgress() to QWebPage object.");
		}
	}

	if( !disconnect( m_pPage, SIGNAL(loadFinished(bool)), this, SLOT(LoadFinished(bool)) ) )
	{
		LOG_LEVEL1("Can't connect LoadFinished() to QWebPage object.");
	}

	if( !disconnect( m_pPage->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(PopulateJavaScriptWindowObject()) ) )
	{
		LOG_LEVEL1("Can't connect javaScriptWindowObjectCleared() to QWebPage object.");
	}

	if( !disconnect( &m_jsSignal, SIGNAL(Emited()), this, SLOT(Completed()) ) )
	{
		LOG_LEVEL1("Can't connect GJavaScriptSignal::Emit() to SwitchPage.");
	}

	if( !disconnect( &m_oTimer, SIGNAL(timeout()), this, SLOT(TimeoutLoadingPage()) ) )
	{
		LOG_LEVEL1("Can't connect QTimer::timeout() to TimeoutLoadingPage().");
	}
}

void GAbsPageLoader::InitializePage()
{
	LOG_LEVEL4(QString("InitializePage(") + QString::number(m_iId) + ").");

	m_pPage = new QWebPage();

	m_pPage->mainFrame()->setScrollBarPolicy( Qt::Horizontal, Qt::ScrollBarAlwaysOff );
	m_pPage->mainFrame()->setScrollBarPolicy( Qt::Vertical, Qt::ScrollBarAlwaysOff   );
	m_pPage->mainFrame()->setZoomFactor( 1.0 );

	if( !GETCONF_BOOL(eDisplayManagerEnableAdvancedCache) )
	{
		m_pPage->settings()->setMaximumPagesInCache(GETCONF_NUM(eDisplayManagerCacheSizeInPagesForTheView));
		LOG_LEVEL3(QString("Setting standard cache to (")+GETCONF_NUM(eDisplayManagerCacheSizeInPagesForTheView)+") pages.");
	}
}

void GAbsPageLoader::LoadStarted()
{
	LOG_LEVEL4(QString("LoadStarted(") + QString::number(m_iId) + ").");
}

void GAbsPageLoader::LoadProgress(int iProgress)
{
	LOG_LEVEL4(QString("LoadProgress(") + QString::number(m_iId) + ")." + QString().sprintf(" [%03d", iProgress) + "%].");
}

void GAbsPageLoader::LoadFinished(bool bIsLoaded)
{
	LOG_LEVEL4( QString("LoadFinished(%1).bIsLoaded(%2)").arg(QString::number(m_iId)).arg(bIsLoaded?("true"):("false")) );

	if( bIsLoaded )
	{
		m_jsHelper.LoadJSLibraries();

		// We need to inject some code in JavaScript here to resolve the fully load of the page.
		m_jsHelper.GetJSCalls().push_back(m_jsSignal.GetDynamicOnLoadCheckCode());
		m_jsHelper.GetJSCalls().push_back(m_jsSignal.GetDynamicOnLoadTriggerCode());

		m_jsHelper.LoadJSCalls();

		m_oTimer.start(CTIMEOUT_EXECUTING_JS);
	}
	else
	{
		// Start a timer to eight seconds if there are problems to load the page.
		m_oTimer.start(CTIMEOUT_LOADING_PAGE);
	}
}

void GAbsPageLoader::Completed()
{
	LOG_LEVEL4(QString("Completed(") + QString::number(m_iId) + ").");

	m_oTimer.stop();

	if( m_pRequest )
	{
		DoCompleted();
		Notify(GLoaderRequestNotify::eContentLoadComplete);
	}
}

void GAbsPageLoader::TimeoutLoadingPage()
{
	LOG_LEVEL4(QString("TimeoutLoadingPage(") + QString::number(m_iId) + ").");

	m_oTimer.stop();

	if( m_pRequest )
	{
		DoTimeout();
		Notify(GLoaderRequestNotify::eContentLoadTimeout);
	}
}

void GAbsPageLoader::PopulateJavaScriptWindowObject()
{
	LOG_LEVEL4(QString("PopulateJavaScriptWindowObject(") + QString::number(m_iId) + ").");

	m_jsHelper.GetMainFrame()->addToJavaScriptWindowObject("JSPageLoadedSignal", &m_jsSignal);
}

void GAbsPageLoader::Notify(GLoaderRequestNotify::eNotifyTypeEnum eType)
{
	LOG_LEVEL4(QString("Notify(") + QString::number(m_iId) + ").");

	GLoaderRequestNotify oNotification(m_iId, m_pRequest->GetSessionId(), eType);

	m_uiCurrentSessionId = m_pRequest->GetSessionId();

	m_uiLastRequestIdProcessed = m_pRequest->GetRequestId();

	oNotification.SetRequest(m_pRequest);

	m_pRequest = 0;

	m_pObserver->Notify(oNotification);
}

void GAbsPageLoader::SetAsActive()
{
	LOG_LEVEL4(QString("SetAsActive(") + QString::number(m_iId) + ").");
	m_bPageActive = true;
}

void GAbsPageLoader::SetAsNonActive()
{
	LOG_LEVEL4(QString("SetAsNonActive(") + QString::number(m_iId) + ").");
	m_bPageActive = false;
}
