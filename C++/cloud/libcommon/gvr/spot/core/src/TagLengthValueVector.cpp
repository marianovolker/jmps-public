/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file TagLengthValueVector.cpp
 * @author Guillermo Paris
 * @date Wednesday, February 19, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstring>
#include <gvr/communication/spot/Conversion.h>
#include <gvr/communication/spot/TagLengthValueVector.h>

 /// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


typedef TagLengthValueVector::TagLengthValue TLV;///< @brief Type Abbreviation.


/******************** CLASS TagLengthValue IMPLEMENTATION *******************/

TagLengthValueVector::TagLengthValue::TagLengthValue()
  : m_blError(false), m_pBytes(NULL), m_iTagID(0), m_uLength(0)
{
}

TagLengthValueVector::TagLengthValue::TagLengthValue(int tagID, std::size_t length, const unsigned char* pTLVData)
  : m_blError(false), m_pBytes(NULL), m_iTagID(0), m_uLength(0)
{
  if(length > CMaxTLVLength)
  {
    m_blError = true;
    return;
  }

  m_pBytes = new unsigned char[length];
  if( m_pBytes == NULL )
  {
    m_blError = true;
    return;
  }

  m_iTagID = tagID;
  m_uLength = length;
  memcpy(m_pBytes, pTLVData, length);
}

TagLengthValueVector::TagLengthValue::TagLengthValue(const TLV& tlv)
  : m_blError(false), m_pBytes(NULL), m_iTagID(0), m_uLength(0)
{
  std::size_t ulength = tlv.getLength();

  if(tlv.hasError() || ulength == 0 || ulength > CMaxTLVLength || tlv.getBytes() == NULL)
  {
    m_blError = true;
    return;
  }

  m_pBytes = new unsigned char [ulength];

  if(m_pBytes == NULL)
  {
    m_blError = true;
    return;
  }

  m_iTagID = tlv.getTagID();
  m_uLength = ulength;
  memcpy(m_pBytes, tlv.getBytes(), ulength);
}

TagLengthValueVector::TagLengthValue::~TagLengthValue()
{
  if( m_pBytes )
  {
    delete[] m_pBytes;
    m_pBytes = NULL;
  }
}

TLV&  TagLengthValueVector::TagLengthValue::operator=(TLV&)
{
  return *this;
}

bool TagLengthValueVector::TagLengthValue::hasError() const
{
  return m_blError;
}

int  TagLengthValueVector::TagLengthValue::getTagID() const
{
  return m_iTagID;
}

std::size_t TagLengthValueVector::TagLengthValue::getLength() const
{
  return m_uLength;
}

const unsigned char*  TagLengthValueVector::TagLengthValue::getBytes() const
{
  return m_pBytes;
}


/******************** CLASS TagLengthValueVector IMPLEMENTATION *******************/

TagLengthValueVector::TagLengthValueVector()
  : m_blError(false), m_bLengthSize(1), m_uLength(0)
{
}

TagLengthValueVector::TagLengthValueVector(unsigned char bLenSize)
  : m_blError(false), m_bLengthSize(bLenSize), m_uLength(0)
{
  if(bLenSize > 4 || bLenSize < 1 || bLenSize == 3) m_blError = true;
}

TagLengthValueVector::TagLengthValueVector(std::size_t nItems, unsigned char bLenSize,
                                           const unsigned char* pRawTLVvector)
  : m_blError(false), m_bLengthSize(bLenSize), m_uLength(0)
{
  if(bLenSize > 4 || bLenSize < 1 || bLenSize == 3 ||
     nItems == 0  || nItems > CMaxVectorLength || pRawTLVvector == NULL)
  {
    m_blError = true; // bad construction parameters
    return;
  }

  parseSourceData(nItems, pRawTLVvector);
}

// Forbidden copy constructor => m_blError(true)
TagLengthValueVector::TagLengthValueVector(TagLengthValueVector&)
: m_blError(true), m_bLengthSize(0), m_uLength(0)
{
}

TagLengthValueVector::~TagLengthValueVector()
{
  destroyElements();
}

// Forbidden assignment operator => m_blError(true)
TagLengthValueVector& TagLengthValueVector::operator=(TagLengthValueVector&)
{
  m_blError = true;
  return *this;
}


bool TagLengthValueVector::hasError() const
{
  return m_blError;
}

std::size_t TagLengthValueVector::getLength() const
{
  return m_blError ? 0 : m_uLength;
}

const TLV * TagLengthValueVector::getElement(std::size_t uIdx) const
{
  if(m_blError || uIdx > (m_uLength -1))
  {
    return NULL;
  }

  return m_TLVvector[uIdx];
}

const TLV * TagLengthValueVector::getElementByID(int id) const
{
  std::size_t uIdx = 0;

  while( uIdx < TagLengthValueVector::m_TLVvector.size() )
  {
    if( m_TLVvector[uIdx]->getTagID() == id ) // has the element been found ?
    {
      return m_TLVvector[uIdx];
    }
    else // element not found, keep searching
    {
      ++uIdx;
    }
  }

  return NULL; // element not found
}

const unsigned char * TagLengthValueVector::getElementData(std::size_t uIdx,
                                                           unsigned int & uTagID, std::size_t & uLen) const
{
  if(m_blError || uIdx > (m_uLength - 1))
  {
    uTagID = 0;
    uLen = 0;
    return NULL;
  }

  uTagID = m_TLVvector[uIdx]->getTagID();
  uLen = m_TLVvector[uIdx]->getLength();

  return m_TLVvector[uIdx]->getBytes();
}

const unsigned char *  TagLengthValueVector::getElementDataByID(int id, std::size_t & uIndex, std::size_t & uLength) const
{
  std::size_t uIdx = 0;

  while( uIdx < TagLengthValueVector::m_TLVvector.size() )
  {
    if( m_TLVvector[uIdx]->getTagID() == id ) // has the element been found ?
    {
      uIndex = uIdx;
      uLength = m_TLVvector[uIdx]->getLength();
      return m_TLVvector[uIdx]->getBytes();
    }
    else // element not found, keep searching
    {
      ++uIdx;
    }
  }

  uIndex = static_cast<std::size_t>( -1 ); // invalid index
  uLength = 0;
  return NULL; // element not found
}

std::size_t TagLengthValueVector::getSerializationSize(void)
{
  if(m_blError) return 0;

  std::size_t uTotalLength = 0;

  for(unsigned int uPos = 0; uPos < m_uLength; uPos++)
  {
    std::size_t uItemLength = m_TLVvector[uPos]->getLength();

    if( uItemLength )
    {
      uTotalLength += ( 1 + m_bLengthSize + uItemLength );
    }
  }

  return uTotalLength;
}

void TagLengthValueVector::clear()
{
  destroyElements();
  m_TLVvector.clear();
  m_blError = false;
  m_uLength = 0;
}

void TagLengthValueVector::destroyElements()
{
  for(unsigned int uPos = 0; uPos < m_TLVvector.size(); uPos++)
  {
    TagLengthValue* pElement = m_TLVvector[uPos];
    if( pElement != NULL )
    {
      delete pElement;
      pElement = NULL;
    }
  }
}

bool TagLengthValueVector::addElement(const TagLengthValue& tlv)
{
  std::size_t ulength = tlv.getLength();

  if(m_blError || tlv.hasError() || ulength == 0 || ulength > CMaxTLVLength || tlv.getBytes() == NULL)
  {
    return false; // bad parameter
  }

  TagLengthValue * pNewTLV = new TagLengthValue(tlv);
  if(pNewTLV == NULL)
  {
    m_blError = true;
    return false;
  }
  else if(pNewTLV->hasError())
  {
    delete pNewTLV;
    m_blError = true;
    return false;
  }

  m_TLVvector.push_back(pNewTLV);
  m_uLength++;
  return true;
}

bool TagLengthValueVector::addElement(unsigned int uTagID, std::size_t uItemLength, const unsigned char* pbData)
{
  if( m_blError || uItemLength == 0 || uItemLength > CMaxTLVLength || pbData == NULL)
  {
    return false; // bad parameter
  }

  TagLengthValue * pNewTLV = new TagLengthValue(uTagID, uItemLength, pbData);
  if(pNewTLV == NULL)
  {
    m_blError = true;
    return false;
  }
  else if(pNewTLV->hasError())
  {
    delete pNewTLV;
    m_blError = true;
    return false;
  }

  m_TLVvector.push_back(pNewTLV);
  m_uLength++;
  return true;
}

std::size_t TagLengthValueVector::serialize(unsigned char* pBuffer)
{
  if( pBuffer == NULL || m_blError ) return 0;

  std::size_t uTotalLength = 0;

  for(unsigned int uPos = 0; uPos < m_uLength; uPos++)
  {
    std::size_t uItemLength = m_TLVvector[uPos]->getLength();

    if( uItemLength )
    {
      uTotalLength += ( 1 + m_bLengthSize + uItemLength );
      *pBuffer++ = static_cast<unsigned char> (m_TLVvector[uPos]->getTagID());

      switch(m_bLengthSize)
      {
        case 1:
          *pBuffer++ = static_cast<unsigned char> (uItemLength);
        break;

        case 2:
          *pBuffer++ = static_cast<unsigned char> (uItemLength / 256); // upper 8 bits of the item length.
          *pBuffer++ = static_cast<unsigned char> (uItemLength % 256); // lower 8 bits of the item length.
        break;

        case 4:
        {
          unsigned char abItemLength[4];

          // Sets the item length.
          Conversion::uint32ToBigEndian(uItemLength, abItemLength);
          *pBuffer++ = abItemLength[0];
          *pBuffer++ = abItemLength[1];
          *pBuffer++ = abItemLength[2];
          *pBuffer++ = abItemLength[3];
        }
        break;
      }

      memcpy(pBuffer, m_TLVvector[uPos]->getBytes(), uItemLength);
      pBuffer += uItemLength;
    }
  }

  return uTotalLength;
}

std::size_t TagLengthValueVector::serialize( std::vector<unsigned char> & outVector)
{
  if(m_blError) return 0;

  outVector.clear();

  std::size_t uTotalLength = 0;

  for(unsigned int uPos = 0; uPos < m_uLength; uPos++)
  {
    std::size_t uItemLength = m_TLVvector[uPos]->getLength();

    if( uItemLength )
    {
      uTotalLength += ( 1 + m_bLengthSize + uItemLength );
      outVector.push_back( static_cast<unsigned char> (m_TLVvector[uPos]->getTagID()) );

      switch(m_bLengthSize)
      {
        case 1:
          outVector.push_back( static_cast<unsigned char> (uItemLength) );
        break;

        case 2:
          outVector.push_back( static_cast<unsigned char> (uItemLength / 256) ); // upper 8 bits of the item length.
          outVector.push_back( static_cast<unsigned char> (uItemLength % 256) ); // lower 8 bits of the item length.
        break;

        case 4:
        {
          std::size_t uLenH = uItemLength / (256 * 256); // upper 16 bits of the item length.
          std::size_t uLenL = uItemLength % (256 * 256); // lower 16 bits of the item length.

          outVector.push_back( static_cast<unsigned char> (uLenH / 256) ); // upper 8 bits of the item length.
          outVector.push_back( static_cast<unsigned char> (uLenH % 256) );
          outVector.push_back( static_cast<unsigned char> (uLenL / 256) );
          outVector.push_back( static_cast<unsigned char> (uLenL % 256) ); // lower 8 bits of the item length.
        }
        break;
      }

      const unsigned char* pBytes = m_TLVvector[uPos]->getBytes();

      outVector.insert(outVector.end(), pBytes, pBytes + uItemLength);
    }
  }

  return uTotalLength;

}

bool TagLengthValueVector::parseSourceData(unsigned int nItems, const unsigned char* pRawTLVvector)
{
  if(m_blError || nItems == 0 || nItems > CMaxVectorLength || pRawTLVvector == NULL)
  {
    m_blError = true; // bad parameters
    return false;
  }

  m_uLength = nItems;

  for(unsigned int uPos = 0; uPos < m_uLength; uPos++)
  {
    int iTagID = *pRawTLVvector++;
    std::size_t uLength = 0;
    TagLengthValue* pTLV = NULL;

    switch(m_bLengthSize)
    {
      case 1:
        uLength = *pRawTLVvector++;
        break;

      case 2:
        uLength = (*pRawTLVvector << 8) + *(pRawTLVvector + 1);
        pRawTLVvector += 2;
        break;

      case 4:
        uLength = (*pRawTLVvector << 24) + (*(pRawTLVvector + 1) << 16) + (*(pRawTLVvector + 2) << 8) + *(pRawTLVvector + 3);
        pRawTLVvector += 4;
        break;
    }

    pTLV = new TagLengthValue(iTagID, uLength, pRawTLVvector);
    if( pTLV == NULL )
    {
      m_blError = true;
      return false;
    }
    else if( pTLV->hasError() )
    {
      delete pTLV;
      m_blError = true;
      return false;
    }

    m_TLVvector.push_back(pTLV);
    pRawTLVvector += uLength;
  }

  m_blError = ( m_uLength == m_TLVvector.size() ? false : true ); // final length verification

  return !m_blError;
}



} // namespace spot

} // namespace communication

} // namespace gvr


