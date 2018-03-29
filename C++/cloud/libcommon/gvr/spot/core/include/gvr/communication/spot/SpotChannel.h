/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotChannel.h
 * @author mvolker
 * @date Tuesday, Enero 06, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTCHANNEL_H_
# define _SPOTCHANNEL_H_


# include <gvr/communication/StreamChannel.h>


  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        /**
         * @brief This is the class for SPOT channel.
         * 
         * @see gvr.communication.IChannel
         * 
         * @since 0.0.0, 2014-12-16
         * @version 0.0.0, 2014-12-16
         * @author mvolker
         */
        class SpotChannel: public gvr::communication::StreamChannel
        {
          public:

            /**
             * @brief This is the status enumeration representation for the channel possible states.
             *        If you need to extend these status you could inherit from this SpotChannelStatus
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct SpotChannelStatus: public StreamChannelStatus
            {
              static StatusType const CLastStatus;
            };

            /**
             * @brief This is the error enumeration representation for the channel possible errors.
             *        If you need to extend these errors you could inherit from this SpotChannelError
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct SpotChannelError: public StreamChannelError
            {
              static ErrorType const CLastError;
            };

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            SpotChannel(const std::string& host, SizeType port);

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            SpotChannel(const Poco::Net::StreamSocket&);

            /**
             * @brief Destructor.
             *
             * @since 0.0.0
             */
            virtual ~SpotChannel (void);


          protected:

            /**
             * @brief Init status.
             *
             * @since 0.0.0
             */
            virtual void init (void);
        };
      }
    }
  }

#endif
