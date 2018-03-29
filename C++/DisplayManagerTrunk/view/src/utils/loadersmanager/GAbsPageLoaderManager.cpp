#include "GAbsPageLoaderManager.h"

#include "view/src/utils/requestsmanager/GPageLoadRequestManager.h"
#include "view/src/utils/loaders/GAbsPageLoader.h"

#include "utils/logger/src/GLogger.h"

GAbsPageLoaderManager::GAbsPageLoaderManager()
	:	m_oPageLoaders(),
		m_bActive(false)
{
	LOG_LEVEL4("GAbsPageLoaderManager()");
}

GAbsPageLoaderManager::~GAbsPageLoaderManager()
{
	LOG_LEVEL4("~GAbsPageLoaderManager()");
}

void GAbsPageLoaderManager::Init(GPageLoadRequestManager* pObserver)
{
	LOG_LEVEL4("Init()");

	m_pObserver = pObserver;

	InitializePageLoaders();
}

void GAbsPageLoaderManager::Stop()
{
	LOG_LEVEL4("Stop()");

	m_pObserver = 0;

	for( GMapPageLoader::const_iterator qIter = m_oPageLoaders.begin(); (qIter != m_oPageLoaders.end()); ++qIter )
	{
		if( !qIter.value().isNull() )
		{
			qIter.value()->Stop();
		}
	}
}

void GAbsPageLoaderManager::InitializePageLoaders()
{
	LOG_LEVEL4("InitializePageLoaders()");

	for( GMapPageLoader::const_iterator qIter = m_oPageLoaders.begin(); (qIter != m_oPageLoaders.end()); ++qIter )
	{
		if( !qIter.value().isNull() )
		{
			qIter.value()->SetObserver(this);
		}
	}
}

unsigned int GAbsPageLoaderManager::GetActivePageLoaderId() const
{
	LOG_LEVEL4("GetActivePageLoaderId()");

	QSharedPointer<GAbsPageLoader> pActivePageLoader(GetActivePageLoader());

	return (!pActivePageLoader.isNull())?(pActivePageLoader->GetId()):(0);
}

QSharedPointer<GAbsPageLoader> GAbsPageLoaderManager::GetActivePageLoader() const
{
	LOG_LEVEL4("GetActivePageLoader()");

	for( GMapPageLoader::const_iterator qIter = m_oPageLoaders.begin(); (qIter != m_oPageLoaders.end()); ++qIter )
	{
		if( qIter.value()->IsActive() )
		{
			return qIter.value();
		}
	}

	return QSharedPointer<GAbsPageLoader>(0);
}

QSharedPointer<GAbsPageLoader> GAbsPageLoaderManager::GetWaitingForLoadingPagePageLoader() const
{
	LOG_LEVEL4("GetWaitingForLoadingPagePageLoader()");

	for( GMapPageLoader::const_iterator qIter = m_oPageLoaders.begin(); (qIter != m_oPageLoaders.end()); ++qIter )
	{
		if( qIter.value()->IsWaitingForLoadingPage() )
		{
			return qIter.value();
		}
	}

	return QSharedPointer<GAbsPageLoader>(0);
}

QSharedPointer<GAbsPageLoader> GAbsPageLoaderManager::GetFirstNonActivePageLoader() const
{
	LOG_LEVEL4("GetFirstNonActivePageLoader()");

	for( GMapPageLoader::const_iterator qIter = m_oPageLoaders.begin(); (qIter != m_oPageLoaders.end()); ++qIter )
	{
		if( !qIter.value()->IsActive() )
		{
			return qIter.value();
		}
	}

	return QSharedPointer<GAbsPageLoader>(0);
}

unsigned int GAbsPageLoaderManager::GetCurrentSessionId()
{
	LOG_LEVEL4("GetCurrentSessionId()");

	QSharedPointer<GAbsPageLoader> pActivePageLoader(GetActivePageLoader());

	return (!pActivePageLoader.isNull())?(pActivePageLoader->GetCurrentSessionId()):(0);
}
