/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageFileBlockUploadResponse.h
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvr/communication/spot/SpotClassifyMessageFileBlockUploadResponse.h>
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
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageFileBlockUploadResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                             ISpotMessage::CDefaulNoEncryptionValue,
                             SpotMessageFileBlockUploadResponse::CFileBlockUploadCommandId,
                             SpotMessageFileBlockUploadResponse::CFileBlockUploadSubCommandId,
                             SpotMessageFileBlockUploadResponse::GetTypeId() );

        return classifyHeader(buff, offset, size, typeId, key);
      }

      SpotClassifyMessageFileBlockUploadResponse::~SpotClassifyMessageFileBlockUploadResponse (void)
      {
      }
    }
  }
}
