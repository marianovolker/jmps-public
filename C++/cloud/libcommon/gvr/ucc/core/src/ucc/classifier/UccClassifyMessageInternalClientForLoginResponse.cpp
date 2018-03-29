/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccClassifyMessageInternalClientForLoginResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/classifier/UccClassifyMessageInternalClientForLoginResponse.h>
#include <gvr/communication/ucc/message/UccMessageLoginResponse.h>
//#include <gvr/communication/ucc/message/UccMessageOther.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      IUccClassifyMessage::IntegrityType UccClassifyMessageInternalClientForLoginResponse::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SizeType structureSize = 0;
        gvr::utils::Json json;
        unsigned char flag = 0;
        offset = 0;
        IntegrityType integrity = parseToStructure(buff,size,structureSize,flag,json);
        if (integrity == IUccClassifyMessage::Integrity::CComplete)
        {
          if (json.get(AbsUccMessageInternal::COperationName).isString())
          {
            std::string operation = json.get(AbsUccMessageInternal::COperationName).asString();
            if (operation.empty() == false)
            {
              if (operation.compare(UccMessageLoginResponse::CMessageOperation) == 0)
              {
                typeId = UccMessageLoginResponse::GetTypeId();
              }
              else
              {
                integrity = IUccClassifyMessage::Integrity::CUnknown;
              }
            }
            else
            {
              integrity = IUccClassifyMessage::Integrity::CUnknown;
            }
          }
          else
          {
            integrity = IUccClassifyMessage::Integrity::CUnknown;
          }

          if (integrity == IUccClassifyMessage::Integrity::CComplete)
          {
            key = json.get(AbsUccMessageInternal::CMessageIdName).asCString();
          }
        }
        return integrity;
      }

      UccClassifyMessageInternalClientForLoginResponse::~UccClassifyMessageInternalClientForLoginResponse (void)
      {
      }
    }
  }
}
