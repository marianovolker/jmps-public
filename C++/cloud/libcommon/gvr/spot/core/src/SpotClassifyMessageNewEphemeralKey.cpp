/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageNewEphemeralKey.cpp
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotClassifyMessageNewEphemeralKey.h"

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageNewEphemeralKey::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];
        if(buff.size() < datasize)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position++] != SpotMessageNewEphemeralKey::CMessageRequestPinpadAppId) //appId
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position++] != SpotMessageNewEphemeralKey::CDefaulNoEncryptionValue) //ssk
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position++] != SpotMessageNewEphemeralKey::CPinpadExtendedMessageCmdId)//command
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        if(buff[position++] != ISpotMessage::CEphemeralKeySubCmdId || buff[position++] != ISpotMessage::CNewEphemeralKeySubSubCmdId)//subcommand
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        offset = 0;
        size = datasize;

        typeId = SpotMessageNewEphemeralKey::GetTypeId();

        key =  (SpotMessageNewEphemeralKey::CMessageRequestPinpadAppId << 24);
        key +=  (SpotMessageNewEphemeralKey::CPinpadExtendedMessageCmdId << 16);
        key += (SpotMessageNewEphemeralKey::CEphemeralKeySubCmdId << 8);
        key += SpotMessageNewEphemeralKey::CNewEphemeralKeySubSubCmdId;

        return ISpotClassifyMessage::Integrity::CComplete;
      }

      SpotClassifyMessageNewEphemeralKey::~SpotClassifyMessageNewEphemeralKey (void)
      {
      }
    }
  }
}
