#include "GAbsView.h"

#include <QSharedPointer>
#include <QMutexLocker>

#include "view/src/utils/requestsmanager/GPageLoadRequestManager.h"
#include "view/src/utils/requests/GUpdatePreloadedPageRequest.h"
#include "view/src/utils/requests/GUpdateCurrentPageRequest.h"
#include "view/src/utils/requests/GUpdateURLCurrentPageRequest.h"
#include "view/src/utils/requests/GLoadEntirePageRequest.h"
#include "view/src/utils/requests/GSetPreloadedPagesRequest.h"

#include "utils/logger/src/GLogger.h"

bool GAbsView::_bSynchWaiting(false);
bool GAbsView::_bSyncResult(false);
QMutex GAbsView::_mtxSynchMutex;
QWaitCondition GAbsView::_cndSynchConditional;

GAbsView::GAbsView():
	m_iSessionId(0)
{
	LOG_LEVEL4("GAbsView()");
}

GAbsView::~GAbsView()
{
	LOG_LEVEL4("~GAbsView()");
}

void GAbsView::Init()
{
	LOG_LEVEL3("Init()");

	QSharedPointer<GPageLoadRequestManager> pGPageLoadRequestManager(GPageLoadRequestManager::GetInstance());

	if( !connect( this, SIGNAL(SignalSendRequest(GUpdatePreloadedPageRequest*)),
				  pGPageLoadRequestManager.data(), SLOT(ReceiveRequest(GUpdatePreloadedPageRequest*)), Qt::QueuedConnection ) )
	{
		LOG_LEVEL1("Can't connect to GPageLoadRequestManager object.");
	}

	if( !connect( this, SIGNAL(SignalSendRequest(GUpdateCurrentPageRequest*)),
				  pGPageLoadRequestManager.data(), SLOT(ReceiveRequest(GUpdateCurrentPageRequest*)), Qt::QueuedConnection ) )
	{
		LOG_LEVEL1("Can't connect to GPageLoadRequestManager object.");
	}

	if( !connect( this, SIGNAL(SignalSendRequest(GUpdateURLCurrentPageRequest*)),
				  pGPageLoadRequestManager.data(), SLOT(ReceiveRequest(GUpdateURLCurrentPageRequest*)), Qt::QueuedConnection ) )
	{
		LOG_LEVEL1("Can't connect to GPageLoadRequestManager object.");
	}

	if( !connect( this, SIGNAL(SignalSendRequest(GLoadEntirePageRequest*)),
				  pGPageLoadRequestManager.data(), SLOT(ReceiveRequest(GLoadEntirePageRequest*)), Qt::QueuedConnection ) )
	{
		LOG_LEVEL1("Can't connect to GPageLoadRequestManager object.");
	}

	bool bIsView(true);
	m_iSessionId = pGPageLoadRequestManager->Subscribe(m_sName, bIsView);
}

void GAbsView::SignalSynchronize(bool bResult)
{
	LOG_LEVEL3("SignalSynchronize()");

	QMutexLocker oLocker(&GAbsView::_mtxSynchMutex);

	GAbsView::_bSyncResult = bResult;

	if( GAbsView::_bSynchWaiting == true )
	{
		GAbsView::_bSynchWaiting = false;

		GAbsView::_cndSynchConditional.wakeOne();
	}
}

void GAbsView::SetView(bool bSynchronize, QStringList& oStrListJSCalls, QStringList& oStrListJSLibraries, QString& strURL, bool bApplyZoomFactor)
{
	LOG_LEVEL4("SetView()");

	QMutexLocker oLocker(&GAbsView::_mtxSynchMutex);

	GLoadEntirePageRequest* pRequest(new GLoadEntirePageRequest(m_iSessionId, bSynchronize, bApplyZoomFactor, oStrListJSCalls, oStrListJSLibraries, strURL));

	emit SignalSendRequest(pRequest);

	GAbsView::_bSynchWaiting = bSynchronize;
	while( GAbsView::_bSynchWaiting == true )
	{
		GAbsView::_bSyncResult = true;

		GAbsView::_cndSynchConditional.wait(&GAbsView::_mtxSynchMutex);

		if( !GAbsView::_bSyncResult )
		{
			throw GEXCEPTION("Last LoadEntirePageRequest result return an error on a sync call.");
		}
	}
}

void GAbsView::SetViewContent(bool bSynchronize, QStringList& oStrListJSCalls)
{
	LOG_LEVEL4("SetViewContent()");

	QMutexLocker oLocker(&GAbsView::_mtxSynchMutex);

	GUpdatePreloadedPageRequest* pRequest(new GUpdatePreloadedPageRequest(m_iSessionId, bSynchronize, oStrListJSCalls));

	emit SignalSendRequest(pRequest);

	GAbsView::_bSynchWaiting = bSynchronize;
	while( GAbsView::_bSynchWaiting == true )
	{
		GAbsView::_bSyncResult = true;

		GAbsView::_cndSynchConditional.wait(&GAbsView::_mtxSynchMutex);

		if( !GAbsView::_bSyncResult )
		{
			throw GEXCEPTION("Last UpdatePreloadedPageRequest result return an error on a sync call.");
		}
	}
}

void GAbsView::UpdateViewContent(bool bSynchronize, QStringList& oStrListJSCalls)
{
	LOG_LEVEL4("UpdateViewContent()");

	QMutexLocker oLocker(&GAbsView::_mtxSynchMutex);

	GUpdateCurrentPageRequest* pRequest(new GUpdateCurrentPageRequest(m_iSessionId, bSynchronize, oStrListJSCalls));

	emit SignalSendRequest(pRequest);

	GAbsView::_bSynchWaiting = bSynchronize;
	while( GAbsView::_bSynchWaiting == true )
	{
		GAbsView::_bSyncResult = true;

		GAbsView::_cndSynchConditional.wait(&GAbsView::_mtxSynchMutex);

		if( !GAbsView::_bSyncResult )
		{
			throw GEXCEPTION("Last UpdateCurrentPageRequest result return an error on a sync call.");
		}
	}
}

void GAbsView::UpdateURLViewContent(bool bSynchronize, QStringList& oStrListJSCalls)
{
	LOG_LEVEL4("UpdateViewContent()");

	QMutexLocker oLocker(&GAbsView::_mtxSynchMutex);

	GUpdateURLCurrentPageRequest* pRequest(new GUpdateURLCurrentPageRequest(m_iSessionId, bSynchronize, oStrListJSCalls));

	emit SignalSendRequest(pRequest);

	GAbsView::_bSynchWaiting = bSynchronize;
	while( GAbsView::_bSynchWaiting == true )
	{
		GAbsView::_bSyncResult = true;

		GAbsView::_cndSynchConditional.wait(&GAbsView::_mtxSynchMutex);

		if( !GAbsView::_bSyncResult )
		{
			throw GEXCEPTION("Last UpdateURLCurrentPageRequest result return an error on a sync call.");
		}
	}
}

void GAbsView::GetScaledXY( int& iXPos, int& iYPos ) const
{
	LOG_LEVEL4("GetScaledXY()");

	QSharedPointer<GPageLoadRequestManager> pGPageLoadRequestManager(GPageLoadRequestManager::GetInstance());

	LOG_LEVEL4(QString("X=") + QString::number(iXPos) + ", Y=" + QString::number(iYPos) + " positions before scaling.");

	if( iXPos == 0 )
	{
		iXPos = -1;
	}
	else
	{
		iXPos = (iXPos - 1) * pGPageLoadRequestManager->GetScaleXFactor();
	}

	if( iYPos == 0 )
	{
		iYPos = -1;
	}
	else
	{
		iYPos = (iYPos - 1) * pGPageLoadRequestManager->GetScaleYFactor();
	}

	LOG_LEVEL4(QString("X=") + QString::number(iXPos) + ", Y=" + QString::number(iYPos) + " positions after scaling.");
}

void GAbsView::GetScaledWidthAndHeight( int& iWidth, int& iHeight ) const
{
	LOG_LEVEL4("GetScaledWidthAndHeight()");

	QSharedPointer<GPageLoadRequestManager> pGPageLoadRequestManager(GPageLoadRequestManager::GetInstance());

	if( iWidth < 1 )
	{
		iWidth = 0;
	}
	else
	{
		iWidth *= pGPageLoadRequestManager->GetScaleXFactor();
	}

	if( iHeight < 1 )
	{
		iHeight = 0;
	}
	else
	{
		iHeight *= pGPageLoadRequestManager->GetScaleYFactor();
	}
}
