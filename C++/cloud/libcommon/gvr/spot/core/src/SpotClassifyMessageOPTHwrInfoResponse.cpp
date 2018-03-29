/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageOPTHwrInfoResponse.cpp
 * @author Guillermo Paris
 * @date Thursday, May 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageOPTHwrInfoResponse.h>
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


SpotClassifyMessageOPTHwrInfoResponse::~SpotClassifyMessageOPTHwrInfoResponse (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessageOPTHwrInfoResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
{
  SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponseOPTAppId,
                       ISpotMessage::CDefaulNoEncryptionValue,
                       SpotMessageOPTHwrInfoResponse::COPTHwrInfoCategoryId,
                       SpotMessageOPTHwrInfoResponse::COPTHwrInfoCommandId,
                       SpotMessageOPTHwrInfoResponse::GetTypeId() );

  return classifyHeader(buff, offset, size, typeId, key);
}


} // namespace spot

} // namespace communication

} // namespace gvr

