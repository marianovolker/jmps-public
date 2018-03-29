/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccClassifyMessageCloudClient.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/classifier/UccClassifyMessageCloudClient.h>
#include <gvr/communication/ucc/message/base/AbsUccMessageCloudEventResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudReset.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownload.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileUpload.h>
#include <gvr/communication/ucc/message/UccMessageCloudInstall.h>
#include <gvr/communication/ucc/message/UccMessageCloudSecurityChange.h>
#include <gvr/communication/ucc/message/UccMessageCloudReconnect.h>


//#include <gvr/communication/ucc/message/UccMessageCloudOther.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      IUccClassifyMessage::IntegrityType UccClassifyMessageCloudClient::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
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
            if (json.get(AbsUccMessageCloudCommandRequest::COperationName).isString())
            {
              std::string operation = json.get(AbsUccMessageCloudCommandRequest::COperationName).asString();
              if (operation.empty() == false)
              {
                if (operation.compare(UccMessageCloudReset::CMessageOperation) == 0)
                {
                  typeId = UccMessageCloudReset::GetTypeId();
                }
                else if (operation.compare(UccMessageCloudFileDownload::CMessageOperation) == 0)
                {
                  typeId = UccMessageCloudFileDownload::GetTypeId();
                }
                else if (operation.compare(UccMessageCloudFileUpload::CMessageOperation) == 0)
                {
                  typeId = UccMessageCloudFileUpload::GetTypeId();
                }
                else if (operation.compare(UccMessageCloudInstall::CMessageOperation) == 0)
                {
                  typeId = UccMessageCloudInstall::GetTypeId();
                }
                else if (operation.compare(UccMessageCloudSecurityChange::CMessageOperation) == 0)
                {
                  typeId = UccMessageCloudSecurityChange::GetTypeId();
                }
                else if (operation.compare(UccMessageCloudReconnect::CMessageOperation) == 0)
                {
                  typeId = UccMessageCloudReconnect::GetTypeId();
                }
//TODO: Add new messages comparisons for ucc cloud messages which has to be recognize by the client side.
//                else if (operation.compare(UccMessageCloudOther::CMessageOperation) == 0)
//                {
//                  typeId = UccMessageCloudOther::GetTypeId();
//                }
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
            else if (json.get(AbsUccMessageCloudEventResponse::CMessageIdName).isString())
            {
              typeId = AbsUccMessageCloudEventResponse::GetTypeId();
              key = json.get(AbsUccMessageCloudEventResponse::CMessageIdName).asCString();
            }
            else
            {
              integrity = IUccClassifyMessage::Integrity::CUnknown;
            }

            if (integrity == IUccClassifyMessage::Integrity::CComplete)
            {
              key = json.get(AbsUccMessageCloudCommandRequest::CMessageIdName).asCString();
            }
          }
          else
          {
            integrity = IUccClassifyMessage::Integrity::CUnknown;
          }
        }
        return integrity;
      }

      UccClassifyMessageCloudClient::~UccClassifyMessageCloudClient (void)
      {
      }
    }
  }
}
