/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ICrlMessage.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/base/ICrlMessage.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      unsigned char const ICrlMessage::CInteralFlagInternalMessage = 0x00;

      ICrlMessage::~ICrlMessage (void)
      {
      }
    }
  }
}
