/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccClassifyMessageInternalClient.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/classifier/UccClassifyMessageInternalClient.h>
#include <gvr/communication/ucc/message/UccMessageFileDownloadResponse.h>
#include <gvr/communication/ucc/message/UccMessageSendToCloudResponse.h>
#include <gvr/communication/ucc/message/UccMessageFileDownloadStatusEvent.h>
#include <gvr/communication/ucc/message/UccMessageSendToCloudStatusEvent.h>
#include <gvr/communication/ucc/message/UccMessageServerUpdateResponse.h>
#include <gvr/communication/ucc/message/UccMessageConnectResponse.h>
#include <gvr/communication/ucc/message/UccMessageStatus.h>
#include <gvr/communication/ucc/message/UccMessageTestDnsResponse.h>
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
      IUccClassifyMessage::IntegrityType UccClassifyMessageInternalClient::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
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
                if (operation.compare(UccMessageStatus::CMessageOperation) == 0)
                {
                  typeId = UccMessageStatus::GetTypeId();
                }
                else if (operation.compare(UccMessageFileDownloadResponse::CMessageOperation) == 0)
                {
                  typeId = UccMessageFileDownloadResponse::GetTypeId();
                }
                else if (operation.compare(UccMessageSendToCloudResponse::CMessageOperation) == 0)
                {
                  typeId = UccMessageSendToCloudResponse::GetTypeId();
                }
                else if (operation.compare(UccMessageFileDownloadStatusEvent::CMessageOperation) == 0)
                {
                  typeId = UccMessageFileDownloadStatusEvent::GetTypeId();
                }
                else if (operation.compare(UccMessageSendToCloudStatusEvent::CMessageOperation) == 0)
                {
                  typeId = UccMessageSendToCloudStatusEvent::GetTypeId();
                }
                else if (operation.compare(UccMessageServerUpdateResponse::CMessageOperation) == 0)
                {
                  typeId = UccMessageServerUpdateResponse::GetTypeId();
                }
                else if (operation.compare(UccMessageConnectResponse::CMessageOperation) == 0)
                {
                  typeId = UccMessageConnectResponse::GetTypeId();
                }
                else if (operation.compare(UccMessageSendToCloudStatusEvent::CMessageOperation) == 0)
                {
                  typeId = UccMessageSendToCloudStatusEvent::GetTypeId();
                }
                else if (operation.compare(UccMessageSendToCloudResponse::CMessageOperation) == 0)
                {
                  typeId = UccMessageSendToCloudResponse::GetTypeId();
                }
                else if (operation.compare(UccMessageTestDnsResponse::CMessageOperation) == 0)
                {
                  typeId = UccMessageTestDnsResponse::GetTypeId();
                }
//TODO: Add new messages comparisons for ucc internal messages which has to be recognize by the client side.
//                else if (operation.compare(UccMessageOther::CMessageOperation) == 0)
//                {
//                  typeId = UccMessageOther::GetTypeId();
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

      UccClassifyMessageInternalClient::~UccClassifyMessageInternalClient (void)
      {
      }
    }
  }
}
