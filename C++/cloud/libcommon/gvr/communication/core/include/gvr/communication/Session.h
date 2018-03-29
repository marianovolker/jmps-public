/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file Session.h
 * @author mgrieco
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SESSION_H_
# define _SESSION_H_

# include <set>
# include <map>

# include <Poco/Mutex.h>
# include <Poco/ScopedLock.h>
# include <Poco/Thread.h>
# include <Poco/Runnable.h>
# include <Poco/Event.h>

# include <gvr/communication/ISession.h>
# include <gvr/communication/IMessage.h>
# include <gvr/communication/IClassifyMessage.h>

# include <gvr/communication/MessageContext.h>

#include <iostream>
#include <iomanip>

using namespace std;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      template <class MsgSpc>
      class Session : public ISession<MsgSpc>
      {
        public:

          /**
           * @brief specifies the template types.
           * @see ISession types
           */
          typedef MsgSpc                                                            Spec;
          typedef ISession<Spec>                                                    ISessionType;
          typedef typename ISessionType::KeyType                                    KeyType;
          typedef typename ISessionType::BufferType                                 BufferType;
          typedef typename ISessionType::SizeType                                   SizeType;
          typedef typename ISessionType::TypeId                                     TypeId;
          typedef typename ISessionType::IMessageType                               IMessageType;
          typedef MessageContext<Spec>                                              MessageContextType;
          typedef typename ISessionType::IClassifyMessageType                       IClassifyMessageType;
          typedef typename ISessionType::ISessionListenerType                       ISessionListenerType;
          typedef typename ISessionType::TimeMillisecondsType                       TimeMillisecondsType;
          typedef typename ISessionType::RetryType                                  RetryType;
          typedef typename std::multimap<ISessionListenerType *, TypeId>::iterator  IteratorListenerType;

          /**
           * @brief This is the status enumeration representation for the session possible states.
           *        If you need to extend these status you could inherit from this SessionStatus
           *        but you remember that the maximum value of this is limited to 256 by the data
           *        size.
           *
           */
          struct SessionStatus: public ISessionType::ISessionStatus
          {
            static StatusType const CSessionOnError;

            static StatusType const CLastStatus;
          };

          /**
           * @brief This is the error enumeration representation for the session possible errors.
           *        If you need to extend these errors you could inherit from this SessionError
           *        but you remember that the maximum value of this is limited to 256 by the data
           *        size.
           *
           */
          struct SessionError: public ISessionType::ISessionError
          {
            static ErrorType const CErrorStartingSenderThread;
            static ErrorType const CErrorStartingReceiverThread;
            static ErrorType const CErrorSendingMessage;
            static ErrorType const CErrorTimeoutSendingMessage;
            static ErrorType const CUnknownError;

            static ErrorType const CLastError;
          };

          /**
           * @brief Constructor.
           *
           * @param timeout is the expiration time measured in milliseconds for the to be sent messages.
           * @param retries is the amount of retries in case timeout is expired. If underlying channel presents an error, no retries will be performed.
           *
           * @since 0.0.0
           */
          Session (TimeMillisecondsType timeoutMilliseconds = 1000, RetryType retries = 2);

          /**
           * @brief Destructor.
           *
           * @since 0.0.0
           */
          virtual ~Session (void);

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
          virtual void subscribe (ISessionListenerType &);

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
          virtual void subscribe (ISessionListenerType &, TypeId);

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
          virtual void unsubscribe (ISessionListenerType &);

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
          virtual void unsubscribe (ISessionListenerType &, TypeId);

          /**
           * @brief Starts the session communication. If the underlying channel is not already started it will also try to start it.
           *
           * @return If an error occurs it will return the appropriate error.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual ErrorType start (void);

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
          virtual ErrorType stop (void);

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
          virtual ErrorType start (IChannel &);

          /**
           * @brief Replace the underlying channel with a new one.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void setChannel (IChannel &);

          /**
           * @brief Unlink channel associated with this session.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void unSetChannel ();

          /**
           * @brief Replace the underlying channel with a new one.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual const IChannel & getChannel ( ) const { return (*m_pChannel); } ;


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
          virtual void setTimeoutDefault (TimeMillisecondsType);

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
          virtual TimeMillisecondsType getTimeoutDefault (void);

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
          virtual void setRetriesDefault (RetryType);

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
          virtual RetryType getRetriesDefault (void);

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
          virtual void addClassifier (IClassifyMessageType &);

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
          virtual void removeClassifier (IClassifyMessageType &);

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
          virtual ErrorType send (IMessageType const &);

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
          virtual ErrorType sendSync (IMessageType const &, IMessageType &, TimeMillisecondsType);

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
          virtual ErrorType sendSync (IMessageType const &, IMessageType &);

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
          virtual void channelEvent (BufferType const &);

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
          virtual void channelEvent (StatusType);

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
          virtual bool isReady();

        protected:

          /**
           * @brief This method dequeue a message and send it throw a channel.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          void dispatchMessages ();

          /**
           * @brief This method verify the entire messages list and wakeup the holding messages.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          void verifyMessagesPendingList ();

          /**
           * @brief Try to locate a complete message using classifiers that was added previously.
           *
           * @return true if an entire message was recognized.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual bool classifyMessage(BufferType&, TypeId&, KeyType&);

          /**
           * @brief Verify if there is a message sync pending into the context messages list.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          bool checkIsSyncMessagePending(BufferType&, TypeId&, KeyType&);

          /**
           * @brief dispatch the unsolicited message to the subscribers.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          void dispatchUnsolicitedMessageToSubscribers(BufferType&, TypeId&, KeyType&);

          /**
           * @brief Helper function to start the session sender thread.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          bool startSessionSenderThread();

          /**
           * @brief Helper function to start the session receiver thread.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          bool startSessionReceiverThread();

          /**
           * @brief This subclass is a helper to create a SessionSender thread.
           *
           * @author mvolker
           */
          class SessionSender: public Poco::Runnable
          {
            public:
              SessionSender();
              virtual ~SessionSender();

              bool start(Session<MsgSpc>&);
              bool stop();

            private:
              bool isRunning();
              bool turnOn();
              bool turnOff();
              void run();

              // Protect the access from the outside, public interface ...
              Poco::FastMutex m_mxSessionSender;

              Poco::FastMutex m_mxSession;
              Session<MsgSpc>* m_pSession;

              Poco::FastMutex m_mxRun;
              bool m_run;

              Poco::Thread m_thread;
              Poco::Event m_readyStarted;
              Poco::Event m_readyStopped;
          };

          /**
           * @brief This subclass is a helper to create a SessionReceiver thread.
           *
           * @author mvolker
           */
          class SessionReceiver: public Poco::Runnable
          {
            public:
              SessionReceiver();
              virtual ~SessionReceiver();

              bool start(Session<MsgSpc>&);
              bool stop();

            private:
              bool isRunning();
              bool turnOn();
              bool turnOff();
              void run();

              // Protect the access from the outside, public interface ...
              Poco::FastMutex m_mxSessionReceiver;

              Poco::FastMutex m_mxSession;
              Session<MsgSpc>* m_pSession;

              Poco::FastMutex m_mxRun;
              bool m_run;

              Poco::Thread m_thread;
              Poco::Event m_readyStarted;
              Poco::Event m_readyStopped;
          };


        protected:

            typedef Poco::ScopedLock<Poco::FastMutex> ScopedLock;

            Poco::FastMutex                                 m_mxSession;

            IChannel *                                      m_pChannel;
            TimeMillisecondsType                            m_timeoutMilliseconds;
            RetryType                                       m_retries;
            BufferType                                      m_buffer;

            Poco::FastMutex                                 m_mxClassifiers;
            std::set<IClassifyMessageType *>                m_classifiers;

            static const unsigned char                      CSubscribeToALLMessages = 0;
            Poco::FastMutex                                 m_mxListeners;
            std::multimap<ISessionListenerType *, TypeId>   m_listeners;

            Poco::FastMutex                                 m_mxMessagesContext;
            std::set<MessageContextType *>                  m_messagesContext;

            SessionSender                                   m_sessionSender;
            SessionReceiver                                 m_sessionReceiver;
      };


      // SessionSender Thread ...
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
      template <class MsgSpc>
      Session<MsgSpc>::SessionSender::SessionSender()
        : m_pSession(0), m_run(false), m_thread("SessionSender"),
          m_readyStarted(), m_readyStopped(),
          m_mxSessionSender(), m_mxSession(), m_mxRun()
      {
      }

      template <class MsgSpc>
      Session<MsgSpc>::SessionSender::~SessionSender()
      {
        stop();
      }

      template <class MsgSpc>
      void Session<MsgSpc>::SessionSender::run()
      {
        // Wait for the signal to begin the thread ...
        m_readyStarted.wait();

        while( isRunning() )
        {
          {
            ScopedLock lock(m_mxSession);
            if(m_pSession)
            {
              m_pSession->dispatchMessages();
            }
          }

          Poco::Thread::sleep(100);
        }

        // Signal that the thread has finished...
        m_readyStopped.set();
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionSender::start(Session<MsgSpc>& session)
      {
        ScopedLock lock(m_mxSessionSender);

        if( !isRunning() )
        {
          try
          {
            turnOn();

            {
              ScopedLock lock(m_mxSession);
              m_pSession = &session;
            }

            m_thread.start(*this);

            // Signal that the thread could begin...
            m_readyStarted.set();
          }
          catch(...)
          {
            return false;
          }
        }

        return true;
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionSender::stop()
      {
        ScopedLock lock(m_mxSessionSender);

        if( isRunning() )
        {
          try
          {
            turnOff();

            {
              ScopedLock lock(m_mxSession);
              m_pSession = 0;
            }

            // Wait for the signal to terminate the thread ...
            m_readyStopped.wait();

            m_thread.join();
          }
          catch(...)
          {
            return false;
          }
        }

        return true;
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionSender::isRunning()
      {
        ScopedLock lock(m_mxRun);
        return m_run;
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionSender::turnOff()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=false);
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionSender::turnOn()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=true);
      }
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////




      // SessionReceiver Thread ...
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
      template <class MsgSpc>
      Session<MsgSpc>::SessionReceiver::SessionReceiver()
        : m_pSession(0), m_run(false), m_thread("SessionReceiver"),
          m_readyStarted(), m_readyStopped(),
          m_mxSessionReceiver(), m_mxSession(), m_mxRun()
      {
      }

      template <class MsgSpc>
      Session<MsgSpc>::SessionReceiver::~SessionReceiver()
      {
        stop();
      }

      template <class MsgSpc>
      void Session<MsgSpc>::SessionReceiver::run()
      {
        // Wait for the signal to begin the thread ...
        m_readyStarted.wait();

        while( isRunning() )
        {
          {
            ScopedLock lock(m_mxSession);
            if(m_pSession)
            {
              m_pSession->verifyMessagesPendingList();
            }
          }

          Poco::Thread::sleep(100);
        }

        // Signal that the thread has finished...
        m_readyStopped.set();
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionReceiver::start(Session<MsgSpc>& session)
      {
        ScopedLock lock(m_mxSessionReceiver);

        if( !isRunning() )
        {
          try
          {
            turnOn();

            {
              ScopedLock lock(m_mxSession);
              m_pSession = &session;
            }

            m_thread.start(*this);

            // Signal that the thread could begin...
            m_readyStarted.set();
          }
          catch(...)
          {
            return false;
          }
        }

        return true;
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionReceiver::stop()
      {
        ScopedLock lock(m_mxSessionReceiver);

        if( isRunning() )
        {
          try
          {
            turnOff();

            {
              ScopedLock lock(m_mxSession);
              m_pSession = 0;
            }

            // Wait for the signal to terminate the thread ...
            m_readyStopped.wait();

            m_thread.join();
          }
          catch(...)
          {
            return false;
          }
        }

        return true;
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionReceiver::isRunning()
      {
        ScopedLock lock(m_mxRun);
        return m_run;
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionReceiver::turnOff()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=false);
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::SessionReceiver::turnOn()
      {
        ScopedLock lock(m_mxRun);
        return (m_run=true);
      }
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////



      // Generic Session
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////

      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      template <class MsgSpc> StatusType const Session<MsgSpc>::SessionStatus::CSessionOnError              = ISessionType::ISessionStatus::CLastStatus + 1;

      template <class MsgSpc> StatusType const Session<MsgSpc>::SessionStatus::CLastStatus                  = ISessionType::ISessionStatus::CLastStatus + 10;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      template <class MsgSpc> ErrorType const Session<MsgSpc>::SessionError::CErrorStartingSenderThread     = ISessionType::ISessionError::CLastError + 1;
      template <class MsgSpc> ErrorType const Session<MsgSpc>::SessionError::CErrorStartingReceiverThread   = ISessionType::ISessionError::CLastError + 2;
      template <class MsgSpc> ErrorType const Session<MsgSpc>::SessionError::CErrorSendingMessage           = ISessionType::ISessionError::CLastError + 3;
      template <class MsgSpc> ErrorType const Session<MsgSpc>::SessionError::CErrorTimeoutSendingMessage    = ISessionType::ISessionError::CLastError + 4;
      template <class MsgSpc> ErrorType const Session<MsgSpc>::SessionError::CUnknownError                  = ISessionType::ISessionError::CLastError + 5;

      template <class MsgSpc> ErrorType const Session<MsgSpc>::SessionError::CLastError                     = ISessionType::ISessionError::CLastError + 10;
      /////////////////////////////////////////////////////////////////////////////////////



      template <class MsgSpc>
      Session<MsgSpc>::Session (TimeMillisecondsType timeoutMilliseconds, RetryType retries)
       : ISessionType(), m_mxSession(), m_pChannel(0), m_timeoutMilliseconds(timeoutMilliseconds), m_retries(retries),
         m_buffer(), m_classifiers(), m_listeners(), m_messagesContext(), m_sessionSender(), m_sessionReceiver(),
         m_mxMessagesContext(), m_mxListeners(), m_mxClassifiers()
      {
        Session::m_descriptionStatusList [ Session::SessionStatus::CSessionOnError             ] = "Session is at error state";
        Session::m_descriptionStatusList [ Session::SessionStatus::CLastStatus                 ] = "Session is at an unknown status";

        Session::m_descriptionErrorList  [ Session::SessionError::CErrorStartingSenderThread   ] = "Session could not start the sender thread.";
        Session::m_descriptionErrorList  [ Session::SessionError::CErrorStartingReceiverThread ] = "Session could not start the receiver thread.";
        Session::m_descriptionErrorList  [ Session::SessionError::CErrorSendingMessage         ] = "Session could not send a message through a link.";
        Session::m_descriptionErrorList  [ Session::SessionError::CErrorTimeoutSendingMessage  ] = "Session has a timeout error sending a message through a link.";
        Session::m_descriptionErrorList  [ Session::SessionError::CUnknownError                ] = "Session has an unknown error.";
      }


      template <class MsgSpc>
      Session<MsgSpc>::~Session (void)
      {
        stop();
      }


      template <class MsgSpc>
      void Session<MsgSpc>::dispatchMessages ()
      {
        ScopedLock lock(m_mxMessagesContext);

        typename std::set<MessageContextType *>::iterator it;
        for( it = m_messagesContext.begin(); it != m_messagesContext.end(); ++it )
        {
          MessageContextType * pMessageContext(dynamic_cast<MessageContextType *>( (*it) ));
          if( pMessageContext->getState() == MessageContextType::eMessageContextReady )
          {
            if( !m_pChannel )
            {
              Session::setError(ISession<MsgSpc>::ISessionError::CErrorNoChannel);
              pMessageContext->setState(MessageContextType::eMessageContextError);
              continue;
            }

            BufferType buffer = pMessageContext->getMessageReq().serialize();

            if( m_pChannel->send(buffer) == IChannel::IChannelError::CNoError )
            {
              pMessageContext->setState(MessageContextType::eMessageContextWaiting);
            }
            else
            {
              pMessageContext->setState(MessageContextType::eMessageContextError);
            }
          }
        }
      }


      template <class MsgSpc>
      void Session<MsgSpc>::verifyMessagesPendingList ()
      {
        ScopedLock lock(m_mxMessagesContext);

        typename std::set<MessageContextType *>::iterator it;
        for( it = m_messagesContext.begin(); it != m_messagesContext.end(); ++it )
        {
          MessageContextType * pMessageContext(dynamic_cast<MessageContextType *>( (*it) ));
          if( pMessageContext->getState() == MessageContextType::eMessageContextError || pMessageContext->getState() == MessageContextType::eMessageContextCompleted )
          {
            pMessageContext->wakeUp();
          }
          else if( pMessageContext->expired() )
          {
            pMessageContext->setState(MessageContextType::eMessageContextTimeout);
            pMessageContext->wakeUp();
          }
        }
      }


      template <class MsgSpc>
      void Session<MsgSpc>::subscribe (ISessionListenerType & listener)
      {
        ScopedLock lock(m_mxListeners);

        bool found(false);

        std::pair < IteratorListenerType, IteratorListenerType > range;
        range = m_listeners.equal_range(&listener);
        IteratorListenerType it = range.first;

        for( ; it != range.second; ++it )
        {
          if( it->second == CSubscribeToALLMessages )
          {
            found=true;
            break;
          }
        }
        if( !found )
        {
          std::cout << "This listener [" << std::hex << &listener << "] is subscribed to this typeId [" << std::hex << static_cast<int>(CSubscribeToALLMessages) << "]." << std::endl;

          m_listeners.insert( std::make_pair(&listener, CSubscribeToALLMessages) );
        }
      }


      template <class MsgSpc>
      void Session<MsgSpc>::subscribe (ISessionListenerType & listener, TypeId typeId)
      {
        ScopedLock lock(m_mxListeners);

        bool found(false);

        std::pair < IteratorListenerType, IteratorListenerType > range;
        range = m_listeners.equal_range(&listener);
        IteratorListenerType it = range.first;

        for( ; it != range.second; ++it )
        {
          if( it->second == typeId )
          {
            found=true;
            break;
          }
        }
        if( !found )
        {
          std::cout << "This listener [" << std::hex << &listener << "] is subscribed to this typeId [" << std::hex << static_cast<int>(typeId) << "]." << std::endl;

          m_listeners.insert( std::make_pair(&listener, typeId) );
        }
      }


      template <class MsgSpc>
      void Session<MsgSpc>::unsubscribe (ISessionListenerType & listener)
      {
        ScopedLock lock(m_mxListeners);

        std::pair < IteratorListenerType, IteratorListenerType > range;
        range = m_listeners.equal_range(&listener);
        IteratorListenerType it = range.first;

        // Here we need to unsubscribe to all events if not a specific was indicated.
        for( ; it != range.second; ++it )
        {
          m_listeners.erase(it);
        }
      }


      template <class MsgSpc>
      void Session<MsgSpc>::unsubscribe (ISessionListenerType & listener, TypeId typeId)
      {
        ScopedLock lock(m_mxListeners);

        std::pair < IteratorListenerType, IteratorListenerType > range;
        range = m_listeners.equal_range(&listener);
        IteratorListenerType it = range.first;

        for( ; it != range.second; ++it )
        {
          if( it->second == typeId )
          {
            m_listeners.erase(it);
          }
        }
      }


      template <class MsgSpc>
      void Session<MsgSpc>::setChannel(IChannel & channel)
      {
        ScopedLock lock(m_mxSession);

        if( m_pChannel )
        {
          m_sessionSender.stop();
          m_sessionReceiver.stop();

          m_pChannel->unsubscribe(*this);
          m_pChannel->stop();

          m_pChannel = 0;
        }

        m_pChannel = &channel;
        m_pChannel->subscribe(*this);
      }

      template <class MsgSpc>
      void Session<MsgSpc>::unSetChannel()
      {
        ScopedLock lock(m_mxSession);

        if( m_pChannel )
        {
          m_pChannel->unsubscribe(*this);
          m_pChannel->stop();

          m_pChannel = 0;
        }
      }


      template <class MsgSpc>
      ErrorType Session<MsgSpc>::start (IChannel & channel)
      {
        StatusType status = Session::getStatus();

        if( status != Session::SessionStatus::CStatusSessionStarted )
        {
          ScopedLock lock(m_mxSession);

          if( m_pChannel )
          {
            m_sessionSender.stop();
            m_sessionReceiver.stop();

            m_pChannel->unsubscribe(*this);
            m_pChannel->stop();

            m_pChannel = 0;
          }

          Session::setError(Session::SessionError::CNoError);
          Session::setStatus(Session::SessionStatus::CSessionOnError);

          m_pChannel = &channel;
          m_pChannel->subscribe(*this);

          if( m_pChannel->start() != IChannel::IChannelError::CNoError )
          {
            return Session::setError(Session::SessionError::CErrorNoChannel);
          }

          if( !m_sessionSender.start(*this) )
          {
            Session::setError(Session::SessionError::CErrorStartingSenderThread);
          }

          if( !m_sessionReceiver.start(*this) )
          {
            Session::setError(Session::SessionError::CErrorStartingReceiverThread);
          }

          return Session::getError();
        }

        return Session::setError(Session::SessionError::CNoError);
      }


      template <class MsgSpc>
      ErrorType Session<MsgSpc>::start (void)
      {
        StatusType status = Session::getStatus();

        if( status != Session::SessionStatus::CStatusSessionStarted )
        {
          ScopedLock lock(m_mxSession);

          if( !m_pChannel )
          {
            return Session::setError(Session::SessionError::CErrorNoChannel);
          }

          while( !m_sessionSender.stop()   ){ Poco::Thread::sleep(5); }
          while( !m_sessionReceiver.stop() ){ Poco::Thread::sleep(5); }
          while( m_pChannel->stop() != IChannel::IChannelError::CNoError ){ Poco::Thread::sleep(5); }

          Session::setError(Session::SessionError::CNoError);
          Session::setStatus(Session::SessionStatus::CStatusSessionStopped);

          m_pChannel->subscribe(*this);

          if( m_pChannel->start() != IChannel::IChannelError::CNoError )
          {
            return Session::setError(Session::SessionError::CErrorNoChannel);
          }

          if( !startSessionSenderThread() )
          {
            Session::setError(Session::SessionError::CErrorStartingSenderThread);
          }

          if( !startSessionReceiverThread() )
          {
            Session::setError(Session::SessionError::CErrorStartingReceiverThread);
          }

          return Session::getError();
        }

        return Session::setError(Session::SessionError::CNoError);
      }


      template <class MsgSpc>
      ErrorType Session<MsgSpc>::stop (void)
      {
        ScopedLock lock(m_mxSession);

        StatusType status = Session::getStatus();
        ErrorType  error  = Session::SessionError::CNoError;

        if( status != Session::SessionStatus::CStatusSessionStopped )
        {
          Session::setError(Session::SessionError::CNoError);

          while( !m_sessionSender.stop()   ){ Poco::Thread::sleep(5); }
          while( !m_sessionReceiver.stop() ){ Poco::Thread::sleep(5); }

          if( !m_pChannel )
          {
            return Session::setError(Session::SessionError::CErrorNoChannel);
          }
          else
          {
            while( m_pChannel->stop() != IChannel::IChannelError::CNoError ){ Poco::Thread::sleep(5); }
          }

          if( m_pChannel )
          {
            m_pChannel->unsubscribe(*this);
          }

          Session::setStatus(Session::SessionStatus::CStatusSessionStopped);

          error = Session::getError();
        }

        return Session::setError(error);
      }


      template <class MsgSpc>
      void Session<MsgSpc>::setTimeoutDefault (TimeMillisecondsType timeoutMilliseconds)
      {
        ScopedLock lock(m_mxSession);

        m_timeoutMilliseconds = timeoutMilliseconds;
      }


      template <class MsgSpc>
      typename Session<MsgSpc>::TimeMillisecondsType Session<MsgSpc>::getTimeoutDefault (void)
      {
        ScopedLock lock(m_mxSession);

        return m_timeoutMilliseconds;
      }


      template <class MsgSpc>
      void Session<MsgSpc>::setRetriesDefault (RetryType retries)
      {
        ScopedLock lock(m_mxSession);

        m_retries = retries;
      }


      template <class MsgSpc>
      typename Session<MsgSpc>::RetryType Session<MsgSpc>::getRetriesDefault (void)
      {
        ScopedLock lock(m_mxSession);

        return m_retries;
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::isReady (void)
      {
        ScopedLock lock(m_mxSession);

        return ( Session<MsgSpc>::getStatus() == Session::SessionStatus::CStatusSessionStarted );
      }

      template <class MsgSpc>
      void Session<MsgSpc>::addClassifier (IClassifyMessageType & classifier)
      {
        ScopedLock lock(m_mxClassifiers);

        typename std::set<IClassifyMessageType *>::iterator found = m_classifiers.find(&classifier);
        if (found == m_classifiers.end())
        {
          m_classifiers.insert(&classifier);
        }
      }


      template <class MsgSpc>
      void Session<MsgSpc>::removeClassifier (IClassifyMessageType & classifier)
      {
        ScopedLock lock(m_mxClassifiers);

        m_classifiers.erase(&classifier);
      }


      template <class MsgSpc>
      ErrorType Session<MsgSpc>::send (IMessageType const & message)
      {
        ScopedLock lock(m_mxSession);

        if( !m_pChannel )
        {
          return Session::setError(Session::SessionError::CErrorNoChannel);
        }

        BufferType buffer = message.serialize();

        if( m_pChannel->send(buffer) != IChannel::IChannelError::CNoError )
        {
          return Session::setError(Session::SessionError::CErrorSendingMessage);
        }

        return Session::setError(Session::SessionError::CNoError);
      }


      template <class MsgSpc>
      ErrorType Session<MsgSpc>::sendSync (IMessageType const & messageReq, IMessageType & messageRes, TimeMillisecondsType timeout)
      {
        {
          ScopedLock lock(m_mxSession);
          if( !m_pChannel )
          {
            return Session::setError(Session::SessionError::CErrorNoChannel);
          }
        }

        MessageContextType *pNewMessageContext(new MessageContextType(messageReq, messageRes, timeout));

        {
          ScopedLock lock(m_mxMessagesContext);
          pNewMessageContext->setState(MessageContextType::eMessageContextReady);
          m_messagesContext.insert(pNewMessageContext);
        }

        pNewMessageContext->wait();


        if( pNewMessageContext->getState() == MessageContextType::eMessageContextCompleted )
        {
          Session::setError(Session::SessionError::CNoError);
        }
        else if( pNewMessageContext->getState() == MessageContextType::eMessageContextTimeout )
        {
          Session::setError(Session::SessionError::CErrorTimeoutSendingMessage);
        }
        else
        {
          Session::setError(Session::SessionError::CErrorSendingMessage);
        }

        {
          ScopedLock lock(m_mxMessagesContext);
          m_messagesContext.erase(pNewMessageContext);
        }

        delete pNewMessageContext;

        return Session::getError();
      }


      template <class MsgSpc>
      ErrorType Session<MsgSpc>::sendSync (IMessageType const & messageReq, IMessageType & messageRes)
      {
        return Session::sendSync (messageReq, messageRes, getTimeoutDefault());
      }


      template <class MsgSpc>
      void Session<MsgSpc>::channelEvent (StatusType status)
      {
        StatusType sessionStatus;

        if( (status == IChannel::IChannelStatus::CStatusConnected) && (Session::getError() == Session::SessionError::CNoError) )
        {
          sessionStatus = Session::SessionStatus::CStatusSessionStarted;
        }
        else if( status == IChannel::IChannelStatus::CStatusDisconnected )
        {
          sessionStatus = Session::SessionStatus::CStatusSessionStopped;
        }
        else
        {
          sessionStatus = Session::SessionStatus::CSessionOnError;
        }

        Session::setStatus(sessionStatus);

        ScopedLock lock(m_mxListeners);
        IteratorListenerType it;
        for( it = m_listeners.begin(); it != m_listeners.end(); ++it )
        {
          ISessionListenerType * pSessionListener(dynamic_cast<ISessionListenerType *>( it->first ));
          pSessionListener->sessionEvent(sessionStatus);
        }
      }


      template <class MsgSpc>
      void Session<MsgSpc>::channelEvent (BufferType const & buffer)
      {
        if( buffer.empty() )
        {
          return;
        }

        SizeType newSize = m_buffer.size() + buffer.size();
        m_buffer.reserve(newSize);
        m_buffer.insert(m_buffer.end(),buffer.begin(),buffer.end());

        BufferType messageBuffer;
        TypeId typeId;
        KeyType key;
        while( classifyMessage(messageBuffer, typeId, key) == true )
        {
          //look for a message context if exists, set the status of the context for the receiver thread wakeup the worker thread.
          if( !checkIsSyncMessagePending(messageBuffer, typeId, key) )
          {
            //If there's not a message context pending with this typeId then we need to inform to all listener that
            //a new unsolicited message was arrived.
            dispatchUnsolicitedMessageToSubscribers(messageBuffer, typeId, key);
          }
        }
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::checkIsSyncMessagePending(BufferType& messageBuffer, TypeId& typeId, KeyType& key)
      {
        ScopedLock lock(m_mxMessagesContext);

        typename std::set<MessageContextType *>::iterator it = m_messagesContext.begin();
        for( ; it != m_messagesContext.end(); ++it )
        {
          MessageContextType * pMessageContext(dynamic_cast<MessageContextType *>( (*it) ));
          if( (pMessageContext->getMessageRes().getKey() == key) && (pMessageContext->getMessageRes().getTypeId() == typeId) )
          {
            pMessageContext->setMessageRes(messageBuffer);
            pMessageContext->setState(MessageContextType::eMessageContextCompleted);
            return true;
          }
        }
        return false;
      }

      template <class MsgSpc>
      void Session<MsgSpc>::dispatchUnsolicitedMessageToSubscribers(BufferType& messageBuffer, TypeId& typeId, KeyType& key)
      {
        ScopedLock lock(m_mxListeners);
        IteratorListenerType it;
        for( it = m_listeners.begin(); it != m_listeners.end(); ++it )
        {
          if( (it->second == CSubscribeToALLMessages) || (it->second == typeId) )
          {
            std::cout << "typeId [" << static_cast<int>(typeId) << "]" << std::endl;

            std::cout << "Notify to listener [" << std::hex << it->first << "], for typeId [" << std::hex << static_cast<int>(it->second) << "]." << std::endl;

            ISessionListenerType * pSessionListener(dynamic_cast<ISessionListenerType *>( it->first ));
            pSessionListener->sessionEvent(messageBuffer, typeId, key);
          }
        }
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::startSessionSenderThread()
      {
        short retries(5);

        while( (retries--) > 0 )
        {
          if( m_sessionSender.start(*this) )
          {
            return true;
          }
          Poco::Thread::sleep(10);
        }
        return false;
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::startSessionReceiverThread()
      {
        short retries(5);

        while( (retries--) > 0 )
        {
          if( m_sessionReceiver.start(*this) )
          {
            return true;
          }
          Poco::Thread::sleep(10);
        }
        return false;
      }

      template <class MsgSpc>
      bool Session<MsgSpc>::classifyMessage(BufferType& messageBuffer, TypeId& typeId, KeyType& key)
      {
        ScopedLock lock(m_mxClassifiers);
        typename std::set<IClassifyMessageType *>::iterator iter = m_classifiers.begin();
        typename std::set<IClassifyMessageType *>::iterator end = m_classifiers.end();

        bool keepClassifing = true;
        for (; (iter != end) && (keepClassifing) && m_buffer.size()>0 ; ++iter)
        {
          IClassifyMessageType * pClassifier( dynamic_cast<IClassifyMessageType *>( (*iter) ) );
          if( pClassifier )
          {
            SizeType messageFoundSize;
            typename IClassifyMessageType::OffsetType offsetFound;
            typename IClassifyMessageType::IntegrityType integrity;
            integrity = pClassifier->classify(m_buffer, offsetFound, messageFoundSize, typeId, key);

            switch( integrity )
            {
              case IClassifyMessageType::Integrity::CComplete:
              {
                if( offsetFound < m_buffer.size() )
                {
                  SizeType remainingSize = m_buffer.size() - offsetFound;
                  if (messageFoundSize <= remainingSize)
                  {
                    remainingSize = remainingSize - messageFoundSize;
                    messageBuffer.reserve(messageFoundSize);
                    messageBuffer.insert(messageBuffer.begin(),m_buffer.begin()+offsetFound,m_buffer.begin()+(offsetFound+messageFoundSize));
                    if (remainingSize > 0)
                    {
                      BufferType remainingBuffer;
                      remainingBuffer.reserve(remainingSize);
                      remainingBuffer.assign(m_buffer.begin()+(offsetFound+messageFoundSize),m_buffer.end());
                      m_buffer.swap(remainingBuffer);
                    }
                    else
                    {
                      m_buffer.clear();
                    }
                    return true;
                  }
                }
              }
              break;

              case IClassifyMessageType::Integrity::CIncomplete:
              {
                keepClassifing = false;
              }
              break;

              case IClassifyMessageType::Integrity::CCorrupt:
              {
                keepClassifing = false;
                m_buffer.clear();
              }
              break;

              case IClassifyMessageType::Integrity::CUnknown:
              default:
              {
                keepClassifing = true;
              }
              break;
            }
          }
        }
        return false;
      }
      ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    }
  }

#endif
