/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccSession.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/session/UccSession.h>
#include <gvr/communication/ucc/classifier/base/IUccClassifyMessage.h>
#include <gvr/communication/ucc/message/UccMessageAny.h>
#include <gvr/utils/Json.h>

namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      StatusType const UccSession::UccSessionStatus::CSessionLogged    = UccSession::AbsSessionType::SessionStatus::CLastStatus + 1;
      StatusType const UccSession::UccSessionStatus::CSessionLoggedOut = UccSession::AbsSessionType::SessionStatus::CLastStatus + 2;
      StatusType const UccSession::UccSessionStatus::CLastStatus       = UccSession::AbsSessionType::SessionStatus::CLastStatus + 10;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      ErrorType const UccSession::UccSessionError::CErrorOnPing           = UccSession::AbsSessionType::SessionError::CLastError + 1;
      ErrorType const UccSession::UccSessionError::CErrorOnLoging        = UccSession::AbsSessionType::SessionError::CLastError + 2;
      ErrorType const UccSession::UccSessionError::CErrorNoUccSessionAvailable = UccSession::AbsSessionType::SessionError::CLastError + 3;

      ErrorType const UccSession::UccSessionError::CLastError                  = UccSession::AbsSessionType::SessionError::CLastError + 10;
      /////////////////////////////////////////////////////////////////////////////////////

      UccSession::UccSession (void)
       : AbsSessionType(CDefaultTimeOut,CDefaultRetries),
         m_pingTimeout(CDefaultPingTimeout),
         m_mtxUcc()
      {
        m_descriptionStatusList [ UccSessionStatus::CSessionLogged              ] = "UccSession is at logged state";
        m_descriptionStatusList [ UccSessionStatus::CSessionLoggedOut           ] = "UccSession is at logged out state";
        m_descriptionStatusList [ UccSessionStatus::CLastStatus                 ] = "UccSession is at unknown state";
        //--
        m_descriptionErrorList  [ UccSessionError::CErrorOnPing                 ] = "UccSession had an error while doing ping.";
        m_descriptionErrorList  [ UccSessionError::CErrorOnLoging               ] = "UccSession had an error while doing login.";
        m_descriptionErrorList  [ UccSessionError::CErrorNoUccSessionAvailable  ] = "No available UccSession to send a message.";
        m_descriptionErrorList  [ UccSessionError::CLastError                   ] = "UccSession has an unknown error.";
        //--
      }

      UccSession::~UccSession (void)
      {
        stop();
      }

      ErrorType UccSession::start (void)
      {
        AbsSessionType::start();
        private_safe_startConnectionCycle();
        return getError();
      }

      ErrorType UccSession::start (UccChannel & channel)
      {
        AbsSessionType::start(channel);
        private_safe_startConnectionCycle();
        return getError();
      }

      ErrorType UccSession::start (UccChannel & channel, TimeMillisecondsType pingTimeout)
      {
        {
          ScopedLock guard(m_mtxUcc);
          m_pingTimeout = pingTimeout;
        }
        AbsSessionType::start(channel);
        private_safe_startConnectionCycle();
        return getError();
      }

      ErrorType UccSession::stop (void)
      {
        private_safe_stopConnectionCycle();

        ErrorType error( setError(AbsSessionType::stop()) );

        unSetChannel();

        return error;
      }

      ErrorType UccSession::sendSync (IMessageType const & messageRq, IMessageType & messageRs, TimeMillisecondsType timeout)
      {
        if (isLogged())
        {
          ScopedLock guard(m_mtxUcc);
          return setError(AbsSessionType::sendSync(messageRq, messageRs, timeout));
        }
        else
        {
          return setError(UccSession::UccSessionError::CErrorNoUccSessionAvailable);
        }
      }

      ErrorType UccSession::sendSync (IMessageType const & messageRq, IMessageType & messageRs)
      {
        return setError(sendSync(messageRq, messageRs,getTimeoutDefault()));
      }

      ErrorType UccSession::send (IMessageType const & messageRq)
      {
        if (isLogged())
        {
          ScopedLock guard(m_mtxUcc);
          return setError(AbsSessionType::send(messageRq));
        }
        else
        {
          return setError(UccSession::UccSessionError::CErrorNoUccSessionAvailable);
        }
      }

     bool UccSession::hasError (void)
     {
       return (getStatus() == UccSession::UccSessionStatus::CSessionOnError);
     }

     bool UccSession::isConnected (void)
     {
       return (getStatus() == UccSession::UccSessionStatus::CStatusSessionStarted);
     }

     bool UccSession::isLogged (void)
     {
       return (getStatus() == UccSession::UccSessionStatus::CSessionLogged);
     }

      UccSession::TimeMillisecondsType UccSession::getPingTimeout (void)
      {
        ScopedLock guard(m_mtxUcc);
        return m_pingTimeout;
      }

      void UccSession::setPingTimeout (TimeMillisecondsType timeout)
      {
        ScopedLock guard(m_mtxUcc);
        m_pingTimeout = timeout;
      }

      bool UccSession::classifyMessage (BufferType& buff, TypeId& type, KeyType& key)
      {
        bool found = AbsSessionType::classifyMessage(buff, type, key);
        if ((!found) && (!m_buffer.empty()))
        {
          SizeType parsedSize = 0;
          SizeType structureSize = 0;
          gvr::utils::Json json;
          unsigned char flag = 0;
          switch (IUccClassifyMessage::parseToStructure(m_buffer,parsedSize,structureSize,flag,json))
          {
          case IClassifyMessageType::Integrity::CComplete:
          case IClassifyMessageType::Integrity::CUnknown:
            {
              if (parsedSize)
              {
                if (m_buffer.size() > parsedSize)
                {
                  buff.reserve(parsedSize);
                  buff.assign(m_buffer.begin(),m_buffer.begin()+(parsedSize));

                  SizeType remainingSize = (m_buffer.size()-parsedSize);
                  BufferType remainingBuffer;
                  remainingBuffer.reserve(remainingSize);
                  remainingBuffer.assign(m_buffer.begin()+(parsedSize),m_buffer.end());
                  m_buffer.swap(remainingBuffer);
                }
                else
                {
                  m_buffer.swap(buff);
                  m_buffer.clear();
                }
                type = UccMessageAny::GetTypeId();
                key = UccMessageAny::CDefaultKey;
                found = true;
              }
              else
              {
                m_buffer.clear();
              }
            }
            break;
          case IClassifyMessageType::Integrity::CIncomplete:
            break;
          default:
            {
              m_buffer.clear();
            }
            break;
          }
        }
        return found;
      }

      void UccSession::private_safe_startConnectionCycle (void)
      {
        ScopedLock guard(m_mtxUcc);
        setStatus(UccSession::UccSessionStatus::CSessionLogged);
      }

      void UccSession::private_safe_stopConnectionCycle (void)
      {
        ScopedLock guard(m_mtxUcc);
        setStatus(UccSession::UccSessionStatus::CSessionLoggedOut);
      }
    }
  }
}
