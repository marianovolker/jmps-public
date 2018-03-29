/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageVgdProcessPackage.cpp
 * @author Guillermo Paris
 * @date Monday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessageVgdProcessPackage.h>


/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


SpotMessageVgdProcessPackage::SpotMessageVgdProcessPackage (void)
:  AbsSpotMessageWithAppCatCmdID (false, CMessageRequestPinpadAppId, CVgdProcessPackageCommandId, CVgdProcessPackageSubCommandId)
{
  m_bCommand = CSave; // save (storage) operation by default

  // This command has always constant length.
  setSerializedSize( CMessageSize );
}

SpotMessageVgdProcessPackage::~SpotMessageVgdProcessPackage (void)
{
}


unsigned char SpotMessageVgdProcessPackage::getCommandOperation(void) const
{
  return m_bCommand;
}

void SpotMessageVgdProcessPackage::setVerifyOperation(void)
{
  m_bCommand = CVerify;
}

void SpotMessageVgdProcessPackage::setSaveOperation(void)
{
  m_bCommand = CSave;
}

ISpotMessage::BufferType SpotMessageVgdProcessPackage::serialize (void) const
{
  // Serialize the header.
  ISpotMessage::BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  retBuffer.push_back( m_bCommand );

  std::cout << "SpotMessageVgdProcessPackage::serialize() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return retBuffer; // TODO: Fix ugly return vectors by value.
}

bool SpotMessageVgdProcessPackage::parse (BufferType const & buff)
{
  if(buff.size() < CMessageSize)  return false; // message too short

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff);

  if( !blRet ) return false; // malformed header

  m_bCommand = buff[CMessageCommandCodeOffset];

  std::cout << "SpotMessageVgdProcessPackage::parse() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return true;
}


} // namespace spot

} // namespace communication

} // namespace gvr


