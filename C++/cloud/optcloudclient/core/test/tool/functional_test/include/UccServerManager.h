/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccServerManager.h
 * @author gparis
 * @date Tuesday, April 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef UCCSERVERMANAGER_H_
#define UCCSERVERMANAGER_H_

#include <Poco/Mutex.h>
#include <Poco/SharedPtr.h>

#include <gvr/communication/StreamChannelServer.h>
#include <gvr/communication/IChannelListener.h>
#include <gvr/communication/IChannelServerListener.h>
#include <gvr/communication/ucc/message/base/IUccMessage.h>
#include <gvr/communication/ucc/session/UccSessionServer.h>

#include "../include/UccAnyMessageWorker.h"

//#include <CloudStatus.h>


/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace cloud
{


/**
 * @brief This class is the administrator between the UCC server session, the server stream channel,
 *        the connection to a client and the UccAnyMessageWorker worker.
 *
 * @see Poco::Net::StreamSocket
 * @see Poco::Runnable
 * @see Poco::Thread
 *
 * @since 0.0.0, 2015-04-28
 * @version 0.0.0, 2015-04-28
 * @author gparis
 */
class UccServerManager
{
public:

  typedef Poco::Net::StreamSocket             Socket; ///< @brief The socket class the channels use.

  /**
   * @brief  This constructor takes an already created server channel
   */
  UccServerManager (gvr::communication::IChannelServer & channelServer);

  /**
   * @brief  This destructor unsubscribes the listeners and closes the connection.
   */
  virtual ~UccServerManager (void);

  /**
   * @brief Sends the request message via the internal server session, in just one
   *        atomic operation. Client side disconnection is also checked to prevent race conditions.
   */
  ErrorType  send (const gvr::communication::ucc::IUccMessage & messageReq);

  void stop(); // TODO: Implement this stopping the worker !!!


protected:

  class ChannelDisconnector; // Type advance declaration.

  /**
   * @brief This class is in charge to initialize the channel and to start the server session when an incoming
   *        connection is requested. Also subscribes the worker to the session events and administers the
   *        disconnection, the connection to a client and the UccAnyMessageWorker worker.
   *
   * @see Poco::Net::StreamSocket
   *
   * @since 0.0.0, 2015-04-28
   * @version 0.0.0, 2015-04-28
   * @author gparis
   */
  class Connection
  {
  public:

    /**
     * @brief This constructor initializes the channel, subscribes the channel and the worker and
     *        starts the server session.
     */
    Connection (const Socket & socket, ChannelDisconnector & disconnector);

    /**
     * @brief The destructor that closes the connection.
     */
    virtual ~Connection (void);

    /**
     * @brief Gets the internal server session.
     */
    gvr::communication::ucc::UccSessionServer*  getServerSession (void);

    /**
     * @brief Gets the internal worker for stopping it (mannager's internal use only).
     */
    UccAnyMessageWorker& getWorker (void);

    /**
     * @brief This method closes the connection to the client.
     */
    void close (void);


  protected:

    bool                  m_blOpen;             ///< @brief Flag to signal the connection is open or not.
    UccAnyMessageWorker   m_worker;             ///< @brief The internal worker processing messages.
    UccChannel            m_uccChannel;         ///< @brief The internal communication channel.
    UccSessionServer      m_uccSessionServer;   ///< @brief The internal server session.
    ChannelDisconnector & m_channelDisconnector;///< @brief Internal reference to the channel disconnecter.
  };


  /**
   * @brief This class is in charge to destroy the connection object when a client disconnection event.
   *
   * @see gvr::communication::IChannelListener
   * @see Poco::Net::StreamSocket
   *
   * @since 0.0.0, 2015-04-28
   * @version 0.0.0, 2015-04-28
   * @author gparis
   */
  class ChannelDisconnector : public gvr::communication::IChannelListener
  {
  public:

    ChannelDisconnector (Poco::SharedPtr<Connection> & qConnection, Poco::FastMutex & mutex);

    void channelEvent (BufferType const &);

    void channelEvent (StatusType status);

    void disconnect();


  protected:

    Poco::FastMutex & m_mutex;                   ///< @brief Synchronization mutex.
    Poco::SharedPtr<Connection> & m_qConnection; ///< @brief Reference to a shared pointer to the connection.
  };

  class ConnectionListener : public IChannelServerListener
  {
  public:

	/**
	 * @brief Constructor for the connection listener object.
	 *
	 * @param qConnection is a reference to a shared pointer. When the connection object is created in
	 *        channelStartedEvent(), this smart pointer will point to it.
	 * @param disconnector is a reference to the disconnecter object.
	 * @param mutex  is a reference to the UccServerManager mutex member.
	 */
    ConnectionListener ( Poco::SharedPtr<Connection> &   qConnection,
                         ChannelDisconnector &           disconnector,
                         Poco::FastMutex &               mutex );

    /**
     * @brief Event handler for the creation a new server socket to establish a new connection to a client.
     *
     * @param socket is the new created socket by the server side. ( like accept() )
     */
    virtual void channelStartedEvent (const Socket & socket);

    Poco::FastMutex &               m_mutex;       ///< @brief Synchronization mutex.
    Poco::SharedPtr<Connection> &   m_qConnection; ///< @brief Reference to a shared pointer to the connection.
    ChannelDisconnector &           m_channelDisconnector; ///< @brief Reference to a disconnecter object.
  };


  Poco::FastMutex              m_mutex;               ///< @brief Synchronization mutex.
  IChannelServer &             m_channelServer;       ///< @brief Reference to the server channel object.
  Poco::SharedPtr<Connection>  m_qConnection;         ///< @brief Shared pointer to the connection object.
  ChannelDisconnector          m_channelDisconnector; ///< @brief The disconnecter object.
  ConnectionListener           m_connectionListener;  ///< @brief The connection listener object.
};


} // Communication Domain

}// Gilbarco Domain

#endif /* UCCSERVERMANAGER_H_ */


