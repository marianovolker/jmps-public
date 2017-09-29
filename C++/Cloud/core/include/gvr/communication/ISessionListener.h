/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ISessionListener.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ISESSIONLISTENER_H_
# define _ISESSIONLISTENER_H_

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
      class ISession;

      /**
       * @brief This is the base class for all session listeners. Provides the entry points a session listener should implement.
       * 
       * @see gvr.communication.ISession
       * @see gvr.communication.MessageSpec
       * 
       */
      template <class MsgSpc>
      class ISessionListener
      {
      public:
        typedef MsgSpc                       Spec;         /// @brief The specification to which this ISessionListener interface belongs.
        typedef typename Spec::KeyType       KeyType;      /// @brief The the identification key type for the message which pairs request to responses (A.K.A. in some protocols as Sequence Identification Field).
        typedef typename Spec::BufferType    BufferType;   /// @brief The buffer container type for the serialized representation of the message.
        typedef typename Spec::SizeType      SizeType;     /// @brief The size type for any size measurable field.
        typedef typename Spec::TypeId        TypeId;       /// @brief The Message class type identification. A unique identifier value which represents a message class.
        typedef ISession<Spec>               ISessionType;
        typedef unsigned char                StatusType;
        typedef unsigned char                ErrorType;


        /**
         * @brief Callback for the retrieved message by the session to which the listener is subscribed.
         * 
         * @param buffer is the message buffer that occur.
         * @param typeId is the received message class type identification of the message received to be parsed.
         * @param key is the received message key identification, which in many protocols is the sequence number.
         * 
         * @exception Throws no exception.
         * 
         * @see gvr.communication.ISessionListener.BufferType
         * @see gvr.communication.ISessionListener.TypeId
         * @see gvr.communication.ISessionListener.KeyType
         * 
         * @since 0.0.0
         */
        virtual void sessionEvent (BufferType const &, TypeId const &, KeyType const &) = 0;

        /**
         * @brief Callback for the triggered status change by the session to which the listener is subscribed.
         * 
         * @param status is the current session status. Session state changes and errors are reported through this callback.
         * 
         * @exception Throws no exception.
         * 
         * @see gvr.communication.ISessionListener.StatusType
         * 
         * @since 0.0.0
         */
        virtual void sessionEvent (StatusType) = 0;

        /**
         * @brief Destructor.
         * 
         * @since 0.0.0
         */
        virtual ~ISessionListener (void) {}
      };
    }
  }

#endif
