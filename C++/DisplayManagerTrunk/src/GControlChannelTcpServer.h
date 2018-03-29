//! GControlChannelTcpServer 
/*!
  Control Channel Custom TCP Server class
*/

#ifndef GCONTROLCHANNELTCPSERVER_H_
#define GCONTROLCHANNELTCPSERVER_H_

#include <QTcpServer>
#include <QList>
#include <QMutex>
#include <QMutexLocker>
#include "GControlChannelHandler.h"
#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"


class GControlChannelTcpServer : public QTcpServer
{	
	Q_OBJECT

public:
	GControlChannelTcpServer(QObject *poParent, GAbsCmdDataFactory *poMsgCmdDataFactory);
	virtual ~GControlChannelTcpServer();
	int GetNumberOfConnections();
	
protected:
	void incomingConnection(int iSocketDescriptor);
	
signals:
	void SignalStopChildren();
	void SignalTcpServerFinished();

public slots:
	void SlotStop();
	void SlotChildrenStopped(GControlChannelHandler* poHandler);
	
private:
	GLogger* m_poLogger;
	QList<GControlChannelHandler *> m_oList;
	QMutex* m_poMux;
	GAbsCmdDataFactory *m_poMsgCmdDataFactory;	
	bool m_bStopSignalReceived;
};

#endif
