/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file spot_unittest.cpp
 * @author mgrieco
 * @date Friday, December 28, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gtest/gtest.h>
#include "../include/gvrspot_unittest/spot_unittest.h"

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

#include <iostream>


/**
 * @brief  Communication Unit Tests.
 * @since 1.0.0
 */

using namespace std;
using namespace gvr::communication::spot;
using Poco::Net::Socket;
using Poco::Net::StreamSocket;
using Poco::Net::SocketAddress;
using Poco::Thread;
using Poco::Timer;
using Poco::TimerCallback;
using Poco::Stopwatch;
using Poco::Timestamp;

///////////////////////////////////////////////////////////////////////////////////////////////////////////
class EchoServer: public Poco::Runnable
{
  public:
    EchoServer()
      : m_socket(SocketAddress()), m_thread("EchoServer"), m_stop(false), m_timer(3000,0), m_stopSend(false)
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
      m_timer.start(TimerCallback<EchoServer>(*this, &EchoServer::onTimer));
    }

    void onTimer(Timer& timer)
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
        if (m_socket.poll(span, Socket::SELECT_READ))
        {
          m_ss = m_socket.acceptConnection();

          try
          {
            m_ss.setBlocking(false);
            char buffer[256];
            int n = 0;

            while (!m_stop)
            {
              if( !m_ss.poll(span, Socket::SELECT_READ) )
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
    StreamSocket m_ss;

    Timer m_timer;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////



// Mock some classes for SpotChannel Unittest ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class ChannelListenerWorker: public Poco::Runnable, public gvr::communication::IChannelListener
{
  public:
    ChannelListenerWorker(std::string listenerName)
      : m_stop(false), m_workerName(listenerName),
        m_buffer(), m_status(SpotChannel::SpotChannelStatus::CStatusInitial),
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

    void setChannel(SpotChannel& channel)
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
   SpotChannel* m_pChannel;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////


// Mock some classes for SpotSession Unittest ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class SpotSessionListenerWorker: public Poco::Runnable, public ISpotSessionListener
{
  public:
    SpotSessionListenerWorker(std::string workerName)
      : m_stop(false), m_workerName(workerName), m_status(), m_typeId(), m_keyType(), m_buffer(), m_pSession(0)
    {
      m_thread.start(*this);
      m_ready.wait();
    }

    virtual ~SpotSessionListenerWorker()
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

      std::cout << "terminating: [" << m_workerName << "]" << std::endl;
    }

    void sessionEvent( BufferType const & buffer, TypeId const & typeId, KeyType const & keyType)
    {
      m_typeId  = typeId;
      m_keyType = keyType;

      m_buffer.clear();
      m_buffer.reserve(buffer.size());
      m_buffer.insert(m_buffer.end(),buffer.begin(),buffer.end());

      std::cout << "SpotSessionListenerWorker: [" << m_workerName << "] has been received an event." << std::endl;
    }

    void sessionEvent( StatusType status )
    {
      m_status = status;

      if( m_pSession )
      {
        std::cout << "SpotSessionListenerWorker: [" << m_workerName << "] has been received a new status [" << m_pSession->getStatusDescription(status) << "]" << std::endl;
      }
      else
      {
        std::cout << "SpotSessionListenerWorker: [" << m_workerName << "] has been received a new status [" << dec << static_cast<int>(status) << "]" << std::endl;
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

    void setSession(SpotSession& session)
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
   SpotSession* m_pSession;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////




/**
* @brief Performs connection throw a SpotChannel.
* @param GSpotTestUT The test case name.
* @param TestSpotChannel The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotChannel)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                    TestSpotChannel                     ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  ChannelListenerWorker listenerOne("Listener One");

  SpotChannel spotChannel("localhost", echoServer.port());

  spotChannel.start();

  spotChannel.subscribe( listenerOne );

  listenerOne.setChannel(spotChannel);

  EXPECT_EQ(spotChannel.getError(), SpotChannel::SpotChannelError::CNoError);

  if( spotChannel.getStatus() == SpotChannel::SpotChannelStatus::CStatusConnected )
  {
    std::string str = "hello";
    gvr::communication::IChannel::BufferType data(str.begin(), str.end());

    spotChannel.send(data);
    EXPECT_EQ(spotChannel.getError(), SpotChannel::SpotChannelError::CNoError);

    // Check the condition in a maximum elased time ...
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Timestamp initialTime;
    const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
    while (
            ( listenerOne.getLastBufferReceived().empty()                ) &&
            ( Timestamp::TimeDiff((Timestamp()-initialTime)) < usTimeout )
          )
    {
      Thread::sleep(1);
    }
    std::cout << "Elapsed time ...[" << std::dec << Timestamp::TimeDiff( (Timestamp()-initialTime) ) << " us]." << std::endl;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    EXPECT_EQ(listenerOne.getLastBufferReceived(), "hello");
  }

  spotChannel.stop();

  std::cout << "SpotChannel was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
*
* @brief Performs several connections and disconnections to check the SpotSession.
* @param GSpotTestUT The test case name.
* @param TestSpotSessionStopStart The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotSessionStopStart)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                TestSpotSessionStopStart                ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  SpotChannel spotChannel("localhost", echoServer.port());

  MockSpotSessionCancelTimers spotSession;

  for(int i=0; i<5; i++)
  {
    std::cout << "Starting the SpotSession( sequence [" << std::dec << i+1 << "] )..." << std::endl;

    spotSession.start(spotChannel);

    EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);

    if( spotSession.getError() != SpotSession::SpotSessionError::CNoError )
    {
      std::cout << "[" << spotSession.getErrorDescription() << "]" << std::endl;
    }

    EXPECT_EQ(spotSession.isConnected(), true);

    std::cout << "Stopping the SpotSession..." << std::endl;

    spotSession.stop();

    EXPECT_EQ(spotSession.isConnected(), false);

    EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);

    std::cout << "SpotSession was stopped..." << std::endl;

    if( spotSession.getError() != SpotSession::SpotSessionError::CNoError )
    {
      std::cout << "[" << spotSession.getErrorDescription() << "]" << std::endl;
    }
  }

  spotSession.stop();

  std::cout << "SpotSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs several send of messages in a async way.
* @param GSpotTestUT The test case name.
* @param TestSpotSessionAsyncSend The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotSessionAsyncSend)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                TestSpotSessionAsyncSend                ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  SpotSessionListenerWorker workerOne("Worker One");

  SpotChannel spotChannel("localhost", echoServer.port());

  MockSpotSessionCancelTimers spotSession;

  spotSession.subscribe(workerOne);

  workerOne.setSession(spotSession);

  spotSession.start(spotChannel);

  EXPECT_EQ(spotSession.isConnected(), true);

  // First asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    SpotMessagePing firstSpotPingMessageRq;
    spotSession.send(firstSpotPingMessageRq);
    EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerOne.getTypeId() != firstSpotPingMessageRq.getTypeId()  ) &&
              ( Timestamp::TimeDiff( (Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Timestamp::TimeDiff( (Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerOne.getTypeId(), firstSpotPingMessageRq.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  // Second asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    SpotMessagePing secondSpotPingMessageRq;
    spotSession.send(secondSpotPingMessageRq);
    EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerOne.getTypeId() != secondSpotPingMessageRq.getTypeId() ) &&
              ( Timestamp::TimeDiff( (Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Timestamp::TimeDiff( (Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerOne.getTypeId(), secondSpotPingMessageRq.getTypeId());

    SpotMessagePing spotPingMessageRq;
    spotPingMessageRq.parse(workerOne.getBuffer());
    EXPECT_EQ(spotPingMessageRq.getTypeId(), secondSpotPingMessageRq.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  std::cout << "Stopping the SpotSession..." << std::endl;

  spotSession.stop();
  EXPECT_EQ(spotSession.isConnected(), false);

  std::cout << "SpotSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs several send of messages in a sync way.
* @param GSpotTestUT The test case name.
* @param TestSpotSessionSyncSend The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotSessionSyncSend)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                TestSpotSessionSyncSend                 ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  SpotChannel spotChannel("localhost", echoServer.port());

  MockSpotSessionCancelTimers spotSession;

  spotSession.start(spotChannel);

  EXPECT_EQ(spotSession.isConnected(), true);

  // First synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  SpotMessagePing firstSpotPingMessageRq;
  spotSession.sendSync(firstSpotPingMessageRq, firstSpotPingMessageRq);
  EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);
  //////////////////////////////////////////////////////////////////////

  // Second synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  SpotMessagePing secondSpotPingMessageRq;
  spotSession.sendSync(secondSpotPingMessageRq, secondSpotPingMessageRq, 1000);
  EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);
  //////////////////////////////////////////////////////////////////////

  // Third synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  SpotMessagePing thirdSpotPingMessageRq;

  // Wait for another type of message to generate the desire condition, the timeout condition.
  SpotMessageLogin thirdSpotPingMessageRs;

  spotSession.sendSync(thirdSpotPingMessageRq, thirdSpotPingMessageRs, 10);
  EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CErrorTimeoutSendingMessage);
  //////////////////////////////////////////////////////////////////////

  // Fourth synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  SpotMessagePing fourthSpotPingMessageRq;
  spotSession.sendSync(fourthSpotPingMessageRq, fourthSpotPingMessageRq, 1000);
  EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);
  //////////////////////////////////////////////////////////////////////

  EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);

  spotSession.stop();

  EXPECT_EQ(spotSession.isConnected(), false);

  std::cout << "SpotSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs a simulation of a disconnection from the other end and if the Listeners respond well.
* @param GSpotTestUT The test case name.
* @param TestSpotSessionCheckDisconnect The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotSessionCheckDisconnect)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////            TestSpotSessionCheckDisconnect              ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  SpotChannel spotChannel("localhost", echoServer.port());

  MockSpotSessionCancelTimers spotSession;

  spotSession.start(spotChannel);

  EXPECT_EQ(spotSession.isConnected(), true);

  long ms_time_to_throw_the_disconnection = 1000; // 1 second
  long ms_time_to_wait_in_send_sync = 2000; // 2 seconds

  echoServer.startTimer(ms_time_to_throw_the_disconnection);

  // Send a message and wait the disconnect from the echo server ...
  //////////////////////////////////////////////////////////////////////
  SpotMessagePing spotPingMessageRq;
  SpotMessageLogin spotLoginMessageRs; // Wait for another type of message to generate the desired situation.
  spotSession.sendSync(spotPingMessageRq, spotLoginMessageRs, ms_time_to_wait_in_send_sync);
  Thread::sleep(10);
  EXPECT_EQ(spotSession.getStatus(), SpotSession::SpotSessionStatus::CStatusSessionStopped);
  //////////////////////////////////////////////////////////////////////

  spotSession.stop();

  EXPECT_EQ(spotSession.isConnected(), false);

  std::cout << "SpotSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
 *
 * Check if this test is still useful ...
 *
 *
* @brief Performs a simulation of a disconnection and the spot session retries connection.
* @param GSpotTestUT The test case name.
* @param TestSpotSessionRetries The test name.
* @since 1.0.0
*
TEST(GSpotTestUT, TestSpotSessionRetries)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////               TestSpotSessionRetries                   ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;
  SpotSessionListenerWorker workerOne("Worker One");
  SpotChannel spotChannel("localhost", echoServer.port());
  MockSpotSessionCancelTimers spotSession;
  spotSession.subscribe(workerOne);
  spotSession.start(spotChannel);
  EXPECT_EQ(spotSession.isReady(), true);
  EXPECT_EQ(workerOne.getStatus(), SpotSession::ISessionStatus::CStatusSessionStarted);
  Poco::Thread::sleep(1000);
  echoServer.stopSend();
  std::cout << spotSession.getCurrentTime() << " [stop echo channel]" << std::endl;
  Poco::Thread::sleep(5000);
  std::cout << spotSession.getCurrentTime() << " [restart echo channel]" << std::endl;
  echoServer.restartSend();
  std::cout << spotSession.getCurrentTime() <<  "[status: [" << spotSession.getStatusDescription() << "]]" << std::endl;
  EXPECT_EQ(workerOne.getStatus(), SpotSession::ISessionStatus::CStatusSessionStarted);
  EXPECT_EQ(spotSession.isConnected(), true);
  spotSession.stop();
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}*/

/**
* @brief Performs a simulation of a disconnection and the spot session retries connection.
* @param GSpotTestUT The test case name.
* @param TestSpotSessionRetries The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotSessionAvoidUnknownMsg)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////           TestSpotSessionAvoidUnknownMsg               ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  EchoServer echoServer;

  SpotChannel spotChannel("localhost", echoServer.port());
  MockSpotSessionNoAllClassifiers spotSession;

  spotSession.start(spotChannel);
  EXPECT_EQ(spotSession.isReady(), true);
  SpotMessagePing spotPingMessageRq;
  SpotMessagePingResponse spotPingMessageRs;
  spotSession.sendSync(spotPingMessageRq, spotPingMessageRs);
  EXPECT_EQ(spotSession.remainingBuffer(), false); //NO remaining buffer because the unknown ping message was discarded.
  spotSession.stop();
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs a simulation of a subscription to a particular message type.
* @param GSpotTestUT The test case name.
* @param TestSpotSessionSpecialMessageSubscribe The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotSessionSpecialMessageSubscribe)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////        TestSpotSessionSpecialMessageSubscribe          ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  SpotSessionListenerWorker workerOne("Worker One");

  SpotSessionListenerWorker workerTwo("Worker Two");

  SpotChannel spotChannel("localhost", echoServer.port());

  MockSpotSessionCancelTimers spotSession;

  SpotMessagePing firstSpotPingMessageRq;

  SpotMessageLogin firstSpotLoginMessageRq;

  SpotMessagePingResponse firstSpotPingMessageRs;

  SpotMessagePing firstSpotPingMessageRq2;

  spotSession.subscribe(workerOne, firstSpotPingMessageRq.getTypeId());

  spotSession.subscribe(workerOne, firstSpotPingMessageRq2.getTypeId());

  spotSession.subscribe(workerTwo, firstSpotLoginMessageRq.getTypeId());

  spotSession.subscribe(workerTwo, firstSpotPingMessageRs.getTypeId());

  EXPECT_EQ(firstSpotPingMessageRq.getTypeId(), SpotMessagePing::GetTypeId());

  workerOne.setSession(spotSession);
  workerTwo.setSession(spotSession);

  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  spotSession.start(spotChannel);

  EXPECT_EQ(spotSession.isConnected(), true);

  // First asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    spotSession.send(firstSpotPingMessageRq);
    EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerOne.getTypeId() != firstSpotPingMessageRq.getTypeId()  ) &&
              ( Timestamp::TimeDiff( (Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Timestamp::TimeDiff( (Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerOne.getTypeId(), firstSpotPingMessageRq.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  // Second asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    spotSession.send(firstSpotLoginMessageRq);
    EXPECT_EQ(spotSession.getError(), SpotSession::SpotSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerTwo.getTypeId() != firstSpotLoginMessageRq.getTypeId() ) &&
              ( Timestamp::TimeDiff( (Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Timestamp::TimeDiff( (Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerTwo.getTypeId(), firstSpotLoginMessageRq.getTypeId());

    SpotMessagePing spotPingMessageRq;
    spotPingMessageRq.parse(workerOne.getBuffer());
    EXPECT_EQ(spotPingMessageRq.getTypeId(), firstSpotPingMessageRq.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  std::cout << "Stopping the SpotSession..." << std::endl;

  spotSession.stop();
  EXPECT_EQ(spotSession.isConnected(), false);

  std::cout << "SpotSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs a verification of the class id types for all messages.
* @param GSpotTestUT The test case name.
* @param TestSpotSessionSpecialMessageSubscribe The test name.
* @since 1.0.0
*/
TEST(GSpotTestUT, TestSpotMessagesTypeIdVerification)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////          TestSpotMessagesTypeIdVerification            ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  SpotMessagePing spotPingMessageRq;
  SpotMessageReset spotResetMessageRq;
  SpotMessagePingResponse spotPingMessageRs;
  SpotMessageLogin spotLoginMessageRq;
  SpotMessageLoginResponse spotLoginMessageRs;
  SpotMessageHardwareInfo spotMessageHardwareInfoRq;
  SpotMessageHardwareInfoResponse spotMessageHardwareInfoRs;
  SpotMessageSystemStatusResponse spotMessageSystemStatusResponseRs;

  EXPECT_EQ( spotPingMessageRq.getTypeId(),                  SpotMessagePing::GetTypeId());
  EXPECT_EQ( spotResetMessageRq.getTypeId(),                 SpotMessageReset::GetTypeId());
  EXPECT_EQ( spotPingMessageRs.getTypeId(),                  SpotMessagePingResponse::GetTypeId());
  EXPECT_EQ( spotLoginMessageRq.getTypeId(),                 SpotMessageLogin::GetTypeId());
  EXPECT_EQ( spotLoginMessageRs.getTypeId(),                 SpotMessageLoginResponse::GetTypeId());
  EXPECT_EQ( spotMessageHardwareInfoRq.getTypeId(),          SpotMessageHardwareInfo::GetTypeId());
  EXPECT_EQ( spotMessageHardwareInfoRs.getTypeId(),          SpotMessageHardwareInfoResponse::GetTypeId());
  EXPECT_EQ( spotMessageSystemStatusResponseRs.getTypeId(),  SpotMessageSystemStatusResponse::GetTypeId());

  std::cout << "[" << std::hex << SpotMessagePing::GetTypeId()                 << "]" << std::endl;
  std::cout << "[" << std::hex << SpotMessageReset::GetTypeId()                << "]" << std::endl;
  std::cout << "[" << std::hex << SpotMessagePingResponse::GetTypeId()         << "]" << std::endl;
  std::cout << "[" << std::hex << SpotMessageLogin::GetTypeId()                << "]" << std::endl;
  std::cout << "[" << std::hex << SpotMessageLoginResponse::GetTypeId()        << "]" << std::endl;
  std::cout << "[" << std::hex << SpotMessageHardwareInfo::GetTypeId()         << "]" << std::endl;
  std::cout << "[" << std::hex << SpotMessageHardwareInfoResponse::GetTypeId() << "]" << std::endl;
  std::cout << "[" << std::hex << SpotMessageSystemStatusResponse::GetTypeId() << "]" << std::endl;

  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}
