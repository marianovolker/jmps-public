//! GControlChannelHandler 
/*!
  Control Channel Handler. It's main purpose is to receive messages, split them in XML's, and dispatch the
  chopped messages to the upper layers.
*/

#ifndef GControlChannelHandler_H_
#define GControlChannelHandler_H_

#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#include <QByteArray>

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"
#include "utils/base/src/GAbsCmdDataFactory.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"
#include "control/src/displaymanagermsgs/xml/GResponseMsgDataXML.h"
	

class GControlChannelHandler : public QThread
{
	Q_OBJECT

public:
	GControlChannelHandler(int iSocketDescriptor, QObject *poParent, GAbsCmdDataFactory *poMsgCmdDataFactory);
	virtual ~GControlChannelHandler();
	void run();

private:
	inline bool IsReady();

	inline void Dispatch();	
	inline void SendResponse(QByteArray & oResponse);
	inline void SendErrorResponse(GResponseMsgData::eResponseCode eResponseCode, QString strErrorMsg);
	void starttrimmed(QByteArray &oBuffer);

public slots:
	void SlotStop();
	void SlotThreadFinished();
	
protected slots:
    void ReadMessage();	

signals:
	void SignalHandlerStopped(GControlChannelHandler* poHandler);
	
private:
	QTcpSocket* m_poTcpSocket;
	int m_iSocketDescriptor;	
	QString m_strCommandName;
	bool bCommandNameFound;
	QByteArray m_oBuffer;
	GAbsCmdDataFactory* m_poMsgCmdDataFactory;
	QObject* m_poParent;

	static const QByteArray CXMLStartTag;
	static const QByteArray CXMLDeclarationEndTag;
	static const QString CXMLCommandEndTag;
};

#endif
