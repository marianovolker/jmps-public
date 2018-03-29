/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageFileDelete.h
 * @author mgrieco
 * @date Friday, March 05, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvr/communication/spot/SpotClassifyMessageFileDelete.h>
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
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageFileDelete::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageRequestPinpadAppId,
                             ISpotMessage::CDefaulNoEncryptionValue,
                             SpotMessageFileDelete::CFileDeleteCommandId,
                             SpotMessageFileDelete::CFileDeleteCommandId,
                             SpotMessageFileDelete::GetTypeId() );

        return classifyHeader(buff, offset, size, typeId, key);
      }

      SpotClassifyMessageFileDelete::~SpotClassifyMessageFileDelete (void)
      {
      }
    }
  }
}
