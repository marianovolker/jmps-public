/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccClassifyMessageRegister.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/base/AbsUccMessageRegister.h>
#include <gvr/communication/ucc/classifier/UccClassifyMessageRegister.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      IUccClassifyMessage::IntegrityType UccClassifyMessageRegister::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SizeType structureSize = 0;
        gvr::utils::Json json;
        unsigned char flag = 0;
        offset = 0;
        IntegrityType integrity = parseToStructure(buff,size,structureSize,flag,json);
        if (integrity == IUccClassifyMessage::Integrity::CComplete)
        {
          if (flag == AbsUccMessageRegister::CInteralFlagRegisterMessage)
          {
            typeId = AbsUccMessageRegister::GetTypeId();
            key = AbsUccMessageRegister::CDefaultKey;
          }
          else
          {
            integrity = IUccClassifyMessage::Integrity::CUnknown;
          }
        }
        return integrity;
      }

      UccClassifyMessageRegister::~UccClassifyMessageRegister (void)
      {
      }
    }
  }
}
