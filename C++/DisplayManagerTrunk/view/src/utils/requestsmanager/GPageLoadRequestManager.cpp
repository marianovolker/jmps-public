#include "GPageLoadRequestManager.h"

#include "utils/base/src/GException.h"
#include "view/src/base/GAbsView.h"
#include "view/src/utils/requests/GUpdatePreloadedPageRequest.h"
#include "view/src/utils/requests/GUpdateCurrentPageRequest.h"
#include "view/src/utils/requests/GUpdateURLCurrentPageRequest.h"
#include "view/src/utils/requests/GLoadEntirePageRequest.h"
#include "view/src/utils/requests/GSetPreloadedPagesRequest.h"
#include "view/src/utils/loadersmanager/GLoadersManagerRequestNotify.h"
#include "config/GDisplayManagerConfig.h"
#include "utils/logger/src/GLogger.h"

#include <QScreen>

QSharedPointer<GPageLoadRequestManager> GPageLoadRequestManager::m_pGPageLoadRequestManager;
QMutex GPageLoadRequestManager::m_mutex;

GPageLoadRequestManager::GPageLoadRequestManager()
	:	m_uiMainViewWidth(0),
		m_uiMainViewHeight(0),
		m_uiSessionIdCounter(1),
		m_uiRequestIdCounter(1),
		m_oPreSetManager(),
		m_oNonPreSetManager(),
		m_qlsReqs(),
		m_qmSessions(),
		m_pMainView(0)
{
	LOG_LEVEL4("GPageLoadRequestManager()");
}

GPageLoadRequestManager::~GPageLoadRequestManager()
{
	LOG_LEVEL4("~GPageLoadRequestManager()");
}

void GPageLoadRequestManager::Init()
{
	LOG_LEVEL4("Init()");

	InitializeMainView();

	m_oNonPreSetManager.Init(this);
	m_oNonPreSetManager.SetId(CNON_PRESET_PAGE_LOADER_MANAGER_ID);

	m_oPreSetManager.Init(this);
	m_oPreSetManager.SetId(CPRESET_PAGE_LOADER_MANAGER_ID);
}

void GPageLoadRequestManager::Stop()
{
	LOG_LEVEL4("Stop()");

	m_pMainView->setPage(0);

	m_oNonPreSetManager.Stop();

	m_oPreSetManager.Stop();

	delete m_pMainView;

	m_pMainView = 0;
}

void GPageLoadRequestManager::InitializeMainView()
{
	LOG_LEVEL4("InitializeMainView()");

	int iWidth (0);
	int iHeight(0);

	m_pMainView = new QWebView();

	if( GETCONF_BOOL(eDisplayManagerAutoAdjustmentViewSize) )
	{
		QScreen * pScreen(QScreen::instance());

		iHeight = pScreen->deviceHeight();
		iWidth  = pScreen->deviceWidth();

		QString strTemp("Window Size detected : [");
		strTemp += QString::number(iWidth)  + "x";
		strTemp += QString::number(iHeight) + "]";

		LOG_LEVEL3(strTemp);

		SETCONF(eDisplayManagerViewHeight, QString::number(iHeight));
		SETCONF(eDisplayManagerViewWidth, QString::number(iWidth));
	}
	else
	{
		iHeight = GETCONF_NUM(eDisplayManagerViewHeight);
		iWidth  = GETCONF_NUM(eDisplayManagerViewWidth);
	}

	m_pMainView->window()->resize(iWidth, iHeight);
	m_pMainView->window()->setFixedSize(iWidth, iHeight);
	m_pMainView->window()->showFullScreen();
	SetMainViewWidth(m_pMainView->width());
	SetMainViewHeight(m_pMainView->height());

	if( (m_pMainView->width() != iWidth) && (m_pMainView->height() != iHeight) )
	{
		throw GException("One of the dimensions exceded the maximun of the screen.");
	}

	LOG_LEVEL3(QString("Width(")  + QString::number(m_pMainView->width()) + ")");
	LOG_LEVEL3(QString("Height(") + QString::number(m_pMainView->height()) + ")");
}

unsigned int GPageLoadRequestManager::Subscribe(QString sName, bool bIsView)
{
	LOG_LEVEL4("Subscribe()");

	QMutexLocker oLocker(&m_qmInternalMutex);

	GPageLoadRequestManagerSession oSession( (bIsView)?(GenerateSessionId()):(0), sName, bIsView);

	m_qmSessions[oSession.GetId()] = oSession;

	LOG_LEVEL3(QString("Subscribe(") + GetSessionName(oSession.GetId()) + ").");

	return oSession.GetId();
}

void GPageLoadRequestManager::ReceiveRequest(GSetPreloadedPagesRequest* pReq)
{
	LOG_LEVEL4("ReceiveRequest(GSetPreloadedPagesRequest)");

	QMutexLocker oLocker(&m_qmInternalMutex);

	if( !m_qmSessions.contains(pReq->GetSessionId()) )
	{
		LOG_LEVEL1("Invalid session id. Discarding request.");
		delete pReq;
		return;
	}

	pReq->SetRequestId(GenerateRequestId());
	pReq->SetNotification(true);

	if( m_qlsReqs.isEmpty() )
	{
		ProcessRequest(pReq);
	}

	m_qlsReqs << pReq;

	LOG_LEVEL4(QString("Quantity of pending requests(") + QString::number(m_qlsReqs.count()) + ").");
}

void GPageLoadRequestManager::ReceiveRequest(GUpdatePreloadedPageRequest* pReq)
{
	LOG_LEVEL4("ReceiveRequest(GUpdatePreloadedPageRequest)");

	QMutexLocker oLocker(&m_qmInternalMutex);

	if( !m_qmSessions.contains(pReq->GetSessionId()) )
	{
		LOG_LEVEL1("Invalid session id. Discarding request.");
		delete pReq;
		return;
	}

	pReq->SetRequestId(GenerateRequestId());
	pReq->SetNotification(true);

	if( m_qlsReqs.isEmpty() )
	{
		ProcessRequest(pReq);
	}

	m_qlsReqs << pReq;

	LOG_LEVEL4(QString("Quantity of pending requests(") + QString::number(m_qlsReqs.count()) + ").");
}

void GPageLoadRequestManager::ReceiveRequest(GLoadEntirePageRequest* pReq)
{
	LOG_LEVEL4("ReceiveRequest(GLoadEntirePageRequest)");

	QMutexLocker oLocker(&m_qmInternalMutex);

	if( !m_qmSessions.contains(pReq->GetSessionId()) )
	{
		LOG_LEVEL1("Invalid session id. Discarding request.");
		delete pReq;
		return;
	}

	pReq->SetRequestId(GenerateRequestId());
	pReq->SetNotification(true);

	if( m_qlsReqs.isEmpty() )
	{
		ProcessRequest(pReq);
	}

	m_qlsReqs << pReq;

	LOG_LEVEL4(QString("Quantity of pending requests(") + QString::number(m_qlsReqs.count()) + ").");
}

void GPageLoadRequestManager::ReceiveRequest(GUpdateCurrentPageRequest* pReq)
{
	LOG_LEVEL4("ReceiveRequest(GUpdateCurrentPageRequest)");

	QMutexLocker oLocker(&m_qmInternalMutex);

	if( !m_qmSessions.contains(pReq->GetSessionId()) )
	{
		LOG_LEVEL1("Invalid session id. Discarding request.");
		delete pReq;
		return;
	}

	pReq->SetRequestId(GenerateRequestId());
	pReq->SetNotification(false);

	if( m_qlsReqs.isEmpty() )
	{
		ProcessRequest(pReq);
	}
	else
	{
		m_qlsReqs << pReq;

		LOG_LEVEL4(QString("Quantity of pending requests(") + QString::number(m_qlsReqs.count()) + ").");
	}
}

void GPageLoadRequestManager::ReceiveRequest(GUpdateURLCurrentPageRequest* pReq)
{
	LOG_LEVEL4("ReceiveRequest(GUpdateURLCurrentPageRequest)");

	QMutexLocker oLocker(&m_qmInternalMutex);

	if( !m_qmSessions.contains(pReq->GetSessionId()) )
	{
		LOG_LEVEL1("Invalid session id. Discarding request.");
		delete pReq;
		return;
	}

	pReq->SetRequestId(GenerateRequestId());
	pReq->SetNotification(false);

	if( m_qlsReqs.isEmpty() )
	{
		ProcessRequest(pReq);
	}
	else
	{
		m_qlsReqs << pReq;

		LOG_LEVEL4(QString("Quantity of pending requests(") + QString::number(m_qlsReqs.count()) + ").");
	}
}

void GPageLoadRequestManager::Notify(GLoadersManagerRequestNotify &oNotify) throw(GException)
{
	LOG_LEVEL4("Notify()");

	QMutexLocker oLocker(&m_qmInternalMutex);

	GAbsPageLoadRequest* pRequest(0);

	for( int i = 0; i < m_qlsReqs.size(); ++i )
	{
		if( m_qlsReqs.at(i)->GetRequestId() == oNotify.GetRequest()->GetRequestId() )
		{
			pRequest = oNotify.GetRequest();
			m_qlsReqs.removeAt(i);
			break;
		}
	}

	if( pRequest != 0 )
	{
		QString sResult("Timeout");
		if( oNotify.GetNofifyType() == GLoadersManagerRequestNotify::eContentLoadComplete )
		{
			m_pMainView->setPage(oNotify.GetWebPage());
			sResult = "Completed";

			SetAsActive(oNotify.GetLoadersManagerId());
		}

		static QString const strTemp("RequestId(%1), has been completed with result (%2) for (%3).");
		LOG_LEVEL4(QString(strTemp) . arg(pRequest->GetRequestId()) . arg(sResult) . arg(GetSessionName(pRequest->GetSessionId())));

		if( pRequest->IsSynchronize() )
		{
			GAbsView::SignalSynchronize( (sResult=="Completed")?(true):(false) );
		}

		delete pRequest;

		ProcessNewRequest();

		return;
	}

	throw(GException("Cannot found the request in the internal queue."));
}

void GPageLoadRequestManager::ProcessNewRequest()
{
	LOG_LEVEL4("ProcessNewRequest()");

	LOG_LEVEL4(QString("Quantity of pending requests(") + QString::number(m_qlsReqs.count()) + ").");

	while( !m_qlsReqs.isEmpty() )
	{
		if( m_qlsReqs.first()->GetTypeOfRequest() == GAbsPageLoadRequest::eUpdateCurrentPageRequest )
		{
			GUpdateCurrentPageRequest* pRequest(static_cast<GUpdateCurrentPageRequest*>(m_qlsReqs.first()));
			ProcessRequest(pRequest);

			m_qlsReqs.removeFirst();
		}
		else if( m_qlsReqs.first()->GetTypeOfRequest() == GAbsPageLoadRequest::eUpdateURLCurrentPageRequest )
		{
			GUpdateURLCurrentPageRequest* pRequest(static_cast<GUpdateURLCurrentPageRequest*>(m_qlsReqs.first()));
			ProcessRequest(pRequest);

			m_qlsReqs.removeFirst();
		}
		else
		{
			break;
		}
	}

	if( !m_qlsReqs.isEmpty() )
	{
		if( m_qlsReqs.first()->GetTypeOfRequest() == GAbsPageLoadRequest::eSetPreloadedPagesRequest )
		{
			GSetPreloadedPagesRequest* pRequest(static_cast<GSetPreloadedPagesRequest*>(m_qlsReqs.first()));
			ProcessRequest(pRequest);
		}
		else if( m_qlsReqs.first()->GetTypeOfRequest() == GAbsPageLoadRequest::eLoadEntirePageRequest )
		{
			GLoadEntirePageRequest* pRequest(static_cast<GLoadEntirePageRequest*>(m_qlsReqs.first()));
			ProcessRequest(pRequest);
		}
		else if( m_qlsReqs.first()->GetTypeOfRequest() == GAbsPageLoadRequest::eUpdatePreloadedPageRequest )
		{
			GUpdatePreloadedPageRequest* pRequest(static_cast<GUpdatePreloadedPageRequest*>(m_qlsReqs.first()));
			ProcessRequest(pRequest);
		}
		else
		{
			throw(GException("Cannot found the request in the internal queue."));
		}
	}

	LOG_LEVEL4(QString("Quantity of pending requests(") + QString::number(m_qlsReqs.count()) + ").");
}

void GPageLoadRequestManager::ProcessRequest(GSetPreloadedPagesRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GSetPreloadedPagesRequest)");

	if( m_oPreSetManager.CanProcessRequest(pRequest) )
	{
		if( m_oNonPreSetManager.IsActive() )
		{
			m_oNonPreSetManager.FlushVideoPatch();
		}

		m_oPreSetManager.ProcessRequest(pRequest);
	}
}

void GPageLoadRequestManager::ProcessRequest(GLoadEntirePageRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GLoadEntirePageRequest)");

	if( m_oNonPreSetManager.CanProcessRequest(pRequest) )
	{
		if( m_oNonPreSetManager.IsActive() )
		{
			m_oNonPreSetManager.FlushVideoPatch(false);
		}

		m_oNonPreSetManager.ProcessRequest(pRequest);
	}
}

void GPageLoadRequestManager::ProcessRequest(GUpdatePreloadedPageRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GUpdatePreloadedPageRequest)");

	if( m_oPreSetManager.CanProcessRequest(pRequest) )
	{
		if( m_oNonPreSetManager.IsActive() )
		{
			m_oNonPreSetManager.FlushVideoPatch();
		}

		m_oPreSetManager.ProcessRequest(pRequest);
	}
}

void GPageLoadRequestManager::ProcessRequest(GUpdateCurrentPageRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GUpdateCurrentPageRequest)");

	QString sResult("discarded");

	if( m_oPreSetManager.IsActive() && m_oPreSetManager.CanProcessRequest(pRequest) )
	{
		m_oPreSetManager.ProcessRequest(pRequest);
		sResult = "completed";
	}

	if( m_oNonPreSetManager.IsActive() && m_oNonPreSetManager.CanProcessRequest(pRequest) )
	{
		m_oNonPreSetManager.ProcessRequest(pRequest);
		sResult = "completed";
	}

	static QString const strTemp("(GUpdateCurrentPageRequest): RequestId(%1), has been completed with result (%2) for (%3).");
	LOG_LEVEL4(QString(strTemp) . arg(pRequest->GetRequestId()) . arg(sResult) . arg(GetSessionName(pRequest->GetSessionId())));

	if( pRequest->IsSynchronize() )
	{
		GAbsView::SignalSynchronize( (sResult=="completed")?(true):(false) );
	}
	delete pRequest;
}

void GPageLoadRequestManager::ProcessRequest(GUpdateURLCurrentPageRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GUpdateURLCurrentPageRequest)");

	QString sResult("discarded");

	if( m_oNonPreSetManager.IsActive() && m_oNonPreSetManager.CanProcessRequest(pRequest) )
	{
		m_oNonPreSetManager.ProcessRequest(pRequest);
		sResult = "completed";
	}

	static QString const strTemp("(GUpdateURLCurrentPageRequest): RequestId(%1), has been completed with result (%2) for (%3).");
	LOG_LEVEL4(QString(strTemp) . arg(pRequest->GetRequestId()) . arg(sResult) . arg(GetSessionName(pRequest->GetSessionId())));

	if( pRequest->IsSynchronize() )
	{
		GAbsView::SignalSynchronize( (sResult=="completed")?(true):(false) );
	}
	delete pRequest;
}

void GPageLoadRequestManager::SetAsActive(unsigned int uiLoadersManagerId)
{
	if( uiLoadersManagerId == CNON_PRESET_PAGE_LOADER_MANAGER_ID )
	{
		LOG_LEVEL4("SetAsActive(NON_PRESET_PAGE_LOADER_MANAGER)");
		m_oNonPreSetManager.SetAsActive();
		m_oPreSetManager.SetAsNonActive();
	}
	else
	{
		LOG_LEVEL4("SetAsActive(PRESET_PAGE_LOADER_MANAGER)");
		m_oPreSetManager.SetAsActive();
		m_oNonPreSetManager.SetAsNonActive();
	}
}

QSharedPointer<GPageLoadRequestManager> GPageLoadRequestManager::GetInstance() throw(GException)
{
	LOG_LEVEL4("GetInstance()");

	QMutexLocker oLocker(&GPageLoadRequestManager::m_mutex);

	if( m_pGPageLoadRequestManager.isNull() )
	{
		m_pGPageLoadRequestManager = QSharedPointer<GPageLoadRequestManager>(new GPageLoadRequestManager());

		if (!m_pGPageLoadRequestManager.isNull())
		{
			m_pGPageLoadRequestManager->Init();
		}
	}

	if( m_pGPageLoadRequestManager.isNull() )
	{
		 throw(GException("GPageLoadRequestManager::GetInstance() made a NULL pointer."));
	}

	return m_pGPageLoadRequestManager;
}
