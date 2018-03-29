/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageNewEphemeralKeyResponse.cpp
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageNewEphemeralKeyResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageNewEphemeralKeyResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        int position(0);
        int datasize = (((std::size_t)buff[position+0])<<8) | buff[position+1];

        if(buff.size() < datasize)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+2] != SpotMessageNewEphemeralKeyResponse::CMessageResponsePinpadAppId) //appId
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+3] != SpotMessageNewEphemeralKeyResponse::CDefaulNoEncryptionValue) //ssk
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+4] != SpotMessageNewEphemeralKeyResponse::CPinpadExtendedMessageCmdId)//command
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+5] != SpotMessageNewEphemeralKeyResponse::CEphemeralKeySubCmdId) //subcommand
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+6] != SpotMessageNewEphemeralKeyResponse::CNewEphemeralKeySubSubCmdId) //subcommand
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        offset = 0;
        size = datasize;

        typeId = SpotMessageNewEphemeralKeyResponse::GetTypeId();

        key =  (SpotMessageNewEphemeralKeyResponse::CMessageResponsePinpadAppId << 24);
        key +=  (SpotMessageNewEphemeralKeyResponse::CPinpadExtendedMessageCmdId << 16);
        key += (SpotMessageNewEphemeralKeyResponse::CEphemeralKeySubCmdId << 8);
        key += SpotMessageNewEphemeralKeyResponse::CNewEphemeralKeySubSubCmdId;

        return ISpotClassifyMessage::Integrity::CComplete;
      }

      SpotClassifyMessageNewEphemeralKeyResponse::~SpotClassifyMessageNewEphemeralKeyResponse (void)
      {
      }
    }
  }
}
