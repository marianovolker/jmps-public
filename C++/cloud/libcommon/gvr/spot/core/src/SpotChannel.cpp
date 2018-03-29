/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotSession.cpp
 * @author mvolker
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <gvr/communication/spot/SpotChannel.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      StatusType const SpotChannel::SpotChannelStatus::CLastStatus  = StreamChannelStatus::CLastStatus + 1;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      ErrorType const SpotChannel::SpotChannelError::CLastError     = StreamChannelError::CLastError + 1;
      /////////////////////////////////////////////////////////////////////////////////////

      SpotChannel::SpotChannel(const std::string& host, SizeType port)
        :StreamChannel(host,port)
      {
        init();
      }

      SpotChannel::SpotChannel(const Poco::Net::StreamSocket& socket)
        :StreamChannel(socket)
      {
        init();
      }

      SpotChannel::~SpotChannel (void)
      {
        stop();
      }

      void SpotChannel::init()
      {
        m_descriptionStatusList [ SpotChannelStatus::CLastStatus  ] = "SpotChannel is at unknown state";

        m_descriptionErrorList  [ SpotChannelError::CLastError    ] = "SpotChannel has an unknown error";

        StreamChannel::setBufferSize(8192);
      }

    }
  }
}
