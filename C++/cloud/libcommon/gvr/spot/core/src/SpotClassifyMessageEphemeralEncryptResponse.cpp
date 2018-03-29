/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageEphemeralEncryptResponse.cpp
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageEphemeralEncryptResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageEphemeralEncryptResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        int position(0);
        int datasize = (((std::size_t)buff[position+0])<<8) | buff[position+1];

        if(buff.size() < datasize)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+2] != SpotMessageEphemeralEncryptResponse::CMessageResponsePinpadAppId) //appId
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+3] != SpotMessageEphemeralEncryptResponse::CDefaulNoEncryptionValue) //ssk
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+4] != SpotMessageEphemeralEncryptResponse::CPinpadExtendedMessageCmdId)//command
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+5] != ISpotMessage::CEphemeralKeySubCmdId)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        unsigned char subCmd = buff[position+6];
        if(subCmd != ISpotMessage::CEphemeralEncryptSubCmdId && subCmd != ISpotMessage::CEphemeralDecryptSubCmdId)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        offset = 0;
        size = datasize;

        typeId = SpotMessageEphemeralEncryptResponse::GetTypeId();

        key =  (SpotMessageEphemeralEncryptResponse::CMessageResponsePinpadAppId << 24);
        key +=  (SpotMessageEphemeralEncryptResponse::CPinpadExtendedMessageCmdId << 16);
        key += (SpotMessageEphemeralEncryptResponse::CEphemeralKeySubCmdId << 8);
        key += SpotMessageEphemeralEncryptResponse::CEphemeralEncryptSubCmdId;

        return ISpotClassifyMessage::Integrity::CComplete;
      }

      SpotClassifyMessageEphemeralEncryptResponse::~SpotClassifyMessageEphemeralEncryptResponse (void)
      {
      }
    }
  }
}
