#ifndef GAbsPageLoader_H
#define	GAbsPageLoader_H

#include <QObject>
#include <QWebPage>
#include <QTimer>

#include "GJavaScriptDynamicOnLoadSignal.h"
#include "GLoaderRequestNotify.h"
#include "GJSHelper.h"
#include "GHTTPSHelper.h"

class GAbsPageLoaderManager;

class GAbsPageLoader : public QObject
{
	Q_OBJECT

public:
    GAbsPageLoader(unsigned int uiId, GAbsPageLoaderManager *pObserver=0);
    virtual ~GAbsPageLoader();

    void Init();
    void Stop();
    
public:
    GAbsPageLoaderManager *GetObserver();
    unsigned int GetId() const;
    QWebPage * GetWebPage (void);
    void SetObserver(GAbsPageLoaderManager *pObserver);
    GAbsPageLoadRequest* GetRequest();
    unsigned int GetCurrentSessionId() const;
    unsigned int GetLastRequestIdProcessed() const;

public:
    void SetAsActive();
    void SetAsNonActive();
    bool IsActive() const;
	void SetAsWaitingForLoadingPage();
	void SetAsNotWaitingForLoadingPage();
	bool IsWaitingForLoadingPage() const;
	void Notify(GLoaderRequestNotify::eNotifyTypeEnum eType);

protected:
    void ConnectSignals();
    void DisconnectSignals();
    void InitializePage();

    virtual void DoCompleted() = 0;
    virtual void DoTimeout() = 0;

protected slots:
    void LoadStarted ();
    void LoadProgress(int iProgress);
    void LoadFinished(bool bVal);
    void Completed();
    void TimeoutLoadingPage();
    void PopulateJavaScriptWindowObject();

protected:

    enum { CTIMEOUT_LOADING_PAGE=5000, CTIMEOUT_EXECUTING_JS=60000 };

    GAbsPageLoaderManager* m_pObserver;
    unsigned int m_iId;
    bool m_bWaitingForLoadingPage;
    bool m_bPageActive;

    QWebPage* m_pPage;
    QTimer m_oTimer;

    GJSHelper m_jsHelper;
    GJavaScriptDynamicOnLoadSignal m_jsSignal;
    GAbsPageLoadRequest* m_pRequest;
    unsigned int m_uiCurrentSessionId;
    unsigned int m_uiLastRequestIdProcessed;
    GHTTPSHelper m_oHTTPSHelper;
};



inline GAbsPageLoaderManager *GAbsPageLoader::GetObserver()
{
    return m_pObserver;
}

inline void GAbsPageLoader::SetObserver(GAbsPageLoaderManager *pObserver)
{
    m_pObserver = pObserver;
}

inline unsigned int GAbsPageLoader::GetId() const
{
    return m_iId;
}

inline QWebPage * GAbsPageLoader::GetWebPage (void)
{
    return m_pPage;
}

inline bool GAbsPageLoader::IsActive() const
{
    return m_bPageActive;
}

inline bool GAbsPageLoader::IsWaitingForLoadingPage() const
{
    return m_bWaitingForLoadingPage;
}

inline void GAbsPageLoader::SetAsWaitingForLoadingPage()
{
	m_bWaitingForLoadingPage = true;
}

inline void GAbsPageLoader::SetAsNotWaitingForLoadingPage()
{
	m_bWaitingForLoadingPage = false;
}

inline GAbsPageLoadRequest* GAbsPageLoader::GetRequest()
{
	return m_pRequest;
}

inline unsigned int GAbsPageLoader::GetCurrentSessionId() const
{
    return m_uiCurrentSessionId;
}

inline unsigned int GAbsPageLoader::GetLastRequestIdProcessed() const
{
	return m_uiLastRequestIdProcessed;
}

#endif
