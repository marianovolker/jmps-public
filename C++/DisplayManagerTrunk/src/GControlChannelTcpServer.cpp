#include "GControlChannelTcpServer.h"

#include "config/GDisplayManagerConfig.h"


GControlChannelTcpServer::GControlChannelTcpServer(QObject *poParent, GAbsCmdDataFactory *poMsgCmdDataFactory)
	: m_poLogger( GLogManager::GetDefaultLogger() ),
	  m_poMsgCmdDataFactory( poMsgCmdDataFactory ),
	  m_bStopSignalReceived(false)
{
	LOG_LEVEL4("GControlChannelTcpServer()");

	m_poMux = new QMutex();
	if( !connect(poParent, SIGNAL(SignalStopChildren()), this, SLOT(SlotStop()), Qt::QueuedConnection) )
	{
		LOG_LEVEL1("Can't connect Control Channel Mgr to Control Channel TcpServer.");
	}
}

GControlChannelTcpServer::~GControlChannelTcpServer()
{
	LOG_LEVEL4("~GControlChannelTcpServer()");
	
	delete m_poMux;
	m_poMux = NULL;
}

void GControlChannelTcpServer::incomingConnection(int iSocketDescriptor)
{
	LOG_LEVEL4("incomingConnection()");

	m_poMux->lock();
	
	bool bCreateNewConnection(false);

	if( m_oList.size() <= 0 )
	{
		bCreateNewConnection=true;
	}
	else
	{
		if( GETCONF_BOOL(eDisplayManagerMultipleConnections) )
		{
			bCreateNewConnection=true;
		}
	}

	if(bCreateNewConnection)
	{
		LOG_LEVEL3("Accepting new connection");

		GControlChannelHandler *poThread = new GControlChannelHandler(iSocketDescriptor, this, m_poMsgCmdDataFactory);
		m_oList.append(poThread);
		poThread->start();

		if( !connect( poThread, SIGNAL(SignalHandlerStopped(GControlChannelHandler*)),
					  this, SLOT(SlotChildrenStopped(GControlChannelHandler*)), Qt::QueuedConnection) )
		{
			LOG_LEVEL1("Can't connect Control Channel Handler to Control Channel TcpServer.");
		}
	}
	else
	{
		LOG_LEVEL3("Rejecting new connection because there is already one");
		::close(iSocketDescriptor);
		LOG_LEVEL3("Closing new connection");
	}
	
	m_poMux->unlock();
}

int GControlChannelTcpServer::GetNumberOfConnections()
{
	LOG_LEVEL4("GetNumberOfConnections()");

	m_poMux->lock();	
	int iSize = m_oList.size();
	m_poMux->unlock();
	
	return iSize;
}

void GControlChannelTcpServer::SlotStop()
{
	LOG_LEVEL4("SlotStop()");

	QMutexLocker oLock(m_poMux);	
	
	close();	
	
	if( m_oList.isEmpty() )
	{
		LOG_LEVEL4("All Sockets have Successfully Finished...");
		emit SignalTcpServerFinished();
	}
	else
	{
		m_bStopSignalReceived = true;
		emit SignalStopChildren();
	}
}

void GControlChannelTcpServer::SlotChildrenStopped(GControlChannelHandler* poHandler)
{
	LOG_LEVEL3("SlotChildrenStopped()");

	QMutexLocker oLock(m_poMux);
	
	m_oList.removeOne(poHandler);
	delete poHandler;
	
	if(m_oList.isEmpty() && m_bStopSignalReceived)
	{
		LOG_LEVEL4("All Sockets have Successfully Finished...");
		emit SignalTcpServerFinished();
	}
}
