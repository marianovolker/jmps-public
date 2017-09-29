#include "GNetworkAccessManagerHelper.h"

#include "utils/base/src/GException.h"
#include "utils/logger/src/GLogger.h"
#include "config/GDisplayManagerConfig.h"

#include <QNetworkDiskCache>
#include <QVariant>
#include <QIODevice>
#include <QUrl>
#include <QDateTime>

GNetworkAccessManagerHelper::GNetworkAccessManagerHelper(QNetworkAccessManager *manager, QObject *parent)
    : QNetworkAccessManager(parent)
{
    LOG_LEVEL4("GNetworkAccessManagerHelper()");

    setCache(manager->cache());
    setCookieJar(manager->cookieJar());
    setProxy(manager->proxy());
    setProxyFactory(manager->proxyFactory());

 	if( GETCONF_BOOL(eDisplayManagerEnableAdvancedCache) )
 	{
 		QString sCacheDirectory ( (!GETCONF_STR(eDisplayManagerCacheLocation).isEmpty())?
											(GETCONF_STR(eDisplayManagerCacheLocation))
											:
											("/tmp/")
								);
 		QNetworkDiskCache *pDiskCache = new QNetworkDiskCache(this);
 		pDiskCache->setMaximumCacheSize(GETCONF_NUM(eDisplayManagerMaximumCacheSize));
		pDiskCache->setCacheDirectory(sCacheDirectory);
		setCache(pDiskCache);
 		LOG_LEVEL3(QString("Setting advanced cache to location (") + sCacheDirectory  + ")" );
		LOG_LEVEL3(QString("Setting max cache size in (") + QString("%1").arg(pDiskCache->maximumCacheSize()) + ")" );

 		connect( this, SIGNAL( finished( QNetworkReply* ) ), this, SLOT( finishedSlot( QNetworkReply* ) ) );
 	}
}

GNetworkAccessManagerHelper::~GNetworkAccessManagerHelper()
{
	LOG_LEVEL4("~GNetworkAccessManagerHelper()");

	disconnect(this, 0, 0, 0);
}

QNetworkReply *GNetworkAccessManagerHelper::createRequest(QNetworkAccessManager::Operation operation, const QNetworkRequest &request, QIODevice *device)
{
	LOG_LEVEL3("createRequest()");

	QNetworkRequest oRequest(request);

	if( GETCONF_BOOL(eDisplayManagerHTTPSSupport)	&&
		 request.url().scheme() != "https"				&&
		 request.url().scheme() != "file"				&&
		 request.url().scheme() != "about" 				 )
	{
		QString sTemp(QString("Discarding url request with an invalid scheme [") + request.url().scheme() + "]");
		LOG_LEVEL1( sTemp );

		QNetworkReplyHelper* pReplyHelper(new QNetworkReplyHelper());
		pReplyHelper->setError(QNetworkReply::ContentAccessDenied, sTemp);
		return pReplyHelper;
	}

	if( GETCONF_BOOL(eDisplayManagerEnableAdvancedCache) )
	{
		oRequest.setAttribute( QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache );
		oRequest.setAttribute( QNetworkRequest::CacheSaveControlAttribute, true );

		LOG_LEVEL4( QString("url-requested: (") + oRequest.url().toString() + ")" );
	}

	return QNetworkAccessManager::createRequest(operation, oRequest, device);
}

void GNetworkAccessManagerHelper::finishedSlot( QNetworkReply* reply )
{
	// Get and display the last modified header
	QVariant lmod = reply->header( QNetworkRequest::LastModifiedHeader );
	if( lmod.isValid() )
	{
		QDateTime lmodDate = lmod.toDateTime();
		QString lmodStr = lmodDate.toString( "dd.MM.yyyy" );
		LOG_LEVEL4(QString("url-requested-reply: Last Modified (%1), url(%2)").arg(lmodStr).arg(reply->url().toString()));
	}

	// Show expiry date
	if( reply->hasRawHeader( "Expires" ) )
	{
		QString expires = reply->rawHeader( "Expires" );
		LOG_LEVEL4(QString("url-requested-reply: Expiry date (%1), url(%2)").arg(expires).arg(reply->url().toString()));
	}

	// Show Cache-Control header
	if( reply->hasRawHeader( "Cache-Control" ) )
	{
		QString cCntrl = reply->rawHeader( "Cache-Control" );
		LOG_LEVEL4(QString("url-requested-reply: Cache-Control (%1), url(%2)").arg(cCntrl).arg(reply->url().toString()));
	}

	// Check if the data was obtained from cache or not
	QVariant fromCache = reply->attribute( QNetworkRequest::SourceIsFromCacheAttribute );
	if( fromCache.toBool() )
	{
		LOG_LEVEL4(QString("url-requested-reply: Data is from cache (%1)").arg(reply->url().toString()));
	}
	else
	{
		LOG_LEVEL4(QString("url-requested-reply: Data is from network (%1)").arg(reply->url().toString()));
	}

	if( reply->error() != QNetworkReply::NoError )
	{
		LOG_LEVEL4(QString("url-requested-reply: Error loading element (%1), the error string is (%2)").arg(reply->url().toString()).arg(reply->errorString()));
	}
}

qint64 QNetworkReplyHelper::readData(char *data, qint64 maxSize)
{
	LOG_LEVEL4(QString("readData(), data[") + data + "], [" + QString("%1").arg(maxSize) + "].");

	return -1;
}
