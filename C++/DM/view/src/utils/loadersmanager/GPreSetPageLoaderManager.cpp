#include "GPreSetPageLoaderManager.h"

#include "view/src/utils/requestsmanager/GPageLoadRequestManager.h"
#include "view/src/utils/loaders/GPreSetPageLoader.h"
#include "view/src/utils/loaders/GLoaderRequestNotify.h"
#include "view/src/utils/requests/GSetPreloadedPagesRequest.h"
#include "view/src/utils/requests/GUpdatePreloadedPageRequest.h"
#include "view/src/utils/requests/GUpdateCurrentPageRequest.h"
#include "GLoadersManagerRequestNotify.h"

#include "utils/logger/src/GLogger.h"

GPreSetPageLoaderManager::GPreSetPageLoaderManager(unsigned int uiPageLoaderQty)
	:	m_eState(GPreSetPageLoaderManager::eIdleIdle)
{
	LOG_LEVEL4("GPreSetPageLoaderManager()");

	CreatePageLoaders(uiPageLoaderQty);
}

GPreSetPageLoaderManager::~GPreSetPageLoaderManager()
{
	LOG_LEVEL4("~GPreSetPageLoaderManager()");
}

void GPreSetPageLoaderManager::CreatePageLoaders(unsigned int uiPageLoadersQty)
{
	LOG_LEVEL4("CreatePageLoaders()");

	for (unsigned int uiJ = 1; (uiJ <= uiPageLoadersQty); ++uiJ)
	{
		QSharedPointer<GAbsPageLoader> pPageLoader(new GPreSetPageLoader(uiJ));
		pPageLoader->Init();
		m_oPageLoaders.insert(uiJ, pPageLoader);
	}
}

bool GPreSetPageLoaderManager::CanProcessRequest(GAbsPageLoadRequest* pRequest)
{
	LOG_LEVEL4("CanProcessRequest()");

	GPreSetPageLoader* pFirstPageLoder (static_cast<GPreSetPageLoader*>(m_oPageLoaders[1].data()));
	GPreSetPageLoader* pSecondPageLoder(static_cast<GPreSetPageLoader*>(m_oPageLoaders[2].data()));

	if(pRequest->GetTypeOfRequest() == GAbsPageLoadRequest::eSetPreloadedPagesRequest)
	{
		switch(m_eState)
		{
			case eIdleProcessing:
			case eActiveProcessing:
			case eProcessingIdle:
			case eProcessingActive:
			case eProcessingProcessing:
				return false;
			break;
			default:
			break;
		}
	}
	else if(pRequest->GetTypeOfRequest() == GAbsPageLoadRequest::eUpdatePreloadedPageRequest)
	{
		switch(m_eState)
		{
			case eActiveProcessing:
			case eProcessingActive:
			case eProcessingProcessing:
				return false;
			break;

			case eIdleProcessing:
				if(pSecondPageLoder->GetRequest()->GetTypeOfRequest() == GAbsPageLoadRequest::eSetPreloadedPagesRequest)
				{
					return false;
				}
			break;

			case eProcessingIdle:
				if(pFirstPageLoder->GetRequest()->GetTypeOfRequest() == GAbsPageLoadRequest::eSetPreloadedPagesRequest)
				{
					return false;
				}
			break;

			default:
			break;
		}
	}
	else if(pRequest->GetTypeOfRequest() == GAbsPageLoadRequest::eUpdateCurrentPageRequest)
	{
		switch(m_eState)
		{
			case eIdleIdle:
			case eIdleProcessing:
			case eProcessingIdle:
			case eProcessingProcessing:
				return false;
			break;

			default:
			break;
		}

		if( (GetCurrentSessionId()!=0) && (GetCurrentSessionId() != pRequest->GetSessionId()) )
		{
			static QString const strTemp("Warning request has a diferent session_id, current session id (%1), request session id (%2).");
			LOG_LEVEL1(QString(strTemp).arg(GetCurrentSessionId()).arg(pRequest->GetSessionId()));

			// We need to accept all types of request at any time.
			// 14/01/2010
			//return false;
		}
	}
	else
	{
		LOG_LEVEL1("This type of request is unsupported(eLoadEntirePageRequest).");
		return false;
	}

	return true;
}

void GPreSetPageLoaderManager::ProcessRequest(GSetPreloadedPagesRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GSetPreloadedPagesRequest)");

	GPreSetPageLoader* pFirstPageLoder (static_cast<GPreSetPageLoader*>(m_oPageLoaders[1].data()));
	GPreSetPageLoader* pSecondPageLoder(static_cast<GPreSetPageLoader*>(m_oPageLoaders[2].data()));

	switch(m_eState)
	{
		case GPreSetPageLoaderManager::eIdleIdle:
			pFirstPageLoder->LoadContent(pRequest);
			pSecondPageLoder->LoadContent(pRequest);
			m_eState = eProcessingProcessing;
		break;

		case GPreSetPageLoaderManager::eIdleActive:
			pFirstPageLoder->LoadContent(pRequest);
			m_eState = eProcessingActive;
		break;

		case GPreSetPageLoaderManager::eIdleProcessing:
			pFirstPageLoder->LoadContent(pRequest);
			m_eState = eProcessingProcessing;
		break;

		case GPreSetPageLoaderManager::eActiveIdle:
			pSecondPageLoder->LoadContent(pRequest);
			m_eState = eActiveProcessing;
		break;

		case GPreSetPageLoaderManager::eActiveProcessing:
			m_eState = eActiveProcessing;
		break;

		case GPreSetPageLoaderManager::eProcessingIdle:
			pSecondPageLoder->LoadContent(pRequest);
			m_eState = eProcessingProcessing;
		break;

		case GPreSetPageLoaderManager::eProcessingActive:
			m_eState = eProcessingActive;
		break;

		case GPreSetPageLoaderManager::eProcessingProcessing:
			m_eState = eProcessingProcessing;
		break;
	}
}

void GPreSetPageLoaderManager::ProcessRequest(GUpdatePreloadedPageRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GUpdatePreloadedPageRequest)");

	GPreSetPageLoader* pFirstPageLoder (static_cast<GPreSetPageLoader*>(m_oPageLoaders[1].data()));
	GPreSetPageLoader* pSecondPageLoder(static_cast<GPreSetPageLoader*>(m_oPageLoaders[2].data()));

	switch(m_eState)
	{
		case GPreSetPageLoaderManager::eIdleIdle:
			pFirstPageLoder->UpdateContent(pRequest);
			m_eState = eProcessingIdle;
		break;

		case GPreSetPageLoaderManager::eIdleActive:
			pFirstPageLoder->UpdateContent(pRequest);
			m_eState = eProcessingActive;
		break;

		case GPreSetPageLoaderManager::eIdleProcessing:
			pFirstPageLoder->UpdateContent(pRequest);
			m_eState = eProcessingProcessing;
		break;

		case GPreSetPageLoaderManager::eActiveIdle:
			pSecondPageLoder->UpdateContent(pRequest);
			m_eState = eActiveProcessing;
		break;

		case GPreSetPageLoaderManager::eProcessingIdle:
			pSecondPageLoder->UpdateContent(pRequest);
			m_eState = eProcessingProcessing;
		break;

		default:
			LOG_LEVEL1("ERROR processing request.");
		break;
	}
}

void GPreSetPageLoaderManager::ProcessRequest(GUpdateCurrentPageRequest* pRequest)
{
	LOG_LEVEL4("ProcessRequest(GUpdateCurrentPageRequest)");

	GPreSetPageLoader* pFirstPageLoder (static_cast<GPreSetPageLoader*>(m_oPageLoaders[1].data()));
	GPreSetPageLoader* pSecondPageLoder(static_cast<GPreSetPageLoader*>(m_oPageLoaders[2].data()));

	switch(m_eState)
	{
		case GPreSetPageLoaderManager::eIdleActive:
			pSecondPageLoder->UpdateContent(pRequest);
		break;

		case GPreSetPageLoaderManager::eActiveIdle:
			pFirstPageLoder->UpdateContent(pRequest);
		break;

		case GPreSetPageLoaderManager::eActiveProcessing:
			pFirstPageLoder->UpdateContent(pRequest);
		break;

		case GPreSetPageLoaderManager::eProcessingActive:
			pSecondPageLoder->UpdateContent(pRequest);
		break;

		default:
			LOG_LEVEL1("ERROR processing request.");
		break;
	}
}

void GPreSetPageLoaderManager::Notify(GLoaderRequestNotify &oNotify)
{
	LOG_LEVEL4("Notify()");

	bool bNotify(true);

	if( oNotify.GetRequest()->GetTypeOfRequest() == GAbsPageLoadRequest::eUpdatePreloadedPageRequest )
	{
		UpdateStateOnNotify(oNotify);
	}
	else
	{
		UpdateStateOnNotifyForPreSetRequest(oNotify);

		if( (m_eState==eIdleProcessing) || (m_eState==eActiveProcessing) || (m_eState==eProcessingIdle) || (m_eState==eProcessingActive) ||
			(m_eState==eProcessingProcessing) )
		{
			bNotify = false;
		}
	}

	if(bNotify)
	{
		GLoadersManagerRequestNotify oLoadersManagerNotify( GetId(),
															oNotify.GetSessionId(),
															static_cast<GLoadersManagerRequestNotify::eNotifyTypeEnum>(oNotify.GetNofifyType()) );
		oLoadersManagerNotify.SetWebPage(m_oPageLoaders[oNotify.GetPageLoaderId()]->GetWebPage());
		oLoadersManagerNotify.SetRequest(oNotify.GetRequest());

		if( m_pObserver )
		{
			m_pObserver->Notify(oLoadersManagerNotify);
		}
	}
}

void GPreSetPageLoaderManager::UpdateStateOnNotify(GLoaderRequestNotify &oNotify)
{
	LOG_LEVEL4("UpdateStateOnNotify()");

	GPreSetPageLoader* pFirstPageLoder (static_cast<GPreSetPageLoader*>(m_oPageLoaders[1].data()));
	GPreSetPageLoader* pSecondPageLoder(static_cast<GPreSetPageLoader*>(m_oPageLoaders[2].data()));

	switch(m_eState)
	{
		case GPreSetPageLoaderManager::eIdleProcessing:
			if( oNotify.GetNofifyType() == GLoaderRequestNotify::eContentLoadComplete )
			{
				pSecondPageLoder->SetAsActive();
				m_eState = eIdleActive;
			}
			else
			{
				m_eState = eIdleIdle;
			}
		break;

		case GPreSetPageLoaderManager::eActiveProcessing:
			if( oNotify.GetNofifyType() == GLoaderRequestNotify::eContentLoadComplete )
			{
				pSecondPageLoder->SetAsActive();
				pFirstPageLoder->SetAsNonActive();
				m_eState = eIdleActive;
			}
			else
			{
				m_eState = eActiveIdle;
			}
		break;

		case GPreSetPageLoaderManager::eProcessingIdle:
			if( oNotify.GetNofifyType() == GLoaderRequestNotify::eContentLoadComplete )
			{
				pFirstPageLoder->SetAsActive();
				m_eState = eActiveIdle;
			}
			else
			{
				m_eState = eIdleIdle;
			}
		break;

		case GPreSetPageLoaderManager::eProcessingActive:
			if( oNotify.GetNofifyType() == GLoaderRequestNotify::eContentLoadComplete )
			{
				pFirstPageLoder->SetAsActive();
				pSecondPageLoder->SetAsNonActive();
				m_eState = eActiveIdle;
			}
			else
			{
				m_eState = eIdleActive;
			}
		break;

		default:
		break;
	}
}

void GPreSetPageLoaderManager::UpdateStateOnNotifyForPreSetRequest(GLoaderRequestNotify &oNotify)
{
	LOG_LEVEL4("UpdateStateOnNotifyForPreSetRequest()");

	GPreSetPageLoader* pFirstPageLoder (static_cast<GPreSetPageLoader*>(m_oPageLoaders[1].data()));
	GPreSetPageLoader* pSecondPageLoder(static_cast<GPreSetPageLoader*>(m_oPageLoaders[2].data()));
	GSetPreloadedPagesRequest* pRequest(static_cast<GSetPreloadedPagesRequest*>(oNotify.GetRequest()));

	switch(m_eState)
	{
		case GPreSetPageLoaderManager::eActiveProcessing:
			if( oNotify.GetNofifyType() == GLoaderRequestNotify::eContentLoadComplete )
			{
				pSecondPageLoder->SetAsActive();
				pFirstPageLoder->SetAsNonActive();

				if( oNotify.GetRequest()->GetRequestId() != pFirstPageLoder->GetLastRequestIdProcessed() )
				{
					pFirstPageLoder->LoadContent(pRequest);
					m_eState = eProcessingActive;
				}
				else
				{
					m_eState = eIdleActive;
				}
			}
			else
			{
				m_eState = eActiveIdle;
			}
		break;

		case GPreSetPageLoaderManager::eProcessingActive:
			if( oNotify.GetNofifyType() == GLoaderRequestNotify::eContentLoadComplete )
			{
				pFirstPageLoder->SetAsActive();
				pSecondPageLoder->SetAsNonActive();

				if( oNotify.GetRequest()->GetRequestId() != pSecondPageLoder->GetLastRequestIdProcessed() )
				{
					pSecondPageLoder->LoadContent(pRequest);
					m_eState = eActiveProcessing;
				}
				else
				{
					m_eState = eActiveIdle;
				}
			}
			else
			{
				m_eState = eIdleActive;
			}
		break;

		case GPreSetPageLoaderManager::eProcessingProcessing:
			if( oNotify.GetNofifyType() == GLoaderRequestNotify::eContentLoadComplete )
			{
				if( oNotify.GetPageLoaderId() == 1 )
				{
					pFirstPageLoder->SetAsActive();
					pSecondPageLoder->SetAsNonActive();
					m_eState = eActiveProcessing;
				}
				else
				{
					pSecondPageLoder->SetAsActive();
					pFirstPageLoder->SetAsNonActive();
					m_eState = eProcessingActive;
				}
			}
			else
			{
				if( oNotify.GetPageLoaderId() == 1 )
				{
					m_eState = eIdleProcessing;
				}
				else
				{
					m_eState = eProcessingIdle;
				}
			}
		break;

		default:
		break;
	}
}
