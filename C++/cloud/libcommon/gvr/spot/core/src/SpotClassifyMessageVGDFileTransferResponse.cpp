/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageVGDFileTransferResponse.cpp
 * @author Guillermo Paris
 * @date Monday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <iostream>
#include <algorithm>
#include <gvr/communication/spot/SpotClassifyMessageVGDFileTransferResponse.h>
#include <gvr/communication/spot/SpotMessageVGDFileTransferResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


typedef IClassifyMessage<SpotMessageSpec>::BufferType    BufferType;
typedef IClassifyMessage<SpotMessageSpec>::SizeType      OffsetType, SizeType;
typedef IClassifyMessage<SpotMessageSpec>::TypeId        TypeId;
typedef IClassifyMessage<SpotMessageSpec>::KeyType       KeyType;


SpotClassifyMessageVGDFileTransferResponse::~SpotClassifyMessageVGDFileTransferResponse (void)
{
}

ISpotClassifyMessage::IntegrityType SpotClassifyMessageVGDFileTransferResponse::classify (
  BufferType const & msgBuffer,
  OffsetType & offset,
  SizeType & size,
  TypeId & typeId,
  KeyType & key )
{
  unsigned char acPattern[4];
  acPattern[0] = static_cast<unsigned char>(ISpotMessage::CMessageResponseSpotAppId);
  acPattern[1] = static_cast<unsigned char>(ISpotMessage::CDefaulNoEncryptionValue);
  acPattern[2] = static_cast<unsigned char>(SpotMessageVGDFileTransferResponse::CVGDFileTransferCategoryId);
  acPattern[3] = 0; // not part of the search

  BufferType::const_iterator it = std::search( msgBuffer.begin(), msgBuffer.end(), acPattern, acPattern + 3 );

  if( it == msgBuffer.end() )
  {
    return ISpotClassifyMessage::Integrity::CUnknown; // proposed header not found in message buffer.
  }

  offset = it - msgBuffer.begin() - ISpotClassifyMessage::CMessageLengthFieldSize;
  size   = (msgBuffer[offset] << 8) + msgBuffer[offset+1];
  typeId = SpotMessageVGDFileTransferResponse::GetTypeId();
  key    = (ISpotMessage::CMessageResponseSpotAppId << 16) +
           (SpotMessageVGDFileTransferResponse::CVGDFileTransferCategoryId << 8);

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


