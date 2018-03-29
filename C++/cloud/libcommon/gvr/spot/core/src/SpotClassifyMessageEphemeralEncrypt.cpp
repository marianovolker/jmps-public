/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageEphemeralEncrypt.cpp
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageEphemeralEncrypt.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageEphemeralEncrypt::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        int position(0);
        int datasize = (((std::size_t)buff[position+0])<<8) | buff[position+1];
        if(buff.size() < datasize)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+2] != SpotMessageEphemeralEncrypt::CMessageResponsePinpadAppId) //appId
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+3] != SpotMessageEphemeralEncrypt::CDefaulNoEncryptionValue) //ssk
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+4] != SpotMessageEphemeralEncrypt::CPinpadExtendedMessageCmdId)//command
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        if(buff[position+5] != ISpotMessage::CEphemeralKeySubCmdId)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        unsigned char subCmd = buff[position+6];
        if(subCmd != ISpotMessage::CEphemeralEncryptSubCmdId && subCmd != ISpotMessage::CEphemeralDecryptSubCmdId)//subcommand
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        offset = 0;
        size = datasize;
        typeId = SpotMessageEphemeralEncrypt::GetTypeId();

        key =  (SpotMessageEphemeralEncrypt::CMessageRequestPinpadAppId << 24);
        key +=  (SpotMessageEphemeralEncrypt::CPinpadExtendedMessageCmdId << 16);
        key += (SpotMessageEphemeralEncrypt::CEphemeralKeySubCmdId << 8);
        key += SpotMessageEphemeralEncrypt::CEphemeralEncryptSubCmdId;

        return ISpotClassifyMessage::Integrity::CComplete;
      }

      SpotClassifyMessageEphemeralEncrypt::~SpotClassifyMessageEphemeralEncrypt (void)
      {
      }
    }
  }
}
