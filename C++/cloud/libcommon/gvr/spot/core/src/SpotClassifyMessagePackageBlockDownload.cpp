/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageBlockDownload.cpp
 * @author Guillermo Paris
 * @date Monday, March 2, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessagePackageBlockDownload.h>
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


SpotClassifyMessagePackageBlockDownload::~SpotClassifyMessagePackageBlockDownload (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessagePackageBlockDownload::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
{
  SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageRequestSetupAppId,
                       ISpotMessage::CDefaulNoEncryptionValue,
                       SpotMessagePackageBlockDownload::CPackageDownloadCategoryId,
                       SpotMessagePackageBlockDownload::CPackageBlockDownloadCommandId,
                       SpotMessagePackageBlockDownload::GetTypeId() );

  return classifyHeader(buff, offset, size, typeId, key);
}


} // namespace spot

} // namespace communication

} // namespace gvr


