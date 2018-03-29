/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudConfigDeviceChecker.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CLOUDCONFIGDEVICECHECKER_H_
# define _CLOUDCONFIGDEVICECHECKER_H_

# include <Poco/Timer.h>

# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>

# include <CloudStatus.h>
# include <gvr/communication/ucc/session/UccSession.h>
# include <gvr/communication/spot/SpotSession.h>

using namespace gvr::communication::ucc;
using namespace gvr::communication::spot;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      /**
       * @brief This class is the cloud config device checker.
       * @since 0.0.0, 2015-03-02
       * @version 0.0.0, 2015-03-02
       * @author pcantarini
       */
      class CloudConfigDeviceChecker
      {
      public:
        typedef std::size_t TimeMillisecondsType;

        /**@brief the default value in milliseconds to restart the cycle.*/
        static TimeMillisecondsType const CDefaultCycleTimeout = 10000;

        /**
         * @brief default destructor.
         */
        virtual ~CloudConfigDeviceChecker (void);

        /**
         * @brief Constructor.
         */
        CloudConfigDeviceChecker (TimeMillisecondsType, UccSession &, SpotSession &, CloudStatus &);

        /**
         * @brief Check if check cycle is active.
         *
         * @return true if check cycle is active, false otherwise.
         */
        bool isChecking  (void) const;

        /**
         * @brief Starts the check cycle.
         */
        void start (void);

        /**
         * @brief Stops the check cycle.
         */
        void stop (void);

        /**
         * @brief Restarts the check cycle.
         */
        void restart (void);

      protected:
        /**
        * @brief Starts the connection cycle after starting a connection to the CRL.
        **/
        virtual void private_safe_startCheckCycle (void);

        /**
        * @brief Stops the connection cycle before stopping a connection to the CRL.
        **/
        virtual void private_safe_stopCheckCycle (void);

        /**
        * @brief Performs the cycle check.
        * @return true if check was successfully performed. False otherwise.
        **/
        virtual bool private_unsafe_doCheck (void);

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
        virtual void private_unsafe_resumeTimer (TimeMillisecondsType);

        /**
        * @brief Perform a timer reset for a cycle.
        **/
        virtual void private_unsafe_resetTimerForCycle (void);

        /**
        * @brief The main timer callback.
        * @param The timer that calls the callback.
        **/
        virtual void private_safe_callbackForTimer (Poco::Timer &);

        /**
         * @brief Checks if this starting up of this application has been a consequence of an immediately
         *        recent package activation operation triggered by the cloud.
         */
        bool private_hasBeenPackageActivationPerformed (void) const;

        /**
         * @brief Notifies the cloud with an event that the last installation was finished OK.
         * @return true if notification was successful. False otherwise.
         */
        bool private_notifyCloudInstallationFinished (void) const;

        /** @brief Current instance synchronization mutex. */
        typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;
        Poco::FastMutex m_mtx;

        /** @brief Keeps track of the checking activity.*/
        bool m_checking;

        /** @brief Cycle timeout for checking. 0=infinite seconds i.e.: no timeout.*/
        TimeMillisecondsType m_cycleTimeout;

        /** @brief The cycle checker timer.*/
        Poco::Timer m_timer;

        /** @brief The ucc session.*/
        UccSession & m_uccSession;

        /** @brief The spot session.*/
        SpotSession & m_spotSession;

        /** @brief The cloud status.*/
        CloudStatus & m_cloudStatus;
      };
    }
  }

#endif
