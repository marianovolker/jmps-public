/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IChannelServerListener.h
 * @author mvolker
 * @date Wednesday, April 15, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ICHANNELSERVERLISTENER_H_
# define _ICHANNELSERVERLISTENER_H_

# include <gvr/communication/IChannelServer.h>

# include <Poco/Net/StreamSocket.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /**
       * @brief This is the base class for all channel server listeners.
       *        Provides the entry points a channel server listener should implement.
       * 
       * @see gvr.communication.IChannelServer
       * 
       * @author mvolker
       */
      class IChannelServerListener
      {
        public:

          /**
           * @brief Callback for the retrieved connected channel to which the listener is subscribed.
           *
           * @param the channel link where the event was occur.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void channelStartedEvent (const Poco::Net::StreamSocket&) = 0;

          /**
           * @brief Destructor.
           *
           * @since 0.0.0
           */
          virtual ~IChannelServerListener (void);
      };
    }
  }

#endif
