/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ISession.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ISESSION_H_
# define _ISESSION_H_

# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>
typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;

# include <gvr/communication/IChannelListener.h>
# include <gvr/communication/ISessionListener.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /**
       * @brief This is a class forwarding specification.
       * 
       * @see gvr.communication.IMessage
       */
      template <class MsgSpc>
      class IMessage;

      /**
       * @brief This is a class forwarding specification.
       * 
       * @see gvr.communication.IClassifyMessage
       */
      template <class MsgSpc>
      class IClassifyMessage;

      /**
       * @brief This is the base class for all session protocols. Provides the interface for the basic features a session should serve.
       * 
       * @see gvr.communication.IChannelListener
       * @see gvr.communication.ISessionListener
       * @see gvr.communication.MessageSpec
       * 
       */
      template <class MsgSpc>
      class ISession : public IChannelListener
      {
        public:

          typedef MsgSpc                                Spec;                 /// @brief The specification to which this ISession interface belongs.
          typedef typename Spec::KeyType                KeyType;              /// @brief The the identification key type for the message which pairs request to responses (A.K.A. in some protocols as Sequence Identification Field).
          typedef typename Spec::SizeType               SizeType;             /// @brief The size type for any size measurable field.
          typedef typename Spec::TypeId                 TypeId;               /// @brief The Message class type identification. A unique identifier value which represents a message class.
          typedef typename Spec::BufferType             BufferType;           /// @brief The buffer container type for the serialized representation of the message.
          typedef IMessage<Spec>                        IMessageType;         /// @brief The message type which this ISession can handle.
          typedef IClassifyMessage<Spec>                IClassifyMessageType; /// @brief The message classifier type which this ISession requires to classify messages.
          typedef ISessionListener<Spec>                ISessionListenerType; /// @brief The session listener type which can be subscribed to this ISessionListener.
          typedef SizeType                              TimeMillisecondsType; /// @brief The time in milliseconds integral type which can represents time measures like timeouts.
          typedef SizeType                              RetryType;            /// @brief The send retries integral type which can represents the amount of times a send can be sent again on failure.


          /**
           * @brief This is the status enumeration representation for the session possible states.
           *        If you need to extend these status you could inherit from this ISessionStatus
           *        but you remember that the maximum value of this is limited to 256 by the data
           *        size.
           *
           */
          struct ISessionStatus
          {
            static StatusType const CStatusInitial;
            static StatusType const CStatusSessionStarted;
            static StatusType const CStatusSessionStopped;

            static StatusType const CLastStatus;
          };

          /**
           * @brief This is the error enumeration representation for the session possible errors.
           *        If you need to extend these errors you could inherit from this ISessionError
           *        but you remember that the maximum value of this is limited to 256 by the data
           *        size.
           *
           */
          struct ISessionError
          {
            static ErrorType const CNoError;
            static ErrorType const CErrorNoChannel;

            static ErrorType const CLastError;
          };

          /**
           * @brief Constructor.
           *
           * @since 0.0.0
           */
          ISession (void);

          /**
           * @brief Destructor.
           *
           * @since 0.0.0
           */
          virtual ~ISession (void);

          /**
           * @brief Subscribe a session listener to this session, so it can receive status events.
           *
           * @param sessionListener is the session listener to be subscribed.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.ISessionListenerType
           *
           * @since 0.0.0
           */
          virtual void subscribe (ISessionListenerType &) = 0;

          /**
           * @brief Unsubscribe a session listener from this session. It will no longer receive status events.
           *
           * @param sessionListener is the session listener to be unsubscribed.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.ISessionListenerType
           *
           * @since 0.0.0
           */
          virtual void unsubscribe (ISessionListenerType &) = 0;

          /**
           * @brief Starts the session communication. If the underlying channel is not already started it will also try to start it.
           *
           * @return If an error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual ErrorType start (void) = 0;

          /**
           * @brief Stops the session communication. Also stops the underlying channel.
           *
           * @return If an error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.ErrorType
           *
           * @since 0.0.0
           */
          virtual ErrorType stop (void) = 0;

          /**
           * @brief Starts the session communication replacing the underlying channel. If the new underlying channel is not already started it will also try to start it.
           *
           * @return If an error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.ErrorType
           *
           * @since 0.0.0
           */
          virtual ErrorType start (IChannel &) = 0;

          /**
           * @brief Sets the session default timeout for messages.
           *
           * @param timeout is the session new default timeout.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.TimeMillisecondsType
           *
           * @since 0.0.0
           */
          virtual void setTimeoutDefault (TimeMillisecondsType) = 0;

          /**
           * @brief Gets the session default timeout for messages.
           *
           * @return The session default timeout.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.TimeMillisecondsType
           *
           * @since 0.0.0
           */
          virtual TimeMillisecondsType getTimeoutDefault (void) = 0;

          /**
           * @brief Sets the session default retries for messages in case of errors and timeouts.
           *
           * @param retries is the session new default retries.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.RetryType
           *
           * @since 0.0.0
           */
          virtual void setRetriesDefault (RetryType) = 0;

          /**
           * @brief Gets the session default retries for messages in case of errors and timeouts.
           *
           * @return The session default retries.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.RetryType
           *
           * @since 0.0.0
           */
          virtual RetryType getRetriesDefault (void) = 0;

          /**
           * @brief Adds a new message classifier to the session.
           *
           * @param classifier is the to be added new message classifier to session.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.IClassifyMessageType
           *
           * @since 0.0.0
           */
          virtual void addClassifier (IClassifyMessageType &) = 0;

          /**
           * @brief Removes a message classifier from the session.
           *
           * @param classifier is the to be removed message classifier from the session.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.IClassifyMessageType
           *
           * @since 0.0.0
           */
          virtual void removeClassifier (IClassifyMessageType &) = 0;

          /**
           * @brief Sends the given message through the underlying channel.
           *
           * @param message is the message to be sent through the underlying channel.
           *
           * @return If an error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.IMessageType
           *
           * @since 0.0.0
           */
          virtual ErrorType send (IMessageType const &) = 0;

          /**
           * @brief Sends the given message through the underlying channel and waits until response or error occurs.
           *
           * @param messageReq is the message to be sent through the underlying channel.
           * @param messageRes is the message to be received through the underlying channel.
           * @param timeout to wait until return with TIMEOUT error.
           *
           * @return If an error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.IMessageType
           *
           * @since 0.0.0
           */
          virtual ErrorType sendSync (IMessageType const &, IMessageType &, TimeMillisecondsType) = 0;

          /**
           * @brief Sends the given message through the underlying channel and waits until response or error occurs.
           *
           * @param messageReq is the message to be sent through the underlying channel.
           * @param messageRes is the message to be received through the underlying channel.
           *
           * @return If an error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.IMessageType
           *
           * @since 0.0.0
           */
          virtual ErrorType sendSync (IMessageType const &, IMessageType &) = 0;

          /**
           * @brief Callback for the retrieved buffer by the channel to which the listener is subscribed.
           *
           * @param buffer is the buffer retrieved from the channel link.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelListener.BufferType
           *
           * @since 0.0.0
           */
          virtual void channelEvent (BufferType const &) = 0;

          /**
           * @brief Callback for the new event has occurred reported by the channel to which the listener is subscribed.
           *
           * @param StatusType is the new event has occurred reported by the channel.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IChannelListener.StatusType
           *
           * @since 0.0.0
           */
          virtual void channelEvent (StatusType) = 0;

          /**
           * @brief This method is to check is the session is ready to operate.
           *
           * @return true if the session is ready.
           *
           * @exception Throws no exception.
           *
           *
           * @since 0.0.0
           */
          virtual bool isReady () = 0;




          /**
           * @brief Retrieves the current session status.
           *
           * @return The current session status.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.StatusType
           *
           * @since 0.0.0
           */
          virtual StatusType getStatus (void);

          /**
           * @brief Retrieves the current session status description.
           *
           * @return The current session status description.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getStatusDescription (void);

          /**
           * @brief Retrieves the current session status description.
           *
           * @return The current session status description.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getStatusDescription (StatusType);

          /**
           * @brief Retrieves the current session error.
           *
           * @return The current session error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.ErrorType
           *
           * @since 0.0.0
           */
          virtual ErrorType getError (void);

          /**
           * @brief Retrieves the current session error description.
           *
           * @return The current session error description.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getErrorDescription (void);




        protected:

          /**
           * @brief Set the current session status.
           *
           * @param The current session status.
           *
           * @return The current session status.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.StatusType
           *
           * @since 0.0.0
           */
          virtual StatusType setStatus (StatusType status);

          /**
           * @brief Set the current session error.
           *
           * @param The current session error.
           *
           * @return The current session error.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISession.ErrorType
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

      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      template <class MsgSpc> StatusType const ISession<MsgSpc>::ISessionStatus::CStatusInitial         = 0;
      template <class MsgSpc> StatusType const ISession<MsgSpc>::ISessionStatus::CStatusSessionStarted  = 1;
      template <class MsgSpc> StatusType const ISession<MsgSpc>::ISessionStatus::CStatusSessionStopped  = 2;
      template <class MsgSpc> StatusType const ISession<MsgSpc>::ISessionStatus::CLastStatus            = 10;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      template <class MsgSpc> StatusType const ISession<MsgSpc>::ISessionError::CNoError                = 0;
      template <class MsgSpc> StatusType const ISession<MsgSpc>::ISessionError::CErrorNoChannel         = 1;
      template <class MsgSpc> StatusType const ISession<MsgSpc>::ISessionError::CLastError              = 10;
      /////////////////////////////////////////////////////////////////////////////////////


      template <class MsgSpc>
      ISession<MsgSpc>::ISession (void):
          m_status(ISession<MsgSpc>::ISessionStatus::CStatusInitial), m_error(ISession<MsgSpc>::ISessionError::CNoError), m_descriptionStatusList(),
          m_descriptionErrorList(), m_mxStatus(), m_mxError()
      {
        m_descriptionStatusList [ ISession<MsgSpc>::ISessionStatus::CStatusInitial            ] = "Session is at initial state";
        m_descriptionStatusList [ ISession<MsgSpc>::ISessionStatus::CStatusSessionStarted     ] = "Session is at started state";
        m_descriptionStatusList [ ISession<MsgSpc>::ISessionStatus::CStatusSessionStopped     ] = "Session is at stopped state";
        m_descriptionStatusList [ ISession<MsgSpc>::ISessionStatus::CLastStatus               ] = "Session is at an unknown state";


        m_descriptionErrorList  [ ISession<MsgSpc>::ISessionError::CNoError                   ] = "NO ERROR at this session";
        m_descriptionErrorList  [ ISession<MsgSpc>::ISessionError::CErrorNoChannel            ] = "Session has no channel to send messages.";
        m_descriptionErrorList  [ ISession<MsgSpc>::ISessionError::CLastError                 ] = "Session has an unknown error";
      }

      template <class MsgSpc>
      ISession<MsgSpc>::~ISession (void)
      {
      }

      template <class MsgSpc>
      StatusType ISession<MsgSpc>::getStatus()
      {
        ScopedLock lock(m_mxStatus);

        return m_status;
      }

      template <class MsgSpc>
      ErrorType ISession<MsgSpc>::getError()
      {
        ScopedLock lock(m_mxError);

        return m_error;
      }

      template <class MsgSpc>
      StatusType ISession<MsgSpc>::setStatus(StatusType status)
      {
        ScopedLock lock(m_mxStatus);

        return (m_status=status);
      }

      template <class MsgSpc>
      ErrorType ISession<MsgSpc>::setError(ErrorType error)
      {
        ScopedLock lock(m_mxError);

        return (m_error=error);
      }

      template <class MsgSpc>
      std::string ISession<MsgSpc>::getStatusDescription()
      {
        ScopedLock lock(m_mxStatus);

        descriptionStatusType::const_iterator it = m_descriptionStatusList.find(m_status);
        return (it == m_descriptionStatusList.end())?(m_descriptionStatusList[ISession<MsgSpc>::ISessionStatus::CLastStatus]):(it->second);
      }

      template <class MsgSpc>
      std::string ISession<MsgSpc>::getStatusDescription(StatusType status)
      {
        ScopedLock lock(m_mxStatus);

        descriptionStatusType::const_iterator it = m_descriptionStatusList.find(status);
        return (it == m_descriptionStatusList.end())?(m_descriptionStatusList[ISession<MsgSpc>::ISessionStatus::CLastStatus]):(it->second);
      }

      template <class MsgSpc>
      std::string ISession<MsgSpc>::getErrorDescription()
      {
        ScopedLock lock(m_mxError);

        descriptionErrorType::const_iterator it = m_descriptionErrorList.find(m_error);
        return (it == m_descriptionErrorList.end())?(m_descriptionErrorList[ISession<MsgSpc>::ISessionError::CLastError]):(it->second);
      }
    }
  }

#endif
