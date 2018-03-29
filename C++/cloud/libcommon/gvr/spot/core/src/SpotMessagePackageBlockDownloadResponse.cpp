/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageBlockDownloadResponse.cpp
 * @author Guillermo Paris
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstring>
#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessagePackageBlockDownloadResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


SpotMessagePackageBlockDownloadResponse::SpotMessagePackageBlockDownloadResponse (void)
:  AbsSpotMessageWithAppCatCmdID (true, CMessageResponseSetupAppId, CPackageBlockDownloadCategoryId, CPackageBlockDownloadCommandId),
   m_uDebianDependencyStringLength(0), m_pcDebianDependencyStringData(NULL)
{
}

SpotMessagePackageBlockDownloadResponse::~SpotMessagePackageBlockDownloadResponse (void)
{
  if( m_pcDebianDependencyStringData )
  {
    delete[] m_pcDebianDependencyStringData;
  }
}


bool SpotMessagePackageBlockDownloadResponse::wasBlockReceivedOK()
{
  return ( getAckCode() == CBlockDownloadedOK );
}

ISpotMessage::SizeType  SpotMessagePackageBlockDownloadResponse::getDebianDependenciesTotalLength (void) const
{
  return m_uDebianDependencyStringLength;
}

const char* SpotMessagePackageBlockDownloadResponse::
            getDebianDependencies (ISpotMessage::SizeType * puTotalLength /* = NULL */) const
{
  if( puTotalLength != NULL )
  {
    *puTotalLength = m_uDebianDependencyStringLength;
  }

  return m_pcDebianDependencyStringData;
}

// For testing only !!  This is a reply message.
ISpotMessage::BufferType  SpotMessagePackageBlockDownloadResponse::serialize (void) const
{
  // First of all: serialize the header.
  BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  if( m_uDebianDependencyStringLength == 0 || m_pcDebianDependencyStringData == NULL )
  {
    return retBuffer; // no more data available
  }

  // Sets the length of dependency strings.
  retBuffer.push_back( static_cast<unsigned char>(m_uDebianDependencyStringLength / 256) );
  retBuffer.push_back( static_cast<unsigned char>(m_uDebianDependencyStringLength % 256) );
  // Appends the dependency strings bulk data.
  retBuffer.insert( retBuffer.end(), m_pcDebianDependencyStringData,
                    m_pcDebianDependencyStringData + m_uDebianDependencyStringLength );

  /* TODO : delete
  std::cout << "SpotMessagePackageBlockDownloadResponse::serialize() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;
*/
  return retBuffer; // TODO: Fixing ugly return vectors by value.
}

bool SpotMessagePackageBlockDownloadResponse::parse (BufferType const & buff)
{
  if(buff.size() < CMinMessageSize)  return false; // message too short

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff); // Length and AckCode are get by this method.

  if( !blRet ) return false; // malformed header

  if( buff.size() < CMessageDependencyLengthIndex + 2 )
  {
	  m_uDebianDependencyStringLength = 0;
  }
  else
  {
    m_uDebianDependencyStringLength = (buff[CMessageDependencyLengthIndex] << 8) + buff[CMessageDependencyLengthIndex+1];
  }
/* TODO delete
  std::cout << std::dec << "SpotMessagePackageBlockDownloadResponse::parse() - Length: " << size() << " bytes. ACK = "
  << int(getAckCode()) << " . Dependencies Size: " << m_uDebianDependencyStringLength << std::endl;
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size())
            << std::endl << std::endl;
  delete[] pcBuffer;
*/
  if( m_uDebianDependencyStringLength == 0 )
  {
    return true; // This response has no debian dependencies specified.
  }

  if( m_pcDebianDependencyStringData != NULL )
  {
    delete[] m_pcDebianDependencyStringData; // if reusing this message, previous msg data is lost.
  }

  m_pcDebianDependencyStringData = new char[ m_uDebianDependencyStringLength ];
  memcpy(m_pcDebianDependencyStringData, &buff.front() + CMessageDependenciesIndex, m_uDebianDependencyStringLength);
  return true;
}


} // namespace gvr

} // namespace communication

} // namespace spot


