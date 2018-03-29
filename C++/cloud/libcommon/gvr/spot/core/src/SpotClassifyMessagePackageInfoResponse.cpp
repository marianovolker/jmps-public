/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageInfoResponse.cpp
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessagePackageInfoResponse.h>
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


SpotClassifyMessagePackageInfoResponse::~SpotClassifyMessagePackageInfoResponse (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessagePackageInfoResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
{
  SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponseSetupAppId,
                       ISpotMessage::CDefaulNoEncryptionValue,
                       SpotMessagePackageInfoResponse::CPackageInfoCategoryId,
                       SpotMessagePackageInfoResponse::CPackageInfoCommandId,
                       SpotMessagePackageInfoResponse::GetTypeId() );

  return classifyHeader(buff, offset, size, typeId, key);
}


} // namespace spot

} // namespace communication

} // namespace gvr

