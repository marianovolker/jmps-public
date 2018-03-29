#include "GNonPreSetPageLoaderManager.h"

#include "view/src/utils/requestsmanager/GPageLoadRequestManager.h"
#include "view/src/utils/loaders/GNonPreSetPageLoader.h"
#include "view/src/utils/loaders/GLoaderRequestNotify.h"
#include "view/src/utils/requests/GLoadEntirePageRequest.h"
#include "view/src/utils/requests/GUpdateURLCurrentPageRequest.h"
#include "view/src/utils/requests/GUpdateCurrentPageRequest.h"
#include "utils/media/src/GMediaManagerFactory.h"
#include "utils/media/src/GAbsMediaManager.h"

#include "GLoadersManagerRequestNotify.h"

#include "utils/logger/src/GLogger.h"

GNonPreSetPageLoaderManager::GNonPreSetPageLoaderManager(unsigned int uiPageLoaderQty)
{
	LOG_LEVEL4("GNonPreSetPageLoaderManager()");

	CreatePageLoaders(uiPageLoaderQty);
}

GNonPreSetPageLoaderManager::~GNonPreSetPageLoaderManager()
{
	LOG_LEVEL4("~GNonPreSetPageLoaderManager()");
}

void GNonPreSetPageLoaderManager::CreatePageLoaders(unsigned int uiPageLoadersQty)
{
	LOG_LEVEL4("CreatePageLoaders()");

	for( unsigned int uiJ = 1; (uiJ <= uiPageLoadersQty); ++uiJ )
	{
		QSharedPointer<GAbsPageLoader> pPageLoader(new GNonPreSetPageLoader(uiJ));
		pPageLoader->Init();
		m_oPageLoaders.insert(uiJ, pPageLoader);
	}
}

void GNonPreSetPageLoaderManager::ProcessRequest(GLoadEntirePageRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GLoadEntirePageRequest)");

	if( !GetFirstNonActivePageLoader().isNull() )
	{
		GNonPreSetPageLoader* pNonActivePageLoader( static_cast<GNonPreSetPageLoader*>(GetFirstNonActivePageLoader().data()) );
		pNonActivePageLoader->LoadContent(pRequest);
	}
	else if( m_oPageLoaders.size() == 1 )
	{
		if( !GetActivePageLoader().isNull() )
		{
			GNonPreSetPageLoader* pActivePageLoader( static_cast<GNonPreSetPageLoader*>(GetActivePageLoader().data()) );
			pActivePageLoader->LoadContent(pRequest);
		}
	}
}

void GNonPreSetPageLoaderManager::ProcessRequest(GUpdateURLCurrentPageRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GUpdateURLCurrentPageRequest)");

	if( !GetActivePageLoader().isNull() )
	{
		GNonPreSetPageLoader* pActivePageLoader( static_cast<GNonPreSetPageLoader*>(GetActivePageLoader().data()) );
		pActivePageLoader->UpdateContent(pRequest);
	}
}

void GNonPreSetPageLoaderManager::ProcessRequest(GUpdateCurrentPageRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GUpdateCurrentPageRequest)");

	if( !GetActivePageLoader().isNull() )
	{
		GNonPreSetPageLoader* pActivePageLoader( static_cast<GNonPreSetPageLoader*>(GetActivePageLoader().data()) );
		pActivePageLoader->UpdateContent(pRequest);
	}
}

void GNonPreSetPageLoaderManager::FlushVideoPatch(bool bForcePageUnload)
{
	LOG_LEVEL4("FlushVideoPatch()");

	if( !GetActivePageLoader().isNull() )
	{
                // Use factory to create the correct instance of the MediaManager
                GMediaManagerFactory mediaManagerFactory;
                QSharedPointer<GAbsMediaManager> pMediaManager = mediaManagerFactory.CreateMediaManager();
		pMediaManager->StopMedia();
		if (bForcePageUnload)
		{
			GNonPreSetPageLoader* pActivePageLoader( static_cast<GNonPreSetPageLoader*>(GetActivePageLoader().data()) );
			pActivePageLoader->GetWebPage()->mainFrame()->setHtml("<HTML><body bgcolor=\"#000000\"></body></HTML>");//Sets a Black screen
		}
	}
}

void GNonPreSetPageLoaderManager::Notify(GLoaderRequestNotify &oNotify)
{
	LOG_LEVEL4("Notify()");

	if( oNotify.GetNofifyType() == GLoaderRequestNotify::eContentLoadComplete )
	{
		if( !GetActivePageLoader().isNull() )
		{
			GetActivePageLoader()->SetAsNonActive();
		}

		m_oPageLoaders[oNotify.GetPageLoaderId()]->SetAsActive();
	}

	GLoadersManagerRequestNotify oLoadersManagerNotify ( GetId(),
														 oNotify.GetSessionId(),
														 static_cast<GLoadersManagerRequestNotify::eNotifyTypeEnum>(oNotify.GetNofifyType()) );
	oLoadersManagerNotify.SetRequest(oNotify.GetRequest());
	oLoadersManagerNotify.SetWebPage(m_oPageLoaders[oNotify.GetPageLoaderId()]->GetWebPage());

	if( m_pObserver )
	{
		m_pObserver->Notify(oLoadersManagerNotify);
	}
}

bool GNonPreSetPageLoaderManager::CanProcessRequest(GAbsPageLoadRequest* pRequest)
{
	LOG_LEVEL4("CanProcessRequest()");

	bool bSomePageLoaderIsActive(false);
	bool bSomePageLoaderIsLoadingAPage(false);

	if( !GetActivePageLoader().isNull() )
	{
		bSomePageLoaderIsActive = true;
	}

	if( !GetWaitingForLoadingPagePageLoader().isNull() )
	{
		bSomePageLoaderIsLoadingAPage = true;
	}

	if( pRequest->GetTypeOfRequest() == GAbsPageLoadRequest::eLoadEntirePageRequest )
	{
		if( bSomePageLoaderIsActive && bSomePageLoaderIsLoadingAPage )
		{
			return false;
		}
	}
	else if( (pRequest->GetTypeOfRequest() == GAbsPageLoadRequest::eUpdateCurrentPageRequest) ||
			 (pRequest->GetTypeOfRequest() == GAbsPageLoadRequest::eUpdateURLCurrentPageRequest) )
	{
		if( !bSomePageLoaderIsActive )
		{
			return false;
		}
		else
		{
			if( GetCurrentSessionId() != pRequest->GetSessionId() )
			{
				static QString const strTemp("Warning request has a diferent session_id, current session id (%1), request session id (%2).");
				LOG_LEVEL1(QString(strTemp).arg(GetCurrentSessionId()).arg(pRequest->GetSessionId()));

				// We need to accept all types of request at any time.
				// 14/01/2010
				//return false;
			}
		}
	}
	else
	{
		LOG_LEVEL1("This type of request is unsupported(eSetPreloadedPagesRequest or eUpdatePreloadedPageRequest).");
		return false;
	}

	return true;
}
