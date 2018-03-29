/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageInfo.cpp
 * @author Guillermo Paris
 * @date Monday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */



#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessagePackageInfo.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


SpotMessagePackageInfo::SpotMessagePackageInfo (void)
:  AbsSpotMessageWithAppCatCmdID (false, CMessageRequestSetupAppId, CPackageInfoCommandId, CPackageInfoSubCommandId),
   m_bPackageSelector(CActivePackages)
{
  setSerializedSize( CMessageSize ); // Always 7 bytes long
}

SpotMessagePackageInfo::~SpotMessagePackageInfo (void)
{
}

void SpotMessagePackageInfo::selectPackageSet(unsigned char bSelectionByte)
{
  if( bSelectionByte > CNewPackages ) return; // unknown value discarded

  m_bPackageSelector = bSelectionByte;
}

ISpotMessage::BufferType SpotMessagePackageInfo::serialize (void) const
{
  // Serialize the header.
  ISpotMessage::BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  // Serialize the rest of the message.

  // Sets the number of items in the request message.
  retBuffer.push_back( m_bPackageSelector );

  std::cout << "SpotMessagePackageInfo::serialize() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return retBuffer; // TODO: Fix ugly return vectors by value.
}

bool SpotMessagePackageInfo::parse (BufferType const & buff)
{
  if(buff.size() < CMessageSize)  return false; // message too short

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff);

  if( !blRet ) return false; // malformed header

  if( buff[CSetSelectionByteOffset] == CNewPackages )
  {
    m_bPackageSelector = CNewPackages;
  }
  else
  {
    m_bPackageSelector = CActivePackages;
  }

  std::cout << "SpotMessagePackageInfo::parse() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return true;
}


} // namespace spot

} // namespace communication

} // namespace gvr


