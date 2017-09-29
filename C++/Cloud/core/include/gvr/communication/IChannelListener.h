/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IChannelListener.h
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ICHANNELLISTENER_H_
# define _ICHANNELLISTENER_H_

# include <gvr/communication/IChannel.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /**
       * @brief This is the base class for all channel listeners. Provides the entry points a channel listener should implement.
       * 
       * @see gvr.communication.IChannel
       * 
       * @author pcantarini
       */
      class IChannelListener
      {
      public:
        typedef IChannel::BufferType BufferType; /// @brief The buffer container type for the serialized representation of the message.
        typedef IChannel::SizeType   SizeType;   /// @brief The size type for any size measurable field.

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
         * @brief Destructor.
         * 
         * @since 0.0.0
         */
        virtual ~IChannelListener (void) = 0;
      };
    }
  }

#endif
