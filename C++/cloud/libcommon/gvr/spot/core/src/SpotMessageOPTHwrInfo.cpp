/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageOPTHwrInfo.cpp
 * @author Guillermo Paris
 * @date Thursday, May 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessageOPTHwrInfo.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


SpotMessageOPTHwrInfo::SpotMessageOPTHwrInfo (void)
:  AbsSpotMessageWithAppCatCmdID (false, CMessageRequestOPTAppId, COPTHwrInfoCommandId, COPTHwrInfoSubCommandId)
{
  // By default this command is set ready to request all available items in SPOT.
  setSerializedSize( CMessageItemDataOffset );
}

SpotMessageOPTHwrInfo::~SpotMessageOPTHwrInfo (void)
{
}

bool SpotMessageOPTHwrInfo::setItems(unsigned int uQty, const unsigned char* pbItemCodes)
{
  if( uQty == 0 || pbItemCodes == NULL ) return false;

  m_ItemVector.assign(pbItemCodes, pbItemCodes + uQty);
  setSerializedSize( CMessageItemDataOffset + m_ItemVector.size() );
  return true;
}

ISpotMessage::BufferType SpotMessageOPTHwrInfo::serialize (void) const
{
  // Serialize the header.
  ISpotMessage::BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  if( m_ItemVector.size() == 0 ) // no specified items means request of ALL available items in SPOT.
  {
    retBuffer.push_back(0); // 0x00 means all available items request.
  }
  else
  {  // Serialize the rest of the message.

    // Sets the number of items in the request message.
    retBuffer.push_back( static_cast<unsigned char>(m_ItemVector.size()) );

    // Copy the item codes to the request message.
    retBuffer.insert(retBuffer.begin() + CMessageItemDataOffset, m_ItemVector.begin(), m_ItemVector.end());
  }

  std::cout << "SpotMessageOPTHwrInfo::serialize() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return retBuffer; // TODO: Fix ugly return vectors by value.
}

bool SpotMessageOPTHwrInfo::parse (BufferType const & buff)
{
  if(buff.size() <= CHeaderSize)  return false; // message too short

  if( buff[CMessageItemQtyOffset] == 0 )
  {
    return true; // This request has no item codes, so this is a request for ALL available items in SPOT.
  }

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff);

  if( !blRet ) return false; // malformed header

  BufferType::const_iterator itDataBegining( buff.begin() + CMessageItemDataOffset );

  m_ItemVector.assign(itDataBegining, itDataBegining + buff[CMessageItemQtyOffset]);

  std::cout << "SpotMessageOPTHwrInfo::parse() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return true;
}


} // namespace spot

} // namespace communication

} // namespace gvr


