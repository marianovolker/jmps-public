/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageLoginResponse.cpp
 * @author mgrieco
 * @date Friday, January 14, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <gvr/communication/spot/SpotClassifyMessageLoginResponse.h>

# include <gvr/communication/spot/ISpotMessageWithAppCatCmdID.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      ISpotClassifyMessage::IntegrityType SpotClassifyMessageLoginResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        int position(0);
        int datasize = (((std::size_t)buff[position+0])<<8) | buff[position+1];
        if(buff.size() < datasize)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+2] != SpotMessageLoginResponse::CMessageResponseSpotAppId) //appId
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+3] != SpotMessageLoginResponse::CDefaulNoEncryptionValue) //ssk
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position+4] != SpotMessageLoginResponse::CSpotMessageId)//command
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        offset = 0;
        size = datasize;
        typeId = SpotMessageLoginResponse::GetTypeId();

        key  = (SpotMessageLoginResponse::CMessageResponseSpotAppId << 16);
        key += (SpotMessageLoginResponse::CDefaulNoEncryptionValue << 8);
        key += SpotMessageLoginResponse::CSpotMessageId;

        return ISpotClassifyMessage::Integrity::CComplete;
      }

      SpotClassifyMessageLoginResponse::~SpotClassifyMessageLoginResponse (void)
      {
      }
    }
  }
}
