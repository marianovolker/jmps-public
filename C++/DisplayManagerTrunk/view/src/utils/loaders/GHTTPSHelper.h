#ifndef GHTTPSHelper_H
#define	GHTTPSHelper_H

#include <QtNetwork/QNetworkAccessManager>
#include <QWebPage>
#include <QList>

#include "GNetworkAccessManagerHelper.h"

class GHTTPSHelper : public QObject
{
	Q_OBJECT

public:
	GHTTPSHelper();
	virtual ~GHTTPSHelper();

	void Init(QWebPage* pPage);
	void Stop();

private:
	void LoadSSLCertificates();
	void ConnectSignals();
	void DisconnectSignals();

public slots:
    void GetSSLErrors(QNetworkReply *reply, const QList<QSslError> &error);

private:
    QWebPage* m_pPage;
    GNetworkAccessManagerHelper* m_pNetAccess;
};

#endif
