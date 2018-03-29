/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageDownload.cpp
 * @author Guillermo Paris
 * @date Monday, March 2, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessagePackageDownload.h>
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


SpotClassifyMessagePackageDownload::~SpotClassifyMessagePackageDownload (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessagePackageDownload::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
{
  SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageRequestSetupAppId,
                       ISpotMessage::CDefaulNoEncryptionValue,
                       SpotMessagePackageDownload::CPackageDownloadCommandId,
                       SpotMessagePackageDownload::CPackageDownloadSubCommandId,
                       SpotMessagePackageDownload::GetTypeId() );

  return classifyHeader(buff, offset, size, typeId, key);
}


} // namespace spot

} // namespace communication

} // namespace gvr


