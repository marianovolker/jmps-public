/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ucc_unittest.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gtest/gtest.h>
#include "../include/gvrucc_unittest/ucc_unittest.h"

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

#include <gvr/communication/ucc/UccChannel.h>
#include <gvr/communication/ucc/IUccSessionListener.h>
#include <gvr/communication/ucc/session/UccSessionClient.h>

#include <gvr/communication/ucc/message/UccMessageAny.h>
#include <gvr/communication/ucc/message/UccMessageCloudDeviceConfigChange.h>
#include <gvr/communication/ucc/message/UccMessageCloudDeviceConfigChangeResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownload.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileUpload.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileUploadResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudInstall.h>
#include <gvr/communication/ucc/message/UccMessageCloudInstallResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudInstallStatus.h>
#include <gvr/communication/ucc/message/UccMessageCloudReset.h>
#include <gvr/communication/ucc/message/UccMessageCloudResetResponse.h>
#include <gvr/communication/ucc/message/UccMessageLogin.h>
#include <gvr/communication/ucc/message/UccMessageLoginResponse.h>
#include <gvr/communication/ucc/message/UccMessageRegisterRequest.h>
#include <gvr/communication/ucc/message/UccMessageRegisterResponse.h>
#include <gvr/communication/ucc/message/UccMessageFileDownload.h>
#include <gvr/communication/ucc/message/UccMessageFileDownloadResponse.h>
#include <gvr/communication/ucc/message/UccMessageFileDownloadStatusEvent.h>
#include <gvr/communication/ucc/message/UccMessageServerUpdate.h>
#include <gvr/communication/ucc/message/UccMessageServerUpdateResponse.h>
#include <gvr/communication/ucc/message/UccMessageStatus.h>
#include <gvr/communication/ucc/message/UccMessageConnect.h>
#include <gvr/communication/ucc/message/UccMessageConnectResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadStatusEvent.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadStatusEventResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileUploadStatusEvent.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileUploadStatusEventResponse.h>
#include <gvr/communication/ucc/message/UccMessageSendToCloud.h>
#include <gvr/communication/ucc/message/UccMessageSendToCloudResponse.h>

#include <gvr/communication/ucc/message/UccMessageSendToCloudStatusEvent.h>

#include <gvr/communication/ucc/message/UccMessageCloudSecurityChange.h>
#include <gvr/communication/ucc/message/UccMessageCloudSecurityChangeResponse.h>

#include <gvr/communication/ucc/classifier/UccClassifyMessageInternalServerForLogin.h>
#include <gvr/communication/ucc/classifier/UccClassifyMessageInternalClientForLoginResponse.h>

#include <gvr/communication/ucc/message/UccMessageCloudReconnect.h>
#include <gvr/communication/ucc/message/UccMessageCloudReconnectResponse.h>

#include <gvr/communication/ucc/message/UccMessageTestDns.h>
#include <gvr/communication/ucc/message/UccMessageTestDnsResponse.h>

#include <gvr/communication/ucc/message/UccMessageCloudDeviceComponentStatusEvent.h>
#include <gvr/communication/ucc/message/UccMessageCloudDeviceComponentStatusEventResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudDeviceStatusEvent.h>
#include <gvr/communication/ucc/message/UccMessageCloudDeviceStatusEventResponse.h>

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

// Mock some classes for UccChannel Unittest ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class ChannelListenerWorker: public Poco::Runnable, public gvr::communication::IChannelListener
{
  public:
    ChannelListenerWorker(std::string listenerName)
      : m_stop(false), m_workerName(listenerName),
        m_buffer(), m_status(gvr::communication::ucc::UccChannel::UccChannelStatus::CStatusInitial),
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

    void setChannel(gvr::communication::ucc::UccChannel& channel)
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
   gvr::communication::ucc::UccChannel* m_pChannel;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////

// Mock some classes for UccSession Unittest ...
///////////////////////////////////////////////////////////////////////////////////////////////////////////
class UccSessionListenerWorker: public Poco::Runnable, public gvr::communication::ucc::IUccSessionListener
{
  public:
    UccSessionListenerWorker(std::string workerName)
      : m_stop(false), m_workerName(workerName), m_status(), m_typeId(), m_keyType(), m_buffer(), m_pSession(0)
    {
      m_thread.start(*this);
      m_ready.wait();
    }

    virtual ~UccSessionListenerWorker()
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

      std::cout << "UccSessionListenerWorker: [" << m_workerName << "] has been received an event." << std::endl;
    }

    void sessionEvent( StatusType status )
    {
      m_status = status;

      if( m_pSession )
      {
        std::cout << "UccSessionListenerWorker: [" << m_workerName << "] has been received a new status [" << m_pSession->getStatusDescription(status) << "]" << std::endl;
      }
      else
      {
        std::cout << "UccSessionListenerWorker: [" << m_workerName << "] has been received a new status [" << std::dec << static_cast<int>(status) << "]" << std::endl;
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

    void setSession(gvr::communication::ucc::UccSession& session)
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
   gvr::communication::ucc::UccSession* m_pSession;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////////////
class MockUccSessionNoConnectionCycle
 : public gvr::communication::ucc::UccSessionClient
{
public:
  MockUccSessionNoConnectionCycle (void) : UccSessionClient()
  {
    addClassifier(m_internalClientClassifierForLoginResponse);
  }

  virtual bool isLogged (void) { return isConnected(); }

  virtual ~MockUccSessionNoConnectionCycle (void) {}

  virtual void removeLoginResponseClassifier (void) { removeClassifier(m_internalClientClassifierForLoginResponse); }
  virtual bool remainingBuffer (void) { return !m_buffer.empty(); }
protected:
  virtual bool private_unsafe_doLogin (void)
  {
    gvr::communication::ucc::UccMessageLogin uccLoginMessageRq;
    gvr::communication::ucc::UccMessageLoginResponse uccLoginMessageRs;

    // This is the only message that could be sent at started session state because this will generate
    // the availability of the communication with SPOT.
    AbsSessionType::sendSync(uccLoginMessageRq,uccLoginMessageRs);

    ErrorType result = getError();

    if (result != UccSession::UccSessionError::CNoError)
    {
      setError(UccSession::UccSessionError::CErrorOnLoging);
      return false;
    }
    return true;
  }

  virtual void private_safe_startConnectionCycle (void) {}
  virtual void private_safe_stopConnectionCycle (void) {}

protected:
  /** @brief the session login response classifier.*/
  gvr::communication::ucc::UccClassifyMessageInternalClientForLoginResponse m_internalClientClassifierForLoginResponse;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////


/**
* @brief Performs connection through a UccChannel.
* @param GUccTestUT The test case name.
* @param TestUccChannel The test name.
* @since 1.0.0
*/
TEST(GUccTestUT, TestUccChannel)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                     TestUccChannel                     ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  ChannelListenerWorker listenerOne("Listener One");

  gvr::communication::ucc::UccChannel uccChannel("localhost", echoServer.port());

  uccChannel.start();

  uccChannel.subscribe( listenerOne );

  listenerOne.setChannel(uccChannel);

  EXPECT_EQ(uccChannel.getError(), gvr::communication::ucc::UccChannel::UccChannelError::CNoError);

  if( uccChannel.getStatus() == gvr::communication::ucc::UccChannel::UccChannelStatus::CStatusConnected )
  {
    std::string str = "hello";
    gvr::communication::IChannel::BufferType data(str.begin(), str.end());

    uccChannel.send(data);
    EXPECT_EQ(uccChannel.getError(), gvr::communication::ucc::UccChannel::UccChannelError::CNoError);

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

  uccChannel.stop();

  std::cout << "UccChannel was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
*
* @brief Performs several connections and disconnections to check the UccSession.
* @param GUccTestUT The test case name.
* @param TestUccSessionStopStart The test name.
* @since 1.0.0
*/
TEST(GUccTestUT, TestUccSessionStopStart)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                 TestUccSessionStopStart                ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  gvr::communication::ucc::UccChannel uccChannel("localhost", echoServer.port());

  MockUccSessionNoConnectionCycle uccSession;

  for(int i=0; i<5; i++)
  {
    std::cout << "Starting the UccSession( sequence [" << std::dec << i+1 << "] )..." << std::endl;

    uccSession.start(uccChannel);

    EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);

    if( uccSession.getError() != gvr::communication::ucc::UccSession::UccSessionError::CNoError )
    {
      std::cout << "[" << uccSession.getErrorDescription() << "]" << std::endl;
    }

    EXPECT_EQ(uccSession.isConnected(), true);

    std::cout << "Stopping the UccSession..." << std::endl;

    uccSession.stop();

    EXPECT_EQ(uccSession.isConnected(), false);

    EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);

    std::cout << "UccSession was stopped..." << std::endl;

    if( uccSession.getError() != gvr::communication::ucc::UccSession::UccSessionError::CNoError )
    {
      std::cout << "[" << uccSession.getErrorDescription() << "]" << std::endl;
    }
  }

  uccSession.stop();

  std::cout << "UccSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs several send of messages in a async way.
* @param GUccTestUT The test case name.
* @param TestUccSessionAsyncSend The test name.
* @since 1.0.0
*/
TEST(GUccTestUT, TestUccSessionAsyncSend)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                 TestUccSessionAsyncSend                ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  UccSessionListenerWorker workerOne("Worker One");

  gvr::communication::ucc::UccChannel uccChannel("localhost", echoServer.port());

  MockUccSessionNoConnectionCycle uccSession;

  gvr::communication::ucc::UccClassifyMessageInternalServerForLogin loginClassifier;
  uccSession.addClassifier(loginClassifier);

  uccSession.subscribe(workerOne);

  workerOne.setSession(uccSession);

  uccSession.start(uccChannel);

  EXPECT_EQ(uccSession.isConnected(), true);

  // First asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    gvr::communication::ucc::UccMessageLogin firstUccLoginMessageRq;
    uccSession.send(firstUccLoginMessageRq);
    EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Poco::Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerOne.getTypeId() != firstUccLoginMessageRq.getTypeId()  ) &&
              ( Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Poco::Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerOne.getTypeId(), firstUccLoginMessageRq.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  // Second asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    gvr::communication::ucc::UccMessageLogin secondUccLoginMessageRq;
    uccSession.send(secondUccLoginMessageRq);
    EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Poco::Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerOne.getTypeId() != secondUccLoginMessageRq.getTypeId() ) &&
              ( Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Poco::Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerOne.getTypeId(), secondUccLoginMessageRq.getTypeId());

    gvr::communication::ucc::UccMessageLogin uccLoginMessageRq;
    uccLoginMessageRq.parse(workerOne.getBuffer());
    EXPECT_EQ(uccLoginMessageRq.getTypeId(), secondUccLoginMessageRq.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  std::cout << "Stopping the UccSession..." << std::endl;

  uccSession.stop();
  EXPECT_EQ(uccSession.isConnected(), false);

  std::cout << "UccSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs several send of messages in a sync way.
* @param GUccTestUT The test case name.
* @param TestUccSessionSyncSend The test name.
* @since 1.0.0
*/
TEST(GUccTestUT, TestUccSessionSyncSend)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                 TestUccSessionSyncSend                 ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  gvr::communication::ucc::UccChannel uccChannel("localhost", echoServer.port());

  MockUccSessionNoConnectionCycle uccSession;

  gvr::communication::ucc::UccClassifyMessageInternalServerForLogin loginClassifier;
  uccSession.addClassifier(loginClassifier);

  uccSession.start(uccChannel);

  EXPECT_EQ(uccSession.isConnected(), true);

  // First synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  gvr::communication::ucc::UccMessageLogin firstUccLoginMessageRq;
  uccSession.sendSync(firstUccLoginMessageRq, firstUccLoginMessageRq);
  EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);
  //////////////////////////////////////////////////////////////////////

  // Second synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  gvr::communication::ucc::UccMessageLogin secondUccLoginMessageRq;
  uccSession.sendSync(secondUccLoginMessageRq, secondUccLoginMessageRq, 1000);
  EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);
  //////////////////////////////////////////////////////////////////////

  // Third synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  gvr::communication::ucc::UccMessageLogin thirdUccLoginMessageRq;

  // Wait for another type of message to generate the desire condition, the timeout condition.
  // Remember the echo server answers with the very same message as the request.
  gvr::communication::ucc::UccMessageLoginResponse thirdUccLoginMessageRs;

  uccSession.sendSync(thirdUccLoginMessageRq, thirdUccLoginMessageRs, 10);
  EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CErrorTimeoutSendingMessage);
  //////////////////////////////////////////////////////////////////////

  // Fourth synchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  gvr::communication::ucc::UccMessageLogin fourthUccLoginMessageRq;
  uccSession.sendSync(fourthUccLoginMessageRq, fourthUccLoginMessageRq, 1000);
  EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);
  //////////////////////////////////////////////////////////////////////

  EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);

  uccSession.stop();

  EXPECT_EQ(uccSession.isConnected(), false);

  std::cout << "UccSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs a simulation of a disconnection from the other end and if the Listeners respond well.
* @param GUccTestUT The test case name.
* @param TestUccSessionCheckDisconnect The test name.
* @since 1.0.0
*/
TEST(GUccTestUT, TestUccSessionCheckDisconnect)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////             TestUccSessionCheckDisconnect              ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  gvr::communication::ucc::UccChannel uccChannel("localhost", echoServer.port());

  MockUccSessionNoConnectionCycle uccSession;

  uccSession.start(uccChannel);

  EXPECT_EQ(uccSession.isConnected(), true);

  long ms_time_to_throw_the_disconnection = 1000; // 1 second
  long ms_time_to_wait_in_send_sync = 2000; // 2 seconds

  echoServer.startTimer(ms_time_to_throw_the_disconnection);

  // Send a message and wait the disconnect from the echo server ...
  //////////////////////////////////////////////////////////////////////
  gvr::communication::ucc::UccMessageLogin uccLoginMessageRq;
  gvr::communication::ucc::UccMessageLoginResponse uccLoginMessageRs; // Wait for another type of message to generate the desired situation.
  uccSession.sendSync(uccLoginMessageRq, uccLoginMessageRs, ms_time_to_wait_in_send_sync);
  Poco::Thread::sleep(10);
  EXPECT_EQ(uccSession.getStatus(), gvr::communication::ucc::UccSession::UccSessionStatus::CStatusSessionStopped);
  //////////////////////////////////////////////////////////////////////

  uccSession.stop();

  EXPECT_EQ(uccSession.isConnected(), false);

  std::cout << "UccSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
 *
 * Check if this test is still useful ...
 *
 *
* @brief Performs a simulation of a disconnection and the ucc session retries connection.
* @param GUccTestUT The test case name.
* @param TestUccSessionRetries The test name.
* @since 1.0.0
*
TEST(GUccTestUT, TestUccSessionRetries)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////                TestUccSessionRetries                   ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;
  gvr::communication::ucc::UccSessionListenerWorker workerOne("Worker One");
  gvr::communication::ucc::UccChannel uccChannel("localhost", echoServer.port());
  MockUccSessionNoConnectionCycle uccSession;
  uccSession.subscribe(workerOne);
  uccSession.start(uccChannel);
  EXPECT_EQ(uccSession.isReady(), true);
  EXPECT_EQ(workerOne.getStatus(), UccSession::ISessionStatus::CStatusSessionStarted);
  Poco::Thread::sleep(1000);
  echoServer.stopSend();
  std::cout << uccSession.getCurrentTime() << " [stop echo channel]" << std::endl;
  Poco::Thread::sleep(5000);
  std::cout << uccSession.getCurrentTime() << " [restart echo channel]" << std::endl;
  echoServer.restartSend();
  std::cout << uccSession.getCurrentTime() <<  "[status: [" << uccSession.getStatusDescription() << "]]" << std::endl;
  EXPECT_EQ(workerOne.getStatus(), UccSession::ISessionStatus::CStatusSessionStarted);
  EXPECT_EQ(uccSession.isConnected(), true);
  uccSession.stop();
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}*/

/**
* @brief Performs a simulation of a disconnection and the ucc session retries connection.
* @param GUccTestUT The test case name.
* @param TestUccSessionAvoidUnknownMsg The test name.
* @since 1.0.0
*/
TEST(GUccTestUT, TestUccSessionAvoidUnknownMsg)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////           TestUccSessionAvoidUnknownMsg               ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  EchoServer echoServer;

  gvr::communication::ucc::UccChannel uccChannel("localhost", echoServer.port());
  MockUccSessionNoConnectionCycle uccSession;
  uccSession.removeLoginResponseClassifier();


  uccSession.start(uccChannel);
  EXPECT_EQ(uccSession.isReady(), true);
  gvr::communication::ucc::UccMessageLogin uccLoginMessageRq;
  gvr::communication::ucc::UccMessageLoginResponse uccLoginMessageRs;
  uccSession.sendSync(uccLoginMessageRq,uccLoginMessageRs);
  EXPECT_EQ(uccSession.remainingBuffer(), false); //NO remaining buffer because the unknown ping message was discarded.
  uccSession.stop();
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs a simulation of a subscription to a particular message type.
* @param GUccTestUT The test case name.
* @param TestUccSessionSpecialMessageSubscribe The test name.
* @since 1.0.0
*/
TEST(GUccTestUT, TestUccSessionSpecialMessageSubscribe)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////         TestUccSessionSpecialMessageSubscribe          ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  EchoServer echoServer;

  UccSessionListenerWorker workerOne("Worker One");

  UccSessionListenerWorker workerTwo("Worker Two");

  gvr::communication::ucc::UccChannel uccChannel("localhost", echoServer.port());

  MockUccSessionNoConnectionCycle uccSession;

  gvr::communication::ucc::UccClassifyMessageInternalServerForLogin loginClassifier;
  uccSession.addClassifier(loginClassifier);

  gvr::communication::ucc::UccMessageLogin forOneUccLoginMessageRq;

  gvr::communication::ucc::UccMessageLoginResponse forTwoUccLoginMessageRs;

  uccSession.subscribe(workerOne, forOneUccLoginMessageRq.getTypeId());

  uccSession.subscribe(workerTwo, forTwoUccLoginMessageRs.getTypeId());

  EXPECT_EQ(forOneUccLoginMessageRq.getTypeId(), gvr::communication::ucc::UccMessageLogin::GetTypeId());

  workerOne.setSession(uccSession);
  workerTwo.setSession(uccSession);

  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  uccSession.start(uccChannel);

  EXPECT_EQ(uccSession.isConnected(), true);

  // First asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    uccSession.send(forOneUccLoginMessageRq);
    EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Poco::Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerOne.getTypeId() != forOneUccLoginMessageRq.getTypeId()  ) &&
              ( Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Poco::Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerOne.getTypeId(), forOneUccLoginMessageRq.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  // Second asynchronous send and response evaluation ...
  //////////////////////////////////////////////////////////////////////
  {
    uccSession.send(forTwoUccLoginMessageRs);
    EXPECT_EQ(uccSession.getError(), gvr::communication::ucc::UccSession::UccSessionError::CNoError);
    // Check the condition in a maximum elapsed time ...
    {
      Poco::Timestamp initialTime;
      const unsigned long usTimeout = 800000; // timeout in microseconds... 1000 microseconds are 1 millisecond.
      while (
              ( workerTwo.getTypeId() != forTwoUccLoginMessageRs.getTypeId() ) &&
              ( Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) < usTimeout )
            )
      {
        Poco::Thread::sleep(1);
      }
      std::cout << "Elapsed time ...[" << std::dec << Poco::Timestamp::TimeDiff( (Poco::Timestamp()-initialTime) ) << " us]." << std::endl;
    }
    EXPECT_EQ(workerTwo.getTypeId(), forTwoUccLoginMessageRs.getTypeId());

    gvr::communication::ucc::UccMessageLoginResponse uccLoginMessageRs;
    uccLoginMessageRs.parse(workerTwo.getBuffer());
    EXPECT_EQ(uccLoginMessageRs.getTypeId(), forTwoUccLoginMessageRs.getTypeId());
  }
  //////////////////////////////////////////////////////////////////////

  std::cout << "Stopping the UccSession..." << std::endl;

  uccSession.stop();
  EXPECT_EQ(uccSession.isConnected(), false);

  std::cout << "UccSession was stopped..." << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

/**
* @brief Performs a verification of the class id types for all messages.
* @param GUccTestUT The test case name.
* @param TestUccSessionSpecialMessageSubscribe The test name.
* @since 1.0.0
*/
TEST(GUccTestUT, TestUccMessagesTypeIdVerification)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////           TestUccMessagesTypeIdVerification            ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  gvr::communication::ucc::UccMessageAny uccAny;
  gvr::communication::ucc::UccMessageCloudDeviceConfigChange uccCloudDeviceConfigChange;
  gvr::communication::ucc::UccMessageCloudDeviceConfigChangeResponse uccCloudDeviceConfigChangeRs;
  gvr::communication::ucc::UccMessageCloudFileDownload uccCloudFileDownload;
  gvr::communication::ucc::UccMessageCloudFileDownloadResponse uccCloudFileDownloadRs;
  gvr::communication::ucc::UccMessageCloudInstall uccCloudInstall;
  gvr::communication::ucc::UccMessageCloudInstallResponse uccCloudInstallRs;
  gvr::communication::ucc::UccMessageCloudInstallStatus uccCloudInstallStatus;
  gvr::communication::ucc::UccMessageCloudReset uccCloudReset;
  gvr::communication::ucc::UccMessageCloudResetResponse uccCloudResetRs;
  gvr::communication::ucc::UccMessageLogin uccLogin;
  gvr::communication::ucc::UccMessageLoginResponse uccLoginRs;
  gvr::communication::ucc::UccMessageRegisterRequest uccRegister;
  gvr::communication::ucc::UccMessageRegisterResponse uccRegisterRs;
  gvr::communication::ucc::UccMessageFileDownload uccFileDownload;
  gvr::communication::ucc::UccMessageFileDownloadResponse uccFileDownloadRs;
  gvr::communication::ucc::UccMessageFileDownloadStatusEvent uccFileDownloadStatusEvent;
  gvr::communication::ucc::UccMessageServerUpdate uccServerUpdate;
  gvr::communication::ucc::UccMessageServerUpdateResponse uccServerUpdateRs;
  gvr::communication::ucc::UccMessageStatus uccStatus;
  gvr::communication::ucc::UccMessageConnect uccConnect;
  gvr::communication::ucc::UccMessageConnectResponse uccConnectRs;
  gvr::communication::ucc::UccMessageCloudFileDownloadStatusEvent uccCloudFileDownloadStatusEvent;
  gvr::communication::ucc::UccMessageCloudFileDownloadStatusEventResponse uccCloudFileDownloadStatusEventRs;
  gvr::communication::ucc::UccMessageSendToCloudStatusEvent uccSendToCloudStatusEvent;

  gvr::communication::ucc::UccMessageCloudFileUpload uccCloudFileUpload;
  gvr::communication::ucc::UccMessageCloudFileUploadResponse uccCloudFileUploadRs;

  gvr::communication::ucc::UccMessageCloudFileUploadStatusEvent uccCloudFileUploadStatusEvent;
  gvr::communication::ucc::UccMessageCloudFileUploadStatusEventResponse uccCloudFileUploadStatusEventRs;

  gvr::communication::ucc::UccMessageCloudSecurityChange uccCloudSecurityChange;
  gvr::communication::ucc::UccMessageCloudSecurityChangeResponse uccCloudSecurityChangeRs;

  gvr::communication::ucc::UccMessageSendToCloud uccSendToCloud;
  gvr::communication::ucc::UccMessageSendToCloudResponse uccSendToCloudRs;

  gvr::communication::ucc::UccMessageCloudReconnect uccCloudReconnect;
  gvr::communication::ucc::UccMessageCloudReconnectResponse uccCloudReconnectRs;

  gvr::communication::ucc::UccMessageTestDns uccTestDns;
  gvr::communication::ucc::UccMessageTestDnsResponse uccTestDnsRs;

  gvr::communication::ucc::UccMessageCloudDeviceComponentStatusEvent uccDeviceComponentStatusEvent;
  gvr::communication::ucc::UccMessageCloudDeviceComponentStatusEventResponse uccDeviceComponentStatusEventRs;

  gvr::communication::ucc::UccMessageCloudDeviceStatusEvent uccDeviceStatusEvent;
  gvr::communication::ucc::UccMessageCloudDeviceStatusEventResponse uccDeviceStatusEventRs;

  EXPECT_EQ( uccAny.getTypeId(),                       gvr::communication::ucc::UccMessageAny::GetTypeId());
  EXPECT_EQ( uccCloudDeviceConfigChange.getTypeId(),   gvr::communication::ucc::UccMessageCloudDeviceConfigChange::GetTypeId());
  EXPECT_EQ( uccCloudDeviceConfigChangeRs.getTypeId(), gvr::communication::ucc::UccMessageCloudDeviceConfigChangeResponse::GetTypeId());
  EXPECT_EQ( uccCloudFileDownload.getTypeId(),         gvr::communication::ucc::UccMessageCloudFileDownload::GetTypeId());
  EXPECT_EQ( uccCloudFileDownloadRs.getTypeId(),       gvr::communication::ucc::UccMessageCloudFileDownloadResponse::GetTypeId());
  EXPECT_EQ( uccCloudInstall.getTypeId(),              gvr::communication::ucc::UccMessageCloudInstall::GetTypeId());
  EXPECT_EQ( uccCloudInstallRs.getTypeId(),            gvr::communication::ucc::UccMessageCloudInstallResponse::GetTypeId());
  EXPECT_EQ( uccCloudInstallStatus.getTypeId(),        gvr::communication::ucc::UccMessageCloudInstallStatus::GetTypeId());
  EXPECT_EQ( uccCloudReset.getTypeId(),                gvr::communication::ucc::UccMessageCloudReset::GetTypeId());
  EXPECT_EQ( uccCloudResetRs.getTypeId(),              gvr::communication::ucc::UccMessageCloudResetResponse::GetTypeId());
  EXPECT_EQ( uccLogin.getTypeId(),                     gvr::communication::ucc::UccMessageLogin::GetTypeId());
  EXPECT_EQ( uccLoginRs.getTypeId(),                   gvr::communication::ucc::UccMessageLoginResponse::GetTypeId());
  EXPECT_EQ( uccRegister.getTypeId(),                  gvr::communication::ucc::UccMessageRegisterRequest::GetTypeId());
  EXPECT_EQ( uccRegisterRs.getTypeId(),                gvr::communication::ucc::UccMessageRegisterResponse::GetTypeId());
  EXPECT_EQ( uccFileDownload.getTypeId(),              gvr::communication::ucc::UccMessageFileDownload::GetTypeId());
  EXPECT_EQ( uccFileDownloadRs.getTypeId(),            gvr::communication::ucc::UccMessageFileDownloadResponse::GetTypeId());
  EXPECT_EQ( uccFileDownloadStatusEvent.getTypeId(),   gvr::communication::ucc::UccMessageFileDownloadStatusEvent::GetTypeId());
  EXPECT_EQ( uccServerUpdate.getTypeId(),              gvr::communication::ucc::UccMessageServerUpdate::GetTypeId());
  EXPECT_EQ( uccServerUpdateRs.getTypeId(),            gvr::communication::ucc::UccMessageServerUpdateResponse::GetTypeId());
  EXPECT_EQ( uccStatus.getTypeId(),                    gvr::communication::ucc::UccMessageStatus::GetTypeId());
  EXPECT_EQ( uccConnect.getTypeId(),                   gvr::communication::ucc::UccMessageConnect::GetTypeId());
  EXPECT_EQ( uccConnectRs.getTypeId(),                 gvr::communication::ucc::UccMessageConnectResponse::GetTypeId());

  EXPECT_EQ( gvr::communication::ucc::AbsUccMessageCloudCommandResponse::GetTypeId(), gvr::communication::ucc::UccMessageCloudFileDownloadResponse::GetTypeId());
  EXPECT_EQ( gvr::communication::ucc::AbsUccMessageCloudCommandResponse::GetTypeId(), gvr::communication::ucc::UccMessageCloudResetResponse::GetTypeId());

  EXPECT_EQ( gvr::communication::ucc::AbsUccMessageCloudEventResponse::GetTypeId(), gvr::communication::ucc::UccMessageCloudDeviceConfigChangeResponse::GetTypeId());
  EXPECT_EQ( gvr::communication::ucc::AbsUccMessageCloudEventResponse::GetTypeId(), gvr::communication::ucc::UccMessageCloudFileDownloadStatusEventResponse::GetTypeId());

  EXPECT_EQ( gvr::communication::ucc::AbsUccMessageRegister::GetTypeId()          , gvr::communication::ucc::UccMessageRegisterRequest::GetTypeId());
  EXPECT_EQ( gvr::communication::ucc::AbsUccMessageRegister::GetTypeId()          , gvr::communication::ucc::UccMessageRegisterResponse::GetTypeId());

  EXPECT_EQ( uccSendToCloudStatusEvent.getTypeId(),   gvr::communication::ucc::UccMessageSendToCloudStatusEvent::GetTypeId());
  EXPECT_EQ( uccCloudFileUpload.getTypeId(),          gvr::communication::ucc::UccMessageCloudFileUpload::GetTypeId());
  EXPECT_EQ( uccCloudFileUploadRs.getTypeId(),        gvr::communication::ucc::UccMessageCloudFileUploadResponse::GetTypeId());

  EXPECT_EQ( uccCloudFileUploadStatusEvent.getTypeId(),          gvr::communication::ucc::UccMessageCloudFileUploadStatusEvent::GetTypeId());
  EXPECT_EQ( uccCloudFileUploadStatusEventRs.getTypeId(),        gvr::communication::ucc::UccMessageCloudFileUploadStatusEventResponse::GetTypeId());

  EXPECT_EQ( uccCloudSecurityChange.getTypeId(),          gvr::communication::ucc::UccMessageCloudSecurityChange::GetTypeId());
  EXPECT_EQ( uccCloudSecurityChangeRs.getTypeId(),        gvr::communication::ucc::UccMessageCloudSecurityChangeResponse::GetTypeId());

  EXPECT_EQ( uccSendToCloud.getTypeId(),                  gvr::communication::ucc::UccMessageSendToCloud::GetTypeId());
  EXPECT_EQ( uccSendToCloudRs.getTypeId(),                  gvr::communication::ucc::UccMessageSendToCloudResponse::GetTypeId());

  EXPECT_EQ( uccCloudReconnect.getTypeId(),          gvr::communication::ucc::UccMessageCloudReconnect::GetTypeId());
  EXPECT_EQ( uccCloudReconnectRs.getTypeId(),        gvr::communication::ucc::UccMessageCloudReconnectResponse::GetTypeId());

  EXPECT_EQ( uccTestDns.getTypeId(),                   gvr::communication::ucc::UccMessageTestDns::GetTypeId());
  EXPECT_EQ( uccTestDnsRs.getTypeId(),                 gvr::communication::ucc::UccMessageTestDnsResponse::GetTypeId());

  EXPECT_EQ( uccDeviceComponentStatusEvent.getTypeId(),   gvr::communication::ucc::UccMessageCloudDeviceComponentStatusEvent::GetTypeId());
  EXPECT_EQ( uccDeviceComponentStatusEventRs.getTypeId(), gvr::communication::ucc::UccMessageCloudDeviceComponentStatusEventResponse::GetTypeId());

  EXPECT_EQ( uccDeviceStatusEvent.getTypeId(),   gvr::communication::ucc::UccMessageCloudDeviceStatusEvent::GetTypeId());
  EXPECT_EQ( uccDeviceStatusEventRs.getTypeId(), gvr::communication::ucc::UccMessageCloudDeviceStatusEventResponse::GetTypeId());

  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageAny::GetTypeId()                                  << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudDeviceConfigChange::GetTypeId()              << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudDeviceConfigChangeResponse::GetTypeId()      << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudFileDownload::GetTypeId()                    << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudFileDownloadResponse::GetTypeId()            << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudInstall::GetTypeId()                         << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudInstallResponse::GetTypeId()                 << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudInstallStatus::GetTypeId()                   << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudReset::GetTypeId()                           << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudResetResponse::GetTypeId()                   << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageLogin::GetTypeId()                                << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageLoginResponse::GetTypeId()                        << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageRegisterRequest::GetTypeId()                      << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageRegisterResponse::GetTypeId()                     << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageFileDownload::GetTypeId()                         << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageFileDownloadResponse::GetTypeId()                 << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageFileDownloadStatusEvent::GetTypeId()              << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageServerUpdate::GetTypeId()                         << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageServerUpdateResponse::GetTypeId()                 << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageConnect::GetTypeId()                              << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageConnectResponse::GetTypeId()                      << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageStatus::GetTypeId()                               << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudFileDownloadStatusEvent::GetTypeId()         << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudFileDownloadStatusEventResponse::GetTypeId() << "]" << std::endl;

  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageSendToCloudStatusEvent::GetTypeId()                << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudFileUpload::GetTypeId()                       << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudFileUploadResponse::GetTypeId()               << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudFileUploadStatusEvent::GetTypeId()            << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudFileUploadStatusEventResponse::GetTypeId()    << "]" << std::endl;

  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudSecurityChange::GetTypeId()                   << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudSecurityChangeResponse::GetTypeId() << "]"    << std::endl;

  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageSendToCloud::GetTypeId() << "]"                    << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageSendToCloudResponse::GetTypeId() << "]"            << std::endl;

  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudReconnect::GetTypeId()                   << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageCloudReconnectResponse::GetTypeId() << "]"    << std::endl;

  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageTestDns::GetTypeId()                   << "]" << std::endl;
  std::cout << "[" << std::hex << gvr::communication::ucc::UccMessageTestDnsResponse::GetTypeId()           << "]"    << std::endl;

  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}

void checkMessage (gvr::communication::ucc::AbsUccMessage & msg, char const * CMsgJson)
{
  {
    gvr::utils::Json json;
    EXPECT_TRUE(json.parse(CMsgJson));

    msg.setJson(json);
    EXPECT_TRUE(msg.validate());
  }

  {
    gvr::communication::ucc::AbsUccMessage::BufferType buff = msg.serialize();
    EXPECT_FALSE(buff.empty());

    EXPECT_TRUE(msg.parse(buff));
  }
}

/**
* @brief Performs a verification for the validation of all messages.
* @param GUccTestUT The test case name.
* @param TestUccSessionSpecialMessageSubscribe The test name.
* @since 1.0.0
*/
TEST(GUccTestUT, TestUccMessages)
{
  std::cout << std::endl << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << "//////           TestUccMessages                              ////////" << std::endl;
  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;

  gvr::communication::ucc::UccMessageAny uccAny;
  gvr::communication::ucc::UccMessageCloudDeviceConfigChange uccCloudDeviceConfigChange;
  gvr::communication::ucc::UccMessageCloudDeviceConfigChangeResponse uccCloudDeviceConfigChangeRs;
  gvr::communication::ucc::UccMessageCloudFileDownload uccCloudFileDownload;
  gvr::communication::ucc::UccMessageCloudFileDownloadResponse uccCloudFileDownloadRs;
  gvr::communication::ucc::UccMessageCloudInstall uccCloudInstall;
  gvr::communication::ucc::UccMessageCloudInstallResponse uccCloudInstallRs;
  gvr::communication::ucc::UccMessageCloudInstallStatus uccCloudInstallStatus;

  gvr::communication::ucc::UccMessageCloudFileUpload uccCloudFileUpload;
  gvr::communication::ucc::UccMessageCloudFileUploadResponse uccCloudFileUploadRs;

  gvr::communication::ucc::UccMessageCloudReset uccCloudReset;
  gvr::communication::ucc::UccMessageCloudResetResponse uccCloudResetRs;
  gvr::communication::ucc::UccMessageLogin uccLogin;
  gvr::communication::ucc::UccMessageLoginResponse uccLoginRs;
  gvr::communication::ucc::UccMessageRegisterRequest uccRegister;
  gvr::communication::ucc::UccMessageRegisterResponse uccRegisterRs;
  gvr::communication::ucc::UccMessageFileDownload uccFileDownload;
  gvr::communication::ucc::UccMessageFileDownloadResponse uccFileDownloadRs;
  gvr::communication::ucc::UccMessageFileDownloadStatusEvent uccFileDownloadStatusEvent;
  gvr::communication::ucc::UccMessageServerUpdate uccServerUpdate;
  gvr::communication::ucc::UccMessageServerUpdateResponse uccServerUpdateRs;
  gvr::communication::ucc::UccMessageStatus uccStatus;
  gvr::communication::ucc::UccMessageConnect uccConnect;
  gvr::communication::ucc::UccMessageConnectResponse uccConnectRs;
  gvr::communication::ucc::UccMessageCloudFileDownloadStatusEvent uccCloudFileDownloadStatusEvent;
  gvr::communication::ucc::UccMessageCloudFileDownloadStatusEventResponse uccCloudFileDownloadStatusEventRs;

  gvr::communication::ucc::UccMessageCloudFileUploadStatusEvent uccCloudFileUploadStatusEvent;
  gvr::communication::ucc::UccMessageCloudFileUploadStatusEventResponse uccCloudFileUploadStatusEventRs;

  gvr::communication::ucc::UccMessageCloudSecurityChange uccCloudSecurityChange;
  gvr::communication::ucc::UccMessageCloudSecurityChangeResponse uccCloudSecurityChangeRs;

  gvr::communication::ucc::UccMessageSendToCloudStatusEvent uccSendToCloudStatusEvent;
  gvr::communication::ucc::UccMessageSendToCloud uccSendToCloud;
  gvr::communication::ucc::UccMessageSendToCloudResponse uccSendToCloudRs;

  gvr::communication::ucc::UccMessageCloudReconnect uccCloudReconnect;
  gvr::communication::ucc::UccMessageCloudReconnectResponse uccCloudReconnectRs;

  gvr::communication::ucc::UccMessageTestDns uccTestDns;
  gvr::communication::ucc::UccMessageTestDnsResponse uccTestDnsRs;

  gvr::communication::ucc::UccMessageCloudDeviceComponentStatusEvent uccCloudDeviceComponentStatusEvent;
  gvr::communication::ucc::UccMessageCloudDeviceComponentStatusEventResponse uccCloudDeviceComponentStatusEventRs;

  gvr::communication::ucc::UccMessageCloudDeviceStatusEvent uccCloudDeviceStatusEvent;
  gvr::communication::ucc::UccMessageCloudDeviceStatusEventResponse uccCloudDeviceStatusEventRs;

  static char const * CUccAny = "{ \"any\": \"any\" }";
  static char const * CUccCloudDeviceConfigChange = "";
  static char const * CUccCloudDeviceConfigChangeRs = "";
  static char const * CUccCloudFileDownload = "";
  static char const * CUccCloudFileDownloadRs = "";

  static char const * CUccCloudFileUpload = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"file-upload-cmd\",  \"deviceId\": \"1\",  \"payload\": { \"fileCategory\": \"log\", \"fileType\": \"pinpad\", \"trackingNumber\": \"98487054230857\",\"url\": \"https://cloud.gso.gilbarco.com/file\"} }";
  static char const * CUccCloudFileUploadRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": true }";
  static char const * CUccCloudFileUploadRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": false, \"code\": \"1\", \"message\": \"General Error\" } }";

  static char const * CUccCloudFileUploadStatusEvent = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\",\"operation\": \"file-upload-event\",\"deviceId\": \"14\",\"payload\": {\"fileCategory\": \"log\",\"fileType\": \"pinpad\",\"fileId\": \"2329D93PW830DI90\",\"trackingNumber\": \"230148910K\",\"status\": \"complete\",\"fileCompression\": \"gzip\" }}";
  static char const * CUccCloudFileUploadStatusEventRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": true }";
  static char const * CUccCloudFileUploadStatusEventRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": false, \"code\": \"1\", \"message\": \"General Error\" } }";

  static char const * CUccCloudInstall = "";
  static char const * CUccCloudInstallRs = "";
  static char const * CUccCloudInstallStatus = "";
  static char const * CUccCloudReset = "";
  static char const * CUccCloudResetRs = "";
  static char const * CUccLogin = "";
  static char const * CUccLoginRs = "";
  static char const * CUccRegister = "{ \"registerDevice\": { \"registrationCode\": \"98124356709812435670\", \"certFormat\": \"PEM\", \"dnsSupported\": true }, \"deviceConfig\": { \"deviceType\": \"FDT\", \"localId\": \"1\", \"assetInfo\": { \"manufacturer\": \"Gilbarco Veeder-Root\", \"model\": \"Raspberry Model\", \"serialNbr\": \"Raspberry serial\", \"firmware\": { \"FDT\": \"1.34.1234\", \"RPI\": \"13.120131206I\", \"Software Name on Raspberry\": \"Version of the software\" }, \"component\": [ { \"name\": \"This would be your modem\", \"remove\": true, \"attributes\": { \"sim\": \"sim identifier\", \"additional attribute name\": \"value of attribute\" } }, { \"name\": \"Raspberry PI\", \"attribute\": { \"RPI attribute name\": \"value to track\" } } ] }, \"events\": [ { \"operation\": \"cloud-device-config-change-event\", \"payload\": \"http://json-schema.gilbarco.com/v1/event/cloud-device-config-change-event.json\" }, { \"operation\": \"status-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/event/status-event.json\" }, { \"operation\": \"file-download-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/event/file-download-event.json\" }, { \"operation\": \"file-upload-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/event/file-upload-event.json\" }, { \"operation\": \"install-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/event/install-event.json\" } ], \"cmds\": [ { \"operation\": \"heartbeat-cmd\", \"request\": \"http://json-schema.gilbarco.com/v1/command/request-heartbeat-cmd.json\", \"response\": \"\" }, { \"operation\": \"reconnect-cmd\", \"request\": \"http://json-schema.gilbarco.com/v1/command/request-reconnect-cmd.json\", \"response\": \"{}\" }, { \"operation\": \"security-change-cmd\", \"request\": \"http://json-schema.gilbarco.com/v1/command/request-security-change-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/v1/command/response-security-change-cmd.json\" }, { \"operation\": \"file-download-cmd\", \"request\": \"http://json-schema.gilbarco.com/fdt/v1/command/request-file-download-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/fdt/v1/command/response-file-download-cmd.json\" }, { \"operation\": \"file-upload-cmd\", \"request\": \"http://json-schema.gilbarco.com/fdt/v1/command/request-file-upload-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/fdt/v1/command/response-file-upload-cmd.json\"  }, { \"operation\": \"install-cmd\", \"request\": \"http://json-schema.gilbarco.com/fdt/v1/command/request-install-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/fdt/v1/command/response-install-cmd.json\" } ], \"associated\": [ { \"deviceType\": \"M5\", \"localId\": \"1\", \"proxied\": true, \"assetInfo\": { \"manufacturer\": \"GilbarcoVeeder-Root\", \"model\": \"M55K2340\", \"serialNbr\": \"12345678\", \"firmware\": { \"BOOT\": \"1.1.0p2824\", \"RECOVERY\": \"02.02.0P2645\", \"OS\": \"02.02.0P2848\", \"PINPAD\": \"02.05.2P2848\", \"EMV\": \"00.00.0p0\", \"OPT\": \"02.02.1p2848\", \"SETUP\": \"02.02.1p2834\", \"CRINDBIOS\": \"99.9.0p25833\", \"OPTCLOUDINTERFACE\": \"0.0.1\" }, \"component\": [ { \"name\": \"CardReader\", \"attributes\": { \"type\": \"82\", \"model\": \"ZU-1873TT243\", \"S/N\": \"1108524588072911\", \"FirmwareVer\": \"RD012-100204_1\" } }, { \"name\": \"Display\", \"remove\": true, \"attributes\": { \"type\": \"A4\" } } ] }, \"events\": [ { \"operation\": \"status-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/events/m5-status-event.json\" }, { \"operation\": \"file-download-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/events/m5-file-download-event.json\" }, { \"operation\": \"file-upload-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/events/m5-file-upload-event.json\" }, { \"operation\": \"install-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/events/m5-install-event.json\" } ], \"cmds\": [ { \"operation\": \"heartbeat-cmd\", \"request\": \"http://json-schema.gilbarco.com/v1/command/request-heartbeat-cmd.json\", \"response\": \"\" }, { \"operation\": \"file-download-cmd\", \"request\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-request-file-download-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-response-file-download-cmd.json\" }, { \"operation\": \"file-upload-cmd\", \"request\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-request-file-upload-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-response-file-upload-cmd.json\" }, { \"operation\": \"install-cmd\", \"request\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-request-install-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-response-install-cmd.json\" } ] }, { \"deviceType\": \"M5\", \"localId\": \"3\", \"proxied\": true, \"assetInfo\": { \"manufacturer\": \"GilbarcoVeeder-Root\", \"model\": \"M55K2340\", \"serialNbr\": \"63981839\", \"firmware\": { \"BOOT\": \"1.1.0p2824\", \"RECOVERY\": \"02.02.0P2645\", \"OS\": \"02.02.0P2848\", \"PINPAD\": \"02.05.2P2848\", \"EMV\": \"00.00.0p0\", \"OPT\": \"02.02.1p2848\", \"SETUP\": \"02.02.1p2834\", \"CRINDBIOS\": \"99.9.0p25833\", \"OPTCLOUDINTERFACE\": \"0.0.1\" }, \"component\": [ { \"name\": \"CardReader\", \"attributes\": { \"type\": \"82\", \"model\": \"ZU-1873TT243\", \"S/N\": \"1108524588072911\", \"FirmwareVer\": \"RD012-100204_1\" } }, { \"name\": \"Display\", \"attributes\": { \"type\": \"A4\" } } ] }, \"events\": [ { \"operation\": \"status-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/events/m5-status-event.json\" }, { \"operation\": \"file-download-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/events/m5-file-download-event.json\" }, { \"operation\": \"file-upload-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/events/m5-file-upload-event.json\" }, { \"operation\": \"install-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/v1/events/m5-install-event.json\" } ], \"cmds\": [ { \"operation\": \"heartbeat-cmd\", \"request\": \"http://json-schema.gilbarco.com/v1/command/request-heartbeat-cmd.json\", \"response\": \"{}\" }, { \"operation\": \"file-download-cmd\", \"request\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-request-file-download-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-response-file-download-cmd.json\" }, { \"operation\": \"file-upload-cmd\", \"request\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-request-file-upload-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-response-file-upload-cmd.json\" }, { \"operation\": \"install-cmd\", \"request\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-request-install-cmd.json\", \"response\": \"http://json-schema.gilbarco.com/fdt/command/v1/m5-response-install-cmd.json\" } ] }, { \"deviceType\": \"E7000\", \"localId\": \"1\", \"proxied\": true, \"assetInfo\": { \"manufacturer\": \"GilbarcoVeeder-Root\", \"model\": \"E7K384D2\", \"serialNbr\": \"93872090\", \"firmware\": { \"OS\": \"02.02.0P2848\" }, \"component\": [ { \"name\": \"fileter\", \"attributes\": { \"type\": \"T345\" } } ] }, \"events\": [ { \"operation\": \"status-event\", \"payload\": \"http://json-schema.gilbarco.com/fdt/events/v1/disp-status-event.json\" } ], \"cmds\": [ { \"operation\": \"heartbeat-cmd\", \"request\": \"http://json-schema.gilbarco.com/command/v1/request-heartbeat-cmd.json\", \"response\": \"{}\" } ] } ] } }";
  static char const * CUccRegisterRsOk = "{ \"success\": true, \"config\": { \"gvrSiteId\": 1234, \"deviceId\": \"32459\", \"connect\": { \"uri\": \"wss://device.gilbarco.com/socket/device/32459\", \"certFormat\": \"JKS\", \"cert\": \"2iorS82312mSFAFOAIW34......\", \"userid\": \"devices/32459\", \"pwd\": \"K45M4%5kB$KWj15P&dIt\" }, \"deviceList\": [ { \"deviceType\": \"FDT\", \"deviceId\": \"32459\", \"localId\": \"1\", \"cloudState\": \"enabled\", \"fileUploadURL\": \"https://transfer.gilbarco.com/services/upload-v1/32459\" }, { \"deviceType\": \"m5\", \"deviceId\": \"38910\", \"localId\": \"1\", \"cloudState\": \"enabled\", \"fileUploadURL\": \"https://transfer.gilbarco.com/services/upload-v1/38910\" }, { \"deviceType\": \"m5\", \"deviceId\": \"31935\", \"localId\": \"3\", \"cloudState\": \"enabled\", \"fileUploadURL\": \"https://transfer.gilbarco.com/services/upload-v1/31935\" }, { \"deviceType\": \"E7000\", \"deviceId\": \"38912\", \"localId\": \"1\", \"cloudState\": \"enabled\", \"fileUploadURL\": \"https://transfer.gilbarco.com/services/upload-v1/38912\" } ] } }";
  static char const * CUccRegisterRsErr = "{ \"message\": \"Device service error\", \"code\": \"DEVICE_SERVICE_ERROR\", \"success\": false, \"errorNumber\": 50002 }";
  static char const * CUccFileDownload = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"receiveFromCloud\", \"deviceId\": \"1\", \"payload\": { \"url\": \"https://cloud.gso.gilbarco.com/file\", \"path\": \"/gilbarco/downloads/file\", \"fileSize\": 10000, \"trackingNumber\": \"98487054230857\", \"checksumType\": \"TheChecksumType\", \"checksum\": \"Checksum\" } }";
  static char const * CUccFileDownloadRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"receiveFromCloud\", \"deviceId\": \"1\", \"payload\": { \"success\": true, \"path\": \"/gilbarco/downloads/file\", \"fileSize\": 10000, \"trackingNumber\":\"98487054230857\" } }";
  static char const * CUccFileDownloadStOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"receiveFromCloud-status\", \"deviceId\": \"1\", \"payload\": { \"status\": \"complete\", \"currentSize\": 10000, \"fileSize\": 10000, \"trackingNumber\":\"98487054230857\" } }";
  static char const * CUccFileDownloadRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"receiveFromCloud\", \"deviceId\": \"1\", \"payload\": { \"success\": false, \"trackingNumber\":\"98487054230857\", \"code\": 1, \"message\": \"General Error\" } }";
  static char const * CUccFileDownloadStErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"receiveFromCloud-status\", \"deviceId\": \"1\", \"payload\": { \"status\": \"transfer-failure\", \"currentSize\": 7000, \"fileSize\": 10000, \"trackingNumber\":\"98487054230857\", \"code\": 2, \"message\":\"transfer-failure\" } }";
  static char const * CUccServerUpdate = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"server update\", \"deviceId\": \"1\", \"payload\": { \"url\": \"https://registration.gso.gilbarco.com\", \"nameserver1\": \"208.67.222.222\", \"nameserver2\": \"8.8.8.8\", \"cert\": \"2iorS82309JSFAFOAIW342iorS82309JSFAFOAIW342iorS82309JSFAFOAIW342iorS82309JSFAFOAIW342iorS82309JSFAFOAIW342iorS82309JSFAFOAIW34\", \"certFormat\": \"PEM\", \"certPwd\": \"K45#mkW93$KWj105&dIQ\", \"user\": \"gilbarco\", \"pwd\": \"K45#mkW93$KWj105&dIQ\" } }";
  static char const * CUccServerUpdateRs = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"server update\", \"deviceId\": \"1\", \"payload\": { \"success\": false, \"code\": 1, \"message\": \"General Error\" } }";
  static char const * CUccConnect = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"connect\", \"deviceId\": \"1\", \"payload\": { \"uri\": \"wss://device.gilbarco.com/socket/device/38910\", \"nameserver1\": \"208.67.222.222\", \"nameserver2\": \"8.8.8.8\", \"certFormat\": \"P12\", \"cert\": \"2iorS82309JSFAFOAIW342iorS82309JSFAFOAIW342iorS82309JSFAFOAIW342iorS82309JSFAFOAIW342iorS82309JSFAFOAIW342iorS82309JSFAFOAIW34\", \"userid\": \"gilbarco\", \"pwd\": \"K45#mkW93$KWj105&dIQ\", \"connectionDelay\": 60 } }";
  static char const * CUccConnectRs = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"connect\", \"deviceId\": \"1\", \"payload\": { \"success\": false, \"code\": 1, \"message\": \"General Error\" } }";
  static char const * CUccStatus = "{  \"operation\": \"status\", \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"deviceId\": \"1714\", \"payload\": { \"operative\": true, \"connection\": \"Connected\", \"registration\": \"Activated\" } }";
  static char const * CUccCloudFileDownloadStatusEvent = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"file-download-event\", \"deviceId\": \"14\", \"payload\": { \"fileType\": \"software-bundle\", \"trackingNumber\": \"2329D93PW830DI90\", \"status\": \"success\" } }";
  static char const * CUccCloudFileDownloadStatusEventRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": true }";
  static char const * CUccCloudFileDownloadStatusEventRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": false, \"code\": \"1\", \"message\": \"General Error\" } }";

  static char const * CUccCloudSecurityChange = "{\"messageId\":\"123e4567-e89b-12d3-a456-426655440000\",\"operation\":\"security-change-cmd\",\"deviceId\":\"14\",\"payload\":{\"uri\":\"site-gw.sandbox.aws.gilbarco.com:8443/gateway/socket/device/14\",\"certFormat\":\"p12\",\"cert\":\"qiurqu83whhf9093JANAPWE9R3J.........\",\"pwd\":\"1ED99md#31093ndJ$KD#\"}}";
  static char const * CUccCloudSecurityChangeRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": true }";
  static char const * CUccCloudSecurityChangeRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": false, \"code\": \"1\", \"message\": \"General Error\" } }";

  static char const * CUccSendToCloudStatusOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"sendToCloud-status\", \"deviceId\": \"1\", \"payload\": { \"status\": \"complete\", \"currentSize\": 10000, \"fileSize\": 10000, \"trackingNumber\":\"98487054230857\", \"fileId\":\"2329D93PW830DI90\" } }";
  static char const * CUccSendToCloudStatusErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"sendToCloud-status\", \"deviceId\": \"1\", \"payload\": { \"status\": \"transfer-failure\", \"currentSize\": 7000, \"fileSize\": 10000, \"trackingNumber\":\"98487054230857\", \"code\": 2, \"message\":\"transfer-failure\" } }";

  static char const * CUccSendToCloud = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"sendToCloud\",\"deviceId\": \"1\",\"payload\": {\"url\": \"https://cloud.gso.gilbarco.com/file\",\"path\": \"/gilbarco/downloads/file\",\"fileSize\": 10000,\"trackingNumber\": \"98487054230857\",\"checksum\": \"Checksum\",\"fileType\": \"pinpad\",\"compressionType\": \"gzip\" } }";;
  static char const * CUccSendToCloudRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"sendToCloud\", \"deviceId\": \"1\", \"payload\": { \"success\": true} }";
  static char const * CUccSendToCloudRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"sendToCloud\", \"deviceId\": \"1\", \"payload\": { \"success\": false, \"trackingNumber\":\"98487054230857\", \"code\": 1, \"message\": \"General Error\" } }";


  static char const * CUccCloudReconnect = "{\"messageId\":\"123e4567-e89b-12d3-a456-426655440000\",\"operation\":\"reconnect-cmd\",\"deviceId\":\"14\"}";
  static char const * CUccCloudReconnectRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": true}";
  static char const * CUccCloudReconnectRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": false, \"code\": \"1\", \"message\": \"General Error\" } }";

  static char const * CUccTestDns = "{\"messageId\":\"123e4567-e89b-12d3-a456-426655440000\",\"operation\":\"testDNS\",\"deviceId\":\"14\", \"payload\": {\"nameserver\":\"208.67.222.222\",\"domain-name\":\"www.gilbarco.com\"} }";
  static char const * CUccTestDnsRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\":\"testDNS\",\"deviceId\":\"14\", \"payload\": { \"success\": true, \"host-ip\":\"64.90.126.8\" } }";
  static char const * CUccTestDnsRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\":\"testDNS\",\"deviceId\":\"14\", \"payload\": { \"success\": false, \"code\": 301, \"message\": \"Couldn't resolve host name\" } }";

  static char const * CUccCloudDeviceComponentStatusEvent = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"component-status-event\", \"deviceId\": \"14\", \"payload\": { \"component\": \"CardReader\", \"status\": \"idle\", \"attribute\": {\"state\": \"1\"}} }";
  static char const * CUccCloudDeviceComponentStatusEventRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": true }";
  static char const * CUccCloudDeviceComponentStatusEventRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": false, \"code\": \"1\", \"message\": \"General Error\" } }";

  static char const * CUccCloudDeviceStatusEvent = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"operation\": \"status-event\", \"deviceId\": \"14\", \"payload\": { \"status\": \"idle\", \"attribute\": {\"state\": \"1\"}} }";
  static char const * CUccCloudDeviceStatusEventRsOk = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": true }";
  static char const * CUccCloudDeviceStatusEventRsErr = "{ \"messageId\": \"123e4567-e89b-12d3-a456-426655440000\", \"success\": false, \"code\": \"1\", \"message\": \"General Error\" } }";

  checkMessage(uccAny,CUccAny);
  checkMessage(uccRegister,CUccRegister);
  checkMessage(uccRegisterRs,CUccRegisterRsOk);
  checkMessage(uccRegisterRs,CUccRegisterRsErr);
  checkMessage(uccFileDownload,CUccFileDownload);
  checkMessage(uccFileDownloadRs,CUccFileDownloadRsOk);
  checkMessage(uccFileDownloadRs,CUccFileDownloadRsErr);
  checkMessage(uccFileDownloadStatusEvent,CUccFileDownloadStOk);
  checkMessage(uccFileDownloadStatusEvent,CUccFileDownloadStErr);
  checkMessage(uccServerUpdate,CUccServerUpdate);
  checkMessage(uccServerUpdateRs,CUccServerUpdateRs);
  checkMessage(uccConnect,CUccConnect);
  checkMessage(uccConnectRs,CUccConnectRs);
  checkMessage(uccStatus,CUccStatus);

  checkMessage(uccCloudFileDownloadStatusEvent,CUccCloudFileDownloadStatusEvent);
  checkMessage(uccCloudFileDownloadStatusEventRs,CUccCloudFileDownloadStatusEventRsOk);
  checkMessage(uccCloudFileDownloadStatusEventRs,CUccCloudFileDownloadStatusEventRsErr);

  checkMessage(uccCloudFileUploadStatusEvent,CUccCloudFileUploadStatusEvent);
  checkMessage(uccCloudFileUploadStatusEventRs,CUccCloudFileUploadStatusEventRsOk);
  checkMessage(uccCloudFileUploadStatusEventRs,CUccCloudFileUploadStatusEventRsErr);

  checkMessage(uccCloudFileUpload,CUccCloudFileUpload);
  checkMessage(uccCloudFileUploadRs,CUccCloudFileUploadRsOk);
  checkMessage(uccCloudFileUploadRs,CUccCloudFileUploadRsErr);

  checkMessage(uccCloudSecurityChange,CUccCloudSecurityChange);
  checkMessage(uccCloudSecurityChangeRs,CUccCloudSecurityChangeRsOk);
  checkMessage(uccCloudSecurityChangeRs,CUccCloudSecurityChangeRsErr);

  checkMessage(uccSendToCloudStatusEvent,CUccSendToCloudStatusOk);
  checkMessage(uccSendToCloudStatusEvent,CUccSendToCloudStatusErr);

  checkMessage(uccSendToCloud,CUccSendToCloud);
  checkMessage(uccSendToCloudRs,CUccSendToCloudRsOk);
  checkMessage(uccSendToCloudRs,CUccSendToCloudRsErr);

  checkMessage(uccCloudReconnect,CUccCloudReconnect);
  checkMessage(uccCloudReconnectRs,CUccCloudReconnectRsOk);
  checkMessage(uccCloudReconnectRs,CUccCloudReconnectRsErr);

  checkMessage(uccTestDns,CUccTestDns);
  checkMessage(uccTestDnsRs,CUccTestDnsRsOk);
  checkMessage(uccTestDnsRs,CUccTestDnsRsErr);

  checkMessage(uccCloudDeviceComponentStatusEvent,CUccCloudDeviceComponentStatusEvent);
  checkMessage(uccCloudDeviceComponentStatusEventRs,CUccCloudDeviceComponentStatusEventRsOk);
  checkMessage(uccCloudDeviceComponentStatusEventRs,CUccCloudDeviceComponentStatusEventRsErr);

  checkMessage(uccCloudDeviceStatusEvent,CUccCloudDeviceStatusEvent);
  checkMessage(uccCloudDeviceStatusEventRs,CUccCloudDeviceStatusEventRsOk);
  checkMessage(uccCloudDeviceStatusEventRs,CUccCloudDeviceStatusEventRsErr);

  std::cout << "//////////////////////////////////////////////////////////////////////" << std::endl;
  std::cout << std::endl << std::endl;
}
