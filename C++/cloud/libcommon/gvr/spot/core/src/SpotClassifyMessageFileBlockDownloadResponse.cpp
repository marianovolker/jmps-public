/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageFileBlockDownloadResponse.h
 * @author mgrieco
 * @date Friday, March 05, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvr/communication/spot/SpotClassifyMessageFileBlockDownloadResponse.h>
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
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageFileBlockDownloadResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                             ISpotMessage::CDefaulNoEncryptionValue,
                             SpotMessageFileBlockDownloadResponse::CFileBlockDownloadCommandId,
                             SpotMessageFileBlockDownloadResponse::CFileBlockDownloadSubCommandId,
                             SpotMessageFileBlockDownloadResponse::GetTypeId() );

        return classifyHeader(buff, offset, size, typeId, key);
      }

      SpotClassifyMessageFileBlockDownloadResponse::~SpotClassifyMessageFileBlockDownloadResponse (void)
      {
      }
    }
  }
}
