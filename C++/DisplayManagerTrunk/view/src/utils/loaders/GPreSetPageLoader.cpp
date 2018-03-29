#include "GPreSetPageLoader.h"

#include "view/src/utils/requests/GSetPreloadedPagesRequest.h"
#include "view/src/utils/requests/GUpdatePreloadedPageRequest.h"
#include "view/src/utils/requests/GUpdateCurrentPageRequest.h"
#include "view/src/utils/loadersmanager/GAbsPageLoaderManager.h"

#include "utils/logger/src/GLogger.h"

GPreSetPageLoader::GPreSetPageLoader(unsigned int uiId, GAbsPageLoaderManager *pObserver)
	:	GAbsPageLoader(uiId, pObserver),
		m_bPartialProcessing(false)
{
	LOG_LEVEL4(QString("GPreSetPageLoader(") + QString::number(m_iId) + ").");
}

GPreSetPageLoader::~GPreSetPageLoader()
{
	LOG_LEVEL4(QString("~GPreSetPageLoader(") + QString::number(m_iId) + ").");
}

void GPreSetPageLoader::LoadContent(GSetPreloadedPagesRequest* pRequest)
{
	LOG_LEVEL4(QString("LoadContent(GSetPreloadedPagesRequest, ") + QString::number(m_iId) + ").");

	if( IsWaitingForLoadingPage() )
	{
		m_pPage->triggerAction(QWebPage::Stop);
	}

	m_jsHelper.SetJSLibraries(pRequest->GetListJSLibraries());

	m_jsHelper.SetJSCalls(pRequest->GetListJSCalls());

	m_jsHelper.SetMainFrame(m_pPage->mainFrame());

	m_pRequest = pRequest;

	SetAsWaitingForLoadingPage();

	m_jsHelper.GetMainFrame()->load(QUrl(pRequest->GetUrl()));

	m_bPartialProcessing = false;
}

void GPreSetPageLoader::UpdateContent(GUpdatePreloadedPageRequest* pRequest)
{
	LOG_LEVEL4(QString("UpdateContent(GUpdatePreloadedPageRequest, ") + QString::number(m_iId) + ").");

	if( IsWaitingForLoadingPage() )
	{
		m_pPage->triggerAction(QWebPage::Stop);
	}

	static QString const qksClearBody("document.body.innerHTML=\"\";document.body.style.border=0;document.body.style.margin=0;document.body.style.padding=0;");
	QStringList qlJSCalls(pRequest->GetListJSCalls());
	qlJSCalls.push_front(qksClearBody);

	m_jsHelper.SetJSCalls(qlJSCalls);

	m_jsHelper.SetMainFrame(m_pPage->mainFrame());

	m_jsHelper.GetJSCalls().push_back(m_jsSignal.GetDynamicOnLoadTriggerCode());
	
	m_pRequest = pRequest;

	SetAsWaitingForLoadingPage();

	m_jsHelper.LoadJSCalls();

	m_bPartialProcessing = true;
}

void GPreSetPageLoader::UpdateContent(GUpdateCurrentPageRequest* pRequest)
{
	LOG_LEVEL4(QString("UpdateContent(GUpdateCurrentPageRequest, ") + QString::number(m_iId) + ").");

	if( IsWaitingForLoadingPage() )
	{
		m_pPage->triggerAction(QWebPage::Stop);
	}

	m_jsHelper.SetJSCalls(pRequest->GetListJSCalls());

	m_jsHelper.SetMainFrame(m_pPage->mainFrame());

	m_jsHelper.LoadJSCalls();

	m_uiLastRequestIdProcessed = pRequest->GetRequestId();

	m_uiCurrentSessionId = pRequest->GetSessionId();
}

void GPreSetPageLoader::DoCompleted()
{
	LOG_LEVEL4(QString("DoCompleted(") + QString::number(m_iId) + ").");

	if( !m_bPartialProcessing )
	{
		GSetPreloadedPagesRequest* pRequest = static_cast<GSetPreloadedPagesRequest*>(m_pRequest);

		LOG_LEVEL3( QString("[") + pRequest->GetUrl() + "] loaded on (" + QString::number(m_iId) + ")." );
	}
	else
	{
		LOG_LEVEL3("[ DOM was changed ].");

		m_bPartialProcessing = false;
	}

	SetAsNotWaitingForLoadingPage();
}

void GPreSetPageLoader::DoTimeout()
{
	LOG_LEVEL4(QString("DoTimeout(") + QString::number(m_iId) + ").");

	if( !m_bPartialProcessing )
	{
		GSetPreloadedPagesRequest* pRequest = static_cast<GSetPreloadedPagesRequest*>(m_pRequest);

		LOG_LEVEL3( QString("Timeout loading [") + pRequest->GetUrl() + "] on (" + QString::number(m_iId) + ")." );
	}
	else
	{
		LOG_LEVEL3("Timeout changing current DOM.");

		m_bPartialProcessing = false;
	}

	SetAsNotWaitingForLoadingPage();
}
