/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageVgdPackageActivationResponse.cpp
 * @author mgrieco
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageVgdPackageActivationResponse.h>
#include <iostream>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageVgdPackageActivationResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                             ISpotMessage::CDefaulNoEncryptionValue,
                             SpotMessagePackageActivationResponse::CVgdPackageActivationCommandId,
                             SpotMessagePackageActivationResponse::CPackageActivationSubCommandId,
                             SpotMessagePackageActivationResponse::GetTypeId() );

        return classifyHeader(buff, offset, size, typeId, key);
      }

      SpotClassifyMessageVgdPackageActivationResponse::~SpotClassifyMessageVgdPackageActivationResponse (void)
      {
      }
    }
  }
}
