/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageInfoResponse.cpp
 * @author Guillermo Paris
 * @date Monday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstring>
#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessagePackageInfoResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{

/********************************* Class Certificate **********************************/

SpotMessagePackageInfoResponse::Certificate::Certificate()
  :  pbData(NULL)
{
}

void SpotMessagePackageInfoResponse::Certificate::setData(const unsigned char* pdata)
{
  if(pbData == NULL)
  {
    pbData = new unsigned char[CLength];
  }

  if(pbData)
  {
    memcpy(pbData, pdata, CLength);
  }
}

SpotMessagePackageInfoResponse::Certificate::~Certificate()
{
  if(pbData)
  {
    delete [] pbData;
    pbData = NULL;
  }
}


/************************ Class SpotMessagePackageInfoResponse ************************/

SpotMessagePackageInfoResponse::SpotMessagePackageInfoResponse (void)
:  AbsSpotMessageWithAppCatCmdID (true, CMessageResponseSetupAppId, CPackageInfoCategoryId, CPackageInfoCommandId)
{
}

SpotMessagePackageInfoResponse::~SpotMessagePackageInfoResponse (void)
{
}


unsigned int SpotMessagePackageInfoResponse::getItemQty (void)
{
  return m_Certificates.size();
}

const unsigned char * SpotMessagePackageInfoResponse::getItemData (ISpotMessage::SizeType   uIndex,
                                                                   ISpotMessage::SizeType   uOffset) const
{
  if( uIndex >= m_Certificates.size() )
  {
    return NULL; // index out of bound
  }

  return m_Certificates[uIndex].getData();
}


ISpotMessage::BufferType  SpotMessagePackageInfoResponse::serialize (void) const
{
  // First of all: serialize the header.
  BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  // Sets the number of items in the request message.
  SizeType uTotalLength = m_Certificates.size();
  retBuffer.push_back( static_cast<unsigned char>(uTotalLength) );

  // Copy the item codes to the request message.
  for(unsigned int uIdx = 0; uIdx < uTotalLength; uIdx++)
  {
    const unsigned char*  pItemData = m_Certificates[uIdx].getData();

    retBuffer.insert(retBuffer.end(), pItemData , pItemData + Certificate::CLength); // appending operation
  }

  std::cout << "SpotMessagePackageInfoResponse::serialize() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return retBuffer; // TODO: Fixing ugly return vectors by value.
}

bool SpotMessagePackageInfoResponse::parse (BufferType const & buff)
{
  if(buff.size() <= CMinMessageSize)  return false; // message too short

  if( buff[CMessageItemQtyOffset] == 0 )
  {
    return false; // This response has no item codes, which is only allowed in requests, but not in a reply message.
  }

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff); // Length and AckCode are get by this method.

  if( !blRet ) return false; // malformed header

  unsigned int nItems =  buff[CMessageItemQtyOffset];

  std::cout << std::dec << "SpotMessagePackageInfoResponse::parse() - Length: " << size() << " bytes. ACK = "
  << int(getAckCode()) << " . Items: " << nItems << std::endl;

  if(getAckCode() != CMessageResponseAckOK || nItems == 0)
  {
    std::cout << "NOTHING TO DO!\n";
    return false;
  }

  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size())
            << std::endl << std::endl;
  delete[] pcBuffer;

  parseSourceData(nItems, &buff.front() + CMessageItemDataOffset);

  return true;
}

void SpotMessagePackageInfoResponse::parseSourceData(unsigned int nItems, const unsigned char* pSourceData)
{
  if( ! m_Certificates.empty() )
  {
    m_Certificates.clear();
  }

  Certificate  nullCert; // null certificate

  for(unsigned int idx = 0; idx < nItems; idx++)
  {
    m_Certificates.push_back(nullCert);
  }

  for(unsigned int idx = 0; idx < nItems; idx++)
  {
    const unsigned char * pData = pSourceData + idx * Certificate::CLength;

    m_Certificates[idx].setData(pData);
  }
}

} // namespace gvr

} // namespace communication

} // namespace spot


