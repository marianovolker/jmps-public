/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlChannel.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLCHANNEL_H_
# define _CRLCHANNEL_H_

# include <gvr/communication/StreamChannel.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Crl Domain
      namespace crl
      {
        /**
         * @brief This is the class for crl channel.
         * 
         * @see gvr.communication.IChannel
         * 
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class CrlChannel: public gvr::communication::StreamChannel
        {
          public:

            /**
             * @brief This is the status enumeration representation for the channel possible states.
             *        If you need to extend these status you could inherit from this CrlChannelStatus
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct CrlChannelStatus: public StreamChannelStatus
            {
              static StatusType const CLastStatus;
            };

            /**
             * @brief This is the error enumeration representation for the channel possible errors.
             *        If you need to extend these errors you could inherit from this CrlChannelError
             *        but you remember that the maximum value of this is limited to 256 by the data
             *        size.
             *
             */
            struct CrlChannelError: public StreamChannelError
            {
              static ErrorType const CLastError;
            };

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            CrlChannel(const std::string& host, SizeType port);

            /**
             * @brief Constructor.
             *
             * @since 0.0.0
             */
            CrlChannel(const Poco::Net::StreamSocket&);

            /**
             * @brief Destructor.
             *
             * @since 0.0.0
             */
            virtual ~CrlChannel (void);


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
