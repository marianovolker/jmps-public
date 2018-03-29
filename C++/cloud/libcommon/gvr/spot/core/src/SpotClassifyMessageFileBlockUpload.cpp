/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageFileBlockUpload.h
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvr/communication/spot/SpotClassifyMessageFileBlockUpload.h>
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
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageFileBlockUpload::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageRequestPinpadAppId,
                             ISpotMessage::CDefaulNoEncryptionValue,
                             SpotMessageFileBlockUpload::CFileBlockUploadCommandId,
                             SpotMessageFileBlockUpload::CFileBlockUploadCommandId,
                             SpotMessageFileBlockUpload::GetTypeId() );

        return classifyHeader(buff, offset, size, typeId, key);
      }

      SpotClassifyMessageFileBlockUpload::~SpotClassifyMessageFileBlockUpload (void)
      {
      }
    }
  }
}
