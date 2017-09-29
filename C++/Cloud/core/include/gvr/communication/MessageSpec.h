/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file MessageSpec.h
 * @author mgrieco
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _MESSAGESPEC_H_
# define _MESSAGESPEC_H_

# include <gvr/communication/IChannel.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /**
     * @brief This is the base class for all message specification specializations.
     *        Provides the means to identify a message class type and specialize the
     *        message key identification field to match responses to requests.
     * 
     * @see gvr.communication.IChannel
     * 
     * @since 0.0.0, 2014-12-16
     * @version 0.0.0, 2014-12-16
     * @author pcantarini
     */
    template <class MsgKy>
    class MessageSpec
    {
      public:
        typedef MsgKy                       KeyType;    /// @brief The the identification key type for the message which pairs request to responses (A.K.A. in some protocols as Sequence Identification Field).
        typedef IChannel::BufferType        BufferType; /// @brief The buffer container type for the serialized representation of the message.
        typedef std::size_t                 SizeType;   /// @brief The size type for any size measurable field.
        typedef SizeType                    TypeId;     /// @brief The Message class type identification. A unique identifier value which represents a message class.

        /**
         * @brief Destructor.
         *
         * @since 0.0.0
         */
        virtual ~MessageSpec(void) {};
    };
  }
}

#endif

