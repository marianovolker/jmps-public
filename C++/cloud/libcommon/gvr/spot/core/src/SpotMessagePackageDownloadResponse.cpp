/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageDownloadResponse.cpp
 * @author Guillermo Paris
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstring>
#include <iostream>
#include <gvr/communication/spot/Conversion.h>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessagePackageDownloadResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{

SpotMessagePackageDownloadResponse::SpotMessagePackageDownloadResponse (void)
:  AbsSpotMessageWithAppCatCmdID (true, CMessageResponseSetupAppId, CPackageDownloadCategoryId, CPackageDownloadCommandId),
   m_uInitialOffset(0)
{
}

SpotMessagePackageDownloadResponse::~SpotMessagePackageDownloadResponse (void)
{
}


bool SpotMessagePackageDownloadResponse::canDownloadBegin()
{
  return  ( getAckCode() == CPackageDownloadAccepted );
}

ISpotMessage::SizeType  SpotMessagePackageDownloadResponse::getInitialOffset (void)
{
  return m_uInitialOffset;
}

// For testing only !!  This is a reply message.
ISpotMessage::BufferType  SpotMessagePackageDownloadResponse::serialize (void) const
{
  // First of all: serialize the header.
  BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  unsigned char abInitialOffset[4];

  // Sets the initial offset of the response message.
  Conversion::uint32ToBigEndian(m_uInitialOffset, abInitialOffset);
  retBuffer.push_back( abInitialOffset[0] );
  retBuffer.push_back( abInitialOffset[1] );
  retBuffer.push_back( abInitialOffset[2] );
  retBuffer.push_back( abInitialOffset[3] );


  std::cout << "SpotMessagePackageDownloadResponse::serialize() - Length: " << size() << " bytes.\n";
  char acBuffer[3 * CMessageSize + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(acBuffer, &retBuffer.front(), size()) << std::endl << std::endl;

  return retBuffer; // TODO: Fixing ugly return vectors by value.
}

bool SpotMessagePackageDownloadResponse::parse (BufferType const & buff)
{
  if(buff.size() < CMessageSize)  return false; // message too short

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff); // Length and AckCode are get by this method.

  if( !blRet ) return false; // malformed header

  m_uInitialOffset =  ( buff[CMessageFileOffsetIndex]  << 24 ) + (buff[CMessageFileOffsetIndex+1] << 16);
  m_uInitialOffset += ( buff[CMessageFileOffsetIndex+2] << 8 ) +  buff[CMessageFileOffsetIndex+3];

  std::cout << std::dec << "SpotMessagePackageDownloadResponse::parse() - Length: " << size() << " bytes. ACK = "
  << int(getAckCode()) << " . Initial Offset: " << m_uInitialOffset << std::endl;

  /* TODO : delete
  char acBuffer[3 * CMessageSize + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(acBuffer, &buff.front(), size())
            << std::endl << std::endl;
*/
  return true;
}


} // namespace gvr

} // namespace communication

} // namespace spot


