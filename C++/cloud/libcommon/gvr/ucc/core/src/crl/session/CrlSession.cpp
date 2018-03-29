/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlSession.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/session/CrlSession.h>
#include <gvr/communication/crl/classifier/base/ICrlClassifyMessage.h>
#include <gvr/communication/crl/message/CrlMessageAny.h>
#include <gvr/utils/Json.h>

namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      StatusType const CrlSession::CrlSessionStatus::CSessionLogged    = CrlSession::AbsSessionType::SessionStatus::CLastStatus + 1;
      StatusType const CrlSession::CrlSessionStatus::CSessionLoggedOut = CrlSession::AbsSessionType::SessionStatus::CLastStatus + 2;
      StatusType const CrlSession::CrlSessionStatus::CLastStatus       = CrlSession::AbsSessionType::SessionStatus::CLastStatus + 10;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      ErrorType const CrlSession::CrlSessionError::CErrorOnPing           = CrlSession::AbsSessionType::SessionError::CLastError + 1;
      ErrorType const CrlSession::CrlSessionError::CErrorOnLoging        = CrlSession::AbsSessionType::SessionError::CLastError + 2;
      ErrorType const CrlSession::CrlSessionError::CErrorNoCrlSessionAvailable = CrlSession::AbsSessionType::SessionError::CLastError + 3;

      ErrorType const CrlSession::CrlSessionError::CLastError                  = CrlSession::AbsSessionType::SessionError::CLastError + 10;
      /////////////////////////////////////////////////////////////////////////////////////

      CrlSession::CrlSession (void)
       : AbsSessionType(CDefaultTimeOut,CDefaultRetries),
         m_pingTimeout(CDefaultPingTimeout),
         m_mtxCrl()
      {
        m_descriptionStatusList [ CrlSessionStatus::CSessionLogged              ] = "CrlSession is at logged state";
        m_descriptionStatusList [ CrlSessionStatus::CSessionLoggedOut           ] = "CrlSession is at logged out state";
        m_descriptionStatusList [ CrlSessionStatus::CLastStatus                 ] = "CrlSession is at unknown state";
        //--
        m_descriptionErrorList  [ CrlSessionError::CErrorOnPing                 ] = "CrlSession had an error while doing ping.";
        m_descriptionErrorList  [ CrlSessionError::CErrorOnLoging               ] = "CrlSession had an error while doing login.";
        m_descriptionErrorList  [ CrlSessionError::CErrorNoCrlSessionAvailable  ] = "No available CrlSession to send a message.";
        m_descriptionErrorList  [ CrlSessionError::CLastError                   ] = "CrlSession has an unknown error.";
        //--
      }

      CrlSession::~CrlSession (void)
      {
        stop();
      }

      ErrorType CrlSession::start (void)
      {
        AbsSessionType::start();
        private_safe_startConnectionCycle();
        return getError();
      }

      ErrorType CrlSession::start (CrlChannel & channel)
      {
        AbsSessionType::start(channel);
        private_safe_startConnectionCycle();
        return getError();
      }

      ErrorType CrlSession::start (CrlChannel & channel, TimeMillisecondsType pingTimeout)
      {
        {
          ScopedLock guard(m_mtxCrl);
          m_pingTimeout = pingTimeout;
        }
        AbsSessionType::start(channel);
        private_safe_startConnectionCycle();
        return getError();
      }

      ErrorType CrlSession::stop (void)
      {
        private_safe_stopConnectionCycle();

        ErrorType error( setError(AbsSessionType::stop()) );

        unSetChannel();

        return error;
      }

      ErrorType CrlSession::sendSync (IMessageType const & messageRq, IMessageType & messageRs, TimeMillisecondsType timeout)
      {
        if (isLogged())
        {
          ScopedLock guard(m_mtxCrl);
          return setError(AbsSessionType::sendSync(messageRq, messageRs, timeout));
        }
        else
        {
          return setError(CrlSession::CrlSessionError::CErrorNoCrlSessionAvailable);
        }
      }

      ErrorType CrlSession::sendSync (IMessageType const & messageRq, IMessageType & messageRs)
      {
        return setError(sendSync(messageRq, messageRs,getTimeoutDefault()));
      }

      ErrorType CrlSession::send (IMessageType const & messageRq)
      {
        if (isLogged())
        {
          ScopedLock guard(m_mtxCrl);
          return setError(AbsSessionType::send(messageRq));
        }
        else
        {
          return setError(CrlSession::CrlSessionError::CErrorNoCrlSessionAvailable);
        }
      }

     bool CrlSession::hasError (void)
     {
       return (getStatus() == CrlSession::CrlSessionStatus::CSessionOnError);
     }

     bool CrlSession::isConnected (void)
     {
       return (getStatus() == CrlSession::CrlSessionStatus::CStatusSessionStarted);
     }

     bool CrlSession::isLogged (void)
     {
       return (getStatus() == CrlSession::CrlSessionStatus::CSessionLogged);
     }

      CrlSession::TimeMillisecondsType CrlSession::getPingTimeout (void)
      {
        ScopedLock guard(m_mtxCrl);
        return m_pingTimeout;
      }

      void CrlSession::setPingTimeout (TimeMillisecondsType timeout)
      {
        ScopedLock guard(m_mtxCrl);
        m_pingTimeout = timeout;
      }

      bool CrlSession::classifyMessage (BufferType& buff, TypeId& type, KeyType& key)
      {
        bool found = AbsSessionType::classifyMessage(buff, type, key);
        if ((!found) && (!m_buffer.empty()))
        {
          SizeType parsedSize = 0;
          SizeType structureSize = 0;
          gvr::utils::Json json;
          unsigned char flag = 0;
          switch (ICrlClassifyMessage::parseToStructure(m_buffer,parsedSize,structureSize,flag,json))
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
                type = CrlMessageAny::GetTypeId();
                key = CrlMessageAny::CDefaultKey;
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

      void CrlSession::private_safe_startConnectionCycle (void)
      {
        ScopedLock guard(m_mtxCrl);
        setStatus(CrlSession::CrlSessionStatus::CSessionLogged);
      }

      void CrlSession::private_safe_stopConnectionCycle (void)
      {
        ScopedLock guard(m_mtxCrl);
        setStatus(CrlSession::CrlSessionStatus::CSessionLoggedOut);
      }
    }
  }
}
