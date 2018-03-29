/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsSpotClassifyMessageHeader.cpp
 * @author Guillermo Paris
 * @date Thursday, February 19, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/AbsSpotClassifyMessageHeader.h>

#include <cstdio>
#include <algorithm>


#include <iostream>
#include <gvr/communication/spot/Utils.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{

AbsSpotClassifyMessageHeader::AbsSpotClassifyMessageHeader (void)
  : m_bAppId(0), m_bSsk(0), m_bCatId(0), m_bCmdId(0), m_classID(0)
{
}

AbsSpotClassifyMessageHeader::~AbsSpotClassifyMessageHeader (void)
{
}

void  AbsSpotClassifyMessageHeader::SetHeaderParameters (unsigned char bAppID, unsigned char bSsk,
                                                         unsigned char bCatID, unsigned char bCmdID,
                                                         TypeId classID)
{
  m_bAppId  = bAppID;
  m_bSsk    = bSsk;
  m_bCatId  = bCatID;
  m_bCmdId  = bCmdID;
  m_classID = classID;
}


ISpotClassifyMessage::IntegrityType AbsSpotClassifyMessageHeader::classifyHeader (BufferType const & msgBuffer, OffsetType & offset, SizeType & size, TypeId & typeID, KeyType & Key)
{
  unsigned char acPattern[4];

  acPattern[0] = m_bAppId;
  acPattern[1] = m_bSsk;
  acPattern[2] = m_bCatId;
  acPattern[3] = m_bCmdId;

  BufferType::const_iterator it = std::search( msgBuffer.begin(), msgBuffer.end(), acPattern, acPattern + 4 );

  if( it == msgBuffer.end() )
  {
    return ISpotClassifyMessage::Integrity::CUnknown; // proposed header not found in message buffer.
  }

  offset = it - msgBuffer.begin() - CMessageLengthFieldSize;
  size = (msgBuffer[offset] << 8) + msgBuffer[offset+1];
  typeID = m_classID;
  Key = (m_bAppId << 16) + (m_bCatId << 8) + m_bCmdId;


/* Information to be logged some day.
  char acDumpBuffer[(msgBuffer.size()*3)+2];
  std::cout << "ClassifyHeader - Message found - Key "<< std::hex << Key << (((offset + size) > msgBuffer.size()) ? " incomplete\n" : " COMPLETE\n") << std::dec;
  std::cout << "AppId: " << int(m_bAppId) << ", Cat: " << int(m_bCatId) << ", Cmd: " << int(m_bCmdId) << ", Size: " << size << ", Offset: " << offset << std::endl << "received buffer - size: " << msgBuffer.size() << ", data:\n" << Utils::dumpBuffer(acDumpBuffer, msgBuffer)  << std::dec << std::endl;
*/
  if( (offset + size) > msgBuffer.size() ) // last part of the message has not been put into the buffer, yet.
  {
    //std::cout << "waiting more bytes.\n";
    return ISpotClassifyMessage::Integrity::CIncomplete;
  }

  //std::cout << "ready to parse.\n";
  return ISpotClassifyMessage::Integrity::CComplete;
}


} // namespace spot

} // namespace communication

} // namespace gvr




