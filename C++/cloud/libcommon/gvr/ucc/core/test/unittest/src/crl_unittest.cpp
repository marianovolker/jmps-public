/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file crl_unittest.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gtest/gtest.h>
#include "../include/gvrucc_unittest/crl_unittest.h"

#include <Poco/Net/Net.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Thread.h>
#include <Poco/Event.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Timespan.h>
#include <Poco/Timer.h>
#include <Poco/Stopwatch.h>
#include <Poco/Timestamp.h>

#include <gvr/communication/crl/CrlChannel.h>
#include <gvr/communication/crl/ICrlSessionListener.h>
#include <gvr/communication/crl/session/CrlSessionClient.h>

#include <gvr/communication/crl/message/CrlMessageAny.h>
#include <gvr/communication/crl/message/CrlMessageCloudStatus.h>
#include <gvr/communication/crl/message/CrlMessageCloudStatusResponse.h>
#include <gvr/communication/crl/message/CrlMessageRegistration.h>
#include <gvr/communication/crl/message/CrlMessageRegistrationResponse.h>

#include <gvr/communication/crl/message/CrlMessageGetDnsConfiguration.h>
#include <gvr/communication/crl/message/CrlMessageGetDnsConfigurationResponse.h>
#include <gvr/communication/crl/message/CrlMessageSetDnsConfiguration.h>
#include <gvr/communication/crl/message/CrlMessageSetDnsConfigurationResponse.h>

#include <gvr/communication/crl/message/CrlMessageTestDns.h>
#include <gvr/communication/crl/message/CrlMessageTestDnsResponse.h>

//////////////////////////////////////////////////////////////////////////
class EchoServer: public Poco::Runnable
{
  public:
    EchoServer()
      : m_socket(Poco::Net::SocketAddress()), m_thread("EchoServer"), m_stop(false), m_timer(3000,0), m_stopSend(false)
    {
       m_thread.start(*this);
       m_ready.wait();
    }

    ~EchoServer()
    {
      m_stop = true;
      m_thread.join();
    }

    void stopSend()
    {
      m_stopSend = true;
    }

    void restartSend()
    {
      m_stopSend = false;
    }

    Poco::UInt16 port() const
    {
      return m_socket.address().port();
    }

    void startTimer(long new_time)
    {
      m_timer.setStartInterval(new_time);
      m_timer.start(Poco::TimerCallback<EchoServer>(*this, &EchoServer::onTimer));
    }

    void onTimer(Poco::Timer& timer)
    {
      std::cout << "timeout in [" << "EchoServer" << "]" << std::endl;
      std::cout << "Closing the socket in [" << "EchoServer" << "]" << std::endl;
      m_socket.close();
      m_ss.close();
      m_stop=true;
      m_ready.set();
      std::cout << "Socket closed in [" << "EchoServer" << "]" << std::endl;
    }

    void run()
    {
      m_ready.set();

      m_socket.setBlocking(false);

      Poco::Timespan span(1000);
      while (!m_stop)
      {
        if (m_socket.poll(span, Poco::Net::Socket::SELECT_READ))
        {
          m_ss = m_socket.acceptConnection();

          try
          {
            m_ss.setBlocking(false);
            char buffer[256];
            int n = 0;

            while (!m_stop)
            {
              if( !m_ss.poll(span, Poco::Net::Socket::SELECT_READ) )
              {
                continue;
              }
              n = m_ss.receiveBytes(buffer, sizeof(buffer));
              if(!m_stopSend)
              {
                m_ss.sendBytes(buffer, n);
              }
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
    bool m_stopSend;
    Poco::Net::StreamSocket m_ss;

    Poco::Timer m_timer;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Mock some classes for CrlChannel Unittest ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class ChannelListenerWorker: public Poco::Runnable, public gvr::communication::IChannelListener
{
  public:
    ChannelListenerWorker(std::string listenerName)
      : m_stop(false), m_workerName(listenerName),
        m_buffer(), m_status(gvr::communication::crl::CrlChannel::CrlChannelStatus::CStatusInitial),
        m_pChannel(0)
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

        Poco::Thread::sleep(10);
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

    virtual void channelEvent ( StatusType channelStatus )
    {
      if( m_pChannel )
      {
        std::cout << "[New channel status - [" << m_pChannel->getStatusDescription(channelStatus) << "]]" << std::endl;
      }
      else
      {
        std::cout << "[New channel status - [" << std::hex << static_cast<int>(channelStatus) << "]]" << std::endl;
      }

      m_status = channelStatus;
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

    void setChannel(gvr::communication::crl::CrlChannel& channel)
    {
      m_pChannel=&channel;
    }

private:
   bool m_stop;
   std::string m_workerName;
   std::string m_lastBufferReceived;
   Poco::Thread m_thread;
   Poco::Event m_ready;
   BufferType m_buffer;
   StatusType m_status;
   gvr::communication::crl::CrlChannel* m_pChannel;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Mock some classes for CrlSession Unittest ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class CrlSessionListenerWorker: public Poco::Runnable, public gvr::communication::crl::ICrlSessionListener
{
  public:
    CrlSessionListenerWorker(std::string workerName)
      : m_stop(false), m_workerName(workerName), m_status(), m_typeId(), m_keyType(), m_buffer(), m_pSession(0)
    {
      m_thread.start(*this);
      m_ready.wait();
    }

    virtual ~CrlSessionListenerWorker()
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
        Poco::Thread::sleep(10);
      }

      std::cout << "terminating: [" << m_workerName << "]" << std::endl;
    }

    void sessionEvent( BufferType const & buffer, TypeId const & typeId, KeyType const & keyType)
    {
      m_typeId  = typeId;
      m_keyType = keyType;

      m_buffer.clear();
      m_buffer.reserve(buffer.size());
      m_buffer.insert(m_buffer.end(),buffer.begin(),buffer.end());

      std::cout << "CrlSessionListenerWorker: [" << m_workerName << "] has been received an event." << std::endl;
    }

    void sessionEvent( StatusType status )
    {
      m_status = status;

      if( m_pSession )
      {
        std::cout << "CrlSessionListenerWorker: [" << m_workerName << "] has been received a new status [" << m_pSession->getStatusDescription(status) << "]" << std::endl;
      }
      else
      {
        std::cout << "CrlSessionListenerWorker: [" << m_workerName << "] has been received a new status [" << std::dec << static_cast<int>(status) << "]" << std::endl;
      }
    }

    KeyType getKeyType() const
    {
      return m_keyType;
    }

    StatusType getStatus() const
    {
      return m_status;
    }

    TypeId getTypeId() const
    {
      return m_typeId;
    }

    BufferType getBuffer() const
    {
      return m_buffer;
    }

    void setSession(gvr::communication::crl::CrlSession& session)
    {
      m_pSession=&session;
    }

private:
   bool m_stop;
   std::string m_workerName;
   Poco::Thread m_thread;
   Poco::Event m_ready;

   StatusType m_status;
   TypeId m_typeId;
   KeyType m_keyType;
   BufferType m_buffer;
   gvr::communication::crl::CrlSession* m_pSession;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////
class MockCrlSessionClientNoConnectionCycle
 : public gvr::communication::crl::CrlSessionClient
{
public:
  MockCrlSessionClientNoConnectionCycle (void) : CrlSessionClient()
  {
  }

  virtual bool isLogged (void) { return isConnected(); }

  virtual ~MockCrlSessionClientNoConnectionCycle (void) {}

  virtual bool remainingBuffer (void) { return !m_buffer.empty(); }
protected:
  virtual bool private_unsafe_doLogin (void)
  {
    gvr::communication::crl::CrlMessageAny crlAnyMessageRq;
    gvr::communication::crl::CrlMessageAny crlAnyMessageRs;

    // This is the only message that could be sent at started session state because this will generate
    // the availability of the communication with SPOT.
    AbsSessionType::sendSync(crlAnyMessageRq,crlAnyMessageRs);

    ErrorType result = getError();

    if (result != CrlSession::CrlSessionError::CNoError)
    {
      setError(CrlSession::CrlSessionError::CErrorOnLoging);
      return false;
    }
    return true;
  }

  virtual void private_safe_startConnectionCycle (void) {}
  virtual void private_safe_stopConnectionCycle (void) {}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* @brief Performs connection through a CrlChannel.
* @param GCrlTestUT The test case name.
* @param TestCrlChannel The test name.
* @since 1.0.0
*/
TEST(GCrlTestUT, TestCrlChannel)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                     TestCrlChannel                     ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  ChannelListenerWorker listenerOne("Listener One");

  gvr::communication::crl::CrlChannel crlChannel("localhost", echoServer.port());

  crlChannel.start();

  crlChannel.subscribe( listenerOne );

  listenerOne.setChannel(crlChannel);

  EXPECT_EQ(crlChannel.getError(), gvr::communication::crl::CrlChannel::CrlChannelError::CNoError);

  if( crlChannel.getStatus() == gvr::communication::crl::CrlChannel::CrlChannelStatus::CStatusConnected )
  {
    std::string str = "hello";
    gvr::communication::IChannel::BufferType data(str.begin(), str.end());

    crlChannel.send(data);
    EXPECT_EQ(crlChannel.getError(), gvr::communication::crl::CrlChannel::CrlChannelError::CNoError);

    // Check the condition in a maximum elased time ...
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Poco::Timestamp initialTime;
    const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
    while (
            ( listenerOne.getLastBufferReceived().empty()                ) &&
            ( Poco::Timestamp::TimeDiff((Poco::Timestamp()-initialTime)) < usTimeout )
          )
    {
      Poco::Thread::sleep(1);
    }
    std::cout << "Elapsed time ...[" << std::dec << Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) << " us]." << std::endl;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    EXPECT_EQ(listenerOne.getLastBufferReceived(), "hello");
  }

  crlChannel.stop();

  std::cout << "CrlChannel was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
*
* @brief Performs several connections and disconnections to check the CrlSession.
* @param GCrlTestUT The test case name.
* @param TestCrlSessionStopStart The test name.
* @since 1.0.0
*/
TEST(GCrlTestUT, TestCrlSessionStopStart)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                 TestCrlSessionStopStart                ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  gvr::communication::crl::CrlChannel crlChannel("localhost", echoServer.port());

  MockCrlSessionClientNoConnectionCycle crlSession;

  for(int i=0; i<5; i++)
  {
    std::cout << "Starting the CrlSession( sequence [" << std::dec << i+1 << "] )..." << std::endl;

    crlSession.start(crlChannel);

    EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);

    if( crlSession.getError() != gvr::communication::crl::CrlSession::CrlSessionError::CNoError )
    {
      std::cout << "[" << crlSession.getErrorDescription() << "]" << std::endl;
    }

    EXPECT_EQ(crlSession.isConnected(), true);

    std::cout << "Stopping the CrlSession..." << std::endl;

    crlSession.stop();

    EXPECT_EQ(crlSession.isConnected(), false);

    EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);

    std::cout << "CrlSession was stopped..." << std::endl;

    if( crlSession.getError() != gvr::communication::crl::CrlSession::CrlSessionError::CNoError )
    {
      std::cout << "[" << crlSession.getErrorDescription() << "]" << std::endl;
    }
  }

  crlSession.stop();

  std::cout << "CrlSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs several send of messages in a async way.
* @param GCrlTestUT The test case name.
* @param TestCrlSessionAsyncSend The test name.
* @since 1.0.0
*/
TEST(GCrlTestUT, TestCrlSessionAsyncSend)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                 TestCrlSessionAsyncSend                ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  CrlSessionListenerWorker workerOne("Worker One");

  gvr::communication::crl::CrlChannel crlChannel("localhost", echoServer.port());

  MockCrlSessionClientNoConnectionCycle crlSession;

  crlSession.subscribe(workerOne);

  workerOne.setSession(crlSession);

  crlSession.start(crlChannel);

  EXPECT_EQ(crlSession.isConnected(), true);

  // First asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    gvr::communication::crl::CrlMessageAny anyMessageRq;
    crlSession.send(anyMessageRq);
    EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Poco::Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerOne.getTypeId() != anyMessageRq.getTypeId()  ) &&
              ( Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Poco::Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerOne.getTypeId(),anyMessageRq.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  // Second asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    {
      gvr::communication::crl::CrlMessageAny anyMessageRq;
      crlSession.send(anyMessageRq);
      EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);
      // Check the condition in a maximum elapsed time ...
      {
        Poco::Timestamp initialTime;
        const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
        while (
                ( workerOne.getTypeId() != anyMessageRq.getTypeId() ) &&
                ( Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) < usTimeout )
              )
        {
          Poco::Thread::sleep(1);
        }
        std::cout << "Elapsed time ...[" << std::dec << Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) << " us]." << std::endl;
      }
      EXPECT_EQ(workerOne.getTypeId(),anyMessageRq.getTypeId());
    }

    {
      gvr::communication::crl::CrlMessageAny anyMessageRq;
      anyMessageRq.parse(workerOne.getBuffer());
      EXPECT_EQ(anyMessageRq.getTypeId(),anyMessageRq.getTypeId());
    }
  }
  //////////////////////////////////////////////////////////////////////

  std::cout << "Stopping the CrlSession..." << std::endl;

  crlSession.stop();
  EXPECT_EQ(crlSession.isConnected(), false);

  std::cout << "CrlSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs several send of messages in a sync way.
* @param GCrlTestUT The test case name.
* @param TestCrlSessionSyncSend The test name.
* @since 1.0.0
*/
TEST(GCrlTestUT, TestCrlSessionSyncSend)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                 TestCrlSessionSyncSend                 ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  gvr::communication::crl::CrlChannel crlChannel("localhost", echoServer.port());

  MockCrlSessionClientNoConnectionCycle crlSession;

  crlSession.start(crlChannel);

  EXPECT_EQ(crlSession.isConnected(), true);

  // First synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    gvr::communication::crl::CrlMessageAny anyMessageRq;
    crlSession.sendSync(anyMessageRq,anyMessageRq);
    EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);
  }
  //////////////////////////////////////////////////////////////////////

  // Second synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    gvr::communication::crl::CrlMessageAny anyMessageRq;
    crlSession.sendSync(anyMessageRq,anyMessageRq,1000);
    EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);
  }
  //////////////////////////////////////////////////////////////////////

  // Third synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  // Wait for another type of message to generate the desire condition, the timeout condition.
  // Remember the echo server answers with the very same message as the request.
  //////////////////////////////////////////////////////////////////////
  {
    gvr::communication::crl::CrlMessageAny anyMessageRq;
    gvr::communication::crl::CrlMessageCloudStatus otherMessageRs;
    crlSession.sendSync(anyMessageRq,otherMessageRs,10);
    EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CErrorTimeoutSendingMessage);
  }
  //////////////////////////////////////////////////////////////////////

  // Fourth synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    gvr::communication::crl::CrlMessageAny anyMessageRq;
    crlSession.sendSync(anyMessageRq,anyMessageRq,1000);
    EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);
  }
  //////////////////////////////////////////////////////////////////////

  EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);

  crlSession.stop();

  EXPECT_EQ(crlSession.isConnected(), false);

  std::cout << "CrlSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs a simulation of a disconnection from the other end and if the Listeners respond well.
* @param GCrlTestUT The test case name.
* @param TestCrlSessionCheckDisconnect The test name.
* @since 1.0.0
*/
TEST(GCrlTestUT, TestCrlSessionCheckDisconnect)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////             TestCrlSessionCheckDisconnect              ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  gvr::communication::crl::CrlChannel crlChannel("localhost", echoServer.port());

  MockCrlSessionClientNoConnectionCycle crlSession;

  crlSession.start(crlChannel);

  EXPECT_EQ(crlSession.isConnected(), true);

  long ms_time_to_throw_the_disconnection = 1000; // 1 second
  long ms_time_to_wait_in_send_sync = 2000; // 2 seconds

  echoServer.startTimer(ms_time_to_throw_the_disconnection);

  // Send a message and wait the disconnect from the echo server ...
  //////////////////////////////////////////////////////////////////////
  // Wait for another type of message to generate the desired situation.
  {
    gvr::communication::crl::CrlMessageAny anyMessageRq;
    gvr::communication::crl::CrlMessageCloudStatus otherMessageRs;
    crlSession.sendSync(anyMessageRq,otherMessageRs,ms_time_to_wait_in_send_sync);
    Poco::Thread::sleep(10);
    EXPECT_EQ(crlSession.getStatus(), gvr::communication::crl::CrlSession::CrlSessionStatus::CStatusSessionStopped);
  }
  //////////////////////////////////////////////////////////////////////

  crlSession.stop();

  EXPECT_EQ(crlSession.isConnected(), false);

  std::cout << "CrlSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
 *
 * Check if this test is still useful ...
 *
 *
* @brief Performs a simulation of a disconnection and the crl session retries connection.
* @param GCrlTestUT The test case name.
* @param TestCrlSessionRetries The test name.
* @since 1.0.0
*
TEST(GCrlTestUT, TestCrlSessionRetries)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                TestCrlSessionRetries                   ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;
  gvr::communication::crl::CrlSessionListenerWorker workerOne("Worker One");
  gvr::communication::crl::CrlChannel crlChannel("localhost", echoServer.port());
  MockCrlSessionClientNoConnectionCycle crlSession;
  crlSession.subscribe(workerOne);
  crlSession.start(crlChannel);
  EXPECT_EQ(crlSession.isReady(), true);
  EXPECT_EQ(workerOne.getStatus(), CrlSession::ISessionStatus::CStatusSessionStarted);
  Poco::Thread::sleep(1000);
  echoServer.stopSend();
  std::cout << crlSession.getCurrentTime() << " [stop echo channel]" << std::endl;
  Poco::Thread::sleep(5000);
  std::cout << crlSession.getCurrentTime() << " [restart echo channel]" << std::endl;
  echoServer.restartSend();
  std::cout << crlSession.getCurrentTime() <<  "[status: [" << crlSession.getStatusDescription() << "]]" << std::endl;
  EXPECT_EQ(workerOne.getStatus(), CrlSession::ISessionStatus::CStatusSessionStarted);
  EXPECT_EQ(crlSession.isConnected(), true);
  crlSession.stop();
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}*/

/**
* @brief Performs a simulation of a disconnection and the crl session retries connection.
* @param GCrlTestUT The test case name.
* @param TestCrlSessionAvoidUnknownMsg The test name.
* @since 1.0.0
*/
TEST(GCrlTestUT, TestCrlSessionAvoidUnknownMsg)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////           TestCrlSessionAvoidUnknownMsg               ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  EchoServer echoServer;

  gvr::communication::crl::CrlChannel crlChannel("localhost", echoServer.port());
  MockCrlSessionClientNoConnectionCycle crlSession;


  crlSession.start(crlChannel);
  EXPECT_EQ(crlSession.isReady(), true);
  {
    gvr::communication::crl::CrlMessageAny anyMessageRq;
    gvr::communication::crl::CrlMessageCloudStatus otherMessageRs;
    crlSession.sendSync(anyMessageRq,otherMessageRs);
    EXPECT_EQ(crlSession.remainingBuffer(), false); //NO remaining buffer because the unknown ping message was discarded.
  }
  crlSession.stop();
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs a simulation of a subscription to a particular message type.
* @param GCrlTestUT The test case name.
* @param TestCrlSessionSpecialMessageSubscribe The test name.
* @since 1.0.0
*/
TEST(GCrlTestUT, TestCrlSessionSpecialMessageSubscribe)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////         TestCrlSessionSpecialMessageSubscribe          ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  CrlSessionListenerWorker workerOne("Worker One");

  CrlSessionListenerWorker workerTwo("Worker Two");

  gvr::communication::crl::CrlChannel crlChannel("localhost", echoServer.port());

  MockCrlSessionClientNoConnectionCycle crlSession;

  gvr::communication::crl::CrlMessageAny forOneCrlMessage;

  gvr::communication::crl::CrlMessageCloudStatusResponse forTwoCrlMessage;

  crlSession.subscribe(workerOne, forOneCrlMessage.getTypeId());

  crlSession.subscribe(workerTwo, forTwoCrlMessage.getTypeId());

  EXPECT_EQ(forOneCrlMessage.getTypeId(), gvr::communication::crl::CrlMessageAny::GetTypeId());

  workerOne.setSession(crlSession);
  workerTwo.setSession(crlSession);

  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  crlSession.start(crlChannel);

  EXPECT_EQ(crlSession.isConnected(), true);

  // First asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    crlSession.send(forOneCrlMessage);
    EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Poco::Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerOne.getTypeId() != forOneCrlMessage.getTypeId()  ) &&
              ( Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Poco::Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerOne.getTypeId(), forOneCrlMessage.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  // Second asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    {
      {
        gvr::utils::Json json;
        json.set(gvr::communication::crl::CrlMessageCloudStatusResponse::CConnectionStatusName) = gvr::communication::crl::CrlMessageCloudStatusResponse::CConnectionConnectedString;
        json.set(gvr::communication::crl::CrlMessageCloudStatusResponse::CRegistrationStatusName) = gvr::communication::crl::CrlMessageCloudStatusResponse::CRegistrationRegisteredString;

        forTwoCrlMessage.setPayload(json);
      }
      crlSession.send(forTwoCrlMessage);
      EXPECT_EQ(crlSession.getError(), gvr::communication::crl::CrlSession::CrlSessionError::CNoError);
      // Check the condition in a maximum elapsed time ...
      {
        Poco::Timestamp initialTime;
        const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
        while (
                ( workerTwo.getTypeId() != forTwoCrlMessage.getTypeId() ) &&
                ( Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) < usTimeout )
              )
        {
          Poco::Thread::sleep(1);
        }
        std::cout << "Elapsed time ...[" << std::dec << Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) << " us]." << std::endl;
      }
      EXPECT_EQ(workerTwo.getTypeId(), forTwoCrlMessage.getTypeId());
    }
    {
      gvr::communication::crl::CrlMessageCloudStatusResponse crlMessageRs;
      crlMessageRs.parse(workerTwo.getBuffer());
      EXPECT_EQ(crlMessageRs.getTypeId(), forTwoCrlMessage.getTypeId());
    }
  }
  //////////////////////////////////////////////////////////////////////

  std::cout << "Stopping the CrlSession..." << std::endl;

  crlSession.stop();
  EXPECT_EQ(crlSession.isConnected(), false);

  std::cout << "CrlSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs a verification of the class id types for all messages.
* @param GCrlTestUT The test case name.
* @param TestCrlSessionSpecialMessageSubscribe The test name.
* @since 1.0.0
*/
TEST(GCrlTestUT, TestCrlMessagesTypeIdVerification)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////           TestCrlMessagesTypeIdVerification            ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  gvr::communication::crl::CrlMessageAny crlAny;
  gvr::communication::crl::CrlMessageCloudStatus crlCloudStatusRq;
  gvr::communication::crl::CrlMessageCloudStatusResponse crlCloudStatusRs;
  gvr::communication::crl::CrlMessageRegistration crlRegistrationRq;
  gvr::communication::crl::CrlMessageRegistrationResponse crlRegistrationRs;

  gvr::communication::crl::CrlMessageGetDnsConfiguration crlGetDnsConfigurationRq;
  gvr::communication::crl::CrlMessageGetDnsConfigurationResponse crlGetDnsConfigurationRs;
  gvr::communication::crl::CrlMessageSetDnsConfiguration crlSetDnsConfigurationRq;
  gvr::communication::crl::CrlMessageSetDnsConfigurationResponse crlSetDnsConfigurationRs;

  gvr::communication::crl::CrlMessageTestDns crlTestDnsRq;
  gvr::communication::crl::CrlMessageTestDnsResponse crlTestDnsRs;

  EXPECT_EQ( crlAny.getTypeId(),            gvr::communication::crl::CrlMessageAny::GetTypeId());
  EXPECT_EQ( crlCloudStatusRq.getTypeId(),  gvr::communication::crl::CrlMessageCloudStatus::GetTypeId());
  EXPECT_EQ( crlCloudStatusRs.getTypeId(),  gvr::communication::crl::CrlMessageCloudStatusResponse::GetTypeId());
  EXPECT_EQ( crlRegistrationRq.getTypeId(), gvr::communication::crl::CrlMessageRegistration::GetTypeId());
  EXPECT_EQ( crlRegistrationRs.getTypeId(), gvr::communication::crl::CrlMessageRegistrationResponse::GetTypeId());

  EXPECT_EQ( crlGetDnsConfigurationRq.getTypeId(), gvr::communication::crl::CrlMessageGetDnsConfiguration::GetTypeId());
  EXPECT_EQ( crlGetDnsConfigurationRs.getTypeId(), gvr::communication::crl::CrlMessageGetDnsConfigurationResponse::GetTypeId());

  EXPECT_EQ( crlSetDnsConfigurationRq.getTypeId(), gvr::communication::crl::CrlMessageSetDnsConfiguration::GetTypeId());
  EXPECT_EQ( crlSetDnsConfigurationRs.getTypeId(), gvr::communication::crl::CrlMessageSetDnsConfigurationResponse::GetTypeId());

  EXPECT_EQ( crlTestDnsRq.getTypeId(), gvr::communication::crl::CrlMessageTestDns::GetTypeId());
  EXPECT_EQ( crlTestDnsRs.getTypeId(), gvr::communication::crl::CrlMessageTestDnsResponse::GetTypeId());

  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageAny::GetTypeId()                  << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageCloudStatus::GetTypeId()          << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageCloudStatusResponse::GetTypeId()  << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageRegistration::GetTypeId()         << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageRegistrationResponse::GetTypeId() << "]" << std::endl;

  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageGetDnsConfiguration::GetTypeId()         << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageGetDnsConfigurationResponse::GetTypeId() << "]" << std::endl;

  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageSetDnsConfiguration::GetTypeId()         << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageSetDnsConfigurationResponse::GetTypeId() << "]" << std::endl;

  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageTestDns::GetTypeId()         << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::crl::CrlMessageTestDnsResponse::GetTypeId() << "]" << std::endl;

  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

void checkMessage (gvr::communication::crl::AbsCrlMessage & msg, char const * CMsgJson)
{
  {
    gvr::utils::Json json;
    EXPECT_TRUE(json.parse(CMsgJson));

    msg.setJson(json);
    EXPECT_TRUE(msg.validate());
  }

  {
    gvr::communication::crl::AbsCrlMessage::BufferType buff = msg.serialize();
    EXPECT_FALSE(buff.empty());

    EXPECT_TRUE(msg.parse(buff));
  }
}

/**
* @brief Performs a verification for the validation of all messages.
* @param GCrlTestUT The test case name.
* @param TestCrlSessionSpecialMessageSubscribe The test name.
* @since 1.0.0
*/
TEST(GCrlTestUT, TestCrlMessages)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////           TestCrlMessages                              ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  gvr::communication::crl::CrlMessageAny crlAny;
  gvr::communication::crl::CrlMessageCloudStatus crlCloudStatusRq;
  gvr::communication::crl::CrlMessageCloudStatusResponse crlCloudStatusRs;
  gvr::communication::crl::CrlMessageRegistration crlRegistrationRq;
  gvr::communication::crl::CrlMessageRegistrationResponse crlRegistrationRs;

  gvr::communication::crl::CrlMessageGetDnsConfiguration crlGetDnsConfigRq;
  gvr::communication::crl::CrlMessageGetDnsConfigurationResponse crlGetDnsConfigRs;

  gvr::communication::crl::CrlMessageSetDnsConfiguration crlSetDnsConfigRq;
  gvr::communication::crl::CrlMessageSetDnsConfigurationResponse crlSetDnsConfigRs;

  gvr::communication::crl::CrlMessageTestDns crlTestDnsRq;
  gvr::communication::crl::CrlMessageTestDnsResponse crlTestDnsRs;

  static char const * CCrlAny = "{ \"any\": \"any\" }";
  static char const * CCrlCloudStatus = "";
  static char const * CCrlCloudStatusResponse = "";
  static char const * CCrlRegistration = "";
  static char const * CCrlRegistrationResponse = "";

  static char const * CCrlGetDnsConfig = "{\"operation\":\"get-dns\",\"payload\":{}}";
  static char const * CCrlGetDnsConfigRs = "{\"operation\":\"get-dns-response\",\"payload\":{\"nameserver1\": \"208.67.222.222\",\"nameserver2\": \"8.8.8.8\"}}";
  static char const * CCrlSetDnsConfig = "{\"operation\":\"set-dns\",\"payload\":{\"nameserver1\": \"208.67.222.222\",\"nameserver2\": \"8.8.8.8\"}}";
  static char const * CCrlSetDnsConfigRsOk = "{\"operation\":\"set-dns-response\",\"payload\":{\"success\": \"true\" }}";
  static char const * CCrlSetDnsConfigRsErr = "{\"operation\":\"set-dns-response\",\"payload\":{\"success\": \"false\", \"error-code\": \"01\", \"error-message\": \"DNS not saved\" } }";

  static char const * CCrlTestDns = "{\"operation\":\"test-dns\",\"payload\":{\"nameserver\": \"208.67.222.222\", \"domain-name\": \"www.gilbarco.com\"}}";
  static char const * CCrlTestDnsRsOk = "{\"operation\": \"test-dns-response\",\"payload\": {\"success\": \"true\", \"host-ip\": \"64.90.126.8\"}}";
  static char const * CCrlTestDnsRsErr = "{\"operation\": \"test-dns-response\",\"payload\": {\"success\": \"false\", \"error-code\": \"01\", \"error-message\": \"Domain not resolved\"}}";


  checkMessage(crlAny,CCrlAny);
//  checkMessage(crlCloudStatusRq,CCrlCloudStatus);
//  checkMessage(crlCloudStatusRs,CCrlCloudStatusResponse);
//  checkMessage(crlRegistrationRq,CCrlRegistration);
//  checkMessage(crlRegistrationRs,CCrlRegistrationResponse);

  checkMessage(crlGetDnsConfigRq,CCrlGetDnsConfig);
  checkMessage(crlGetDnsConfigRs,CCrlGetDnsConfigRs);

  checkMessage(crlSetDnsConfigRq,CCrlSetDnsConfig);
  checkMessage(crlSetDnsConfigRs,CCrlSetDnsConfigRsOk);
  checkMessage(crlSetDnsConfigRs,CCrlSetDnsConfigRsErr);

  checkMessage(crlTestDnsRq,CCrlTestDns);
  checkMessage(crlTestDnsRs,CCrlTestDnsRsOk);
  checkMessage(crlTestDnsRs,CCrlTestDnsRsErr);

  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}
