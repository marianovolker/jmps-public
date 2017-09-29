/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file StreamChannelServer.cpp
 * @author mvolker
 * @date Monday, April 13, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <gvr/communication/StreamChannelServer.h>

# include <Poco/Exception.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      StatusType const StreamChannelServer::StreamChannelServerStatus::CLastStatus  = StreamChannel::StreamChannelStatus::CLastStatus + 1;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      ErrorType const StreamChannelServer::StreamChannelServerError::CLastError     = StreamChannel::StreamChannelError::CLastError + 1;
      /////////////////////////////////////////////////////////////////////////////////////

      /* Public Interface ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      StreamChannelServer::StreamChannelServer(const std::string& host, SizeType port)
        : IChannelServer(),
          m_host(host),
          m_port(port),
          m_mxStreamChannelServer(),
          m_mxConnectionReceiver(),
          m_mxSocket(),
          m_mxListeners(),
          m_ChannelServerConnectionReceiver(),
          m_socket()
      {
        m_descriptionStatusList [ StreamChannelServerStatus::CLastStatus            ] = "Stream Channel Server is at unknown state";

        m_descriptionErrorList  [ StreamChannelServerError::CLastError              ] = "Stream Channel Server has an unknown error";

        start();
      }


      StreamChannelServer::~StreamChannelServer (void)
      {
        stop();
      }


      void StreamChannelServer::subscribe ( IChannelServerListener & listener )
      {
        ScopedLock lock(m_mxListeners);

        std::set<IChannelServerListener *>::iterator found = m_listeners.find(&listener);
        if( found == m_listeners.end() )
        {
          m_listeners.insert(&listener);
        }
      }


      void StreamChannelServer::unsubscribe ( IChannelServerListener & listener )
      {
        ScopedLock lock(m_mxListeners);

        m_listeners.erase(&listener);
      }


      void StreamChannelServer::notifyStartedChannelToSubscribers( const Poco::Net::StreamSocket& socket )
      {
        ScopedLock lock(m_mxListeners);

        std::set<IChannelServerListener *>::iterator it;
        for (it = m_listeners.begin(); it != m_listeners.end(); ++it)
        {
          (*it)->channelStartedEvent(socket);
        }
      }


      ErrorType StreamChannelServer::start (void)
      {
        ScopedLock lock(m_mxStreamChannelServer);

        StatusType status(getStatus());

        if( status != IChannelServer::IChannelServerStatus::CStatusStarted )
        {
          setError(IChannelServer::IChannelServerError::CNoError);

          Poco::Net::SocketAddress address(m_host, m_port);

          try
          {
            m_socket.bind(address, true);
            m_socket.listen();
          }
          catch(Poco::Exception&)
          {
            return setError(IChannelServer::IChannelServerError::CErrorListening);
          }

          if( getError() == IChannelServer::IChannelServerError::CNoError )
          {
            while( !startConnectionReceiver() ){ Poco::Thread::sleep(5); }
          }

          setStatus(IChannelServer::IChannelServerStatus::CStatusStarted);

          return getError();
        }

        return setError(IChannelServer::IChannelServerError::CErrorAlreadyStarted);
      }


      ErrorType StreamChannelServer::stop (void)
      {
        ScopedLock lock(m_mxStreamChannelServer);

        StatusType status(getStatus());

        if( status != IChannelServer::IChannelServerStatus::CStatusStopped )
        {
          setError(IChannelServer::IChannelServerError::CNoError);

          while( !stopConnectionReceiver() ){ Poco::Thread::sleep(5); }

          m_socket.close();

          setStatus(IChannelServer::IChannelServerStatus::CStatusStopped);

          return getError();
        }

        return setError(IChannelServer::IChannelServerError::CNoError);
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /* End Public Interface ********/



      /* Protected Interface ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      bool StreamChannelServer::startConnectionReceiver ()
      {
        ScopedLock lock(m_mxConnectionReceiver);

        if( !m_ChannelServerConnectionReceiver.start(*this) )
        {
          return false;
        }

        return true;
      }

      bool StreamChannelServer::stopConnectionReceiver ()
      {
        ScopedLock lock(m_mxConnectionReceiver);

        if( !m_ChannelServerConnectionReceiver.stop() )
        {
          return false;
        }

        return true;
      }

      void StreamChannelServer::waitIncomingConnection ()
      {
        ScopedLock socketLock(m_mxSocket);

        const SizeType CCommonTimeout = 100000;

        Poco::Timespan timeout(CCommonTimeout);
        if( !m_socket.poll(timeout, (Poco::Net::Socket::SELECT_READ|Poco::Net::Socket::SELECT_ERROR)) )
        {
          // timeout was occurred so we do nothing.
          return;
        }

        notifyStartedChannelToSubscribers( m_socket.acceptConnection() );
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////




      /* Thread StreamChannelServer Receiver ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      StreamChannelServer::ChannelServerConnectionReceiver::ChannelServerConnectionReceiver()
        : m_pStreamChannelServer(NULL), m_run(false), m_thread("ChannelServerConnectionReceiver"),
          m_readyStarted(), m_readyStopped(),
          m_mxChannelServerConnectionReceiver(), m_mxChannelServer(), m_mxRun()
      {
      }

      StreamChannelServer::ChannelServerConnectionReceiver::~ChannelServerConnectionReceiver()
      {
        stop();
      }

      bool StreamChannelServer::ChannelServerConnectionReceiver::start(StreamChannelServer& manager)
      {
        ScopedLock lock(m_mxChannelServerConnectionReceiver);

        if( !isRunning() )
        {
          try
          {
            turnOn();

            {
              ScopedLock lock(m_mxChannelServer);
              m_pStreamChannelServer = &manager;
            }

            m_thread.start(*this);

            // Signal that the thread could begin...
            m_readyStarted.set();
          }
          catch(...)
          {
            return false;
          }
        }

        return true;
      }

      bool StreamChannelServer::ChannelServerConnectionReceiver::stop()
      {
        ScopedLock lock(m_mxChannelServerConnectionReceiver);

        if( isRunning() )
        {
          try
          {
            turnOff();

            {
              ScopedLock lock(m_mxChannelServer);
              m_pStreamChannelServer = 0;
            }

            // Wait for the signal to terminate the thread ...
            m_readyStopped.wait();

            m_thread.join();
          }
          catch (...)
          {
            return false;
          }
        }

        return true;
      }

      void StreamChannelServer::ChannelServerConnectionReceiver::run()
      {
        // Wait for the signal to begin the thread ...
        m_readyStarted.wait();

        while( isRunning() )
        {
          {
            ScopedLock lock(m_mxChannelServer);
            if( m_pStreamChannelServer )
            {
              m_pStreamChannelServer->waitIncomingConnection();
            }
          }
        }

        // Signal that the thread has finished...
        m_readyStopped.set();
      }

      bool StreamChannelServer::ChannelServerConnectionReceiver::isRunning()
      {
        ScopedLock lock(m_mxRun);
        return m_run;
      }

      bool StreamChannelServer::ChannelServerConnectionReceiver::turnOff()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=false);
      }

      bool StreamChannelServer::ChannelServerConnectionReceiver::turnOn()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=true);
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////

  }
}
