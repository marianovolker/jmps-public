/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccChannel.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/UccChannel.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      // Initial values for states
      /////////////////////////////////////////////////////////////////////////////////////
      StatusType const UccChannel::UccChannelStatus::CLastStatus  = StreamChannelStatus::CLastStatus + 1;
      /////////////////////////////////////////////////////////////////////////////////////

      // Initial values for errors
      /////////////////////////////////////////////////////////////////////////////////////
      ErrorType const UccChannel::UccChannelError::CLastError     = StreamChannelError::CLastError + 1;
      /////////////////////////////////////////////////////////////////////////////////////

      UccChannel::UccChannel(const std::string& host, SizeType port)
        :StreamChannel(host,port)
      {
        init();
      }

      UccChannel::UccChannel(const Poco::Net::StreamSocket& socket)
        :StreamChannel(socket)
      {
        init();
      }

      UccChannel::~UccChannel (void)
      {
      }

      void UccChannel::init()
      {
        m_descriptionStatusList [ UccChannelStatus::CLastStatus  ] = "UccChannel is at unknown state";

        m_descriptionErrorList  [ UccChannelError::CLastError    ] = "UccChannel has an unknown error";

        StreamChannel::setBufferSize(8192);
      }

    }
  }
}
