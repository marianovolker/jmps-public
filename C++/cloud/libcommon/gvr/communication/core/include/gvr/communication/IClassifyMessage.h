/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IClassifyMessage.h
 * @author mgrieco
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ICLASSIFYMESSAGE_H_
# define _ICLASSIFYMESSAGE_H_

#include <gvr/communication/IMessage.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /**
       * @brief This is the base class for all message classifiers.
       * A classifier should perform a minimal parse for the given buffer and return the necessary identification information on the message to be fully parsed later.
       * 
       * @see gvr.communication.ISession
       * 
       * @since 0.0.0, 2014-12-16
       * @version 0.0.0, 2014-12-16
       * @author pcantarini
       */
      template <class MsgSpc>
      class IClassifyMessage
      {
      public:
        typedef MsgSpc                    Spec;          /// @brief The specification to which this IClassifyMessage interface belongs.
        typedef typename Spec::KeyType    KeyType;       /// @brief The the identification key type for the message which pairs request to responses (A.K.A. in some protocols as Sequence Identification Field).
        typedef typename Spec::BufferType BufferType;    /// @brief The buffer container type for the serialized representation of the message.
        typedef typename Spec::SizeType   SizeType;      /// @brief The size type for any size measurable field.
        typedef typename Spec::TypeId     TypeId;        /// @brief The Message class type identification. A unique identifier value which represents a message class.
        typedef typename Spec::SizeType   OffsetType;    /// @brief The offset type for any size measurable field.
        typedef typename Spec::SizeType   IntegrityType; /// @brief The integrity enumeration representation for the classifier possible return status on the pre-parsed buffer.
        typedef IMessage<Spec>            IMessageType;         /// @brief The message type which this ISession can handle.

        /**
         * @brief This is the integrity enumeration representation for the classifier possible return status on the pre-parsed buffer.
         * 
         * @see gvr.communication.IClassifyMessage.IntegrityType
         * 
         * @since 0.0.0, 2014-12-16
         * @version 0.0.0, 2014-12-16
         * @author pcantarini
         */
        struct Integrity
        {
          static IntegrityType const CComplete   = 0; ///< The given buffer contains a known message by the classifier and it is completed.
          static IntegrityType const CIncomplete = 1; ///< The given buffer contains a known message by the classifier but it is no completed yet.
          static IntegrityType const CUnknown    = 2; ///< The given buffer contains an unknown but valid message recognized by the classifier.
          static IntegrityType const CCorrupt    = 3; ///< The given buffer is corrupted and should be purged.
        };

        /**
         * @brief Briefly parses the given buffer and tries to classify the underlying message in it.
         * If classification is successful, return value is Integrity::CComplete, all necessary information regarding message extraction from the provided buffer and its identification should be returned.
         * 
         * @param buffer is the buffer to be analyzed.
         * @param offset is the buffer offset at which the recognized message starts.
         * @param size is the sub-buffer size from offset at which the recognized message ends.
         * @param type is the recognized message class type.
         * @param key is the recognized message identification key which matches a response to a request.
         * 
         * @retrun The integrity state of the analyzed buffer and the underlying message in it.
         * 
         * @exception Throws no exception.
         * 
         * @see gvr.communication.IClassifyMessage.IntegrityType
         * @see gvr.communication.IClassifyMessage.BufferType
         * @see gvr.communication.IClassifyMessage.OffsetType
         * @see gvr.communication.IClassifyMessage.SizeType
         * @see gvr.communication.IClassifyMessage.TypeId
         * @see gvr.communication.IClassifyMessage.KeyType
         * 
         * @since 0.0.0
         */
        virtual IntegrityType classify (BufferType const &, OffsetType &, SizeType &, TypeId &, KeyType &) = 0;

        /**
         * @brief Destructor.
         * 
         * @since 0.0.0
         */
        virtual ~IClassifyMessage (void) { }
      };
    }
  }

#endif
