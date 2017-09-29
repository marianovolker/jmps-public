#include "GNonPreSetPageLoader.h"

#include "view/src/utils/requests/GLoadEntirePageRequest.h"
#include "view/src/utils/requests/GUpdateURLCurrentPageRequest.h"
#include "view/src/utils/requests/GUpdateCurrentPageRequest.h"
#include "view/src/utils/loadersmanager/GAbsPageLoaderManager.h"

#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"

GNonPreSetPageLoader::GNonPreSetPageLoader(unsigned int uiId, GAbsPageLoaderManager *pObserver)
	:	GAbsPageLoader(uiId, pObserver)
{
	LOG_LEVEL4(QString("GNonPreSetPageLoader(") + QString::number(m_iId) + ").");
}

GNonPreSetPageLoader::~GNonPreSetPageLoader()
{
	LOG_LEVEL4(QString("~GNonPreSetPageLoader(") + QString::number(m_iId) + ").");
}

void GNonPreSetPageLoader::LoadContent(GLoadEntirePageRequest* pRequest)
{
	LOG_LEVEL4(QString("LoadContent(GLoadEntirePageRequest, ") + QString::number(m_iId) + ").");

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
}

void GNonPreSetPageLoader::UpdateContent(GUpdateURLCurrentPageRequest* pRequest)
{
	LOG_LEVEL4(QString("UpdateContent(GUpdateURLCurrentPageRequest, ") + QString::number(m_iId) + ").");

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

void GNonPreSetPageLoader::UpdateContent(GUpdateCurrentPageRequest* pRequest)
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

void GNonPreSetPageLoader::DoCompleted()
{
	LOG_LEVEL4(QString("DoCompleted(") + QString::number(m_iId) + ").");

	GLoadEntirePageRequest* pRequest = static_cast<GLoadEntirePageRequest*>(m_pRequest);

	LOG_LEVEL3(QString("[") + pRequest->GetUrl() + "] loaded on (" + QString::number(m_iId) + ").");

	// We need to reset the zoom factor here if the previous load changed it.
	m_jsHelper.GetMainFrame()->setZoomFactor( 1.0 );

	// We need to apply the zoom factor algorithm if the page requires that.
	if( pRequest->GetApplyZoom() && GETCONF_BOOL(eDisplayManagerApplyZoomFactor) )
	{
		qreal oWidthFrame  = m_jsHelper.GetMainFrame()->contentsSize().width();
		qreal oWidthScreen = GETCONF_NUM(eDisplayManagerViewWidth);
		qreal oZoomFactor  = oWidthScreen/oWidthFrame;
		qreal oDelta	   = 10.0;

		LOG_LEVEL3(QString("WidthFrame = ") + QString::number(oWidthFrame) );
		LOG_LEVEL3(QString("WidthScreen= ") + QString::number(oWidthScreen));

		if( (oWidthScreen+oDelta) < oWidthFrame )
		{
			LOG_LEVEL3(QString().sprintf("Applying ZoomFactor(%f)", oZoomFactor));
			m_jsHelper.GetMainFrame()->setZoomFactor( oZoomFactor );
		}
	}

	SetAsNotWaitingForLoadingPage();
}

void GNonPreSetPageLoader::DoTimeout()
{
	LOG_LEVEL4(QString("DoTimeout(") + QString::number(m_iId) + ").");

	GLoadEntirePageRequest* pRequest = static_cast<GLoadEntirePageRequest*>(m_pRequest);

	LOG_LEVEL3(QString("Timeout loading [") + pRequest->GetUrl() + "] on (" + QString::number(m_iId) + ").");

	SetAsNotWaitingForLoadingPage();
}
