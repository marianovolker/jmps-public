/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UnsignedBinaryFile.cpp
 * @author Guillermo Paris
 * @date Wednesday, March 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <gvr/communication/spot/UnsignedBinaryFile.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


UnsignedBinaryFile::UnsignedBinaryFile (const std::string & sFilename, std::size_t uInitialOffset /* = 0 */)
  : AbsSpotPackage(sFilename, uInitialOffset)
{
}

UnsignedBinaryFile::~UnsignedBinaryFile (void)
{
}



/*===================== VIRTUAL METHODS ===================*/


std::size_t  UnsignedBinaryFile::setCertificateSize (void)
{
  return 0; // no certificate in this kind of files.
}

std::size_t  UnsignedBinaryFile::setSignatureSize (void)
{
  return 0; // no signature in this kind of files.
}



} // namespace spot

} // namespace communication

} // namespace gvr



