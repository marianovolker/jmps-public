/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccClassifyMessageCloudServer.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/classifier/UccClassifyMessageCloudServer.h>
#include <gvr/communication/ucc/message/base/AbsUccMessageCloudCommandResponse.h>
#include <gvr/communication/ucc/message/base/AbsUccMessageCloudCommandRequest.h>
#include <gvr/communication/ucc/message/UccMessageCloudInstallStatus.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadStatusEvent.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileUploadStatusEvent.h>


/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      IUccClassifyMessage::IntegrityType UccClassifyMessageCloudServer::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SizeType structureSize = 0;
        gvr::utils::Json json;
        unsigned char flag = 0;
        offset = 0;
        IntegrityType integrity = parseToStructure(buff,size,structureSize,flag,json);
        if (integrity == IUccClassifyMessage::Integrity::CComplete)
        {
          if (flag == AbsUccMessageCloud::CInteralFlagCloudMessage)
          {
            if (json.get(AbsUccMessageCloudEventRequest::COperationName).isString())
            {
              std::string operation = json.get(AbsUccMessageCloudCommandRequest::COperationName).asString();
              if (operation.empty() == false)
              {
                if (operation.compare(UccMessageCloudInstallStatus::CMessageOperation) == 0)
                {
                  typeId = UccMessageCloudInstallStatus::GetTypeId();
                }
                if (operation.compare(UccMessageCloudFileDownloadStatusEvent::CMessageOperation) == 0)
                {
                  typeId = UccMessageCloudFileDownloadStatusEvent::GetTypeId();
                }
                if (operation.compare(UccMessageCloudFileUploadStatusEvent::CMessageOperation) == 0)
                {
                  typeId = UccMessageCloudFileUploadStatusEvent::GetTypeId();
                }
//TODO: Add new messages comparisons for ucc cloud messages which has to be recognize by the client side.
//              else if (operation.compare(UccMessageCloudOther::CMessageOperation) == 0)
//              {
//                typeId = UccMessageCloudOther::GetTypeId();
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
            else if (json.get(AbsUccMessageCloudCommandResponse::CMessageIdName).isString())
            {
              typeId = AbsUccMessageCloudCommandResponse::GetTypeId();
              key = json.get(AbsUccMessageCloudCommandResponse::CMessageIdName).asCString();
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
        return integrity;
      }

      UccClassifyMessageCloudServer::~UccClassifyMessageCloudServer (void)
      {
      }
    }
  }
}
