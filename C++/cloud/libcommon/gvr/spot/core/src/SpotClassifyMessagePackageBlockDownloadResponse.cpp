/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageBlockDownloadResponse.cpp
 * @author Guillermo Paris
 * @date Monday, March 2, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessagePackageBlockDownloadResponse.h>
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


SpotClassifyMessagePackageBlockDownloadResponse::~SpotClassifyMessagePackageBlockDownloadResponse (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessagePackageBlockDownloadResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
{
  SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponseSetupAppId,
                       ISpotMessage::CDefaulNoEncryptionValue,
                       SpotMessagePackageBlockDownloadResponse::CPackageBlockDownloadCategoryId,
                       SpotMessagePackageBlockDownloadResponse::CPackageBlockDownloadCommandId,
                       SpotMessagePackageBlockDownloadResponse::GetTypeId() );

  return classifyHeader(buff, offset, size, typeId, key);
}


} // namespace spot

} // namespace communication

} // namespace gvr

