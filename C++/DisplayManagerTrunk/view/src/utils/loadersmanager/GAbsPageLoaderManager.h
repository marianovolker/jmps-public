#ifndef GAbsPageLoaderManager_H
#define	GAbsPageLoaderManager_H

#include <QSharedPointer>

#include "view/src/utils/loaders/GLoaderRequestNotify.h"
#include "view/src/utils/loaders/GAbsPageLoader.h"

class GPageLoadRequestManager;

class GAbsPageLoaderManager
{
public:
	GAbsPageLoaderManager();
	virtual ~GAbsPageLoaderManager();

	void Init(GPageLoadRequestManager* pObserver);
	void Stop();

	unsigned int GetActivePageLoaderId() const;
	QSharedPointer<GAbsPageLoader> GetActivePageLoader() const;
	QSharedPointer<GAbsPageLoader> GetWaitingForLoadingPagePageLoader() const;
	QSharedPointer<GAbsPageLoader> GetFirstNonActivePageLoader() const;
	unsigned int GetCurrentSessionId();
	unsigned int GetId() const;
	void SetId( unsigned int );
	void SetAsActive();
	void SetAsNonActive();
	bool IsActive() const;

	virtual void CreatePageLoaders(unsigned int uiPageLoadersQty) = 0;
	virtual void Notify(GLoaderRequestNotify &oNotify) = 0;
	virtual bool CanProcessRequest(GAbsPageLoadRequest* pRequest) = 0;

protected:
	void InitializePageLoaders();

protected:
	GPageLoadRequestManager* m_pObserver;
	typedef QMap<unsigned int, QSharedPointer<GAbsPageLoader> > GMapPageLoader;
	GMapPageLoader m_oPageLoaders;
	unsigned int m_uiId;
	bool m_bActive;
};

inline unsigned int GAbsPageLoaderManager::GetId() const
{
	return m_uiId;
}

inline void GAbsPageLoaderManager::SetId( unsigned int uiId )
{
	m_uiId = uiId;
}

inline void GAbsPageLoaderManager::SetAsActive()
{
	m_bActive = true;
}

inline void GAbsPageLoaderManager::SetAsNonActive()
{
	m_bActive = false;
}

inline bool GAbsPageLoaderManager::IsActive() const
{
	return m_bActive;
}

#endif

