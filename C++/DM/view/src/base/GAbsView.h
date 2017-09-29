#ifndef GAbsView_H_
#define GAbsView_H_

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMutex>
#include <QWaitCondition>

class GUpdatePreloadedPageRequest;
class GUpdateCurrentPageRequest;
class GUpdateURLCurrentPageRequest;
class GLoadEntirePageRequest;
class GSetPreloadedPagesRequest;

class GAbsView : public QObject
{
	Q_OBJECT

public:
	GAbsView();
	virtual ~GAbsView();

public:
	virtual void Update(bool) = 0;
	static void SignalSynchronize(bool);

protected:
	void Init();
	void GetScaledXY( int& iXPos, int& iYPos ) const;
	void GetScaledWidthAndHeight( int& iWidth, int& iHeight ) const;
	void SetView(bool bSynchronize, QStringList& oStrListJSCalls, QStringList& oStrListJSLibraries, QString& strURL, bool bApplyZoomFactor = false);
	void SetViewContent(bool bSynchronize, QStringList& oStrListJSCalls);
	void UpdateViewContent(bool bSynchronize, QStringList& oStrListJSCalls);
	void UpdateURLViewContent(bool bSynchronize, QStringList& oStrListJSCalls);

private:
	int m_iSessionId;
	static bool _bSynchWaiting;
	static bool _bSyncResult;
	static QMutex _mtxSynchMutex;
	static QWaitCondition _cndSynchConditional;

protected:
	QString m_sName;

signals:
    void SignalSendRequest(GUpdatePreloadedPageRequest* pReq);
    void SignalSendRequest(GUpdateCurrentPageRequest* pReq);
    void SignalSendRequest(GUpdateURLCurrentPageRequest* pReq);
    void SignalSendRequest(GSetPreloadedPagesRequest* pReq);
    void SignalSendRequest(GLoadEntirePageRequest* pReq);
};

#endif
