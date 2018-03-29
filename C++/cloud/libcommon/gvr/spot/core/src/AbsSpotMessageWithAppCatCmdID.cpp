/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsSpotMessageWithAppCatCmdID.cpp
 * @author Guillermo Paris
 * @date Monday, February 9, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <gvr/communication/spot/AbsSpotMessageWithAppCatCmdID.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


AbsSpotMessageWithAppCatCmdID::AbsSpotMessageWithAppCatCmdID (bool blReply, unsigned char bAppID,
                                                              unsigned char bCatID, unsigned char bCmdID)
  : m_blReply(blReply), m_bCatId(bCatID), m_bCmdId(bCmdID), m_bAckCode(0), m_uTotalLength(0)
{
  m_bAppId   = ( blReply ? bAppID | CResponseMessageAppmask : bAppID & CRequestMessageAppmask );
}

AbsSpotMessageWithAppCatCmdID::~AbsSpotMessageWithAppCatCmdID (void)
{  // Assigns physical address to this virtual destructor method,
}  // avoiding this way potential future link errors.


unsigned char AbsSpotMessageWithAppCatCmdID::getSpotAppId (void) const
{
  return m_bAppId;
}

unsigned char AbsSpotMessageWithAppCatCmdID::getSsk (void) const
{
  return CDefaulNoEncryptionValue;
}

unsigned char AbsSpotMessageWithAppCatCmdID::getCommand(void) const
{
  return m_bCatId;
}

unsigned char AbsSpotMessageWithAppCatCmdID::getSubCommand(void) const
{
  return m_bCmdId;
}

unsigned char AbsSpotMessageWithAppCatCmdID::getAckCode( void ) const
{
  return m_bAckCode;
}

void AbsSpotMessageWithAppCatCmdID::setAckCode( unsigned char const bAckCode )
{
  m_bAckCode = bAckCode;
}

ISpotMessage::KeyType AbsSpotMessageWithAppCatCmdID::getKey (void) const
{
  return (m_bAppId << 16) + (m_bCatId << 8) + m_bCmdId;
}

ISpotMessage::SizeType AbsSpotMessageWithAppCatCmdID::size (void) const
{
  return m_uTotalLength;
}

ISpotMessage::BufferType AbsSpotMessageWithAppCatCmdID::serialize (void) const
{
  BufferType buff;
  // std::cout << "AbsSpotMessageWithAppCatCmdID::serialize : length= " << std::dec << size() << std::endl;
  buff.push_back( (size() & 0x0000FF00) >> 8 );
  buff.push_back( (size() & 0x000000FF));
  buff.push_back( getSpotAppId() );
  buff.push_back( getSsk() );
  buff.push_back( getCommand() );
  buff.push_back( getSubCommand() );

  if( m_blReply ) buff.push_back( getAckCode() );

  return buff;
}

bool AbsSpotMessageWithAppCatCmdID::parse (BufferType const & buff)
{
    if(buff.size() < CHeaderSize)
    {
      return false;
    }

    // Message length calculation.
    int iDatasize = ( static_cast<SizeType>(buff[CMessageLengthOffsetH]) << 8 ) | buff[CMessageLengthOffsetL];

    if(buff.size() < iDatasize)
    {
      std::cout << "Buffered Message too short, Size = " << buff.size()
                << ", but in header is " << iDatasize << std::endl;
      return false;
    }
    else
    {
      m_uTotalLength = iDatasize;
    }

    if(buff[CMessageAppIdOffset] != m_bAppId)
    {
      return false;
    }
    else if(buff[CMessageSSKOffset] != getSsk())
    {
      return false;
    }
    else if(buff[CMessageCatIdOffset] != m_bCatId)
    {
      return false;
    }
    else if(buff[CMessageCmdIdOffset] != m_bCmdId)
    {
      return false;
    }

    if( m_blReply ) m_bAckCode = buff[CMessageACKOffset];

    return true;
}


///////////////////////////////////
//
// Protected and private members.
//
///////////////////////////////////

void AbsSpotMessageWithAppCatCmdID::setAppId(unsigned char bAppId)
{
  m_bAppId = bAppId;
}

void AbsSpotMessageWithAppCatCmdID::setSubCommand(unsigned char bSubCommand)
{
  m_bCmdId = bSubCommand;
}

void AbsSpotMessageWithAppCatCmdID::setSerializedSize(SizeType uTotalSize)
{
  m_uTotalLength = uTotalSize;
}

} // namespace spot

} // namespace communication

} // namespace gvr


