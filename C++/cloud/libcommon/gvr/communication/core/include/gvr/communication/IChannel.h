/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IChannel.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ICHANNEL_H_
# define _ICHANNEL_H_

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
       * @see gvr.communication.IChannelListener
       */
      class IChannelListener;

      /**
       * @brief This is the base class for all channels. Provides the interface for the basic features a channel should serve.
       * 
       * @see gvr.communication.IChannelListener
       * 
       */
      class IChannel
      {
        public:
          typedef std::vector<unsigned char> BufferType; /// @brief The buffer container type for the serialized representation of the message.
          typedef std::size_t                SizeType;   /// @brief The size type for any size measurable field.

          /**
           * @brief This is the status enumeration representation for the channel possible states.
           *        If you need to extend these status you could inherit from this IChannelStatus
           *        but you remember that the maximum value of this is limited to 256 by the data
           *        size.
           *
           */
          struct IChannelStatus
          {
            static StatusType const CStatusInitial;
            static StatusType const CStatusDisconnected;
            static StatusType const CStatusConnected;

            static StatusType const CLastStatus;

          };

          /**
           * @brief This is the error enumeration representation for the channel possible errors.
           *        If you need to extend these errors you could inherit from this IChannelError
           *        but you remember that the maximum value of this is limited to 256 by the data
           *        size.
           *
           */
          struct IChannelError
          {
            static ErrorType const CNoError;
            static ErrorType const CErrorConnecting;
            static ErrorType const CErrorDisconnecting;
            static ErrorType const CErrorAlreadyConnected;
            static ErrorType const CErrorSending;
            static ErrorType const CErrorReceiving;

            static ErrorType const CLastError;

          };



          /**
           * @brief Constructor.
           *
           * @since 0.0.0
           */
          IChannel (void);

          /**
           * @brief Destructor.
           *
           * @since 0.0.0
           */
          virtual ~IChannel (void);

          /**
           * @brief Subscribe a channel listener to this channel, so it can receive status events.
           *
           * @param channelListener is the channel listener to be subscribed.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelListener
           *
           * @since 0.0.0
           */
          virtual void subscribe (IChannelListener &) = 0;

          /**
           * @brief Unsubscribe a channel listener from this channel. It will no longer receive status events.
           *
           * @param channelListener is the channel listener to be unsubscribed.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelListener
           *
           * @since 0.0.0
           */
          virtual void unsubscribe (IChannelListener &) = 0;

          /**
           * @brief Starts the channel communication.
           *
           * @return If and error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannel.ErrorType
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
           * @see gvr.communication.IChannel.ErrorType
           *
           * @since 0.0.0
           */
          virtual ErrorType stop (void) = 0;

          /**
           * @brief Writes the given buffer to the channel link.
           *
           * @param buffer is the buffer to be written to the channel link.
           *
           * @return If and error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannel.ErrorType
           * @see gvr.communication.IChannel.BufferType
           *
           * @since 0.0.0
           */
          virtual ErrorType send (BufferType const &) = 0;

          /**
           * @brief Retrieves the current channel status.
           *
           * @return The current channel status.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannel.StatusType
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
           * @see gvr.communication.IChannel.ErrorType
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

          /**
           * @brief Retrieves the buffer size to read from the link.
           *
           * @return The buffer size to read from the link.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          unsigned short getBufferSize();


          /**
           * @brief Change the buffer size to read from the link.
           *
           * @param The buffer size to read from the link.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          void setBufferSize(unsigned short bufferSize);


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
           * @see gvr.communication.IChannel.StatusType
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
           * @see gvr.communication.IChannel.ErrorType
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

          /**
           * @brief Mutex for buffersize data member.
           */
          Poco::FastMutex m_mxBufferSize;

          /**
           * @brief Buffer size to read from the link.
           */
          unsigned short m_bufferSize;
      };
    }
  }

#endif
