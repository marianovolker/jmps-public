/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlSessionClient.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLSESSIONCLIENT_H_
# define _CRLSESSIONCLIENT_H_

# include <gvr/communication/crl/session/CrlSession.h>
# include <gvr/communication/crl/classifier/CrlClassifyMessageClient.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Crl Domain
      namespace crl
      {
        /**
         * @brief This class is the crl session client.
         * @see gvr.communication.Session
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class CrlSessionClient : public CrlSession
        {
        public:
          /**
           * @brief Default Constructor.
           * @since 0.0.0
           */
          CrlSessionClient (void);

          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          virtual ~CrlSessionClient (void);

          /**
           * @brief start session.
           * To avoid c++ name hidding we bring the declaration of all start methods from base.
           * @since 0.0.0
           */
          using CrlSession::start;

          /**
           * @brief redefine the inherited type. Stop the session worker.
           * @see Session
           */
          virtual ErrorType stop (void);

          /**
           * @brief Start the session for the given channel and pingTimeout parameters.
           * @param channel
           * @param pingTimeot ping timeout parameter to crl login
           * @param retries retries cycle to perform for crl login
           * @param retryTimeout retry timeout within retry cycle for crl login
           * @param retryCycleTimeout retry wait timeout between retry cycles for crl login
           * @return returns the session status resulting.
           * */
          virtual ErrorType start (CrlChannel & channel, TimeMillisecondsType pingTimeout, RetryType retries, TimeMillisecondsType retryTimeout, TimeMillisecondsType retryCycleTimeout);

        protected:
          /**
          * @brief Starts the connection cycle after starting a connection to the CRL.
          **/
          virtual void private_safe_startConnectionCycle (void);

          /**
          * @brief Stops the connection cycle before stopping a connection to the CRL.
          **/
          virtual void private_safe_stopConnectionCycle (void);

          /**
          * @brief Sends a ping to the connected crl session.
          * @return false if the ping failed, true in other case.
          **/
          virtual bool private_unsafe_doPing (void);

          /**
          * @brief Sends a login to the connected crl session.
          * @return false if the login failed, true in other case.
          **/
          virtual bool private_unsafe_doLogin (void);

          /**
          * @brief Performs the ping actions to the connected crl session.
          **/
          virtual void private_unsafe_tryPing (void);

          /**
          * @brief Performs the login actions to the connected crl session.
          **/
          virtual void private_unsafe_tryLogin (void);

          /**
          * @brief Perform a pause to the timer. You could not stop a timer inside the callback timer,
          *        so you need to call this method from the timer callback and when you need to
          *        restart the timer you must to restart that calling the restartTimer.
          **/
          virtual void private_unsafe_pauseTimer (void);

          /**
          * @brief Perform a resume to the timer for a timeout received as parameter.
          * @param The timeout for the resume timer.
          **/
          virtual void private_unsafe_resumeTimer (TimeMillisecondsType milliseconds);

          /**
          * @brief Perform a timer reset for a retry.
          **/
          virtual void private_unsafe_resetTimerForRetry (void);

          /**
          * @brief Perform a timer reset for a retry.
          **/
          virtual void private_unsafe_resetTimerForRetryCycle (void);

          /**
          * @brief Perform a timer reset for a retry.
          **/
          virtual void private_unsafe_resetTimerForPing (void);

          /**
          * @brief The main timer callback to send a logging message to CRL.
          * @param The timer that calls the callback.
          **/
          virtual void private_safe_callbackForTimer (Poco::Timer &);

          /**@brief the default value in milliseconds to retry a connection within a cycle.*/
          static TimeMillisecondsType const CDefaultRetryTimeout = 1000;

          /**@brief the default value in milliseconds to restart the retry connection cycle.*/
          static TimeMillisecondsType const CDefaultRetryCycleTimeout = 1000;

          /** @brief connection retries pending.*/
          RetryType m_pendingRetries;

          /** @brief ping timeout login request value. 0=infinite seconds i.e.: no timeout.*/
          TimeMillisecondsType m_retryTimeout;

          /** @brief ping timeout login request value. 0=infinite seconds i.e.: no timeout.*/
          TimeMillisecondsType m_retryCycleTimeout;

          /** @brief the session timer for connection retries.*/
          Poco::Timer m_timer;

          CrlClassifyMessageClient m_clientClassifier;
        };
      }
    }
  }

#endif
