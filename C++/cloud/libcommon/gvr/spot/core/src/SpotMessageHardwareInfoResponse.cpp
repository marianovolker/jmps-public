/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageHardwareInfoResponse.cpp
 * @author Guillermo Paris
 * @date Monday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstring>
#include <cstdio>
#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessageHardwareInfoResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


const char * SpotMessageHardwareInfoResponse::CpcKeypad      = "keypad";
const char * SpotMessageHardwareInfoResponse::CpcCardReader  = "card reader";
const char * SpotMessageHardwareInfoResponse::CpcDisplay     = "display";

const char * SpotMessageHardwareInfoResponse::CpcSerialNo    = "serial number";
const char * SpotMessageHardwareInfoResponse::CpcHwrID       = "hardware ID";
const char * SpotMessageHardwareInfoResponse::CpcHwrConf     = "hardware configuration";
const char * SpotMessageHardwareInfoResponse::CpcType        = "type";
const char * SpotMessageHardwareInfoResponse::CpcModel       = "model";
const char * SpotMessageHardwareInfoResponse::CpcFunctions   = "functions";
const char * SpotMessageHardwareInfoResponse::CpcXresolution = "X resolution";
const char * SpotMessageHardwareInfoResponse::CpcYresolution = "Y resolution";
const char * SpotMessageHardwareInfoResponse::CpcColorDepth  = "color depth";

const char * SpotMessageHardwareInfoResponse::CpcDisplayClueForM3  = "MCF5";
const char * SpotMessageHardwareInfoResponse::CpcDisplayClueForM5  = "OMAP";
const char * SpotMessageHardwareInfoResponse::CpcDisplayClue1ForM7 = "UPM";
const char * SpotMessageHardwareInfoResponse::CpcDisplayClue2ForM7 = "LCD module";

const char * SpotMessageHardwareInfoResponse::CpcM3  = "M3";
const char * SpotMessageHardwareInfoResponse::CpcM5  = "M5";
const char * SpotMessageHardwareInfoResponse::CpcM7  = "M7";
const char * SpotMessageHardwareInfoResponse::CpcUnknown  = "unknown";

SpotMessageHardwareInfoResponse::SpotMessageHardwareInfoResponse (void)
:  AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CHardwareInfoCategoryId, CHardwareInfoCommandId)
{
}

SpotMessageHardwareInfoResponse::~SpotMessageHardwareInfoResponse (void)
{
}


unsigned int SpotMessageHardwareInfoResponse::getItemQty (void)
{
  return m_TLVvector.getLength();
}

const unsigned char * SpotMessageHardwareInfoResponse::getItemData (ISpotMessage::SizeType uIndex,
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

bool SpotMessageHardwareInfoResponse::getSerialNumber (std::string & sOut) const
{
  return getSubItemDescription(eSPOT_SERIALNUMBER, sOut, 16);
}

bool SpotMessageHardwareInfoResponse::getHardwareId (std::string & sOut) const
{
  return getTwoByteSubItemCHexNumberAsString(eSECURITY_HWRID, sOut);
}

bool SpotMessageHardwareInfoResponse::getSpotModel (std::string & sOut) const
{
  std::string sDisplayModel;

  if( ! getSubItemDescription(eDISPLAY_DEVMODEL, sDisplayModel, 16) )
  {
    sOut.clear();
    return false; // Display device type not found in reply.
  }

  if( sDisplayModel.compare( 0,std::strlen(CpcDisplayClueForM3),CpcDisplayClueForM3,std::strlen(CpcDisplayClueForM3) ) == 0 )
  {
    sOut = CpcM3;
  }
  else if( sDisplayModel.compare( 0,std::strlen(CpcDisplayClueForM5),CpcDisplayClueForM5,std::strlen(CpcDisplayClueForM5) ) == 0 )
  {
    sOut = CpcM5;
  }
  else if( sDisplayModel.compare( 0,std::strlen(CpcDisplayClue1ForM7),CpcDisplayClue1ForM7,std::strlen(CpcDisplayClue1ForM7) ) == 0)
  {
    sOut = CpcM7;
  }
  else if( sDisplayModel.compare( 0,std::strlen(CpcDisplayClue2ForM7),CpcDisplayClue2ForM7,std::strlen(CpcDisplayClue2ForM7) ) == 0)
  {
    sOut = CpcM7;
  }
  else // SPOT type could not be properly identified.
  {
    sOut = CpcUnknown;
  }

  return true;
}

bool SpotMessageHardwareInfoResponse::getKeypadInfo (std::string & sDeviceName, DeviceInfoMap & map) const
{
  bool blret = true;
  std::string sField;

  sDeviceName = CpcKeypad;
  map.clear();

  blret &= getSubItemDescription(eKEYBOARD_SERIALNUMBER, sField, 16);
  map.insert(make_pair(CpcSerialNo , sField));

  blret &= getTwoByteSubItemCHexNumberAsString(eKEYBOARD_HWRID, sField);
  map.insert(make_pair(CpcHwrID , sField));

  blret &= getOneByteSubItemCHexNumberAsString(eKEYBOARD_DEVTYPE, sField);
  map.insert(make_pair(CpcType , sField));

  return blret;
}

bool SpotMessageHardwareInfoResponse::getCardReaderInfo (std::string & sDeviceName, DeviceInfoMap & map) const
{
  bool blret = true;
  std::string sField;

  sDeviceName = CpcCardReader;
  map.clear();

  blret &= getSubItemDescription(eCARDREADER_SERIALNUMBER, sField, 16);
  map.insert(make_pair(CpcSerialNo , sField));

  blret &= getTwoByteSubItemCHexNumberAsString(eCARDREADER_HWRID, sField);
  map.insert(make_pair(CpcHwrID , sField));

  blret &= getOneByteSubItemCHexNumberAsString(eCARDREADER_DEVTYPE, sField);
  map.insert(make_pair(CpcType , sField));

  blret &= getOneByteSubItemCHexNumberAsString(eCARDREADER_DEVFUNCTION, sField);
  map.insert(make_pair(CpcFunctions , sField));

  blret &= getSubItemDescription(eCARDREADER_DEVMODEL, sField, 16);
  map.insert(make_pair(CpcModel , sField));

  return blret;
}

bool SpotMessageHardwareInfoResponse::getDisplayInfo (std::string & sDeviceName, DeviceInfoMap & map) const
{
  bool blret = true;
  std::string sField;

  sDeviceName = CpcDisplay;
  map.clear();

  blret &= getOneByteSubItemCHexNumberAsString(eDISPLAY_DEVTYPE, sField);
  map.insert(make_pair(CpcType , sField));

  blret &= getSubItemBytesAsDelimitedHexString(eDISPLAY_DEVFUNCTION, sField);
  map.insert(make_pair(CpcFunctions , sField));

  blret &= getSubItemDescription(eDISPLAY_DEVMODEL, sField, 16);
  map.insert(make_pair(CpcModel , sField));

  SizeType uIdx = 0;
  SizeType uLength = 5;
  const unsigned char * pbData = m_TLVvector.getElementDataByID(eDISPLAY_DEVHWRCONFIG, uIdx, uLength);
  if( pbData == NULL || uLength == 0 )
  {
    return false; // data not found, some error happened.
  }

  unsigned int ubh = pbData[0];
  unsigned int ubl = pbData[1];
  char acHexText[8];

  snprintf( acHexText, 8, "%u", 256 * ubh + ubl );
  acHexText[6] = '\0'; // safety
  sField = acHexText;
  map.insert(make_pair(CpcXresolution , sField));

  ubh = pbData[2];
  ubl = pbData[3];
  snprintf( acHexText, 8, "%u", 256 * ubh + ubl );
  acHexText[6] = '\0'; // safety
  sField = acHexText;
  map.insert(make_pair(CpcYresolution , sField));

  ubl = pbData[4];
  snprintf( acHexText, 8, "%u", ubl);
  acHexText[4] = '\0'; // safety
  sField = acHexText;
  map.insert(make_pair(CpcColorDepth , sField));

  return blret;

}

ISpotMessage::BufferType  SpotMessageHardwareInfoResponse::serialize (void) const
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

  std::cout << "SpotMessageHardwareInfoResponse::serialize() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return retBuffer; // TODO: Fixing ugly return vectors by value.
}

bool SpotMessageHardwareInfoResponse::parse (BufferType const & buff)
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

  std::cout << std::dec << "SpotMessageHardwareInfoResponse::parse() - Length: " << size() << " bytes. ACK = "
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
    std::cout << "SpotMessageHardwareInfoResponse::parse() - FAILED !!\n";
    return false; // malformed message body
  }

  return true;
}

bool SpotMessageHardwareInfoResponse::getSubItemDescription(int iTagID, std::string & sOut,
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

bool SpotMessageHardwareInfoResponse::getTwoByteDecimalIntegerSubItem(int iTagID, std::string & sOut) const
{
  SizeType uIdx = 0;
  SizeType uLength = 2;
  const unsigned char * pbData = m_TLVvector.getElementDataByID(iTagID, uIdx, uLength);
  if( pbData && uLength > 0 )
  {
    unsigned int uhb = pbData[0];
    unsigned int ulb = pbData[1];
    char acDecText[8];
    snprintf( acDecText, 8, "%u", 256 * uhb + ulb);
    acDecText[7] = '\0'; // safety
    sOut = acDecText;
    return true;
  }
  else // data not found, some error happened.
  {
    sOut.clear();
    return false;
  }
}

bool SpotMessageHardwareInfoResponse::getOneByteSubItemCHexNumberAsString(int iTagID, std::string & sOut) const
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

bool SpotMessageHardwareInfoResponse::getTwoByteSubItemCHexNumberAsString(int iTagID, std::string & sOut) const
{
  SizeType uIdx = 0;
  SizeType uLength = 2;
  const unsigned char * pbData = m_TLVvector.getElementDataByID(iTagID, uIdx, uLength);
  if( pbData && uLength > 0 )
  {
    unsigned int uhb = pbData[0];
    unsigned int ulb = pbData[1];
    char acHexText[8];
    snprintf( acHexText, 8, "0x%02X%02X", uhb, ulb);
    acHexText[7] = '\0'; // safety
    sOut = acHexText;
    return true;
  }
  else // data not found, some error happened.
  {
    sOut.clear();
    return false;
  }
}

bool SpotMessageHardwareInfoResponse::getSubItemBytesAsDelimitedHexString (
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


