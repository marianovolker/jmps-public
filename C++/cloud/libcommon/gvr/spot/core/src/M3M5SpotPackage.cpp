/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file M3M5SpotPackage.cpp
 * @author Guillermo Paris
 * @date Tuesday, March 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvr/communication/spot/M3M5SpotPackage.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


M3M5SpotPackage::M3M5SpotPackage (const std::string & sFilename, std::size_t uInitialOffset /* = 0 */)
  : AbsSpotPackage(sFilename, uInitialOffset)
{
}

M3M5SpotPackage::~M3M5SpotPackage (void)
{
}



/*===================== VIRTUAL METHODS ===================*/


std::size_t  M3M5SpotPackage::setCertificateSize (void)
{
  return CCertificateSize;
}

std::size_t  M3M5SpotPackage::setSignatureSize (void)
{
  return 0; // no signature in M3 or M5 packages
}



} // namespace spot

} // namespace communication

} // namespace gvr


