/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccChannel.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCCHANNEL_H_
# define _UCCCHANNEL_H_

# include <gvr/communication/StreamChannel.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Ucc Domain
      namespace ucc
      {
        /**
         * @brief This is the class for Ucc channel.
         * 
         * @see gvr.communication.IChannel
         * 
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class UccChannel: public gvr::communication::StreamChannel
        {
          public:

            /**
             * @brief This is the status enumeration representation for the channel possible states.
             *        If you need to extend these status you could inherit from this UccChannelStatus
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct UccChannelStatus: public StreamChannelStatus
            {
              static StatusType const CLastStatus;
            };

            /**
             * @brief This is the error enumeration representation for the channel possible errors.
             *        If you need to extend these errors you could inherit from this UccChannelError
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct UccChannelError: public StreamChannelError
            {
              static ErrorType const CLastError;
            };

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            UccChannel(const std::string& host, SizeType port);

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            UccChannel(const Poco::Net::StreamSocket&);

            /**
             * @brief Destructor.
             *
             * @since 0.0.0
             */
            virtual ~UccChannel (void);


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
