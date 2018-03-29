/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageHardwareInfoResponse.cpp
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageHardwareInfoResponse.h>
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


SpotClassifyMessageHardwareInfoResponse::~SpotClassifyMessageHardwareInfoResponse (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessageHardwareInfoResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
{
  SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                       ISpotMessage::CDefaulNoEncryptionValue,
                       SpotMessageHardwareInfoResponse::CHardwareInfoCategoryId,
                       SpotMessageHardwareInfoResponse::CHardwareInfoCommandId,
                       SpotMessageHardwareInfoResponse::GetTypeId() );

  return classifyHeader(buff, offset, size, typeId, key);
}


} // namespace spot

} // namespace communication

} // namespace gvr

