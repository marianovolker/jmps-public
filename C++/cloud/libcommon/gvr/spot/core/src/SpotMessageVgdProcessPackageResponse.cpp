/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageVgdProcessPackageResponse.cpp
 * @author Guillermo Paris
 * @date Monday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessageVgdProcessPackageResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


SpotMessageVgdProcessPackageResponse::SpotMessageVgdProcessPackageResponse (void)
:  AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CVgdProcessPackageCategoryId, CVgdProcessPackageCommandId)
{
  // This command has always constant length.
  setSerializedSize( CMessageSize );
}

SpotMessageVgdProcessPackageResponse::~SpotMessageVgdProcessPackageResponse (void)
{
}


bool SpotMessageVgdProcessPackageResponse::hasBeenProcessedOK() const
{
  return ( CVGDPkgActivationOK == getAckCode() );
}

ISpotMessage::BufferType  SpotMessageVgdProcessPackageResponse::serialize (void) const
{
  // In this message the work is just serialize the header.
  return AbsSpotMessageWithAppCatCmdID::serialize() ;
}

bool SpotMessageVgdProcessPackageResponse::parse (BufferType const & buff)
{
  if(buff.size() < CMessageSize)  return false; // message too short

  // In this message the work is just parse the header + ACK code.
  return AbsSpotMessageWithAppCatCmdID::parse(buff); // Length and AckCode are get by this method.
}


} // namespace gvr

} // namespace communication

} // namespace spot


