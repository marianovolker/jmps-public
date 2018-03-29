/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageHardwareInfo.cpp
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageHardwareInfo.h>
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


SpotClassifyMessageHardwareInfo::~SpotClassifyMessageHardwareInfo (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessageHardwareInfo::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
{
  SetHeaderParameters( ISpotMessageWithAppCatCmdID::CMessageRequestPinpadAppId,
                       ISpotMessage::CDefaulNoEncryptionValue,
                       SpotMessageHardwareInfo::CHardwareInfoCommandId,
                       SpotMessageHardwareInfo::CHardwareInfoSubCommandId,
                       SpotMessageHardwareInfo::GetTypeId() );

  return classifyHeader(buff, offset, size, typeId, key);
}


} // namespace spot

} // namespace communication

} // namespace gvr


