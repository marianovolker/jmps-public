/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageFileDownload.h
 * @author mgrieco
 * @date Friday, March 05, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvr/communication/spot/SpotClassifyMessageFileDownload.h>
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
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageFileDownload::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageRequestPinpadAppId,
                             ISpotMessage::CDefaulNoEncryptionValue,
                             SpotMessageFileDownload::CFileDownloadCommandId,
                             SpotMessageFileDownload::CFileDownloadCommandId,
                             SpotMessageFileDownload::GetTypeId() );

        return classifyHeader(buff, offset, size, typeId, key);
      }

      SpotClassifyMessageFileDownload::~SpotClassifyMessageFileDownload (void)
      {
      }
    }
  }
}
