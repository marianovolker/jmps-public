/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotClassifyMessagePing.cpp
 * @author mgrieco
 * @date Friday, January 14, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include "../include/gvr/communication/spot/SpotClassifyMessagePing.h"

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {
      ISpotClassifyMessage::IntegrityType SpotClassifyMessagePing::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SpotMessagePing message = SpotMessagePing();
        if( !message.parse(buff) )
        {
          return ISpotClassifyMessage::Integrity::CUnknown;
        }

        offset = 0;
        size = message.size();
        typeId = message.getTypeId();
        key = message.getKey();
        return ISpotClassifyMessage::Integrity::CComplete;
      }

      SpotClassifyMessagePing::~SpotClassifyMessagePing (void)
      {
      }
    }
  }
}
