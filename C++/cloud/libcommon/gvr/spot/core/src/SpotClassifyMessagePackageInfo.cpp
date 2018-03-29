/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePackageInfo.cpp
 * @author Guillermo Paris
 * @date Tuesday, February 24, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessagePackageInfo.h>
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


SpotClassifyMessagePackageInfo::~SpotClassifyMessagePackageInfo (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessagePackageInfo::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
{
  SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageRequestSetupAppId,
                       ISpotMessage::CDefaulNoEncryptionValue,
                       SpotMessagePackageInfo::CPackageInfoCommandId,
                       SpotMessagePackageInfo::CPackageInfoSubCommandId,
                       SpotMessagePackageInfo::GetTypeId() );

  return classifyHeader(buff, offset, size, typeId, key);
}


} // namespace spot

} // namespace communication

} // namespace gvr


