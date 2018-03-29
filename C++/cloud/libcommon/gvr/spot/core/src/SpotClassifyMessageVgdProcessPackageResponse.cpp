/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageVgdProcessPackageResponse.cpp
 * @author Guillermo Paris
 * @date Tuesday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageVgdProcessPackageResponse.h>
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


SpotClassifyMessageVgdProcessPackageResponse::~SpotClassifyMessageVgdProcessPackageResponse (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessageVgdProcessPackageResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
{
  SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                       ISpotMessage::CDefaulNoEncryptionValue,
                       SpotMessageVgdProcessPackageResponse::CVgdProcessPackageCategoryId,
                       SpotMessageVgdProcessPackageResponse::CVgdProcessPackageCommandId,
                       SpotMessageVgdProcessPackageResponse::GetTypeId() );

  return classifyHeader(buff, offset, size, typeId, key);
}


} // namespace spot

} // namespace communication

} // namespace gvr

