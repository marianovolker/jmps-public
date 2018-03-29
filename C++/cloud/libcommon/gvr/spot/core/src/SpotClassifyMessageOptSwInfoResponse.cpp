/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageOptSwInfoResponse.cpp
 * @author mgrieco
 * @date Monday, May 11, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageOptSwInfoResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {

      ISpotClassifyMessage::IntegrityType SpotClassifyMessageOptSwInfoResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];
        if(buff.size() < datasize)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position++] != SpotMessageOptSwInfoResponse::CMessageResponseOPTAppId) //appId
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position++] != SpotMessageOptSwInfoResponse::CDefaulNoEncryptionValue) //ssk
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position++] != SpotMessageOptSwInfoResponse::COptSwInfoMessageResponseId)//command
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        if(buff[position++] != SpotMessageOptSwInfoResponse::COptSwInfoMessageResponseSubCmd)//sub-command
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        offset = 0;
        size = datasize;

        typeId = SpotMessageOptSwInfoResponse::GetTypeId();

        key = (SpotMessageOptSwInfoResponse::COptSwInfoMessageResponseId << 8);
        key += SpotMessageOptSwInfoResponse::COptSwInfoMessageResponseSubCmd;

        return ISpotClassifyMessage::Integrity::CComplete;
      }

      SpotClassifyMessageOptSwInfoResponse::~SpotClassifyMessageOptSwInfoResponse (void)
      {
      }
    }
  }
}
