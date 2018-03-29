/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageDebianInfo.cpp
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageDebianInfo.h>
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
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageDebianInfo::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageRequestSetupAppId,
                             ISpotMessage::CDefaulNoEncryptionValue,
                             SpotMessageDebianInfo::CDebianInfoCommandId,
                             SpotMessageDebianInfo::CDebianInfoSubCommandId,
                             SpotMessageDebianInfo::GetTypeId() );

        return classifyHeader(buff, offset, size, typeId, key);
      }

      SpotClassifyMessageDebianInfo::~SpotClassifyMessageDebianInfo (void)
      {
      }
    }
  }
}
