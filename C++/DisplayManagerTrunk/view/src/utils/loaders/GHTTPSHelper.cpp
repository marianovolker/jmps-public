#include "GHTTPSHelper.h"

#include <QtNetwork/QSslConfiguration>
#include <QtNetwork/QSslSocket>
#include <QtNetwork/QSslError>
#include <QtNetwork/QNetworkReply>
#include <QByteArray>
#include <QFile>

#include "GNetworkAccessManagerHelper.h"

#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"

GHTTPSHelper::GHTTPSHelper()
	:	m_pPage(0),
		m_pNetAccess(0)
{
	LOG_LEVEL4("GHTTPSHelper()");
}

GHTTPSHelper::~GHTTPSHelper()
{
	LOG_LEVEL4("~GHTTPSHelper()");
}

void GHTTPSHelper::Init(QWebPage* pPage)
{
	LOG_LEVEL3("Init()");

	if( pPage != 0 )
	{
		m_pPage = pPage;
		m_pNetAccess = new GNetworkAccessManagerHelper(m_pPage->networkAccessManager(), this);
		m_pPage->setNetworkAccessManager(m_pNetAccess);
	}

	ConnectSignals();
	LoadSSLCertificates();
}

void GHTTPSHelper::Stop()
{
	LOG_LEVEL3("Stop()");

	DisconnectSignals();

	delete m_pNetAccess;

	m_pNetAccess = 0;

	m_pPage = 0;
}

void GHTTPSHelper::LoadSSLCertificates()
{
	LOG_LEVEL3("LoadSSLCertificates()");

	QByteArray certByteArray;

	QFile file(GETCONF_STR(eDisplayManagerPathForCertificateFile));

	if( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		LOG_LEVEL1("Failed to open certificate file");
		return;
	}
	else
	{
		while( !file.atEnd() )
		{
			certByteArray += file.readLine();
		}
	}

	if( certByteArray.isNull() || certByteArray.isEmpty() )
	{
		LOG_LEVEL1("Invalid certificate string");
		return;
	}

	QSslCertificate caCert = QSslCertificate(certByteArray, QSsl::Pem);
	if( caCert.isNull() )
	{
		LOG_LEVEL1("Invalid caCert certificate");
		return;
	}
	else
	{
		LOG_LEVEL4(QString("SSL Certificate.issuerInfo(%1)").arg(caCert.issuerInfo(QSslCertificate::Organization)));
	}

	QSslSocket::addDefaultCaCertificate(caCert);
}

void GHTTPSHelper::ConnectSignals()
{
	LOG_LEVEL3("ConnectSignals()");

	if( !connect(m_pNetAccess, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),
				 this, SLOT(GetSSLErrors(QNetworkReply*, const QList<QSslError>&))) )
	{
		LOG_LEVEL1("Can't connect GetSSLErrors() to QNetworkAccessManager object.");
	}
}

void GHTTPSHelper::DisconnectSignals()
{
	LOG_LEVEL3("DisconnectSignals()");

	if( !disconnect(m_pNetAccess, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),
					this, SLOT(GetSSLErrors(QNetworkReply*, const QList<QSslError>&))) )
	{
		LOG_LEVEL1("Can't disconnect GetSSLErrors() from QNetworkAccessManager object.");
	}
}

void GHTTPSHelper::GetSSLErrors(QNetworkReply *reply, const QList<QSslError> &qlErrors)
{
	LOG_LEVEL3("GetSSLErrors()");

	for (int i = 0; i < qlErrors.count(); ++i)
	{
		LOG_LEVEL1( QString("SSL error = (%1)").arg(qlErrors.at(i).errorString()) );
	}

	LOG_LEVEL4(QString("The access to (") + reply->url().toString() + ") has produced SSL errors.");
}
