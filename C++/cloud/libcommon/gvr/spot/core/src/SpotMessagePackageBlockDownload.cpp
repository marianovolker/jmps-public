/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageBlockDownload.cpp
 * @author Guillermo Paris
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessagePackageBlockDownload.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


SpotMessagePackageBlockDownload::SpotMessagePackageBlockDownload (void)
:  AbsSpotMessageWithAppCatCmdID (false, CMessageResponseSetupAppId, CPackageDownloadCategoryId, CPackageBlockDownloadCommandId),
   m_uBlockLength(0), m_pbBlockData(NULL)
{
  setSerializedSize( CMinMessageSize ); // Minimal length (like The final block message) to be overridden.
}

SpotMessagePackageBlockDownload::~SpotMessagePackageBlockDownload (void)
{
}


void SpotMessagePackageBlockDownload::setBlock(ISpotMessage::SizeType length,
                                               const unsigned char* pbData /* = NULL */)
{
  if( length == 0 || pbData == NULL )
  {
    m_uBlockLength = 0;
    m_pbBlockData = NULL;
    setSerializedSize(CMinMessageSize); // last block message signaling end of transfer.
  }
  else
  {
    m_uBlockLength = length;
    m_pbBlockData = pbData;
    setSerializedSize(m_uBlockLength + CMinMessageSize);
  }
}


ISpotMessage::BufferType SpotMessagePackageBlockDownload::serialize (void) const
{
  // Serialize the header.
  ISpotMessage::BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  // Serialize the rest of the message.
  if( m_uBlockLength != 0 && m_pbBlockData == NULL ) return retBuffer; // bad settings, nothing to do.

  // Sets the block length.
  retBuffer.push_back( static_cast<unsigned char>(m_uBlockLength / 256) );
  retBuffer.push_back( static_cast<unsigned char>(m_uBlockLength % 256) );

  // Sets the block data.
  if( m_uBlockLength == 0  ) return retBuffer; // no data, final block message.

  retBuffer.insert(retBuffer.end(), m_pbBlockData, m_pbBlockData + m_uBlockLength);

  /* TODO : delete
char* pcBuffer = new char[3 * size() + 2];
  std::cout << "SpotMessagePackageBlockDownload::serialize() - Length: " << size() << " bytes.\n";
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;
*/
  return retBuffer; // TODO: Fix ugly return vectors by value.
}

bool SpotMessagePackageBlockDownload::parse (BufferType const & buff)
{
  if(buff.size() < CMinMessageSize)  return false; // message too short

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff);

  if( !blRet ) return false; // malformed header

  m_uBlockLength = ( buff[CBlockSizeOffset] << 8 ) + buff[CBlockSizeOffset + 1];

  m_pbBlockData = &buff.front() + CBlockDataOffset; // volatile !!  just for unit testing comparisons !!

  /* TODO : delete
  std::cout << "SpotMessagePackageBlockDownload::parse() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;
*/
  return true;
}


} // namespace spot

} // namespace communication

} // namespace gvr


