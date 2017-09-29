#ifndef GPageLoadRequestManager_H
#define	GPageLoadRequestManager_H

#include <QObject>
#include <QtWebKit/QWebView>
#include <QList>
#include <QMap>
#include <QSharedPointer>
#include <QMutex>

#include "utils/base/src/GException.h"
#include "view/src/utils/loadersmanager/GNonPreSetPageLoaderManager.h"
#include "view/src/utils/loadersmanager/GPreSetPageLoaderManager.h"
#include "view/src/utils/requests/GAbsPageLoadRequest.h"
#include "GPageLoadRequestManagerSession.h"

class GUpdatePreloadedPageRequest;
class GUpdateCurrentPageRequest;
class GUpdateURLCurrentPageRequest;
class GLoadEntirePageRequest;
class GSetPreloadedPagesRequest;
class GLoadersManagerRequestNotify;

class GPageLoadRequestManager:	public QObject
{
	Q_OBJECT

public:

	static QSharedPointer<GPageLoadRequestManager> GetInstance() throw(GException);
	virtual ~GPageLoadRequestManager();

	void Init();
	void Stop();
	unsigned int Subscribe(QString, bool bIsView);
	float GetScaleXFactor() const;
	float GetScaleYFactor() const;
	QString GetViewResolution() const;

public:
	void Notify(GLoadersManagerRequestNotify &oNotify) throw(GException);

protected:
	GPageLoadRequestManager();

private:
	void InitializeMainView();
	unsigned int GetMainViewHeight() const;
	unsigned int GetMainViewWidth() const;
	void SetMainViewWidth(const unsigned int&);
	void SetMainViewHeight(const unsigned int&);
	unsigned int GenerateSessionId();
	unsigned int GenerateRequestId();
	QString GetSessionName(unsigned int) const;
	void ProcessNewRequest();
	void SetAsActive(unsigned int);
	void ProcessRequest(GSetPreloadedPagesRequest*);
	void ProcessRequest(GUpdatePreloadedPageRequest*);
	void ProcessRequest(GLoadEntirePageRequest*);
	void ProcessRequest(GUpdateCurrentPageRequest*);
	void ProcessRequest(GUpdateURLCurrentPageRequest*);


public slots:
	void ReceiveRequest(GSetPreloadedPagesRequest*);
	void ReceiveRequest(GUpdatePreloadedPageRequest*);
	void ReceiveRequest(GLoadEntirePageRequest*);
	void ReceiveRequest(GUpdateCurrentPageRequest*);
	void ReceiveRequest(GUpdateURLCurrentPageRequest*);

private:
	static QSharedPointer<GPageLoadRequestManager> m_pGPageLoadRequestManager;
	static QMutex m_mutex;
	QMutex m_qmInternalMutex;
	unsigned int m_uiMainViewWidth;
	unsigned int m_uiMainViewHeight;
	unsigned int m_uiSessionIdCounter;
	unsigned int m_uiRequestIdCounter;
	GPreSetPageLoaderManager m_oPreSetManager;
	GNonPreSetPageLoaderManager m_oNonPreSetManager;
	QList<GAbsPageLoadRequest*> m_qlsReqs;
	QMap<unsigned int, GPageLoadRequestManagerSession> m_qmSessions;
	QWebView* m_pMainView;

	enum
	{
		CLEGACY_SCREEN_WIDTH_KEY			= 320,
		CLEGACY_SCREEN_HEIGHT_KEY			= 240,
		CMAX_REQUEST_ID						= 999999,
		CNON_PRESET_PAGE_LOADER_MANAGER_ID	= 1,
		CPRESET_PAGE_LOADER_MANAGER_ID		= 2,
		CMAX_SESSION_ID						= 99
	};
};


inline float GPageLoadRequestManager::GetScaleXFactor() const
{
	return (static_cast<float>(GetMainViewWidth())/CLEGACY_SCREEN_WIDTH_KEY);
}

inline float GPageLoadRequestManager::GetScaleYFactor() const
{
	return (static_cast<float>(GetMainViewHeight())/CLEGACY_SCREEN_HEIGHT_KEY);
}

inline QString GPageLoadRequestManager::GetViewResolution() const
{
	return (QString::number(GetMainViewWidth()) + "x" + QString::number(GetMainViewHeight()));
}

inline unsigned int GPageLoadRequestManager::GetMainViewWidth() const
{
	return m_uiMainViewWidth;
}

inline unsigned int GPageLoadRequestManager::GetMainViewHeight() const
{
	return m_uiMainViewHeight;
}

inline void GPageLoadRequestManager::SetMainViewWidth(unsigned int const & uikMainViewWidth)
{
	m_uiMainViewWidth = uikMainViewWidth;
}

inline void GPageLoadRequestManager::SetMainViewHeight(unsigned int const & uikMainViewHeight)
{
	m_uiMainViewHeight = uikMainViewHeight;
}

inline unsigned int GPageLoadRequestManager::GenerateSessionId()
{
	return (m_uiSessionIdCounter<CMAX_SESSION_ID)? (m_uiSessionIdCounter++):(1);
}

inline unsigned int GPageLoadRequestManager::GenerateRequestId()
{
	return (m_uiRequestIdCounter<CMAX_REQUEST_ID)? (m_uiRequestIdCounter++):(1);
}

inline QString GPageLoadRequestManager::GetSessionName(unsigned int uiSessionId) const
{
	return m_qmSessions.contains(uiSessionId)? (m_qmSessions[uiSessionId].GetName()):(QString(""));
}

#endif
