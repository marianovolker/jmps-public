/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsSpotPackage.cpp
 * @author Guillermo Paris
 * @date Monday, March 9, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstring>
#include <gvr/communication/spot/AbsSpotPackage.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


AbsSpotPackage::AbsSpotPackage (const std::string & sFilename, std::size_t uInitialOffset /* = 0 */)
  : m_pFile(NULL), m_uInitialOffset(uInitialOffset),
    m_uBlockSize(0), m_uCertificateSize(0), m_uSignatureSize(0),
    m_uFileLength(0), m_strFilename(sFilename), m_iLastErrorCode(0),
    m_pBlockBuffer(NULL), m_pCertificateData(NULL), m_pSignatureData(NULL)
{
  m_blInitialized = false;
  m_blError = true; // This object needs to be initialized in order to be functional.
}

AbsSpotPackage::~AbsSpotPackage (void)
{
  if( m_pCertificateData != NULL )
  {
    delete[] m_pCertificateData;
    m_pCertificateData = NULL;
  }

  if( m_pSignatureData != NULL )
  {
    delete[] m_pSignatureData;
    m_pSignatureData = NULL;
  }

  if( m_pBlockBuffer != NULL )
  {
    delete[] m_pBlockBuffer;
    m_pBlockBuffer = NULL;
  }

  if( m_pFile != NULL )
  {
    fclose(m_pFile);
    m_pFile = NULL;
  }
}


bool  AbsSpotPackage::initialize (std::size_t uBlockSize)
{
  if( m_blInitialized || m_strFilename.empty() )
  {
    m_iLastErrorCode = ( m_blInitialized ? eError0x01 : eError0x02 );
    return false; // already initialized object or invalid empty filename
  }

  m_pFile = fopen( m_strFilename.c_str(), "rb");
  if( m_pFile == NULL || fseek(m_pFile, 0L, SEEK_END) != 0)
  {
    m_iLastErrorCode = ( m_pFile == NULL ? eError0x03 : eError0x04 );
    return false;
  }

  m_blInitialized = true; // this object can not be initialized again

  m_uFileLength = static_cast<std::size_t>( ftell(m_pFile) );
  if( m_uFileLength == 0 )
  {
    m_iLastErrorCode = eError0x05;
    return false;
  }

  std::size_t uCertSize = setCertificateSize(); // derived class job.
  std::size_t uSigSize  = setSignatureSize();   // derived class job.
  int iPackageSize = m_uFileLength - uCertSize - uSigSize; // useful part of the file
  if( iPackageSize <= 0 || m_uInitialOffset >= iPackageSize )
  {
    m_iLastErrorCode = eError0x06;
    return false; // some size parameter is wrong
  }

  m_uBlockSize = uBlockSize;
  m_uCertificateSize = uCertSize;
  m_uSignatureSize   = uSigSize;

  m_pBlockBuffer = new unsigned char[m_uBlockSize];
  if( m_pBlockBuffer == NULL )
  {
    m_iLastErrorCode = eError0x07;
    return false;
  }

  memset( m_pBlockBuffer, 0, m_uBlockSize );

  if( m_uSignatureSize != 0)
  {
    m_pSignatureData = new unsigned char[m_uSignatureSize];
    if( m_pSignatureData == NULL )
    {
      m_iLastErrorCode = eError0x08;
      return false;
    }

    if( fseek(m_pFile, -m_uSignatureSize, SEEK_END) != 0 )
    {
      m_iLastErrorCode = eError0x09;
      return false;
    }

    std::size_t uRead = fread( m_pSignatureData, 1, m_uSignatureSize, m_pFile );
    if( uRead != m_uSignatureSize )
    {
      m_iLastErrorCode = eError0x0A;
      return false; // some kind of reading error
    }
  }

  if( m_uCertificateSize != 0 )
  {
    m_pCertificateData = new unsigned char[m_uCertificateSize];
    if( m_pCertificateData == NULL )
    {
      m_iLastErrorCode = eError0x0B;
      return false;
    }

    if( fseek(m_pFile, - (m_uCertificateSize + m_uSignatureSize), SEEK_END) != 0 )
    {
      m_iLastErrorCode = eError0x0C;
      return false;
    }

    std::size_t uRead = fread( m_pCertificateData, 1, m_uCertificateSize, m_pFile );
    if( uRead != m_uCertificateSize )
    {
      m_iLastErrorCode = eError0x0D;
      return false; // some kind of reading error
    }
  }

  if( fseek(m_pFile, m_uInitialOffset, SEEK_SET) != 0 )
  {
    m_iLastErrorCode = eError0x0E;
    return false; // could not return to initial position for later readings.
  }

  m_blError = false; // Now this object has become operative.
  return true;
}

bool  AbsSpotPackage::setInitialOffset(std::size_t uInitialOffset)
{
  if( m_blError ) return false; // pre-existent error condition

  if( uInitialOffset == m_uInitialOffset ) return true; // nothing to do

  if( fseek(m_pFile, uInitialOffset, SEEK_SET) != 0 )
  {
    m_iLastErrorCode = eError0x0F;
    m_blError = true;
    return false; // could not return to initial position for later readings.
  }

  m_uInitialOffset = uInitialOffset;
  return true;

}

std::size_t  AbsSpotPackage::getCertificateSize (void) const
{
  return m_uCertificateSize;
}

std::size_t  AbsSpotPackage::getSignatureSize (void) const
{
  return m_uSignatureSize;
}

const unsigned char *  AbsSpotPackage::getCertificateData (void) const
{
  return m_pCertificateData;
}

const unsigned char *  AbsSpotPackage::getSignatureData (void) const
{
  return m_pSignatureData;
}

std::size_t  AbsSpotPackage::fileLength (void) const // whole file length
{
  return m_uFileLength;
}

std::size_t  AbsSpotPackage::packageLength (void) const // package data length
{
  return m_uFileLength - m_uSignatureSize - m_uCertificateSize;
}

const unsigned char *  AbsSpotPackage::getNextBlock (std::size_t & uReadingSize)
{
  uReadingSize = 0;

  if( feof(m_pFile) ) return NULL; // Odd case in packages with no certificate and signature.

  std::size_t idxData = static_cast<std::size_t>( ftell(m_pFile) );
  std::size_t idxEndData = m_uFileLength - m_uSignatureSize - m_uCertificateSize; // package length

  if( idxData >= idxEndData) return NULL; // All the data has already been consumed.

  std::size_t uRemainingBytes = idxEndData - idxData;
  uReadingSize = ( uRemainingBytes < m_uBlockSize ? uRemainingBytes : m_uBlockSize );
  std::size_t uRead = fread( m_pBlockBuffer, 1, uReadingSize, m_pFile );
  if( uRead != uReadingSize)
  {
    m_iLastErrorCode = eError0x10;
    uReadingSize = 0;
    m_blError = true; // some kind of reading error.
    return NULL;
  }

  return m_pBlockBuffer;
}

bool  AbsSpotPackage::hasErrors (void) const
{
  return m_blError;
}

int  AbsSpotPackage::getLastOperationError (void) const
{
  return m_iLastErrorCode;
}

// Using this method derived classes can read bytes out of sequence or search a pattern into the package file.
int  AbsSpotPackage::peekBytes(unsigned char* pOutputBuffer, std::size_t uBeginning, std::size_t ulength)
{
  std::size_t idxEndData = m_uFileLength - m_uSignatureSize - m_uCertificateSize; // package length

  if( pOutputBuffer == NULL || uBeginning >= m_uFileLength )
  {
    return -1; // error, bad parameters
  }

  long lfp = ftell(m_pFile);

  int iRet = fread(pOutputBuffer, 1, ulength, m_pFile);
  fseek(m_pFile, lfp, SEEK_SET); // restore the internal file pointer
  return iRet;
}

} // namespace spot

} // namespace communication

} // namespace gvr

