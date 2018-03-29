/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageDownload.cpp
 * @author Guillermo Paris
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */



#include <iostream>
#include <gvr/communication/spot/Conversion.h>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessagePackageDownload.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


SpotMessagePackageDownload::SpotMessagePackageDownload (void)
:  AbsSpotMessageWithAppCatCmdID (false, CMessageResponseSetupAppId, CPackageDownloadCommandId, CPackageDownloadSubCommandId),
   m_bCommand(CSave), m_bOption(CRestart), m_uFileLength(0), m_pbCertificate(NULL)
{
  setSerializedSize( CMessageSize ); // Always 260 bytes long
}

SpotMessagePackageDownload::~SpotMessagePackageDownload (void)
{
}


void SpotMessagePackageDownload::setVerifyOperation(void)
{
  m_bCommand = CVerify;
}

void SpotMessagePackageDownload::setDownloadOperation(void)
{
  m_bCommand = CSave;
}

void SpotMessagePackageDownload::setRestartMode(void)
{
  m_bOption = CRestart;
}

void SpotMessagePackageDownload::setResumeMode(void)
{
  m_bOption = CResume;
}

void SpotMessagePackageDownload::setFileLength(ISpotMessage::SizeType length)
{
  m_uFileLength = length;
}

void SpotMessagePackageDownload::setCertificateData(const unsigned char* pbCertificateData)
{
  m_pbCertificate = pbCertificateData;
}


ISpotMessage::BufferType SpotMessagePackageDownload::serialize (void) const
{
  // Serialize the header.
  ISpotMessage::BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  // Serialize the rest of the message.
  if( m_uFileLength == 0 || m_pbCertificate == NULL ) return retBuffer; // bad settings, nothing to do.

  unsigned char abFileLength[4];

  // Sets the file length.
  Conversion::uint32ToBigEndian(m_uFileLength, abFileLength);
  retBuffer.push_back( abFileLength[0] );
  retBuffer.push_back( abFileLength[1] );
  retBuffer.push_back( abFileLength[2] );
  retBuffer.push_back( abFileLength[3] );

  // Sets the certificate data.
  retBuffer.insert(retBuffer.end(), m_pbCertificate, m_pbCertificate + CCertificateSize);

  // Sets the current operation (command save or verify)
  retBuffer.push_back( m_bCommand );

  // Sets the current operation mode (restart from the beginning or resume previous download)
  retBuffer.push_back( m_bOption );

  /*char acBuffer[3 * CMessageSize + 2];
  std::cout << "SpotMessagePackageDownload::serialize() - Length: " << size() << " bytes.\n";
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(acBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
   */

  return retBuffer; // TODO: Fix ugly return vectors by value.
}

bool SpotMessagePackageDownload::parse (BufferType const & buff)
{
  if(buff.size() < CMessageSize)  return false; // message too short

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff);

  if( !blRet ) return false; // malformed header

  m_uFileLength =  ( buff[CFileSizeOffset]  << 24 ) + (buff[CFileSizeOffset+1] << 16);
  m_uFileLength += ( buff[CFileSizeOffset+2] << 8 ) +  buff[CFileSizeOffset+3];

  m_pbCertificate = &buff.front() + CCertificateOffset; // volatile !!  just for unit testing comparisons !!

  m_bCommand = buff[CCommandOffset];
  m_bOption  = buff[COptionOffset];

  /* TODO : delete
  std::cout << "SpotMessagePackageDownload::parse() - Length: " << size() << " bytes.\n";
  char acBuffer[3 * CMessageSize + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(acBuffer, &buff.front(), size()) << std::endl << std::endl;
*/
  return true;
}


} // namespace spot

} // namespace communication

} // namespace gvr


