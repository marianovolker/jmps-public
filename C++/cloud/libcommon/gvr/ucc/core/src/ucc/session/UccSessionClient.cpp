/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccSessionClient.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/session/UccSessionClient.h>

namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      UccSessionClient::UccSessionClient (void)
       : UccSession(),
         m_pendingRetries(CDefaultRetries),
         m_retryTimeout(CDefaultRetryTimeout),
         m_retryCycleTimeout(CDefaultRetryCycleTimeout),
         m_timer(0,0)
      {
        addClassifier(m_registerClassifier);
        addClassifier(m_cloudClientClassifier);
        addClassifier(m_internalClientClassifier);
      }

      UccSessionClient::~UccSessionClient (void)
      {
        stop();
      }

      ErrorType UccSessionClient::start (UccChannel & channel, TimeMillisecondsType pingTimeout, RetryType retries, TimeMillisecondsType retryTimeout, TimeMillisecondsType retryCycleTimeout)
      {
        {
          ScopedLock guard(m_mtxUcc);
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

      ErrorType UccSessionClient::stop (void)
      {
        private_safe_stopConnectionCycle();

        ErrorType error( setError(AbsSessionType::stop()) );

        unSetChannel();

        return error;
      }

      void UccSessionClient::private_safe_startConnectionCycle (void)
      {
        ScopedLock guard(m_mtxUcc);
        m_pendingRetries = 0;
        m_timer.start(Poco::TimerCallback<UccSessionClient>(*this,&UccSessionClient::private_safe_callbackForTimer));
      }

      void UccSessionClient::private_safe_stopConnectionCycle (void)
      {
        ScopedLock guard(m_mtxUcc);
        m_timer.restart(0);
        m_pendingRetries = 0;
      }

      bool UccSessionClient::private_unsafe_doPing (void)
      {
        return true;
      }

      bool UccSessionClient::private_unsafe_doLogin (void)
      {
        return true;
      }

      void UccSessionClient::private_unsafe_tryPing (void)
      {
        if (private_unsafe_doPing())
        {
          std::cout << "[Ucc Session Logged - Ping]" << std::endl;
          private_unsafe_resetTimerForPing();
        }
        else
        {
          setStatus(UccSession::UccSessionStatus::CSessionOnError);
          std::cout << "[Ucc Session Lost]" << std::endl;
          private_unsafe_tryLogin();
        }
      }

      void UccSessionClient::private_unsafe_tryLogin (void)
      {
        if (m_pendingRetries < 1)
        {
          m_pendingRetries = getRetriesDefault();
          std::cout << "[Ucc Resetting Pending Retries: "<< m_pendingRetries << " - Waiting for new retry cycle]" << std::endl;
        }

        if (getStatus() != UccSession::UccSessionStatus::CStatusSessionStarted)
        {
          AbsSessionType::stop();
          AbsSessionType::start();
        }

        if ((getStatus() == UccSession::UccSessionStatus::CStatusSessionStarted) && (private_unsafe_doLogin()))
        {
          setStatus(UccSession::UccSessionStatus::CSessionLogged);
          std::cout << "[Ucc Session Logged]" << std::endl;
          m_pendingRetries = 0;
          private_unsafe_resetTimerForPing();
        }
        else
        {
          AbsSessionType::stop();
          if (m_pendingRetries > 1)
          {
            --m_pendingRetries;
            std::cout << "[Ucc Pending Retries: "<< m_pendingRetries << "]" << std::endl;
            private_unsafe_resetTimerForRetry();
          }
          else
          {
            m_pendingRetries = getRetriesDefault();
            std::cout << "[Ucc Resetting Pending Retries: "<< m_pendingRetries << " - Waiting for new retry cycle]" << std::endl;
            private_unsafe_resetTimerForRetryCycle();
          }
        }
      }

      void UccSessionClient::private_unsafe_pauseTimer (void)
      {
        m_timer.restart(0);
      }

      void UccSessionClient::private_unsafe_resumeTimer (TimeMillisecondsType milliseconds)
      {
        m_timer.restart(milliseconds);
      }

      void UccSessionClient::private_unsafe_resetTimerForRetry (void)
      {
        private_unsafe_resumeTimer(m_retryTimeout);
      }

      void UccSessionClient::private_unsafe_resetTimerForRetryCycle (void)
      {
        private_unsafe_resumeTimer(m_retryCycleTimeout);
      }

      void UccSessionClient::private_unsafe_resetTimerForPing (void)
      {
        private_unsafe_resumeTimer(m_pingTimeout);
      }

      void UccSessionClient::private_safe_callbackForTimer (Poco::Timer & timer)
      {
        ScopedLock guard(m_mtxUcc);
        //  If an error occurs elsewhere or if channelEvent disconnected is received,
        // and session was already connected, this check will fail and session will try to reconnect.
        if (getStatus() == UccSession::UccSessionStatus::CSessionLogged)
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
