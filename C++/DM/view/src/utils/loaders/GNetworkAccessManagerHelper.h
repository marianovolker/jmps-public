#ifndef GNetworkAccessManagerHelper_H
#define	GNetworkAccessManagerHelper_H

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkProxy>

class GNetworkAccessManagerHelper : public QNetworkAccessManager
{
	Q_OBJECT

public:
	GNetworkAccessManagerHelper(QNetworkAccessManager *manager, QObject *parent);
	virtual ~GNetworkAccessManagerHelper();

private slots:
    void finishedSlot( QNetworkReply* );

private:
	virtual QNetworkReply *createRequest(QNetworkAccessManager::Operation operation, const QNetworkRequest &request, QIODevice *device);
};




class QNetworkReplyHelper : public QNetworkReply
{
public:
    QNetworkReplyHelper();
    virtual ~QNetworkReplyHelper();

    virtual void abort();
    virtual void setError(NetworkError errorCode, const QString & errorString);
    qint64 readData(char *data, qint64 maxSize);
};

inline QNetworkReplyHelper::QNetworkReplyHelper() : QNetworkReply()
{

}

inline QNetworkReplyHelper::~QNetworkReplyHelper()
{

}

inline void QNetworkReplyHelper::abort()
{
	return;
}

inline void QNetworkReplyHelper::setError(NetworkError errorCode, const QString & errorString)
{
	QNetworkReply::setError(errorCode,errorString);
}

#endif
