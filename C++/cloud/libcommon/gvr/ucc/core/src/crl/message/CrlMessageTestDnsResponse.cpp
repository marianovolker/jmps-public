/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageTestDnsResponse.cpp
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageTestDnsResponse.h>
#include <algorithm>
#include <cctype>
#include <iostream>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      char const * CrlMessageTestDnsResponse::CMessageOperation = "test-dns-response";

      char const * CrlMessageTestDnsResponse::CSuccessName = "success";
      char const * CrlMessageTestDnsResponse::CHostIpName = "host-ip";
      char const * CrlMessageTestDnsResponse::CErrorCodeName = "error-code";
      char const * CrlMessageTestDnsResponse::CErrorMessageName = "error-message";

      char const * CrlMessageTestDnsResponse::CSuccessTrue = "true";
      char const * CrlMessageTestDnsResponse::CSuccessFalse = "false";


      CrlMessageTestDnsResponse::~CrlMessageTestDnsResponse (void)
      {
      }

      CrlMessageTestDnsResponse::CrlMessageTestDnsResponse(void)
       : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageTestDnsResponse::validate (void)
      {
        bool result = AbsCrlMessageInternal::validate();

        // Validate required fields.
        if (result)
        {
          if (result && (!m_json.get(CPayloadName).isNull()))
          {
            result = result && m_json.get(CPayloadName).isObject();
            if (result)
            {
              result = result && m_json.get(CPayloadName).get(CSuccessName).isString();
              if (result && (m_json.get(CPayloadName).get(CSuccessName).asString().compare(CSuccessTrue) != 0))
              {
                if (result && (!m_json.get(CPayloadName).get(CErrorCodeName).isNull()))
                {
                  result = result && m_json.get(CPayloadName).get(CErrorCodeName).isString();
                }

                if (result && (!m_json.get(CPayloadName).get(CErrorMessageName).isNull()))
                {
                  result = result && m_json.get(CPayloadName).get(CErrorMessageName).isString();
                }
              }
              else
              {
                result = result && m_json.get(CPayloadName).get(CHostIpName).isString();
                result = result && !m_json.get(CPayloadName).get(CHostIpName).isEmpty();
              }
            }
          }
        }
        return result;
      }

      CrlMessageTestDnsResponse::TypeId CrlMessageTestDnsResponse::GetTypeId (void)
      {
        return _getTypeId<CrlMessageTestDnsResponse>();
      };

      CrlMessageTestDnsResponse::TypeId CrlMessageTestDnsResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
