/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlSessionClient.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/session/CrlSessionClient.h>

namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      CrlSessionClient::CrlSessionClient (void)
       : CrlSession(),
         m_pendingRetries(CDefaultRetries),
         m_retryTimeout(CDefaultRetryTimeout),
         m_retryCycleTimeout(CDefaultRetryCycleTimeout),
         m_timer(0,0)
      {
        addClassifier(m_clientClassifier);
      }

      CrlSessionClient::~CrlSessionClient (void)
      {
        stop();
      }

      ErrorType CrlSessionClient::start (CrlChannel & channel, TimeMillisecondsType pingTimeout, RetryType retries, TimeMillisecondsType retryTimeout, TimeMillisecondsType retryCycleTimeout)
      {
        {
          ScopedLock guard(m_mtxCrl);
          m_pingTimeout = pingTimeout;
          m_pendingRetries = 0;
          m_retryTimeout = retryTimeout;
          m_retryCycleTimeout = retryCycleTimeout;
        }
        setRetriesDefault(retries);
        AbsSessionType::start(channel);
        private_safe_startConnectionCycle();
        return getError();
      }

      ErrorType CrlSessionClient::stop (void)
      {
        private_safe_stopConnectionCycle();

        ErrorType error( setError(AbsSessionType::stop()) );

        unSetChannel();

        return error;
      }

      void CrlSessionClient::private_safe_startConnectionCycle (void)
      {
        ScopedLock guard(m_mtxCrl);
        m_pendingRetries = 0;
        m_timer.start(Poco::TimerCallback<CrlSessionClient>(*this,&CrlSessionClient::private_safe_callbackForTimer));
      }

      void CrlSessionClient::private_safe_stopConnectionCycle (void)
      {
        ScopedLock guard(m_mtxCrl);
        m_timer.restart(0);
        m_pendingRetries = 0;
      }

      bool CrlSessionClient::private_unsafe_doPing (void)
      {
        return true;
      }

      bool CrlSessionClient::private_unsafe_doLogin (void)
      {
        return true;
      }

      void CrlSessionClient::private_unsafe_tryPing (void)
      {
        if (private_unsafe_doPing())
        {
          std::cout << "[Crl Session Logged - Ping]" << std::endl;
          private_unsafe_resetTimerForPing();
        }
        else
        {
          setStatus(CrlSession::CrlSessionStatus::CSessionOnError);
          std::cout << "[Crl Session Lost]" << std::endl;
          private_unsafe_tryLogin();
        }
      }

      void CrlSessionClient::private_unsafe_tryLogin (void)
      {
        if (m_pendingRetries < 1)
        {
          m_pendingRetries = getRetriesDefault();
          std::cout << "[Crl Resetting Pending Retries: "<< m_pendingRetries << " - Waiting for new retry cycle]" << std::endl;
        }

        if (getStatus() != CrlSession::CrlSessionStatus::CStatusSessionStarted)
        {
          AbsSessionType::stop();
          AbsSessionType::start();
        }

        if ((getStatus() == CrlSession::CrlSessionStatus::CStatusSessionStarted) && (private_unsafe_doLogin()))
        {
          setStatus(CrlSession::CrlSessionStatus::CSessionLogged);
          std::cout << "[Crl Session Logged]" << std::endl;
          m_pendingRetries = 0;
          private_unsafe_resetTimerForPing();
        }
        else
        {
          AbsSessionType::stop();
          if (m_pendingRetries > 1)
          {
            --m_pendingRetries;
            std::cout << "[Crl Pending Retries: "<< m_pendingRetries << "]" << std::endl;
            private_unsafe_resetTimerForRetry();
          }
          else
          {
            m_pendingRetries = getRetriesDefault();
            std::cout << "[Crl Resetting Pending Retries: "<< m_pendingRetries << " - Waiting for new retry cycle]" << std::endl;
            private_unsafe_resetTimerForRetryCycle();
          }
        }
      }

      void CrlSessionClient::private_unsafe_pauseTimer (void)
      {
        m_timer.restart(0);
      }

      void CrlSessionClient::private_unsafe_resumeTimer (TimeMillisecondsType milliseconds)
      {
        m_timer.restart(milliseconds);
      }

      void CrlSessionClient::private_unsafe_resetTimerForRetry (void)
      {
        private_unsafe_resumeTimer(m_retryTimeout);
      }

      void CrlSessionClient::private_unsafe_resetTimerForRetryCycle (void)
      {
        private_unsafe_resumeTimer(m_retryCycleTimeout);
      }

      void CrlSessionClient::private_unsafe_resetTimerForPing (void)
      {
        private_unsafe_resumeTimer(m_pingTimeout);
      }

      void CrlSessionClient::private_safe_callbackForTimer (Poco::Timer & timer)
      {
        ScopedLock guard(m_mtxCrl);
        //  If an error occurs elsewhere or if channelEvent disconnected is received,
        // and session was already connected, this check will fail and session will try to reconnect.
        if (getStatus() == CrlSession::CrlSessionStatus::CSessionLogged)
        {
          private_unsafe_tryPing();
        }
        else
        {
          private_unsafe_tryLogin();
        }
        std::cout << std::endl;
      }
    }
  }
}
