/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccSession.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCSESSION_H_
# define _UCCSESSION_H_

# include <gvr/communication/Session.h>
# include <gvr/communication/ucc/UccChannel.h>
# include <gvr/communication/ucc/UccMessageSpec.h>

# include <Poco/Timer.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Ucc Domain
      namespace ucc
      {
        /**
         * @brief This class is the ucc session.
         * @see gvr.communication.Session
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class UccSession : public Session<UccMessageSpec>
        {
          public:
            typedef Session<UccMessageSpec> AbsSessionType;

            /**
             * @brief This is the status enumeration representation for the session possible states.
             *        If you need to extend these status you could inherit from this UccSessionStatus.
             *
             */
            struct UccSessionStatus: public AbsSessionType::SessionStatus
            {
              static StatusType const CSessionLogged;
              static StatusType const CSessionLoggedOut;

              static StatusType const CLastStatus;
            };

            /**
             * @brief This is the error enumeration representation for the session possible errors.
             *        If you need to extend these errors you could inherit from this UccSessionError.
             *
             */
            struct UccSessionError: public AbsSessionType::SessionError
            {
              static ErrorType const CErrorOnPing;
              static ErrorType const CErrorOnLoging;
              static ErrorType const CErrorNoUccSessionAvailable;

              static ErrorType const CLastError;
            };

            /**
            * @brief Default Constructor.
            * @since 0.0.0
            */
            UccSession (void);

            /**
             * @brief Destructor.
             * @since 0.0.0
             */
            virtual ~UccSession (void);

            /**
             * @brief redefine the inherited type. Starts the session worker.
             * @see Session
             */
            virtual ErrorType start (void);

            /**
             * @brief redefine the inherited type. Start the session worker.
             * @see Session
             */
            virtual ErrorType start (UccChannel &);

            /**
             * @brief Start the session for the given channel and pingTimeout parameters.
             * @param channel
             * @param pingTimeot ping timeout parameter to ucc login
             * @return returns the session status resulting.
             * */
            virtual ErrorType start (UccChannel & channel, TimeMillisecondsType pingTimeout);

            /**
             * @brief redefine the inherited type. Stop the session worker.
             * @see Session
             */
            virtual ErrorType stop (void);

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
            virtual bool hasError (void);

            /**
              * @brief Perform a verification if the SpotSession is connected.
              * @return true if the session is connected.
              */
            virtual bool isConnected (void);

            /**
              * @brief Perform a verification if the SpotSession is ready to operate.
              * @return true if the session is ready.
              */
            virtual bool isLogged (void);

            /**
             * @brief Gets the ping timeout login value.
             * @return the current ping timeout login value.
             */
            virtual TimeMillisecondsType getPingTimeout (void);

            /**
             * @brief Sets the new login ping timeout field value
             * @param pingTimeout, login ping timeout request value. 0=infinite seconds i.e.: no timeout.
             */
            virtual void setPingTimeout (TimeMillisecondsType);

          protected:
            /**
             * @brief Redefine the inherited type classifyMessage.
             *        This method try to find a spot protocol message
             *        using classifiers that were added before in the
             *        list of classifiers.
             * @see Session
             */
            virtual bool classifyMessage (BufferType&, TypeId&, KeyType&);

            /**
            * @brief Starts the connection cycle after starting a connection to the UCC.
            **/
            virtual void private_safe_startConnectionCycle (void);

            /**
            * @brief Stops the connection cycle before stopping a connection to the UCC.
            **/
            virtual void private_safe_stopConnectionCycle (void);

            /** @brief default session retries value.*/
            static RetryType const CDefaultRetries = 3;

            /** @brief default session timeout value.*/
            static TimeMillisecondsType const CDefaultTimeOut = 1000;

            /**@brief the default value in milliseconds to send a new ping*/
            static TimeMillisecondsType const CDefaultPingTimeout = 5000;

            /** @brief ping timeout login request value. 0=infinite seconds i.e.: no timeout.*/
            TimeMillisecondsType m_pingTimeout;

            typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;
            Poco::FastMutex m_mtxUcc;
        };
      }
    }
  }

#endif
