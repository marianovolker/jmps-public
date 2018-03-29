/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageSoftwareInfoResponse.cpp
 * @author Guillermo Paris
 * @date Monday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <cstring>
#include <iostream>
#include <gvr/communication/spot/Utils.h>
#include <gvr/communication/spot/SpotMessageSoftwareInfoResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{

/// @brief Communication Domain
namespace communication
{

/// @brief Spot Domain
namespace spot
{


typedef SpotMessageSoftwareInfoResponse::ItemID       ItemID;
typedef SpotMessageSoftwareInfoResponse::ItemIDLabel  ItemIDLabel;

const char * SpotMessageSoftwareInfoResponse::CpcFirmware   = "firmware";
const char * SpotMessageSoftwareInfoResponse::CpcSecurity   = "security";
const char * SpotMessageSoftwareInfoResponse::CpcKeypad     = "keypad";
const char * SpotMessageSoftwareInfoResponse::CpcCardReader = "card reader";

const ItemIDLabel  SpotMessageSoftwareInfoResponse::aCItemLabels[SpotMessageSoftwareInfoResponse::CMaxItems] =
{
  { eSPOT_OS_VERSION,                     "OS"},
  { eSWF_PKG1,                            "Pinpad"},
  { eSWF_PKG2,                            "EMV"},
  { eSWF_PKG3,                            "Setup"},
  { eSWF_PKG4,                            "ROMfs4"},
  { eSWF_PKG5,                            "ROMfs5"},
  { eSWF_PKG6,                            "ROMfs6"},
  { eSWF_PKG7,                            "CRINDBIOS"},
  { eSWF_PKG8,                            "ROMfs8"},
  { eFWR_CR_VER,                          "Card Reader firmware"},
  { eFWR_KP_VER,                          "Keypad firmware"},
  { eFWR_SEC_VER,                         "Security Module version"},
  { eM7_SPOT_SYS_BUILD_VERSION,           "Build version"},
  { eM7_CR_FWR_VERSION,                   "Card Reader firmware"},
  { eM7_SPOT_M7_FW_VERSION,               "Vanguard PCI string identify"},
  { eM7_SPOT_SYS_PCI_APPLICATION_VERSION, "Vanguard PCI APPLICATION version"}
};

SpotMessageSoftwareInfoResponse::SpotMessageSoftwareInfoResponse (void)
:  AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CSoftwareInfoCategoryId, CSoftwareInfoCommandId)
{
}

SpotMessageSoftwareInfoResponse::~SpotMessageSoftwareInfoResponse (void)
{
}


unsigned int SpotMessageSoftwareInfoResponse::getItemQty (void)
{
  return m_TLVvector.getLength();
}

const unsigned char * SpotMessageSoftwareInfoResponse::getItemData (ISpotMessage::SizeType   uIndex,
                                                                    unsigned int &           iTagID,
                                                                    ISpotMessage::SizeType & length) const
{
  if( uIndex >= m_TLVvector.getLength() )
  {
    length = 0;
    return NULL; // index out of bound
  }

  return m_TLVvector.getElementData(uIndex, iTagID, length);
}

bool SpotMessageSoftwareInfoResponse::getAllSoftwareDescription (std::string & sLabel,
                                                                 SoftwareInfoMap & map ) const
{
  bool blret = true;

  sLabel = CpcFirmware;
  map.clear();

  std::string sItemLabel, sField;

  for(int i = 0; i < CMaxItems; i++)
  {
    bool blresult = true;

    blresult &= getSubItemLabel(aCItemLabels[i].itemID, sItemLabel);
    blresult &= getSubItemDescription(aCItemLabels[i].itemID, sField, 64);

    if( blresult )
    {
      map.insert(make_pair(sItemLabel, sField));
    }

    blret &= blresult;
  }

  return blret;
}

bool SpotMessageSoftwareInfoResponse::getCardReaderFirmwareVersion (std::string & sLabel,
                                                                    std::string & sFwrDescrip) const
{
  bool blret = true;
  blret &= getSubItemLabel(eFWR_CR_VER, sLabel);
  blret &= getSubItemDescription(eFWR_CR_VER, sFwrDescrip, 16);
  return blret;
}

bool SpotMessageSoftwareInfoResponse::getKeypadFirmwareVersion (std::string & sLabel,
                                                                std::string & sFwrDescrip) const
{
  bool blret = true;
  blret &= getSubItemLabel(eFWR_KP_VER, sLabel);
  blret &= getSubItemDescription(eFWR_KP_VER, sFwrDescrip, 16);
  return blret;
}

bool SpotMessageSoftwareInfoResponse::getSecurityModuleFirmwareVersion (std::string & sLabel,
                                                                        std::string & sFwrDescrip) const
{
  bool blret = true;
  blret &= getSubItemLabel(eFWR_SEC_VER, sLabel);
  blret &= getSubItemDescription(eFWR_SEC_VER, sFwrDescrip, 16);
  return blret;
}


ISpotMessage::BufferType  SpotMessageSoftwareInfoResponse::serialize (void) const
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

  std::cout << "SpotMessageSoftwareInfoResponse::serialize() - Length: " << size() << " bytes.\n";
  char* pcBuffer = new char[3 * size() + 2];
  std::cout << "\nMessage Data: " << Utils::dumpBuffer(pcBuffer, &retBuffer.front(), size()) << std::endl << std::endl;
  delete[] pcBuffer;

  return retBuffer; // TODO: Fixing ugly return vectors by value.
}

bool SpotMessageSoftwareInfoResponse::parse (BufferType const & buff)
{
  if(buff.size() <= CMinMessageSize)  return false; // message too short

  if( buff[CMessageItemQtyOffset] == 0 )
  {
    return false; // This response has no item codes, which is only allowed in requests, but not in a reply message.
  }

  bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff); // Length and AckCode are get by this method.

  if( !blRet ) return false; // malformed header

  unsigned int nItems =  buff[CMessageItemQtyOffset];

  std::cout << std::dec << "SpotMessageSoftwareInfoResponse::parse() - Length: " << size() << " bytes. ACK = "
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
    std::cout << "SpotMessageSoftwareInfoResponse::parse() - FAILED !!\n";
    return false; // malformed message body
  }

  return true;
}


bool SpotMessageSoftwareInfoResponse::getSubItemDescription(int iTagID, std::string & sOut,
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

bool SpotMessageSoftwareInfoResponse::getSubItemLabel(int iTagID, std::string & sOut) const
{
  for(int i = 0; i < CMaxItems; i++)
  {
    if( iTagID == aCItemLabels[i].itemID )
    {
      sOut = aCItemLabels[i].itemLabel;
      return true;
    }
  }

  sOut.clear();
  return false;
}



} // namespace gvr

} // namespace communication

} // namespace spot


