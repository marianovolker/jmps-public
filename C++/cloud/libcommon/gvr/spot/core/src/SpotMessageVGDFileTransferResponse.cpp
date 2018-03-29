/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageVGDFileTransferResponse.cpp
 * @author Guillermo Paris
 * @date Monday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstring>
#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/Conversion.h>
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


SpotMessageVGDFileTransferResponse::SpotMessageVGDFileTransferResponse (void) :

  AbsSpotMessageWithAppCatCmdID ( true, CMessageResponseSpotAppId,
                                  CVGDFileTransferCategoryId, CVGDFileDownloadTransferCommandId ),

  m_uFileLength(0), m_uBlockLength(0), m_pbBlockData(NULL)
{
  setSerializedSize( CMinMessageSize ); // Minimal length as in Download File/Block.
}

SpotMessageVGDFileTransferResponse::~SpotMessageVGDFileTransferResponse (void)
{
  if( m_pbBlockData != NULL )
  {
    delete[] m_pbBlockData;
  }
}


bool SpotMessageVGDFileTransferResponse::canDownloadBegin()
{
  return  ( getAckCode() == 0 );
}

bool SpotMessageVGDFileTransferResponse::wasBlockTransferOK()
{
  return  ( getAckCode() == CBlockTransferOK );
}

unsigned char SpotMessageVGDFileTransferResponse::getOperationCommand (void) const
{
  return getSubCommand();
}

void SpotMessageVGDFileTransferResponse::setOperationCommand(unsigned char bCommandCode)
{
  setSubCommand( bCommandCode );
}

ISpotMessage::SizeType SpotMessageVGDFileTransferResponse::getFileLength () const
{
  return m_uFileLength;
}

const unsigned char* SpotMessageVGDFileTransferResponse::getBlock( ISpotMessage::SizeType & length)

{
  length = m_uBlockLength;
  return m_pbBlockData;
}

ISpotMessage::KeyType SpotMessageVGDFileTransferResponse::getKey (void) const
{
  return (getSpotAppId() << 16) + (getCommand() << 8);
}

ISpotMessage::BufferType SpotMessageVGDFileTransferResponse::serialize (void) const
{
  // Serialize the header. Not calling base class because this is actually a 5 byte header instead of 6.
  BufferType retBuffer;
  std::cout << "SpotMessageVGDFileTransferResponse::serialize : length= " << size() << std::endl;
  retBuffer.push_back( (size() & 0x0000FF00) >> 8 );
  retBuffer.push_back( (size() & 0x000000FF));
  retBuffer.push_back( getSpotAppId() );
  retBuffer.push_back( getSsk() );
  retBuffer.push_back( getCommand() );
  retBuffer.push_back( getAckCode() );

  // Serialize the rest of the message.

  if( getSubCommand() == CVGDFileUploadTransferCommandId )
  { // Sets the block length.
    unsigned char acFileLength[4];

    Conversion::uint32ToBigEndian( m_uFileLength, acFileLength );
    retBuffer.push_back( acFileLength[0] );
    retBuffer.push_back( acFileLength[1] );
    retBuffer.push_back( acFileLength[2] );
    retBuffer.push_back( acFileLength[3] );
  }
  else if( getSubCommand() == CVGDBlockUploadTransferCommandId )
  {
    if( m_uBlockLength == 0 || m_pbBlockData == NULL ) return retBuffer; // bad settings, nothing to do.

    // Serializes the block data.
    retBuffer.insert(retBuffer.end(), m_pbBlockData, m_pbBlockData + m_uBlockLength);
  }

  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "SpotMessageVGDFileTransferResponse::serialize() - Length: " << size() << " bytes.\n";
  std::cout << "SpotMessageVGDFileTransferResponse Serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl;
  delete[] pcBuffer;

  return retBuffer; // TODO: Fix ugly return vectors by value.
}

bool SpotMessageVGDFileTransferResponse::parse (BufferType const & buff)
{
  if(buff.size() < CMinMessageSize)  return false; // message too short

  // Parse the header. Not calling base class because this is actually a 5 byte header instead of 6.
  // Message length calculation.
  SizeType uMsgSize = ( static_cast<SizeType>(buff[CMessageLengthOffsetH]) << 8 ) | buff[CMessageLengthOffsetL];

  if(buff.size() < uMsgSize)
  {
    return false; // the message present into the buffer is not complete.
  }
  else
  {
    setSerializedSize( uMsgSize );
  }

  if(buff[CMessageAppIdOffset] != getSpotAppId())
  {
    return false; // bad App ID
  }
  else if(buff[CMessageSSKOffset] != getSsk())
  {
    return false;
  }
  else if(buff[CMessageCatIdOffset] != getCommand())
  {
    return false; // bad Cat ID
  }

  setAckCode( buff[CFileSizeOffset - 1] );

  if( getSubCommand() == CVGDFileUploadTransferCommandId )
  { // Sets the block length.
    m_uFileLength = ( buff[CFileSizeOffset] << 24 ) + ( buff[CFileSizeOffset + 1] << 16 ) +
                    ( buff[CFileSizeOffset + 2] << 8 ) + buff[CFileSizeOffset + 3];
  }
  else if( getSubCommand() == CVGDBlockUploadTransferCommandId )
  {
    SizeType length = uMsgSize - CFileDataOffset;

    if( length != m_uBlockLength )
    {
      delete[] m_pbBlockData;
      m_pbBlockData == NULL;
    }

    if( m_pbBlockData == NULL )
    {
      m_pbBlockData = new unsigned char[length];
    }

    memcpy(m_pbBlockData, &buff.front() + CFileDataOffset, length);
    m_uBlockLength = length;
  }

  std::cout << "SpotMessageVGDFileTransferResponse::parse() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return true;
}


} // namespace spot

} // namespace communication

} // namespace gvr


