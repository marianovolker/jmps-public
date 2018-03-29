/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccServerManager.cpp
 * @author gparis
 * @date Wednesday, April 29, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <Poco/ScopedLock.h>
#include <gvrlog/log.h>
#include <gvr/communication/spot/Utils.h>

#include "../include/UccServerManager.h"

DEFINE_LOG_MODULE(srvmgr);


/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace cloud
{

using namespace gvr::communication;
using gvr::communication::spot::Utils;

typedef Poco::ScopedLock<Poco::FastMutex>   ScopedLock; ///< @brief Automatic scope life mutex.


//============================= Class UccServerManager =============================

UccServerManager::UccServerManager (IChannelServer & server)
 : m_channelServer(server), m_mutex(), m_qConnection(),
   m_channelDisconnector(m_qConnection, m_mutex),
   m_connectionListener(m_qConnection, m_channelDisconnector, m_mutex)
{
  LOG_Trace(srvmgr, "Starting UccServerManager...");
  m_channelServer.subscribe(m_connectionListener);
  LOG_Notice(srvmgr, "UccServerManager started. Server channel subscripted.");
}

UccServerManager::~UccServerManager (void)
{
  LOG_Trace(srvmgr, "Stopping UccServerManager...");
  m_channelServer.unsubscribe(m_connectionListener);

  ScopedLock lock(m_mutex);
  if(m_qConnection)
  {
    LOG_Trace(srvmgr, "Stopping worker...");
    m_qConnection->getWorker().stop();
    LOG_Trace(srvmgr, "Closing connection...");
    m_qConnection->close();
    m_qConnection = NULL;
    LOG_Trace(srvmgr, "Connection closed.");
  }
  LOG_Notice(srvmgr, "UccServerManager stopped. Server channel unsubscripted.");
}

ErrorType  UccServerManager::send (const ucc::IUccMessage & messageReq)
{
  ScopedLock lock(m_mutex);

  Connection* pc = m_qConnection.get();

  if(pc == NULL) return ucc::UccSession::UccSessionError::CErrorNoChannel; // Connection has just been closed.

  ucc::UccSessionServer* ps = pc->getServerSession();

  if(ps == NULL) return ucc::UccSession::UccSessionError::CErrorNoUccSessionAvailable;// Can also be NULL when closing connection.

  return ps->send( messageReq );
}


//============================= Inner Class Connection =============================

UccServerManager::Connection::Connection (const Socket & socket, ChannelDisconnector & disconnector)

  : m_blOpen(false), m_uccSessionServer(), m_channelDisconnector(disconnector), m_uccChannel(socket), m_worker(&m_uccSessionServer)
{
  LOG_Trace(srvmgr, "Subscribing channel for disconnection events...");
  m_uccChannel.subscribe(disconnector);
  LOG_Trace(srvmgr, "Subscribing worker to the server session...");
  // The following line does automatically this one:  m_uccSessionServer.subscribe(m_worker);
  m_worker.start(); // This calls doSubscriptions() via AbsOPTCloudClientWorker.run()
  LOG_Trace(srvmgr, "Starting server session...");
  m_uccSessionServer.start(m_uccChannel);
  m_blOpen = true;
  LOG_Notice(srvmgr, "Server session started. Connection is open.");
}

UccServerManager::Connection::~Connection (void)
{
  if(m_blOpen) close();
}


ucc::UccSessionServer*  UccServerManager::Connection::getServerSession (void)
{
  return & m_uccSessionServer;
}

UccAnyMessageWorker& UccServerManager::Connection::getWorker (void)
{
  return m_worker;
}

void UccServerManager::Connection::close (void)
{
  LOG_Trace(srvmgr, "Unsubscribing worker to the server session...");
  m_worker.doUnsubscriptions();
  LOG_Trace(srvmgr, "Stopping worker ...");
  m_worker.stop();
  LOG_Trace(srvmgr, "Unsubscribing channel for disconnection events...");
  m_uccChannel.unsubscribe(m_channelDisconnector);
  LOG_Trace(srvmgr, "Stopping server session...");
  m_uccSessionServer.stop();
  m_blOpen = false;
  LOG_Notice(srvmgr, "Server session stopped. Connection is closed.");
}


//============================= Inner Class ChannelDisconnector =============================

UccServerManager::ChannelDisconnector::ChannelDisconnector(Poco::SharedPtr<Connection> & qConnection,
                                                           Poco::FastMutex & mutex)
: m_qConnection(qConnection), m_mutex(mutex)
{
}

void UccServerManager::ChannelDisconnector::channelEvent (BufferType const & evtBuffer)
{
  unsigned int uBinaryLength = evtBuffer.size();
  char * pcHexText = new char[3 * uBinaryLength + 2];

  LOG_VTrace( srvmgr, "ChannelDisconnector got this event buffer:\n%s\n\n",
                      Utils::dumpBuffer(pcHexText, evtBuffer) );
  delete pcHexText;
}

void UccServerManager::ChannelDisconnector::channelEvent (StatusType status)
{
  LOG_Trace( srvmgr, "ChannelDisconnector got a status event from the channel:");
  LOG_VTrace( srvmgr, "ChannelDisconnector: Status code: %02Xh, connection: %p\n", status, m_qConnection.get() );

  if( m_qConnection && (status == UccChannel::IChannelStatus::CStatusDisconnected) )
  {
    LOG_Trace( srvmgr, "ChannelDisconnector is going to close the connection...");
    m_qConnection->close();
    m_qConnection = NULL;
    LOG_Debug( srvmgr, "ChannelDisconnector closed the connection.");
  }
}

void UccServerManager::ChannelDisconnector::disconnect()
{
  if( m_qConnection )
  {
    LOG_Trace( srvmgr, "ChannelDisconnector is going to close the connection...");
    m_qConnection->close();
    m_qConnection = NULL;
    LOG_Debug( srvmgr, "ChannelDisconnector closed the connection.");
  }

}

//============================= Inner Class ConnectionListener =============================

UccServerManager::ConnectionListener::ConnectionListener(Poco::SharedPtr<Connection> & qConnection,
                                                         ChannelDisconnector & disconnector,
                                                         Poco::FastMutex & mutex)

: m_qConnection(qConnection), m_channelDisconnector(disconnector), m_mutex(mutex)
{
}

void UccServerManager::ConnectionListener::channelStartedEvent (const Socket & socket)
{
  ScopedLock lock(m_mutex);

  if (!m_qConnection)
  {
    m_qConnection = new Connection(socket, m_channelDisconnector);
    LOG_VDebug( srvmgr, "ConnectionListener opened a new connection %s initiated from %s.\n",
                socket.address().toString().c_str(), socket.peerAddress().toString().c_str() );
  }
  else // this server is designed for only one connection.
  {
    LOG_VTrace
    (
     srvmgr,
     "ConnectionListener discarded a new connection %s, initiated from %s, because the only connection is already open.\n",
     socket.address().toString().c_str(),
     socket.peerAddress().toString().c_str()
    );

    Socket socketAux(socket);
    socketAux.shutdown();
    socketAux.close();
  }
}



} // Communication Domain

}// Gilbarco Domain


