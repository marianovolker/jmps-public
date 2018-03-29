//! GControlChannelMgr 
/*!
  Control Channel Manager class.
  Why do we receive a pointer to MsgCmdDataFactory ?. We need it for testing purposes.
  It's passed on to ControlChannelTcpServer, and so on to the ControlChannelHandler, when a connection is opened.
  See tests/controlchannel for further details.
*/

#ifndef GCONTROLCHANNELMGR_H_
#define GCONTROLCHANNELMGR_H_

#include <QThread>
#include <QWaitCondition>
#include <QMutex>

#include "GControlChannelTcpServer.h"
#include "control/src/displaymanagermsgs/factory/GMsgCmdDataFactory.h"


class GControlChannelMgr : public QThread
{
	Q_OBJECT
	
public:
	GControlChannelMgr(int iPort, GAbsCmdDataFactory *poMsgCmdDataFactory = GMsgCmdDataFactory::GetInstance() );
	virtual ~GControlChannelMgr();
	
	void run();
	void WaitUntilIsReady();
	int GetNumberOfConnections();
	void Stop();
	void ForceStop();
	
signals:
	void SignalStopChildren();

public slots:
	void SlotChildrenStopped();	
	
private:
	GControlChannelTcpServer* m_oTcpServer;
	GAbsCmdDataFactory *m_poMsgCmdDataFactory;
	QWaitCondition  m_oWaitCondition;
	QMutex m_oMutex;
	int m_iPort;
	bool m_bIsReady;
};

#endif
