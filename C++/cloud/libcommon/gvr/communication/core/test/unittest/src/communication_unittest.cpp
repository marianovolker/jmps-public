/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file communication_unittest.cpp
 * @author mvolker
 * @date Monday, April 28, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */
#include <gtest/gtest.h>
#include "../include/gvrcommunication_unittest/communication_unittest.h"

#include <Poco/Net/Net.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Thread.h>
#include <Poco/Event.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Timespan.h>

#include <iostream>

/**
 * @brief  Communication Unit Tests.
 * @since 1.0.0
 */

using namespace std;
using namespace gvr::communication;
using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Thread;



///////////////////////////////////////////////////////////////////////////////////////////////////////////
class EchoServer: public Poco::Runnable
{
  public:
    EchoServer():m_socket(SocketAddress()), m_thread("EchoServer"), m_stop(false)
    {
       m_thread.start(*this);
       m_ready.wait();
    }

     ~EchoServer()
     {
        m_stop = true;
        m_thread.join();
     }

     Poco::UInt16 port() const
     {
        return m_socket.address().port();
     }

     void run()
     {
        m_ready.set();

        Poco::Timespan span(1000);
        while (!m_stop)
        {
           if (m_socket.poll(span, Socket::SELECT_READ))
           {
              StreamSocket ss = m_socket.acceptConnection();

              std::cout << "starting: [" << "EchoServer" << "]" << std::endl;

              try
              {
                 char buffer[256];
                 int n = ss.receiveBytes(buffer, sizeof(buffer));

                 while (n > 0 && !m_stop)
                 {
                   std::cout << "EchoServer has received data..." << std::endl;

                   ss.sendBytes(buffer, n);

                   std::cout << "EchoServer sent the same data to the other end..." << std::endl;

                   std::cout << "EchoServer is waiting for another chunk of data..." << std::endl;

                   n = ss.receiveBytes(buffer, sizeof(buffer));

                   std::cout << "EchoServer is reading data from socket..." << std::endl;
                 }
              }
              catch (Poco::Exception& exc)
              {
                 std::cerr << "EchoServer: " << exc.displayText() << std::endl;
              }
           }
        }

        std::cout << "EchoServer is terminating..." << std::endl;
     }

  private:
    Poco::Net::ServerSocket m_socket;
    Poco::Thread m_thread;
    Poco::Event m_ready;
    bool m_stop;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////


// Mock some classes for StreamChannel Unittest ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class ChannelListenerWorker: public Poco::Runnable, public gvr::communication::IChannelListener
{
  public:
    ChannelListenerWorker(std::string listenerName)
      : m_stop(false), m_workerName(listenerName),
        m_buffer(), m_status(gvr::communication::IChannel::IChannelError::CNoError)
    {
      m_thread.start(*this);
      m_ready.wait();
    }

    virtual ~ChannelListenerWorker()
    {
      m_stop = true;
      m_thread.join();
    }

    void run()
    {
      m_ready.set();

      std::cout << "starting: [" << m_workerName << "]" << std::endl;

      const gvr::communication::IChannel::BufferType& vectorBuffer(getBuffer());

      while( !m_stop )
      {
        if( vectorBuffer.size() > 0 )
        {
          m_lastBufferReceived.clear();

          m_lastBufferReceived = std::string( vectorBuffer.begin(), vectorBuffer.end() );

          emptyBuffer();
        }

        Thread::sleep(10);
      }
    }

    const std::string& getLastBufferReceived() const
    {
      return m_lastBufferReceived;
    }

    virtual void channelEvent ( BufferType const & buffer )
    {
      std::cout << "ChannelListenerWorker: [" << m_workerName << "] has been received data." << std::endl;

      m_buffer = buffer;
    }

    virtual void channelEvent ( StatusType status )
    {
      m_status = status;
    }

    const BufferType& getBuffer() const
    {
      return m_buffer;
    }

    StatusType getStatus() const
    {
      return m_status;
    }

    void emptyBuffer()
    {
      m_buffer.clear();
    }

private:
   bool m_stop;
   std::string m_workerName;
   std::string m_lastBufferReceived;
   Poco::Thread m_thread;
   Poco::Event m_ready;
   BufferType m_buffer;
   StatusType m_status;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Mock some classes for StreamChannelServer Unittest ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class ChannelServerListenerWorker: public Poco::Runnable, public gvr::communication::IChannelServerListener
{
  public:
    ChannelServerListenerWorker()
      : m_stop(false), m_workerName("ChannelServerListenerWorker"),
        m_pChannel(0)
    {
      m_thread.start(*this);
      m_ready.wait();
    }

    virtual ~ChannelServerListenerWorker()
    {
      m_stop = true;
      m_thread.join();
    }

    void run()
    {
      m_ready.set();

      std::cout << "starting: [" << m_workerName << "]" << std::endl;

      while( !m_stop )
      {
        Thread::sleep(10);
      }
    }

    virtual void channelStartedEvent (const Poco::Net::StreamSocket& socket)
    {
      std::cout << "ChannelServerListenerWorker: [" << m_workerName << "] has been received a new connection." << std::endl;

      m_pChannel = new StreamChannel(socket);
    }

    StreamChannel* getChannel()
    {
      return m_pChannel;
    }


private:
   bool m_stop;
   std::string m_workerName;
   Poco::Thread m_thread;
   Poco::Event m_ready;
   StreamChannel* m_pChannel;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* @brief Performs a StreamChannel send/receive message test.
* @param GCommunicationTestUT The test case name.
* @param TestStreamChannelSendMessage The test name.
* @since 1.0.0
*/
TEST(GCommunicationTestUT, TestStreamChannelSendMessage)
{
  std::cout << std::endl << std::endl;

  EchoServer echoServer;

  ChannelListenerWorker listenerOne("Listener One");

  ChannelListenerWorker listenerTwo("Listener Two");

  StreamChannel streamChannel("localhost", echoServer.port());

  streamChannel.start();

  streamChannel.subscribe( listenerOne );

  streamChannel.subscribe( listenerTwo );

  EXPECT_EQ(streamChannel.getStatus(), StreamChannel::StreamChannelStatus::CStatusConnected);

  if( streamChannel.getStatus() == StreamChannel::StreamChannelStatus::CStatusConnected )
  {
    std::string str = "hello";
    gvr::communication::IChannel::BufferType data(str.begin(), str.end());

    streamChannel.send(data);
    EXPECT_EQ(streamChannel.getError(), gvr::communication::StreamChannel::StreamChannelError::CNoError);

    Thread::sleep(500);

    EXPECT_EQ(listenerOne.getLastBufferReceived(), "hello");

    EXPECT_EQ(listenerTwo.getLastBufferReceived(), "hello");
  }

  std::cout << std::endl << std::endl;

}

/**
* @brief Performs a StreamChannelServer test.
* @param GCommunicationTestUT The test case name.
* @param TestStreamChannelServerTest The test name.
* @since 1.0.0
*/
TEST(GCommunicationTestUT, TestStreamChannelServerTest)
{
  std::cout << std::endl << std::endl;

  StreamChannelServer server("localhost", 10999);

  ChannelServerListenerWorker serverListener;

  ChannelListenerWorker listenerOne("Listener One");

  ChannelListenerWorker listenerTwo("Listener Two");

  StreamChannel streamChannel("localhost", server.getPort());

  streamChannel.subscribe( listenerOne );

  server.subscribe(serverListener);

  streamChannel.start();

  Thread::sleep(50);

  EXPECT_EQ(streamChannel.getStatus(), StreamChannel::StreamChannelStatus::CStatusConnected);

  Thread::sleep(100);

  StreamChannel* pStreamChannelTwo(serverListener.getChannel());

  if( pStreamChannelTwo )
  {
    pStreamChannelTwo->subscribe( listenerTwo );

    pStreamChannelTwo->start();

    EXPECT_EQ(pStreamChannelTwo->getStatus(), StreamChannel::StreamChannelStatus::CStatusConnected);

    std::string str = "hello, channel server!.";
    gvr::communication::IChannel::BufferType data(str.begin(), str.end());

    streamChannel.send(data);
    EXPECT_EQ(streamChannel.getError(), gvr::communication::StreamChannel::StreamChannelError::CNoError);

    Thread::sleep(100);

    EXPECT_EQ(listenerTwo.getLastBufferReceived(), str.c_str());

    str = "hello, channel client!.";
    gvr::communication::IChannel::BufferType data2(str.begin(), str.end());

    pStreamChannelTwo->send(data2);
    EXPECT_EQ(pStreamChannelTwo->getError(), gvr::communication::StreamChannel::StreamChannelError::CNoError);

    Thread::sleep(100);

    EXPECT_EQ(listenerOne.getLastBufferReceived(), str.c_str());

    streamChannel.stop();

    delete pStreamChannelTwo;
  }

  server.stop();

  std::cout << std::endl << std::endl;
}
