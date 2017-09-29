/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IMessage.h
 * @author mgrieco
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _IMESSAGE_H_
# define _IMESSAGE_H_

# include <vector>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /**
       * @brief This class is the base class for all messages that share the same message specification.
       * 
       * @see gvr.communication.MessageSpec
       * 
       * @since 0.0.0, 2014-12-16
       * @version 0.0.0, 2014-12-16
       * @author pcantarini
       */
      template <class MsgSpc>
      class IMessage
      {
        public:
          typedef MsgSpc                    Spec;       /// @brief The specification to which this IMessage interface belongs.
          typedef typename Spec::KeyType    KeyType;    /// @brief The the identification key type for the message which pairs request to responses (A.K.A. in some protocols as Sequence Identification Field).
          typedef typename Spec::BufferType BufferType; /// @brief The buffer container type for the serialized representation of the message.
          typedef typename Spec::SizeType   SizeType;   /// @brief The size type for any size measurable field.
          typedef typename Spec::TypeId     TypeId;     /// @brief The Message class type identification. A unique identifier value which represents a message class.

          /**
           * @brief Retrieves the current instance Message class type identification.
           *
           * @return The current Message class type identification.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.TypeId
           *
           * @since 0.0.0
           */
          virtual TypeId getTypeId (void) const = 0;

          /**
           * @brief Retrieves the current Message key identification, which in many protocols is the sequence number.
           *
           * @return The current Message key identification.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.KeyType
           *
           * @since 0.0.0
           */
          virtual KeyType getKey (void) const  = 0;

          /**
           * @brief Retrieves the current size which could be obtained if Message would be serialized.
           *
           * @return The current Message size if serialized.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.SizeType
           *
           * @since 0.0.0
           */
          virtual SizeType size (void) const = 0;

          /**
           * @brief Retrieves the serialized Message into a new buffer.
           *
           * @return The serialized Message to a buffer.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.BufferType
           *
           * @since 0.0.0
           */
          virtual BufferType serialize (void) const  = 0;

          /**
           * @brief Parse the given buffer and fill the Message fields with the obtained data.
           *
           * @param The given buffer to parse.
           *
           * @return True if parse process is successful. False otherwise.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.BufferType
           *
           * @since 0.0.0
           */
          virtual bool parse (BufferType const &) = 0;

          /**
           * @brief Destructor.
           *
           * @since 0.0.0
           */
          virtual ~IMessage (void){};

        protected:

          template <class T>
          static TypeId _getTypeId( )
          {
              static const T * instance = 0;
              return reinterpret_cast<TypeId>(&instance);
          }

      };
    }
  }

#endif
