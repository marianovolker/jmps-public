/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessageOptSwInfo.cpp
 * @author mgrieco
 * @date Monday, May 11, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/spot/SpotClassifyMessageOptSwInfo.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {

      ISpotClassifyMessage::IntegrityType SpotClassifyMessageOptSwInfo::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SpotMessageOptSwInfo message = SpotMessageOptSwInfo();
        int position(0);
        int datasize = (((std::size_t)buff[position++])<<8) | buff[position++];
        if(buff.size() < datasize)
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position++] != SpotMessageOptSwInfo::CMessageResponseOPTAppId) //appId
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position++] != SpotMessageOptSwInfo::CDefaulNoEncryptionValue) //ssk
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }
        if(buff[position++] != SpotMessageOptSwInfo::COptSwInfoMessageId)//command
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        if(buff[position++] != SpotMessageOptSwInfo::COptSwInfoMessageSubCmd)//sub-command
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        offset = 0;
        size = datasize;

        typeId = SpotMessageOptSwInfo::GetTypeId();

        key = (ISpotMessage::CEphemeralKeySubCmdId << 8)+ ISpotMessage::CNewEphemeralKeySubSubCmdId;

        return ISpotClassifyMessage::Integrity::CComplete;
      }

      SpotClassifyMessageOptSwInfo::~SpotClassifyMessageOptSwInfo (void)
      {
      }
    }
  }
}
