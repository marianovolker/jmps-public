/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccClassifyMessageInternalServer.cpp
 * @author mgrieco
 * @date Monday, Apr 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/classifier/UccClassifyMessageInternalServer.h>
#include <gvr/communication/ucc/message/UccMessageFileDownload.h>
#include <gvr/communication/ucc/message/UccMessageSendToCloud.h>
#include <gvr/communication/ucc/message/UccMessageServerUpdate.h>
#include <gvr/communication/ucc/message/UccMessageConnect.h>
#include <gvr/communication/ucc/message/UccMessageTestDns.h>

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
      IUccClassifyMessage::IntegrityType UccClassifyMessageInternalServer::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SizeType structureSize = 0;
        gvr::utils::Json json;
        unsigned char flag = 0;
        offset = 0;
        IntegrityType integrity = parseToStructure(buff,size,structureSize,flag,json);
        if (integrity == IUccClassifyMessage::Integrity::CComplete)
        {
          if (flag == AbsUccMessageInternal::CInteralFlagInternalMessage)
          {
            if (json.get(AbsUccMessageInternal::COperationName).isString())
            {
              std::string operation = json.get(AbsUccMessageInternal::COperationName).asString();
              if (operation.empty() == false)
              {
                if (operation.compare(UccMessageFileDownload::CMessageOperation) == 0)
                {
                  typeId = UccMessageFileDownload::GetTypeId();
                }
                else if (operation.compare(UccMessageSendToCloud::CMessageOperation) == 0)
                {
                  typeId = UccMessageSendToCloud::GetTypeId();
                }
                else if (operation.compare(UccMessageServerUpdate::CMessageOperation) == 0)
                {
                  typeId = UccMessageServerUpdate::GetTypeId();
                }
                else if (operation.compare(UccMessageConnect::CMessageOperation) == 0)
                {
                  typeId = UccMessageConnect::GetTypeId();
                }
                else if (operation.compare(UccMessageTestDns::CMessageOperation) == 0)
                {
                  typeId = UccMessageTestDns::GetTypeId();
                }
//TODO: Add new messages comparisons for ucc internal messages which has to be recognize by the server side.
//              else if (operation.compare(UccMessageOther::CMessageOperation) == 0)
//              {
//                typeId = UccMessageOther::GetTypeId();
//              }
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
          else
          {
            integrity = IUccClassifyMessage::Integrity::CUnknown;
          }
        }
        return integrity;
      }

      UccClassifyMessageInternalServer::~UccClassifyMessageInternalServer (void)
      {
      }
    }
  }
}
