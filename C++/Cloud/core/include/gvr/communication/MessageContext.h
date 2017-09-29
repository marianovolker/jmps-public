/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file MessageContext.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _MESSAGECONTEXT_H_
# define _MESSAGECONTEXT_H_

# include <Poco/Event.h>
# include <Poco/Timestamp.h>
#include <iostream>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /**
       * @brief This class is the base class for all messages context that share the same message specification.
       * 
       * @since 0.0.0, 2014-12-16
       * @version 0.0.0, 2014-12-16
       * @author mvolker
       */
      template <class MsgSpc>
      class MessageContext
      {
        public:
          typedef MsgSpc                                      Spec;
          typedef ISession<Spec>                              ISessionType;
          typedef typename ISessionType::IMessageType         IMessageType;
          typedef typename ISessionType::BufferType           BufferType;
          typedef typename ISessionType::TimeMillisecondsType TimeMillisecondsType;

          enum eMessageContextStates
          {
              eMessageContextCreated   = 0,
              eMessageContextReady        ,
              eMessageContextWaiting      ,
              eMessageContextError        ,
              eMessageContextTimeout      ,
              eMessageContextCompleted
          };

          /**
           * @brief Constructor.
           *
           * @since 0.0.0
           */
          MessageContext (IMessageType const & messageReq, IMessageType & messageRes, TimeMillisecondsType timeout)
            : m_state(eMessageContextCreated), m_messageReq(messageReq), m_messageRes(messageRes),
             m_wait(true), m_initialTime(), m_timeout(timeout) { }

          /**
           * @brief Destructor.
           *
           * @since 0.0.0
           */
          virtual ~MessageContext (void) { }

          const Poco::Timestamp& getInitialTime() const
          {
            return m_initialTime;
          }

          void wakeUp()
          {
            m_wait.set();
          }

          void wait()
          {
            m_wait.wait();
          }

          eMessageContextStates getState() const
          {
            return m_state;
          }

          void setState(eMessageContextStates state)
          {
            m_state = state;
          }

          const IMessageType& getMessageReq() const
          {
            return m_messageReq;
          }

          const IMessageType& getMessageRes() const
          {
            return m_messageRes;
          }

          void setMessageRes(BufferType const & buffer)
          {
            if(!m_messageRes.parse(buffer))
            {
              std::cout << "Message Context - Response parse error" << std::endl;
            }
          }

          bool expired( void ) const
          {
            // Here we need to convert from milliseconds to seconds resolutions because in some platforms
            // like M3 the base 64 is not supported...
            return ( (Poco::Timestamp().epochTime() - m_initialTime.epochTime()) > (m_timeout/1000) );
          }

        private:
          eMessageContextStates m_state;
          IMessageType const & m_messageReq;
          IMessageType & m_messageRes;
          Poco::Event m_wait;
          Poco::Timestamp m_initialTime;
          TimeMillisecondsType m_timeout;
      };
    }
  }

#endif
