/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IChannelServer.h
 * @author mvolker
 * @date Wednesday, April 15, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ICHANNELSERVER_H_
# define _ICHANNELSERVER_H_

# include <vector>
# include <map>
# include <string>

# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>

  typedef unsigned char StatusType;
  typedef unsigned char ErrorType;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /**
       * @brief This is a class forwarding specification.
       * 
       * @see gvr.communication.IChannelServerListener
       */
      class IChannelServerListener;

      /**
       * @brief This is the base class for all channels. Provides the interface for the basic features a channel should serve.
       * 
       * @see gvr.communication.IChannelServerListener
       * 
       */
      class IChannelServer
      {
        public:

          typedef std::size_t SizeType;

          /**
           * @brief This is the status enumeration representation for the channel possible states.
           *        If you need to extend these status you could inherit from this IChannelServerStatus
           *        but you remember that the maximum value of this is limited to 256 by the data
           *        size.
           *
           */
          struct IChannelServerStatus
          {
            static StatusType const CStatusInitial;
            static StatusType const CStatusStarted;
            static StatusType const CStatusStopped;

            static StatusType const CLastStatus;

          };

          /**
           * @brief This is the error enumeration representation for the channel possible errors.
           *        If you need to extend these errors you could inherit from this IChannelServerError
           *        but you remember that the maximum value of this is limited to 256 by the data
           *        size.
           *
           */
          struct IChannelServerError
          {
            static ErrorType const CNoError;
            static ErrorType const CErrorListening;
            static ErrorType const CErrorAccepting;
            static ErrorType const CErrorClosing;
            static ErrorType const CErrorAlreadyStarted;

            static ErrorType const CLastError;

          };



          /**
           * @brief Constructor.
           *
           * @since 0.0.0
           */
          IChannelServer (void);

          /**
           * @brief Destructor.
           *
           * @since 0.0.0
           */
          virtual ~IChannelServer (void);

          /**
           * @brief Subscribe a channel listener to this channel, so it can receive status events.
           *
           * @param channelListener is the channel listener to be subscribed.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelServerListener
           *
           * @since 0.0.0
           */
          virtual void subscribe (IChannelServerListener &) = 0;

          /**
           * @brief Unsubscribe a channel listener from this channel. It will no longer receive status events.
           *
           * @param channelListener is the channel listener to be unsubscribed.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelServerListener
           *
           * @since 0.0.0
           */
          virtual void unsubscribe (IChannelServerListener &) = 0;

          /**
           * @brief Starts the channel communication.
           *
           * @return If and error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelServer.ErrorType
           *
           * @since 0.0.0
           */
          virtual ErrorType start (void) = 0;

          /**
           * @brief Stops the channel communication.
           *
           * @return If and error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelServer.ErrorType
           *
           * @since 0.0.0
           */
          virtual ErrorType stop (void) = 0;

          /**
           * @brief Retrieves the current channel status.
           *
           * @return The current channel status.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelServer.StatusType
           *
           * @since 0.0.0
           */
          virtual StatusType getStatus (void);

          /**
           * @brief Retrieves the current channel status description.
           *
           * @return The current channel status description.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getStatusDescription (void);

          /**
           * @brief Retrieves the current channel status description.
           *
           * @return The current channel status description.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getStatusDescription (StatusType);

          /**
           * @brief Retrieves the current channel error.
           *
           * @return The current channel error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelServer.ErrorType
           *
           * @since 0.0.0
           */
          virtual ErrorType getError (void);

          /**
           * @brief Retrieves the current channel error description.
           *
           * @return The current channel error description.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getErrorDescription (void);

          /**
           * @brief Retrieves the current channel error description.
           *
           * @return The current channel error description.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getErrorDescription (ErrorType);



        protected:

          /**
           * @brief Set the current channel status.
           *
           * @param The current channel status.
           *
           * @return The current channel connection status.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelServer.StatusType
           *
           * @since 0.0.0
           */
          virtual StatusType setStatus (StatusType status);

          /**
           * @brief Set the current channel error.
           *
           * @param The current channel error.
           *
           * @return The current channel error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelServer.ErrorType
           *
           * @since 0.0.0
           */
          virtual ErrorType setError (ErrorType status);

          /**
           * @brief Status type description.
           */
          typedef std::map<StatusType, std::string> descriptionStatusType;

          /**
           * @brief Error type description.
           */
          typedef std::map<ErrorType, std::string> descriptionErrorType;

          /**
           * @brief Status list description.
           */
          descriptionStatusType m_descriptionStatusList;

          /**
           * @brief Error list description.
           */
          descriptionErrorType m_descriptionErrorList;

        private:
          /**
           * @brief Current status.
           */
          StatusType m_status;

          /**
           * @brief Current error.
           */
          ErrorType m_error;

          /**
           * @brief Mutex for status data member.
           */
          Poco::FastMutex m_mxStatus;

          /**
           * @brief Mutex for error data member.
           */
          Poco::FastMutex m_mxError;
      };
    }
  }

#endif
