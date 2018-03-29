/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlClassifyMessageServer.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/classifier/CrlClassifyMessageServer.h>
#include <gvr/communication/crl/message/CrlMessageCloudStatus.h>
#include <gvr/communication/crl/message/CrlMessageRegistration.h>
#include <gvr/communication/crl/message/CrlMessageGetDnsConfiguration.h>
#include <gvr/communication/crl/message/CrlMessageSetDnsConfiguration.h>
#include <gvr/communication/crl/message/CrlMessageTestDns.h>

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
      ICrlClassifyMessage::IntegrityType CrlClassifyMessageServer::classify (BufferType const & buff, OffsetType & offset, SizeType & size, TypeId & typeId, KeyType & key)
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
              if (operation.compare(CrlMessageCloudStatus::CMessageOperation) == 0)
              {
                typeId = CrlMessageCloudStatus::GetTypeId();
              }
              else if (operation.compare(CrlMessageRegistration::CMessageOperation) == 0)
              {
                typeId = CrlMessageRegistration::GetTypeId();
              }
              else if (operation.compare(CrlMessageGetDnsConfiguration::CMessageOperation) == 0)
              {
                typeId = CrlMessageGetDnsConfiguration::GetTypeId();
              }
              else if (operation.compare(CrlMessageSetDnsConfiguration::CMessageOperation) == 0)
              {
                typeId = CrlMessageSetDnsConfiguration::GetTypeId();
              }
              else if (operation.compare(CrlMessageTestDns::CMessageOperation) == 0)
              {
                typeId = CrlMessageTestDns::GetTypeId();
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

      CrlClassifyMessageServer::~CrlClassifyMessageServer (void)
      {
      }
    }
  }
}
