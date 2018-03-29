#include "GControlChannelMgr.h"

#include "utils/logger/src/GLogger.h"
#include "utils/logger/src/GLogConfig.h"
#include "utils/logger/src/GLogManager.h"

GControlChannelMgr::GControlChannelMgr(int iPort, GAbsCmdDataFactory *poMsgCmdDataFactory)
	: m_oTcpServer(NULL),
	  m_poMsgCmdDataFactory( poMsgCmdDataFactory ),	  
	  m_iPort(iPort),
	  m_bIsReady(false)
{	
	LOG_LEVEL4("GControlChannelMgr()");

	moveToThread(this);
}

GControlChannelMgr::~GControlChannelMgr()
{
	LOG_LEVEL4("~GControlChannelMgr()");
	
	if( m_oTcpServer != NULL )
	{
		delete m_oTcpServer;
		m_oTcpServer = NULL;
	}
}

void GControlChannelMgr::run()
{
	LOG_LEVEL4("run()");

	LOG_LEVEL3("Starting ControlChannel Manager thread");

	m_oTcpServer = new GControlChannelTcpServer(this, m_poMsgCmdDataFactory);
	if( !connect(m_oTcpServer, SIGNAL(SignalTcpServerFinished()), this, SLOT(SlotChildrenStopped()), Qt::QueuedConnection) )
	{
		LOG_LEVEL1("Can't connect Control Channel TcpServer to Control Channel Mgr.");
	}
	
	if( m_oTcpServer->listen(QHostAddress::Any, m_iPort) )
	{	
		LOG_LEVEL3(QString("GControlChannelTcpServer is listening on port: ") + QString::number(m_iPort));

		m_oMutex.lock();
		m_bIsReady = true;
		m_oWaitCondition.wakeAll();
		m_oMutex.unlock();
		
		exec();
	}

	LOG_LEVEL4("The event loop of this thread has been finished");
}

//! Blocking call to wait, until the TcpServer is listening. For testing purposes.
void GControlChannelMgr::WaitUntilIsReady()
{
	LOG_LEVEL4("WaitUntilIsReady()");

	m_oMutex.lock();

	while (!m_bIsReady)
	{
		m_oWaitCondition.wait(&m_oMutex);
	}
	
	m_oMutex.unlock();

	LOG_LEVEL3("GControlChannelMgr is ready");
}

int GControlChannelMgr::GetNumberOfConnections()
{
	LOG_LEVEL4("GetNumberOfConnections()");

	return m_oTcpServer->GetNumberOfConnections();
}

void GControlChannelMgr::Stop()
{
	LOG_LEVEL4("Stop()");

	emit SignalStopChildren();
}

void GControlChannelMgr::ForceStop()
{
	LOG_LEVEL4("ForceStop()");

	emit SignalStopChildren();

	quit();
}

void GControlChannelMgr::SlotChildrenStopped()
{
	LOG_LEVEL4("SlotChildrenStopped()");

	quit();
}

