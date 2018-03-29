/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlChannel.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/CrlChannel.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      StatusType const CrlChannel::CrlChannelStatus::CLastStatus  = StreamChannelStatus::CLastStatus + 1;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      ErrorType const CrlChannel::CrlChannelError::CLastError     = StreamChannelError::CLastError + 1;
      /////////////////////////////////////////////////////////////////////////////////////

      CrlChannel::CrlChannel(const std::string& host, SizeType port)
        :StreamChannel(host,port)
      {
        init();
      }

      CrlChannel::CrlChannel(const Poco::Net::StreamSocket& socket)
        :StreamChannel(socket)
      {
        init();
      }

      CrlChannel::~CrlChannel (void)
      {
      }

      void CrlChannel::init()
      {
        m_descriptionStatusList [ CrlChannelStatus::CLastStatus  ] = "CrlChannel is at unknown state";

        m_descriptionErrorList  [ CrlChannelError::CLastError    ] = "CrlChannel has an unknown error";

        StreamChannel::setBufferSize(8192);
      }

    }
  }
}
