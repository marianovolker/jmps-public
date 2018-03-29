
/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotSession.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTSESSION_H_
# define _SPOTSESSION_H_

#include <gvr/communication/Session.h>
#include <gvr/communication/spot/SpotChannel.h>
#include <gvr/communication/spot/SpotMessageSpec.h>
#include <gvr/communication/spot/SpotMessageReset.h>
#include <gvr/communication/spot/SpotClassifyMessagePing.h>
#include <gvr/communication/spot/SpotClassifyMessageLogin.h>
#include <gvr/communication/spot/SpotClassifyMessageHardwareInfo.h>
#include <gvr/communication/spot/SpotClassifyMessageSoftwareInfo.h>
#include <gvr/communication/spot/SpotClassifyMessagePackageInfo.h>
#include <gvr/communication/spot/SpotClassifyMessagePingResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageLoginResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageHardwareInfoResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageSoftwareInfoResponse.h>

#include <gvr/communication/spot/SpotClassifyMessageOPTHwrInfoResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageOptSwInfoResponse.h>

#include <gvr/communication/spot/SpotClassifyMessagePackageInfoResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageSystemStatusResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageDebianInfo.h>
#include <gvr/communication/spot/SpotClassifyMessageDebianInfoResponse.h>
#include <gvr/communication/spot/SpotClassifyMessagePackageDownloadResponse.h>
#include <gvr/communication/spot/SpotClassifyMessagePackageBlockDownloadResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageVGDFileTransferResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageVgdProcessPackageResponse.h>
#include <gvr/communication/spot/SpotClassifyMessagePackageActivation.h>
#include <gvr/communication/spot/SpotClassifyMessagePackageActivationResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageVgdPackageActivationResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageFileDownloadResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageFileDeleteResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageFileUploadResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageFileBlockUploadResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageFileBlockDownloadResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageEphemeralEncryptResponse.h>
#include <gvr/communication/spot/SpotClassifyMessageNewEphemeralKeyResponse.h>

#include <Poco/Timer.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        class SpotSession : public Session<SpotMessageSpec>
        {
          public:

            typedef Session<SpotMessageSpec> AbsSessionType;

            enum  EDevicePlatform
            {
              eUnknown, eM3, eM5, eM7, eTotalPlatforms
            };

            /**
             * @brief This is the status enumeration representation for the session possible states.
             *        If you need to extend these status you could inherit from this SpotSessionStatus.
             *
             */
            struct SpotSessionStatus: public AbsSessionType::SessionStatus
            {
              static StatusType const CSessionLogged;
              static StatusType const CSessionLoggedOut;

              static StatusType const CLastStatus;
            };

            /**
             * @brief This is the error enumeration representation for the session possible errors.
             *        If you need to extend these errors you could inherit from this SpotSessionError.
             *
             */
            struct SpotSessionError: public AbsSessionType::SessionError
            {
              static ErrorType const CErrorSendingPing;
              static ErrorType const CErrorSendingLogging;
              static ErrorType const CErrorNoSpotSessionAvailable;
              static ErrorType const CErrorThereIsAlreadyLoggedSessionWithThatAppId;

              static ErrorType const CLastError;
            };

            /**
            * @brief Default Constructor.
            * @param The initial default device platform to be overridden later by scanning
            *        the hardware info response.
            * @since 0.0.0
            */
            SpotSession (EDevicePlatform ePlatform = eM5);

            /**
             * @brief Destructor.
             * @since 0.0.0
             */
            virtual ~SpotSession (void);

            /**
             * @brief redefine the inherited type. Starts the session worker.
             * @see Session
             */
            virtual ErrorType start ();

            /**
             * @brief redefine the inherited type. Start the session worker.
             * @see Session
             */
            virtual ErrorType start (SpotChannel &);

            /**
             * @brief Start the session for the given channel, client id and login parameters.
             * @param channel
             * @param clientId client id application to spot login
             * @param moduloLen modulo len parameter to spot login
             * @param pingTimeot ping timeout parameter to spot login
             * @return returns the session status resulting.
             * */
            virtual ErrorType start (SpotChannel & channel, unsigned char const clientId, unsigned char const moduloLen, unsigned char const pingTimeout);

            /**
             * @brief redefine the inherited type. Stop the session worker.
             * @see Session
             */
            virtual ErrorType stop ();

            /**
             * @brief redefine the inherited type. Sends the given message through the underlying
             *        channel and waits until response or error occurs.
             * @see gvr.communication.Session.sendSync
             */
            virtual ErrorType sendSync (IMessageType const &, IMessageType &, TimeMillisecondsType);

            /**
             * @brief redefine the inherited type. Sends the given message through the underlying
             *        channel and waits until response or error occurs.
             * @see gvr.communication.Session.sendSync
             */
            virtual ErrorType sendSync (IMessageType const &, IMessageType &);

            /**
             * @brief redefine the inherited type. Sends the given message through the underlying
             *        channel.
             * @see gvr.communication.Session.send
             */
            virtual ErrorType send (IMessageType const &);

             /**
               * @brief Perform a verification if the SpotSession has errors.
               * @return true if the session has errors.
               */
            virtual bool hasError(){ ScopedLock l(m_mxSpotSession); return (getStatus()==SpotSession::SpotSessionStatus::CSessionOnError); };

            /**
              * @brief Perform a verification if the SpotSession is connected.
              * @return true if the session is connected.
              */
            virtual bool isConnected(){ ScopedLock l(m_mxSpotSession); return (getStatus() == SpotSession::SpotSessionStatus::CStatusSessionStarted); };

            /**
              * @brief Perform a verification if the SpotSession is ready to operate.
              * @return true if the session is ready.
              */
            virtual bool isLogged(){ ScopedLock l(m_mxSpotSession); return (getStatus() == SpotSession::SpotSessionStatus::CSessionLogged); };

            /**
             * @brief Gets the client id login value.
             * @return the current client id login value.
             */
            unsigned char getClientId();

            /**
             * @brief Gets the modulo len login value.
             * @return the current modulo len login value.
             */
            unsigned char getModuloLen();

            /**
             * @brief Gets the ping timeout login value.
             * @return the current ping timeout login value.
             */
            unsigned char getPingTimeout();

            /**
             * @brief Sets the new modulo len login field value
             * @param modulolen, login modulo length request value. Length of PMK RSA module (zero = no Key required).
             */
            void setClientId(unsigned char const);

            /**
             * @brief Sets the new modulo len login field value
             * @param modulolen, login modulo length request value. Length of PMK RSA module (zero = no Key required).
             */
            void setModuloLen(unsigned char const);

            /**
             * @brief Sets the new login ping timeout field value
             * @param pingTimeout, login ping timeout request value. 0=infinite seconds i.e.: no timeout.
             */
            void setPingTimeout(unsigned char const);

            /**
             * @brief Helper method to log current time for testing propose.
             * @return the current time string
             * */
            std::string getCurrentTime() const;

            /*
             * @brief Returns the platform of the device this session is connected to.
             * @note  The session should be able to properly identify the device platform.
             *
             * @return The identified device platform, or eUnknown if this couldn't be properly identified.
             */
            EDevicePlatform  getDevicePlatform() const;

            /*
             * @brief Returns the platform of the device this session is connected to.
             * @note  The session should be able to properly identify the device platform.
             *
             * @return The mnemonic C string for the currrent device platform, or "unknown" if this couldn't be properly identified.
             */
            const char*  getDevicePlatformAcronym() const;


          protected:

            /**
             * @brief redefine the inherited type. channelEvent. starts the session worker.
             * @see Session
             */
            virtual void channelEvent (StatusType status);

            /**
            * @brief The main timer callback to send a logging message to SPOT.
            * @param The timer that calls the callback.
            **/
            virtual void callbackToSendLogin(Poco::Timer& timer);

            /**
            * @brief The main timer callback to retry a new connection with SPOT.
            * @param The timer that calls the callback.
            **/
            virtual void callbackToRetryConnection(Poco::Timer& timer);

            /**
            * @brief The main timer callback to send a ping message to SPOT.
            * @param The timer that calls the callback.
            **/
            virtual void callbackToSendPing(Poco::Timer& timer);

            /**
            * @brief Perform a pause to the timer. You could not stop a timer inside the callback timer,
            *        so you need to call this method from the timer callback and when you need to
            *        restart the timer you must to restart that calling the restartTimer.
            * @param The timer to pause.
            **/
            virtual void pauseTimer(Poco::Timer& timer) {timer.restart(0);};

            /**
            * @brief Perform a resume to the timer for a timeout received as parameter.
            * @param The timeout for the resume timer.
            **/
            virtual void resumeTimer(Poco::Timer& timer, long milliseconds) {timer.restart(milliseconds);};

            /**
            * @brief Perform a restart of the timer. You could not call this method from the callback timer,
            *        you will generate a deadlock.
            * @param The timer to restart.
            **/
            virtual void restartSendLoginTimer();

            /**
            * @brief Perform a restart of the timer. You could not call this method from the callback timer,
            *        you will generate a deadlock.
            * @param The timer to restart.
            **/
            virtual void restartRetryConnectionTimer();

            /**
            * @brief Perform a restart of the timer. You could not call this method from the callback timer,
            *        you will generate a deadlock.
            * @param The timer to restart.
            **/
            virtual void restartSendPingTimer();

            /**
            * @brief Sends a ping to the connected spot session.
            * @return false if the ping failed, true in other case.
            **/
            virtual bool ping();

            /**
            * @brief Sends a login to the connected spot session.
            * @return false if the login failed, true in other case.
            **/
            virtual bool login();

            /**
             * @brief Redefine the inherited type classifyMessage.
             *        This method try to find a spot protocol message
             *        using classifiers that were added before in the
             *        list of classifiers.
             * @see Session
             */
            virtual bool classifyMessage(BufferType&, TypeId&, KeyType&);

            /**
             * @brief Obtain the remained buffer from the session.
             * @see Session
             */
            virtual bool remainingBuffer() { ScopedLock l(m_mxSpotSession); return (m_buffer.size()>0); }


            static const char * CapcPlatforms[eTotalPlatforms];///< Acronyms for supported platforms.

            /** @brief default session timeout value.*/
            static const TimeMillisecondsType CDefaultTimeOut = 1000;

            /** @brief default session retries value.*/
            static const RetryType CDefaultRetries = 3;

            /** @brief default connection session retries value.*/
            static const RetryType CDefaultConnectionRetries = 2;

            /**@brief the default value in milliseconds to retry a login*/
            static const int CDefaultTimeToSendLogin = 1000;

            /**@brief the default value in milliseconds between each try to start the session connection*/
            static const int CDefaultTimeBetweenEachRetryConnection = 1000;

            /**@brief the default value in milliseconds to re-start the session connection*/
            static const int CDefaultTimeToRetryConnection = 1000;

            /**@brief the default value in milliseconds to send a new ping*/
            static const int CDefaultTimeToSendPing = 5000;

            /**
             * Defaults spot messages classifiers
             **/
            /** @brief ping message classifier.*/
            SpotClassifyMessagePing                 m_pingClassifier;

            /** @brief login message classifier.*/
            SpotClassifyMessageLogin                m_loginClassifier;

            /** @brief ping response message classifier.*/
            SpotClassifyMessagePingResponse         m_pingResponseClassifier;

            /** @brief login response message classifier.*/
            SpotClassifyMessageLoginResponse        m_loginResponseClassifier;

            /** @brief system status response message classifier.*/
            SpotClassifyMessageSystemStatusResponse m_systemStatusClassifier;

            /** @brief hardware info response message classifier.*/
            SpotClassifyMessageHardwareInfoResponse m_hardwareInfoResponseClassifier;

            /** @brief software info response message classifier.*/
            SpotClassifyMessageSoftwareInfoResponse m_softwareInfoResponseClassifier;




            /** @brief hardware info response message classifier.*/
            SpotClassifyMessageOPTHwrInfoResponse m_optHardwareInfoResponseClassifier;

            /** @brief software info response message classifier.*/
            SpotClassifyMessageOptSwInfoResponse m_optSoftwareInfoResponseClassifier;




            /** @brief package info response message classifier.*/
            SpotClassifyMessagePackageInfoResponse  m_packageInfoResponseClassifier;

            /** @brief debian info response message classifier.*/
            SpotClassifyMessageDebianInfoResponse   m_debianInfoResponseClassifier;

            /** @brief file upload response message classifier.*/
            SpotClassifyMessageFileUploadResponse m_fileUploadResponseClassifier;

            /** @brief file block upload response message classifier.*/
            SpotClassifyMessageFileBlockUploadResponse m_fileUploadBlockResponseClassifier;

            /** @brief file download response message classifier.*/
            SpotClassifyMessageFileDownloadResponse m_fileDownloadResponseClassifier;

            /** @brief file block download response message classifier.*/
            SpotClassifyMessageFileBlockDownloadResponse m_fileDownloadBlockResponseClassifier;

            /** @brief file delete response message classifier.*/
            SpotClassifyMessageFileDeleteResponse m_fileDeleteResponseClassifier;


            /** @brief package download response message classifier.*/
            SpotClassifyMessagePackageDownloadResponse       m_packageDownloadResponseClassifier;

            /** @brief package block download response message classifier.*/
            SpotClassifyMessagePackageBlockDownloadResponse  m_packageBlockDownloadResponseClassifier;

            /** @brief VGD file transfer response message classifier.*/
            SpotClassifyMessageVGDFileTransferResponse       m_VgdFileTransferResponseClassifier;

            /** @brief VGD process package response message classifier.*/
            SpotClassifyMessageVgdProcessPackageResponse     m_VgdProcessPackageResponseClassifier;

            /** @brief Vanguard package activation response message classifier.*/
            SpotClassifyMessageVgdPackageActivationResponse  m_VgdPackageActivationResponseClassifier;

            /** @brief Package activation response message classifier for m3/m5.*/
            SpotClassifyMessagePackageActivationResponse     m_packageActivationResponseClassifier;

            /** @brief new ephemeral key response message classifier.*/
            SpotClassifyMessageNewEphemeralKeyResponse       m_newEphemeralKeyResponseClassifier;

            /** @brief ephemeral encrypt response message classifier.*/
            SpotClassifyMessageEphemeralEncryptResponse      m_ephemeralEncryptResponseClassifier;


            /** @brief client application id login value.*/
            unsigned char m_clientId;

            /** @brief modulo length login request value. Length of PMK RSA module (zero = no Key required).*/
            unsigned char m_moduloLen;

            /** @brief ping timeout login request value. 0=infinite seconds i.e.: no timeout.*/
            unsigned char m_pingTimeout;

            /** @brief login retries pending.*/
            int m_pendingLoginRetries;

            /** @brief connection retries pending.*/
            int m_pendingConnectionRetries;

            /** @brief The device platform to which this session is connected to .*/
            EDevicePlatform  m_eDevicePlatform;

            /** @brief the timer to send the first N retries login.*/
            Poco::Timer m_timerToSendLogin;

            /** @brief the timer to retry a new connection.*/
            Poco::Timer m_timerToRetryConnect;

            /** @brief the timer to retry a new connection.*/
            Poco::Timer m_timerToSendPing;

            typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;
            Poco::FastMutex m_mxSpotSession;
        };
      }
    }
  }

#endif
