
/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotSession.cpp
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <gvr/communication/spot/SpotSession.h>

# include <gvr/communication/spot/Utils.h>
# include <iostream>

# include <Poco/LocalDateTime.h>
# include <Poco/DateTimeFormatter.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      StatusType const SpotSession::SpotSessionStatus::CSessionLogged      =   SpotSession::AbsSessionType::SessionStatus::CLastStatus + 1;
      StatusType const SpotSession::SpotSessionStatus::CSessionLoggedOut   =   SpotSession::AbsSessionType::SessionStatus::CLastStatus + 2;
      StatusType const SpotSession::SpotSessionStatus::CLastStatus         =   SpotSession::AbsSessionType::SessionStatus::CLastStatus + 10;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      ErrorType const SpotSession::SpotSessionError::CErrorSendingPing                              =   SpotSession::AbsSessionType::SessionError::CLastError + 1;
      ErrorType const SpotSession::SpotSessionError::CErrorSendingLogging                           =   SpotSession::AbsSessionType::SessionError::CLastError + 2;
      ErrorType const SpotSession::SpotSessionError::CErrorNoSpotSessionAvailable                   =   SpotSession::AbsSessionType::SessionError::CLastError + 3;
      ErrorType const SpotSession::SpotSessionError::CErrorThereIsAlreadyLoggedSessionWithThatAppId =   SpotSession::AbsSessionType::SessionError::CLastError + 4;

      ErrorType const SpotSession::SpotSessionError::CLastError                                     =   SpotSession::AbsSessionType::SessionError::CLastError + 10;
      /////////////////////////////////////////////////////////////////////////////////////

      // Mnemonics for supported platforms
      const char * SpotSession::CapcPlatforms[eTotalPlatforms] = { "unknown", "m3", "m5", "m7" };


      SpotSession::SpotSession ( EDevicePlatform ePlatform /* = eM7 */ )
        : AbsSessionType(CDefaultTimeOut, CDefaultRetries),
          m_clientId(0), m_moduloLen(0), m_pingTimeout(0),
          m_eDevicePlatform(ePlatform),
          m_pendingConnectionRetries(CDefaultConnectionRetries),
          m_pendingLoginRetries(CDefaultRetries),
          m_timerToSendLogin(CDefaultTimeToSendLogin, 0),
          m_timerToRetryConnect(CDefaultTimeBetweenEachRetryConnection, 0),
          m_timerToSendPing(CDefaultTimeToSendPing, 0),
          m_mxSpotSession()
      {
        //--
        m_descriptionStatusList [ SpotSessionStatus::CSessionLogged                                 ] = "SpotSession is at logged state";
        m_descriptionStatusList [ SpotSessionStatus::CSessionLoggedOut                              ] = "SpotSession is at logged out state";
        m_descriptionStatusList [ SpotSessionStatus::CLastStatus                                    ] = "SpotSession is at unknown state";
        //--
        m_descriptionErrorList  [ SpotSessionError::CErrorSendingPing                               ] = "SpotSession could not send a message ping.";
        m_descriptionErrorList  [ SpotSessionError::CErrorSendingLogging                            ] = "SpotSession could not send a message ping.";
        m_descriptionErrorList  [ SpotSessionError::CErrorNoSpotSessionAvailable                    ] = "No available SpotSession to send a message.";
        m_descriptionErrorList  [ SpotSessionError::CErrorThereIsAlreadyLoggedSessionWithThatAppId  ] = "There is already a logged session with that application id.";
        m_descriptionErrorList  [ SpotSessionError::CLastError                                      ] = "SpotSession has an unknown error.";
        //--

        addClassifier(m_pingResponseClassifier);
        addClassifier(m_loginResponseClassifier);
        addClassifier(m_hardwareInfoResponseClassifier);
        addClassifier(m_softwareInfoResponseClassifier);
        addClassifier(m_optHardwareInfoResponseClassifier);
        addClassifier(m_optSoftwareInfoResponseClassifier);
        addClassifier(m_packageInfoResponseClassifier);
        addClassifier(m_debianInfoResponseClassifier);
        addClassifier(m_packageDownloadResponseClassifier);
        addClassifier(m_packageBlockDownloadResponseClassifier);
        addClassifier(m_VgdFileTransferResponseClassifier);
        addClassifier(m_VgdProcessPackageResponseClassifier);
        addClassifier(m_VgdPackageActivationResponseClassifier);
        addClassifier(m_packageActivationResponseClassifier);
        addClassifier(m_fileUploadResponseClassifier);
        addClassifier(m_fileUploadBlockResponseClassifier);
        addClassifier(m_fileDownloadResponseClassifier);
        addClassifier(m_fileDownloadBlockResponseClassifier);
        addClassifier(m_fileDeleteResponseClassifier);
        addClassifier(m_fileDeleteResponseClassifier);
        addClassifier(m_newEphemeralKeyResponseClassifier);
        addClassifier(m_ephemeralEncryptResponseClassifier);
        addClassifier(m_systemStatusClassifier);
      }

      SpotSession::~SpotSession (void)
      {
        stop();
      }

      ErrorType SpotSession::start ()
      {
        ScopedLock local(m_mxSpotSession);

        if( AbsSessionType::start() == AbsSessionType::SessionError::CNoError )
        {
          restartSendLoginTimer();
        }
        else
        {
          std::cout << "Spot Session Error - [" << getErrorDescription() << "]" << std::endl;

          restartRetryConnectionTimer();
        }

        return getError();
      }

      ErrorType SpotSession::start (SpotChannel & channel)
      {
        setChannel(channel);

        return start();
      }

      ErrorType SpotSession::start (SpotChannel & channel, unsigned char const clientId, unsigned char const moduloLen, unsigned char const pingTimeout)
      {
        m_clientId    = clientId;
        m_moduloLen   = moduloLen;
        m_pingTimeout = pingTimeout;

        return start(channel);
      }

      ErrorType SpotSession::stop ()
      {
        ScopedLock local(m_mxSpotSession);

        pauseTimer(m_timerToSendLogin);
        pauseTimer(m_timerToRetryConnect);
        pauseTimer(m_timerToSendPing);

        ErrorType error( setError(AbsSessionType::stop()) );

        unSetChannel();

        return error;
      }

      ErrorType SpotSession::sendSync( IMessageType const & messageRq, IMessageType & messageRs, TimeMillisecondsType timeout )
      {
        if( isLogged() )
        {
          ScopedLock local(m_mxSpotSession);

          return setError(AbsSessionType::sendSync(messageRq, messageRs, timeout));
        }
        else
        {
          return setError(SpotSession::SpotSessionError::CErrorNoSpotSessionAvailable);
        }
      }

      ErrorType SpotSession::sendSync( IMessageType const & messageRq, IMessageType & messageRs )
      {
        return setError(sendSync(messageRq, messageRs, getTimeoutDefault()));
      }

      ErrorType SpotSession::send( IMessageType const & messageRq )
      {
        if( isLogged() )
        {
          ScopedLock local(m_mxSpotSession);

          return setError(AbsSessionType::send(messageRq));
        }
        else
        {
          return setError(SpotSession::SpotSessionError::CErrorNoSpotSessionAvailable);
        }
      }

      unsigned char SpotSession::getClientId()
      {
        ScopedLock local(m_mxSpotSession);

        return m_clientId;
      }

      unsigned char SpotSession::getModuloLen()
      {
        ScopedLock local(m_mxSpotSession);

        return m_moduloLen;
      }

      unsigned char SpotSession::getPingTimeout()
      {
        ScopedLock local(m_mxSpotSession);

        return m_pingTimeout;
      }

      void SpotSession::setClientId(unsigned char const id)
      {
        ScopedLock local(m_mxSpotSession);

        m_clientId=id;
      }

      void SpotSession::setModuloLen(unsigned char const len)
      {
        ScopedLock local(m_mxSpotSession);

        m_moduloLen=len;
      }

      void SpotSession::setPingTimeout(unsigned char const time)
      {
        ScopedLock local(m_mxSpotSession);

        m_pingTimeout=time;
      }


      SpotSession::EDevicePlatform  SpotSession::getDevicePlatform() const
      {
        return m_eDevicePlatform;
      }

      const char*  SpotSession::getDevicePlatformAcronym() const
      {
        return CapcPlatforms[m_eDevicePlatform];
      }


      ///////////////////////////////////////
      // Protected methods ...
      ///////////////////////////////////////

      void SpotSession::channelEvent (StatusType channelStatus)
      {
        StatusType previousState(getStatus());
        if( previousState == SpotSession::SpotSessionStatus::CSessionOnError )
        {
          std::cout << "[Spot Session Error: [" << getErrorDescription() << "]]" << std::endl;
        }

        // This will generate a change state in the base class.
        AbsSessionType::channelEvent(channelStatus);

        StatusType currentState(getStatus());
      }

      void SpotSession::callbackToSendLogin(Poco::Timer&)
      {
        if( (getStatus() == SpotSession::SpotSessionStatus::CStatusSessionStarted) && ((m_pendingLoginRetries--) > 0) )
        {
          if( login() || (getStatus() == SpotSession::SpotSessionStatus::CSessionLogged) )
          {
            pauseTimer(m_timerToSendLogin);

            std::cout << "[Spot Session is Logged]" << std::endl;

            restartSendPingTimer();
          }
          else
          {
            resumeTimer(m_timerToSendLogin, CDefaultTimeToSendLogin);
          }
        }
        else
        {
          setStatus(SpotSession::SpotSessionStatus::CSessionOnError);

          pauseTimer(m_timerToSendLogin);

          restartRetryConnectionTimer();
        }
      }

      void SpotSession::callbackToRetryConnection(Poco::Timer&)
      {
        if( getStatus() == SpotSession::SpotSessionStatus::CStatusSessionStarted )
        {
          m_pendingLoginRetries = CDefaultRetries;

          pauseTimer(m_timerToRetryConnect);

          restartSendLoginTimer();
        }
        else
        {
          setStatus(SpotSession::SpotSessionStatus::CSessionOnError);

          AbsSessionType::stop();

          AbsSessionType::start();

          std::cout << "[[Spot Session - the connection was retried] - [" << m_pendingConnectionRetries << "]]" << std::endl;

          if( (m_pendingConnectionRetries--) > 0 )
          {
            resumeTimer(m_timerToRetryConnect, CDefaultTimeBetweenEachRetryConnection);
          }
          else
          {
            resumeTimer(m_timerToRetryConnect, CDefaultTimeToRetryConnection);
            m_pendingConnectionRetries = CDefaultConnectionRetries;
          }
        }
      }

      void SpotSession::callbackToSendPing(Poco::Timer&)
      {
        if( (getStatus() != SpotSession::SpotSessionStatus::CSessionLogged) || (!ping()) )
        {
          setStatus(SpotSession::SpotSessionStatus::CSessionOnError);

          m_pendingConnectionRetries = CDefaultConnectionRetries;

          pauseTimer(m_timerToSendPing);

          restartRetryConnectionTimer();
        }
        else
        {
          resumeTimer(m_timerToSendPing, CDefaultTimeToSendPing);

          std::cout << "[Spot Session Logged - Ping]" << std::endl;
        }
      }

      void SpotSession::restartSendLoginTimer( )
      {
        m_timerToSendLogin.stop();
        m_timerToSendLogin.start(Poco::TimerCallback<SpotSession>(*this, &SpotSession::callbackToSendLogin));
      }

      void SpotSession::restartRetryConnectionTimer( )
      {
        m_timerToRetryConnect.stop();
        m_timerToRetryConnect.start(Poco::TimerCallback<SpotSession>(*this, &SpotSession::callbackToRetryConnection));
      }

      void SpotSession::restartSendPingTimer( )
      {
        m_timerToSendPing.stop();
        m_timerToSendPing.start(Poco::TimerCallback<SpotSession>(*this, &SpotSession::callbackToSendPing));
      }

      bool SpotSession::ping()
      {
        SpotMessagePing spotPingMessageRq;
        SpotMessagePingResponse spotPingMessageRs;

        sendSync(spotPingMessageRq, spotPingMessageRs, 3000);
        unsigned char ack = spotPingMessageRs.getAckCode();

        ErrorType result = getError();

        if( (result != SpotSession::SpotSessionError::CNoError) || (ack != ISpotMessage::CErrorCodeNoError) )
        {
          std::cout << "login result - " << std::hex << result << "  ping ackcode - " << std::hex << ack << " - ping fail" << std::endl;
          std::cout << "Error - [" << getErrorDescription() << "]" << std::endl;
          setError(SpotSession::SpotSessionError::CErrorSendingPing);

          return false;
        }

        return true;
      }

      bool SpotSession::login()
      {
        SpotMessageLogin spotLoginMessageRq;

        spotLoginMessageRq.setClientId(getClientId());
        spotLoginMessageRq.setModuloLen(getModuloLen());
        spotLoginMessageRq.setPingTimeout(getPingTimeout());

        SpotMessageLoginResponse spotLoginMessageRs;

        // This is the only message that could be sent at started session state because this will generate
        // the availability of the communication with SPOT.
        AbsSessionType::sendSync(spotLoginMessageRq, spotLoginMessageRs, 10000);

        unsigned char ack = spotLoginMessageRs.getAckCode();

        ErrorType result = getError();

        if( (result != SpotSession::SpotSessionError::CNoError) || (ack != ISpotMessage::CErrorCodeNoError) )
        {
          if( ack == ISpotMessage::CErrorCodeUserAlreadyLogged )
          {
            setError(SpotSession::SpotSessionError::CErrorThereIsAlreadyLoggedSessionWithThatAppId);
          }

          std::cout << "login result [" << std::dec << static_cast<int>(result) << "]  login ackcode [" << std::hex << static_cast<int>(ack) << "] - login fail." << std::endl;
          std::cout << "Error - [" << getErrorDescription() << "]" << std::endl;

          setError(SpotSession::SpotSessionError::CErrorSendingLogging);

          return false;
        }

        setStatus(SpotSession::SpotSessionStatus::CSessionLogged);

        return true;
      }

      std::string SpotSession::getCurrentTime() const
      {
        return Poco::DateTimeFormatter::format(Poco::LocalDateTime(), "%Y/%m/%d %H:%M:%S");
      }

      bool SpotSession::classifyMessage(BufferType& buff, TypeId& type, KeyType& key)
      {
        bool found = AbsSessionType::classifyMessage(buff, type, key);
        if( !found && m_buffer.size() > 2 )
        {
          int position = 0;
          int messageFoundSize = (((std::size_t)m_buffer[(position+0)])<<8) | m_buffer[(position+1)];
          SizeType remainingSize = m_buffer.size();
          if( messageFoundSize <= remainingSize )
          {
            std::cout << "parsing unknown spot message..." << std::endl;
            std::cout << "[messageFoundSize=" << messageFoundSize << "]" << std::endl;
            std::cout << "[remainingSize=" << remainingSize << "]" << std::endl;

            remainingSize = remainingSize - messageFoundSize;
            buff.reserve(messageFoundSize);
            buff.insert(buff.begin(), m_buffer.begin(), (m_buffer.begin()+messageFoundSize));
            if( remainingSize > 0 )
            {
              BufferType remainingBuffer;
              remainingBuffer.reserve(remainingSize);
              remainingBuffer.assign( (m_buffer.begin()+messageFoundSize),m_buffer.end() );
              m_buffer.swap(remainingBuffer);
            }
            else
            {
              std::cout << "Discarding spot message because there is not a classifier for that." << std::endl;
              std::cout << "[AppId="     << std::hex << static_cast<int>(m_buffer[(position+2)]) << "]." << std::endl;
              std::cout << "[Ssk="       << std::hex << static_cast<int>(m_buffer[(position+3)]) << "]." << std::endl;
              std::cout << "[Command="   << std::hex << static_cast<int>(m_buffer[(position+4)]) << "]." << std::endl;

              {
                char pcBuffer[(m_buffer.size()*3)+2];
                std::cout << "Message Data: " << Utils::dumpBuffer(pcBuffer, &m_buffer.front(), m_buffer.size()) << std::endl;
              }

              m_buffer.clear();
            }
          }
        }
        return found;
      }
    }
  }
}
