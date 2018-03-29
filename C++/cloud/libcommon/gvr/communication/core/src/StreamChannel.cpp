/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file StreamChannel.cpp
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <gvr/communication/StreamChannel.h>

//# include <iostream>

# include <Poco/Timespan.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      StatusType const StreamChannel::StreamChannelStatus::CLastStatus            = IChannel::IChannelStatus::CLastStatus + 1;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      ErrorType const StreamChannel::StreamChannelError::CErrorCreatingReceiver   = IChannel::IChannelError::CLastError + 1;
      ErrorType const StreamChannel::StreamChannelError::CErrorStoppingReceiver   = IChannel::IChannelError::CLastError + 2;
      ErrorType const StreamChannel::StreamChannelError::CLastError               = IChannel::IChannelError::CLastError + 3;
      /////////////////////////////////////////////////////////////////////////////////////


      /* Public Interface ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      StreamChannel::StreamChannel(const std::string& host, SizeType port)
        : IChannel(), m_host(host), m_port(port), m_socket(),
          m_listeners(), m_channelReceiver(),
          m_mxStreamChannel(), m_mxReceiver(),
          m_mxSocket(), m_mxListeners(),
          m_socketConnected(false)
      {
        init();
      }


      StreamChannel::StreamChannel(const Poco::Net::StreamSocket& socket)
        : IChannel(), m_socket(socket),
          m_listeners(), m_channelReceiver(),
          m_mxStreamChannel(), m_mxReceiver(),
          m_mxSocket(), m_mxListeners(),
          m_socketConnected(true)
      {
        init();
        m_host = m_socket.address().host().toString();
        m_port = m_socket.address().port();
      }


      StreamChannel::~StreamChannel (void)
      {
        stop();
      }


      void StreamChannel::subscribe ( IChannelListener & listener )
      {
        ScopedLock lock(m_mxListeners);

        std::set<IChannelListener *>::iterator found = m_listeners.find(&listener);
        if( found == m_listeners.end() )
        {
          m_listeners.insert(&listener);
        }

        //std::cout << "m_listeners.size(" << std::dec << m_listeners.size() << ");" << std::endl;
      }


      void StreamChannel::unsubscribe ( IChannelListener & listener )
      {
        ScopedLock lock(m_mxListeners);

        m_listeners.erase(&listener);

        //std::cout << "m_listeners.size(" << std::dec << m_listeners.size() << ");" << std::endl;
      }


      ErrorType StreamChannel::start (void)
      {
        ScopedLock lock(m_mxStreamChannel);

        StatusType status(getStatus());

        if( status != IChannel::IChannelStatus::CStatusConnected )
        {
          setError(IChannel::IChannelError::CNoError);

          connect();

          if( getError() == IChannel::IChannelError::CNoError )
          {
            while( !startReceiver() ){ Poco::Thread::sleep(5); }
          }

          return getError();
        }

        return setError(IChannel::IChannelError::CErrorAlreadyConnected);
      }


      ErrorType StreamChannel::stop (void)
      {
        ScopedLock lock(m_mxStreamChannel);

        StatusType status(getStatus());

        if( status == IChannel::IChannelStatus::CStatusConnected )
        {
          setError(IChannel::IChannelError::CNoError);

          while( !stopReceiver() ){ Poco::Thread::sleep(5); }

          disconnect();

          return getError();
        }
        else
        {
          setError(IChannel::IChannelError::CNoError);

          while( !stopReceiver() ){ Poco::Thread::sleep(5); }

          return getError();
        }
      }


      ErrorType StreamChannel::send (BufferType const & buffer)
      {
        ScopedLock lock(m_mxStreamChannel);

        StatusType status(getStatus());

        if( status == IChannel::IChannelStatus::CStatusConnected )
        {
          try
          {
            /*
            std::cout << "StreamChannel::send:";
            for (unsigned i=0; i<buffer.size(); ++i)
            {
              std::cout << std::hex << static_cast<int>(buffer[i]) << ' ';
            }
            std::cout << std::endl;
            */

            SizeType nBytesSent = m_socket.sendBytes(&buffer.front(), buffer.size());

            if( nBytesSent != buffer.size() )
            {
              throw("Error sending...");
            }

            return setError(IChannel::IChannelError::CNoError);
          }
          catch (...)
          {
          }
        }

        return setError(IChannel::IChannelError::CErrorSending);
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /* End Public Interface ********/




      /* Protected Interface ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      void StreamChannel::notifyToSubscribers ( BufferType const & buffer )
      {
        ScopedLock lock(m_mxListeners);

        std::set<IChannelListener *>::iterator it;
        for (it = m_listeners.begin(); it != m_listeners.end(); ++it)
        {
          (*it)->channelEvent(buffer);
        }
      }


      void StreamChannel::notifyToSubscribers ( StatusType status )
      {
        ScopedLock lock(m_mxListeners);

        std::set<IChannelListener *>::iterator it;
        for (it = m_listeners.begin(); it != m_listeners.end(); ++it)
        {
          (*it)->channelEvent(status);
        }
      }


      void StreamChannel::disconnect ()
      {
        ScopedLock lock(m_mxSocket);

        try
        {
          m_socket.shutdown();
          m_socket.close();
          m_socketConnected = false;

          notifyToSubscribers( setStatus(IChannel::IChannelStatus::CStatusDisconnected) );
        }
        catch(...)
        {
          setError(IChannel::IChannelError::CErrorDisconnecting);
        }
      }


      void StreamChannel::connect ()
      {
        ScopedLock lock(m_mxSocket);

        try
        {
          const int CConnectionTimeout = 20000; // 20 milliseconds ...

          Poco::Timespan timeout(CConnectionTimeout);

          if(!m_socketConnected)
          {
            m_socket.connect(Poco::Net::SocketAddress(m_host, m_port), timeout);
          }

          m_socket.setBlocking(false);
          m_socket.setReceiveBufferSize(getBufferSize());

          notifyToSubscribers( setStatus(IChannel::IChannelStatus::CStatusConnected) );
        }
        catch(...)
        {
          setError(IChannel::IChannelError::CErrorConnecting);
        }
      }


      bool StreamChannel::startReceiver ()
      {
        ScopedLock lock(m_mxReceiver);

        if( !m_channelReceiver.start(*this) )
        {
          setError(StreamChannelError::CErrorCreatingReceiver);

          return false;
        }

        return true;
      }


      bool StreamChannel::stopReceiver ()
      {
        ScopedLock lock(m_mxReceiver);

        if( !m_channelReceiver.stop() )
        {
          setError(StreamChannelError::CErrorStoppingReceiver);

          return false;
        }

        return true;
      }


      void StreamChannel::init()
      {
        m_descriptionStatusList [ StreamChannelStatus::CLastStatus            ] = "Stream Channel is at unknown state";

        m_descriptionErrorList  [ StreamChannelError::CErrorCreatingReceiver  ] = "Stream Channel has an error creating the receiver thread.";
        m_descriptionErrorList  [ StreamChannelError::CErrorStoppingReceiver  ] = "Stream Channel has an error stopping the receiver thread.";
        m_descriptionErrorList  [ StreamChannelError::CLastError              ] = "Stream Channel has an unknown error";
      }


      void StreamChannel::receive ()
      {
        if( getStatus() != IChannel::IChannelStatus::CStatusConnected )
        {
          return;
        }

        // Check if there's data or any error in the socket.
        {
          ScopedLock lock(m_mxSocket);

          const int CPollTimeout = 500000; // 500 milliseconds ...

          Poco::Timespan timeout(CPollTimeout);
          if( !m_socket.poll(timeout, (Poco::Net::Socket::SELECT_READ|Poco::Net::Socket::SELECT_ERROR)) )
          {
            // timeout was occurred so we do nothing.
            return;
          }
        }

        // New data received.
        SizeType nBytesReceived = 0;
        unsigned short bufferSize = getBufferSize();
        unsigned char buffer[bufferSize];
        try
        {
          ScopedLock lock(m_mxSocket);
          nBytesReceived = m_socket.receiveBytes( buffer, bufferSize );
        }
        catch (...){}

        // Notify to observers.
        if( nBytesReceived > 0 )
        {
          BufferType vectorBuffer(nBytesReceived);
          std::copy(buffer, (buffer+nBytesReceived), vectorBuffer.begin());

          /*
          std::cout << "StreamChannel::received:";
          for (unsigned i=0; i<vectorBuffer.size(); ++i)
          {
            std::cout << std::hex << static_cast<int>(vectorBuffer[i]) << ' ';
          }
          std::cout << std::endl;
          */

          notifyToSubscribers(vectorBuffer);

          return;
        }

        // Is probably that we receive a disconnect from the socket, so we need to notify this situation.
        disconnect();
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////







      /* Thread Channel Receiver ********/
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      StreamChannel::ChannelReceiver::ChannelReceiver()
        : m_pStreamChannel(NULL), m_run(false), m_thread("ChannelReceiver"),
          m_readyStarted(), m_readyStopped(),
          m_mxChannelReceiver(), m_mxChannel(), m_mxRun()
      {
      }


      StreamChannel::ChannelReceiver::~ChannelReceiver()
      {
        stop();
      }


      bool StreamChannel::ChannelReceiver::start(StreamChannel& channel)
      {
        ScopedLock lock(m_mxChannelReceiver);

        if( !isRunning() )
        {
          try
          {
            turnOn();

            {
              ScopedLock lock(m_mxChannel);
              m_pStreamChannel = &channel;
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


      bool StreamChannel::ChannelReceiver::stop()
      {
        ScopedLock lock(m_mxChannelReceiver);

        if( isRunning() )
        {
          try
          {
            turnOff();

            {
              ScopedLock lock(m_mxChannel);
              m_pStreamChannel = 0;
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


      void StreamChannel::ChannelReceiver::run()
      {
        // Wait for the signal to begin the thread ...
        m_readyStarted.wait();

        while( isRunning() )
        {
          {
            ScopedLock lock(m_mxChannel);
            if( m_pStreamChannel )
            {
              m_pStreamChannel->receive();
            }
          }
        }

        // Signal that the thread has finished...
        m_readyStopped.set();
      }


      bool StreamChannel::ChannelReceiver::isRunning()
      {
        ScopedLock lock(m_mxRun);
        return m_run;
      }


      bool StreamChannel::ChannelReceiver::turnOff()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=false);
      }


      bool StreamChannel::ChannelReceiver::turnOn()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=true);
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////
      /* End Thread Receiver ********/
  }
}
