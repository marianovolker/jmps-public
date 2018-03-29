/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageTestDnsResponse.cpp
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageTestDnsResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageTestDnsResponse::CMessageOperation = "testDNS";

      char const * UccMessageTestDnsResponse::CPayloadName = "payload";
      char const * UccMessageTestDnsResponse::CSuccessName = "success";
      char const * UccMessageTestDnsResponse::CHostIpName = "host-ip";
      char const * UccMessageTestDnsResponse::CErrorCodeName = "code";
      char const * UccMessageTestDnsResponse::CErrorMessageName = "message";

      UccMessageTestDnsResponse::~UccMessageTestDnsResponse (void)
      {
      }

      UccMessageTestDnsResponse::UccMessageTestDnsResponse(void) : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageTestDnsResponse::validate (void)
      {
        bool result = AbsUccMessageInternal::validate();

        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CMessageIdName).isString();
          result = result && (!m_json.get(CMessageIdName).asString().empty());

          result = result && m_json.get(CDeviceIdName).isString();
          result = result && (!m_json.get(CDeviceIdName).asString().empty());

          result = result && m_json.get(CPayloadName).isObject();
          if (result)
          {
            result = result && m_json.get(CPayloadName).get(CSuccessName).isBoolean();
            if (result)
            {
              if (m_json.get(CPayloadName).get(CSuccessName).asBoolean())
              {
                result = result && m_json.get(CPayloadName).get(CHostIpName).isString();
                result = result && (!m_json.get(CPayloadName).get(CHostIpName).asString().empty());
              }
              else
              {
                result = result && m_json.get(CPayloadName).get(CErrorCodeName).isNumber();
                result = result && m_json.get(CPayloadName).get(CErrorMessageName).isString();
              }
            }
          }
        }
        return result;
      }

      UccMessageTestDnsResponse::TypeId UccMessageTestDnsResponse::GetTypeId (void)
      {
        return _getTypeId<UccMessageTestDnsResponse>();
      };

      UccMessageTestDnsResponse::TypeId UccMessageTestDnsResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
