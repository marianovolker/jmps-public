/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageFileDeleteResponse.h
 * @author mgrieco
 * @date Friday, March 05, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvr/communication/spot/SpotClassifyMessageFileDeleteResponse.h>
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
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageFileDeleteResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                             ISpotMessage::CDefaulNoEncryptionValue,
                             SpotMessageFileDeleteResponse::CFileDeleteCommandId,
                             SpotMessageFileDeleteResponse::CFileDeleteSubCommandId,
                             SpotMessageFileDeleteResponse::GetTypeId() );

        return classifyHeader(buff, offset, size, typeId, key);
      }

      SpotClassifyMessageFileDeleteResponse::~SpotClassifyMessageFileDeleteResponse (void)
      {
      }
    }
  }
}
