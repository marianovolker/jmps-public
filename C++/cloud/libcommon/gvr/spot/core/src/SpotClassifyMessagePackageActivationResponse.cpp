/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageActivationResponse.cpp
 * @author mgrieco
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessagePackageActivationResponse.h>
#include <iostream>
#include <iomanip>

using namespace std;

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      ISpotClassifyMessage::IntegrityType SpotClassifyMessagePackageActivationResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponseSetupAppId,
                             ISpotMessage::CDefaulNoEncryptionValue,
                             SpotMessagePackageActivationResponse::CPackageActivationCommandId,
                             SpotMessagePackageActivationResponse::CPackageActivationSubCommandId,
                             SpotMessagePackageActivationResponse::GetTypeId() );

        return classifyHeader(buff, offset, size, typeId, key);
      }

      SpotClassifyMessagePackageActivationResponse::~SpotClassifyMessagePackageActivationResponse (void)
      {
      }
    }
  }
}
