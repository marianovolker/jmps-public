/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageVGDFileTransfer.cpp
 * @author Guillermo Paris
 * @date Monday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/Conversion.h>
#include <gvr/communication/spot/SpotMessageVGDFileTransfer.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


SpotMessageVGDFileTransfer::SpotMessageVGDFileTransfer (void) :

  AbsSpotMessageWithAppCatCmdID ( false, CMessageRequestSpotAppId,
                                  CVGDFileTransferCategoryId, CVGDFileUploadTransferCommandId ),

  m_uFileLength(0), m_uBlockLength(0), m_pbBlockData(NULL)
{
  setSerializedSize( CMinMessageSize ); // Minimal length as in Upload File/Block.
}

SpotMessageVGDFileTransfer::~SpotMessageVGDFileTransfer (void)
{

}


unsigned char SpotMessageVGDFileTransfer::getOperationCommand(void ) const
{
  return getSubCommand();
}

void SpotMessageVGDFileTransfer::setOperationCommand(unsigned char bCommandCode)
{
  setSubCommand( bCommandCode );
}

void SpotMessageVGDFileTransfer::setFileLength(ISpotMessage::SizeType length)
{
  m_uFileLength = length;
  setSerializedSize( CMinMessageSize + sizeof(uint32_t) );
}

void SpotMessageVGDFileTransfer::setBlock( ISpotMessage::SizeType length,
                                           const unsigned char* pbData )
{
  if( length == 0 || pbData == NULL )
  {
    m_uBlockLength = 0;
    m_pbBlockData = NULL;
    setSerializedSize(CMinMessageSize); // minimum message length as in file/block upload.
  }
  else
  {
    m_uBlockLength = length;
    m_pbBlockData = pbData;
    setSerializedSize(m_uBlockLength + CMinMessageSize);
  }
}


ISpotMessage::BufferType SpotMessageVGDFileTransfer::serialize (void) const
{
  // Serialize the header.
  ISpotMessage::BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  // Serialize the rest of the message.
  if( m_uFileLength == 0 && m_pbBlockData == NULL ) return retBuffer; // bad settings, nothing to do.

  if( getSubCommand() == CVGDFileDownloadTransferCommandId )
  { // Sets the block length.
    unsigned char acFileLength[4];

    Conversion::uint32ToBigEndian( m_uFileLength, acFileLength );
    retBuffer.push_back( acFileLength[0] );
    retBuffer.push_back( acFileLength[1] );
    retBuffer.push_back( acFileLength[2] );
    retBuffer.push_back( acFileLength[3] );
  }
  else if( getSubCommand() == CVGDBlockDownloadTransferCommandId )
  {
    if( m_uBlockLength == 0  ) return retBuffer; // no data, erroneous block message.

    // Sets the block data.
    retBuffer.insert(retBuffer.end(), m_pbBlockData, m_pbBlockData + m_uBlockLength);
  }

//char* pcBuffer = new char[3 * size() + 2];
  std::cout << "SpotMessageVGDFileTransfer::serialize() - Length: " << std::dec << size() << " bytes.\n";
//std::cout << "SpotMessageVGDFileTransfer serialized Message Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl;
//delete[] pcBuffer;

  return retBuffer; // TODO: Fix ugly return vectors by value.
}

bool SpotMessageVGDFileTransfer::parse (BufferType const & buff)
{
  if(buff.size() < CMinMessageSize)  return false; // message too short

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff);

  if( !blRet ) return false; // malformed header

  if( getSubCommand() == CVGDFileDownloadTransferCommandId )
  { // Sets the block length.
    m_uFileLength = ( buff[CFileSizeOffset] << 24 ) + ( buff[CFileSizeOffset + 1] << 16 ) +
                    ( buff[CFileSizeOffset + 2] << 8 ) + buff[CFileSizeOffset + 3];
  }
  else if( getSubCommand() == CVGDBlockDownloadTransferCommandId )
  {
    m_uBlockLength = buff.size() - CFileDataOffset;
    m_pbBlockData = &buff.front() + CFileDataOffset; // volatile !!  just for unit testing comparisons !!
  }

  std::cout << "SpotMessageVGDFileTransfer::parse() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return true;
}


} // namespace spot

} // namespace communication

} // namespace gvr


