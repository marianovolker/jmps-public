/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlClassifyMessageClient.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/classifier/CrlClassifyMessageClient.h>
#include <gvr/communication/crl/message/CrlMessageCloudStatusResponse.h>
#include <gvr/communication/crl/message/CrlMessageRegistrationResponse.h>
#include <gvr/communication/crl/message/CrlMessageGetDnsConfigurationResponse.h>
#include <gvr/communication/crl/message/CrlMessageSetDnsConfigurationResponse.h>
#include <gvr/communication/crl/message/CrlMessageTestDnsResponse.h>
//#include <gvr/communication/crl/message/CrlMessageCloudOther.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      ICrlClassifyMessage::IntegrityType CrlClassifyMessageClient::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
      {
        SizeType structureSize = 0;
        gvr::utils::Json json;
        unsigned char flag = 0;
        offset = 0;
        IntegrityType integrity = parseToStructure(buff,size,structureSize,flag,json);
        if (integrity == ICrlClassifyMessage::Integrity::CComplete)
        {
          if (json.get(AbsCrlMessageInternal::COperationName).isString())
          {
            std::string operation = json.get(AbsCrlMessageInternal::COperationName).asString();
            if (operation.empty() == false)
            {
              if (operation.compare(CrlMessageCloudStatusResponse::CMessageOperation) == 0)
              {
                typeId = CrlMessageCloudStatusResponse::GetTypeId();
              }
              else if (operation.compare(CrlMessageRegistrationResponse::CMessageOperation) == 0)
              {
                typeId = CrlMessageRegistrationResponse::GetTypeId();
              }
              else if (operation.compare(CrlMessageGetDnsConfigurationResponse::CMessageOperation) == 0)
              {
                typeId = CrlMessageGetDnsConfigurationResponse::GetTypeId();
              }
              else if (operation.compare(CrlMessageSetDnsConfigurationResponse::CMessageOperation) == 0)
              {
                typeId = CrlMessageSetDnsConfigurationResponse::GetTypeId();
              }
              else if (operation.compare(CrlMessageTestDnsResponse::CMessageOperation) == 0)
              {
                typeId = CrlMessageTestDnsResponse::GetTypeId();
              }
//TODO: Add new messages comparisons for crl cloud messages which has to be recognize by the client side.
//              else if (operation.compare(CrlMessageCloudOther::CMessageOperation) == 0)
//              {
//                typeId = CrlMessageCloudOther::GetTypeId();
//              }
              else
              {
                integrity = ICrlClassifyMessage::Integrity::CUnknown;
              }
            }
            else
            {
              integrity = ICrlClassifyMessage::Integrity::CUnknown;
            }
          }
          else
          {
            integrity = ICrlClassifyMessage::Integrity::CUnknown;
          }

          if (integrity == ICrlClassifyMessage::Integrity::CComplete)
          {
            key = AbsCrlMessageInternal::CDefaultKey;
          }
        }
        return integrity;
      }

      CrlClassifyMessageClient::~CrlClassifyMessageClient (void)
      {
      }
    }
  }
}
