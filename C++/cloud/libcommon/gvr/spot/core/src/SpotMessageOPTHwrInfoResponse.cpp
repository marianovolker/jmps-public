/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageOPTHwrInfoResponse.cpp
 * @author Guillermo Paris
 * @date Thursday, May 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstring>
#include <cstdio>
#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessageOPTHwrInfoResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


const char * SpotMessageOPTHwrInfoResponse::CpcPrinter           = "printer";
const char * SpotMessageOPTHwrInfoResponse::CpcContactlessReader = "contactless reader";
const char * SpotMessageOPTHwrInfoResponse::CpcIOModule          = "I/O module";

const char * SpotMessageOPTHwrInfoResponse::CpcSerialNo          = "serial number";
const char * SpotMessageOPTHwrInfoResponse::CpcType              = "type";
const char * SpotMessageOPTHwrInfoResponse::CpcModel             = "model";
const char * SpotMessageOPTHwrInfoResponse::CpcFunctions         = "functions";

const char * SpotMessageOPTHwrInfoResponse::CpcUnknown           = "unknown";
const char * SpotMessageOPTHwrInfoResponse::CpcThermalPrinter    = "thermal printer";
const char * SpotMessageOPTHwrInfoResponse::CpcDotMatrixPrinter  = "dot matrix printer";

SpotMessageOPTHwrInfoResponse::SpotMessageOPTHwrInfoResponse (void)
:  AbsSpotMessageWithAppCatCmdID (true, CMessageResponseOPTAppId, COPTHwrInfoCategoryId, COPTHwrInfoCommandId)
{
}

SpotMessageOPTHwrInfoResponse::~SpotMessageOPTHwrInfoResponse (void)
{
}


unsigned int SpotMessageOPTHwrInfoResponse::getItemQty (void)
{
  return m_TLVvector.getLength();
}

const unsigned char * SpotMessageOPTHwrInfoResponse::getItemData (ISpotMessage::SizeType uIndex,
                                                                  unsigned int &         iTagID,
                                                                  ISpotMessage::SizeType & length) const
{
  if( uIndex >= m_TLVvector.getLength() )
  {
    length = 0;
    return NULL; // index out of bound
  }

  return m_TLVvector.getElementData(uIndex, iTagID, length);
}

bool SpotMessageOPTHwrInfoResponse::getPrinterSerialNumber (std::string & sOut) const
{
  return getSubItemDescription(ePRINTER_SERIALNUMBER, sOut, 16);
}

bool SpotMessageOPTHwrInfoResponse::getContactlessSerialNumber (std::string & sOut) const
{
  return getSubItemDescription(eOTI_CONTACTLESS_READER_SERIALNUMBER, sOut, 16);
}

bool SpotMessageOPTHwrInfoResponse::getIOModuleSerialNumber (std::string & sOut) const
{
  return getSubItemDescription(eIOMODULE_SERIALNUMBER, sOut, 16);
}

bool SpotMessageOPTHwrInfoResponse::getPrinterInfo (std::string & sDeviceName, DeviceInfoMap & map) const
{
  bool blret = true;
  std::string sField;

  sDeviceName = CpcPrinter;
  map.clear();

  blret &= getSubItemDescription(ePRINTER_DEVMODEL, sField, 16);
  map.insert(make_pair(CpcModel , sField));

  blret &= getSubItemDescription(ePRINTER_SERIALNUMBER, sField, 16);
  map.insert(make_pair(CpcSerialNo , sField));

  int iPrinterType = 0;

  blret &= getOneByteIntegerSubItem(ePRINTER_DEVTYPE, iPrinterType);
  switch(iPrinterType)
  {
    case eThermal:
      sField = CpcThermalPrinter;
      break;

    case eDotMatrix:
      sField = CpcDotMatrixPrinter;
      break;

    case eUnknown:
    default:
      sField = CpcUnknown;
  }
  map.insert(make_pair(CpcType , sField));

  blret &= getFourByteSubItemCHexNumberAsString(ePRINTER_DEVFUNCTIONS, sField);
  map.insert(make_pair(CpcFunctions , sField));

  return blret;
}

bool SpotMessageOPTHwrInfoResponse::getContactlessReaderInfo (std::string & sDeviceName, DeviceInfoMap & map) const
{
  bool blret = true;
  std::string sField;

  sDeviceName = CpcContactlessReader;
  map.clear();

  blret &= getSubItemDescription(eOTI_CONTACTLESS_READER_SERIALNUMBER, sField, 16);
  map.insert(make_pair(CpcSerialNo , sField));

  // To be continued someday, when more contactless info be available from SPOT.
  // map.insert(make_pair(Cpcxxx , sField));

  return blret;
}

bool SpotMessageOPTHwrInfoResponse::getIOModuleInfo (std::string & sDeviceName, DeviceInfoMap & map) const
{
  bool blret = true;
  std::string sField;

  sDeviceName = CpcIOModule;
  map.clear();

  blret &= getSubItemDescription(eIOMODULE_DEVMODEL, sField, 16);
  map.insert(make_pair(CpcModel , sField));

  blret &= getSubItemDescription(eIOMODULE_SERIALNUMBER, sField, 16);
  map.insert(make_pair(CpcSerialNo , sField));

  blret &= getOneByteSubItemCHexNumberAsString(eIOMODULE_DEVTYPE, sField);

  blret &= getSubItemBytesAsDelimitedHexString(eIOMODULE_DEVFUNCTIONS, sField);
  map.insert(make_pair(CpcFunctions , sField));

  return blret;
}


ISpotMessage::BufferType  SpotMessageOPTHwrInfoResponse::serialize (void) const
{
  // First of all: serialize the header.
  BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

  // Sets the number of items in the request message.
  SizeType uTotalLength = m_TLVvector.getLength();
  retBuffer.push_back( static_cast<unsigned char>(uTotalLength) );

  // Copy the item codes to the request message.
  for(unsigned int uIdx = 0; uIdx < uTotalLength; uIdx++)
  {
    const TagLengthValueVector::TagLengthValue* pTLVItem  = m_TLVvector.getElement(uIdx);
    const unsigned char*                        pItemData = pTLVItem->getBytes();
    SizeType                                    uItemLength = pTLVItem->getLength();

    retBuffer.push_back( static_cast<unsigned char>(pTLVItem->getTagID()) );
    retBuffer.push_back( static_cast<unsigned char>(uItemLength) );
    retBuffer.insert(retBuffer.end(), pItemData , pItemData + uItemLength); // appending operation
  }

  std::cout << "SpotMessageOPTHwrInfoResponse::serialize() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return retBuffer; // TODO: Fixing ugly return vectors by value.
}

bool SpotMessageOPTHwrInfoResponse::parse (BufferType const & buff)
{
  if(buff.size() <= CMinMessageSize)  return false; // message too short

  if( buff[CMessageItemQtyOffset] == 0 )
  {
    return false; // This response has no item codes, which is only allowed in requests, but not in a reply message.
  }

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff); // Length and AckCode are get by this method.

  if( !blRet )
  {
    std::cout << "Bad header parsed!!!\n";
    return false; // malformed header
  }

  unsigned int nItems =  buff[CMessageItemQtyOffset];

  std::cout << std::dec << "SpotMessageOPTHwrInfoResponse::parse() - Length: " << size() << " bytes. ACK = "
  << getAckCode() << " . Items: " << nItems << std::endl;

  if(getAckCode() != CMessageResponseAckOK || nItems == 0)
  {
    std::cout << "NOTHING TO DO!\n";
    return false;
  }

  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &buff.front(), size())
            << std::endl << std::endl;
  delete[] pcBuffer;

  blRet = m_TLVvector.parseSourceData(nItems, &buff.front() + CMessageItemDataOffset);

  if( !blRet )
  {
    std::cout << "SpotMessageOPTHwrInfoResponse::parse() - FAILED !!\n";
    return false; // malformed message body
  }

  return true;
}


bool SpotMessageOPTHwrInfoResponse::getSubItemDescription(int iTagID, std::string & sOut,
                                                          ISpotMessage::SizeType maxLength /* = 0 */) const
{
  ISpotMessage::SizeType uSubItemLength = 0;
  const unsigned char * pbData = NULL;
  const TagLengthValueVector::TagLengthValue* pTLV = m_TLVvector.getElementByID(iTagID);

  if( pTLV && (uSubItemLength = pTLV->getLength()) > 0  &&  (pbData = pTLV->getBytes()) != NULL )
  {
    if( maxLength == 0 ) maxLength = CMaxItemLength; // default maximum value.

    ISpotMessage::SizeType uLength = ( uSubItemLength < maxLength ? uSubItemLength : maxLength );
    char abItemText[uLength + 1];
    memcpy(abItemText, pbData, uLength);
    abItemText[uLength] = '\0'; // items are sometimes just byte vector with no string terminators.
    sOut = abItemText;
    return true;
  }
  else // sub-item not found
  {
    sOut.clear();
    return false;
  }
}

bool SpotMessageOPTHwrInfoResponse::getOneByteIntegerSubItem(int iTagID, int & iOut) const
{
  SizeType uIdx = 0;
  SizeType uLength = 1;
  iOut = -1;
  const unsigned char * pbData = m_TLVvector.getElementDataByID(iTagID, uIdx, uLength);
  if( pbData && uLength > 0 )
  {
    iOut = static_cast<int>( *pbData );
    return true;
  }
  else // data not found, some error happened.
  {
    return false;
  }

}

bool SpotMessageOPTHwrInfoResponse::getFourByteDecimalIntegerSubItem(int iTagID, std::string & sOut) const
{
  SizeType uIdx = 0;
  SizeType uLength = 2;
  const unsigned char * pbData = m_TLVvector.getElementDataByID(iTagID, uIdx, uLength);
  if( pbData && uLength > 0 )
  {
    unsigned int uhw = 256 * pbData[0] + pbData[1];
    unsigned int ulw = 256 * pbData[2] + pbData[3];
    char acDecText[16];
    snprintf( acDecText, 16, "%u", 256 * 256 * uhw + ulw);
    acDecText[15] = '\0'; // safety
    sOut = acDecText;
    return true;
  }
  else // data not found, some error happened.
  {
    sOut.clear();
    return false;
  }
}

bool SpotMessageOPTHwrInfoResponse::getOneByteSubItemCHexNumberAsString(int iTagID, std::string & sOut) const
{
  SizeType uIdx = 0;
  SizeType uLength = 1;
  const unsigned char * pbData = m_TLVvector.getElementDataByID(iTagID, uIdx, uLength);
  if( pbData && uLength > 0 )
  {
    char acHexText[8];
    snprintf( acHexText, 8, "0x%02X", *pbData );
    acHexText[5] = '\0'; // safety
    sOut = acHexText;
    return true;
  }
  else // data not found, some error happened.
  {
    sOut.clear();
    return false;
  }
}

bool SpotMessageOPTHwrInfoResponse::getFourByteSubItemCHexNumberAsString(int iTagID, std::string & sOut) const
{
  SizeType uIdx = 0;
  SizeType uLength = 0;
  const unsigned char * pbData = m_TLVvector.getElementDataByID(iTagID, uIdx, uLength);
  if( pbData && uLength > 0 )
  {
    char acHexText[12];
    snprintf( acHexText, 11, "0x%02X%02X%02X%02X", pbData[0], pbData[1], pbData[2], pbData[3] );
    acHexText[11] = '\0'; // safety
    sOut = acHexText;
    return true;
  }
  else // data not found, some error happened.
  {
    sOut.clear();
    return false;
  }
}

bool SpotMessageOPTHwrInfoResponse::getSubItemBytesAsDelimitedHexString (
int iTagID,
std::string & sOut,
const char* szSeparator /* = NULL */) const

{
  SizeType uIdx = 0;
  SizeType uLength = 0;
  const unsigned char * pbData = m_TLVvector.getElementDataByID(iTagID, uIdx, uLength);

  sOut.clear();

  if( pbData == NULL || uLength == 0 )
  {
    return false;
  }

  const char * pcSeparator = ( szSeparator != NULL ? szSeparator : " ");
  uIdx = 0;

  while(uIdx < uLength)
  {
    unsigned int ub = pbData[uIdx++];
    char acHexText[4];

    snprintf( acHexText, 3, "%02X", ub);
    acHexText[2] = '\0'; // safety
    acHexText[3] = '\0'; // safety
    sOut += acHexText;
    if(uIdx < uLength - 1) // if it is not the last byte to convert
    {
      sOut += pcSeparator;
    }
  }

  return true;
}



} // namespace gvr

} // namespace communication

} // namespace spot


