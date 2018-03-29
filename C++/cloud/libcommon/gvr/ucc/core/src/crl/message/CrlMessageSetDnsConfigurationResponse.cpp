/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageSetDnsConfigurationResponse.cpp
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageSetDnsConfigurationResponse.h>
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
      char const * CrlMessageSetDnsConfigurationResponse::CMessageOperation = "set-dns-response";

      char const * CrlMessageSetDnsConfigurationResponse::CSuccessName = "success";
      char const * CrlMessageSetDnsConfigurationResponse::CErrorCodeName = "error-code";
      char const * CrlMessageSetDnsConfigurationResponse::CErrorMessageName = "error-message";

      char const * CrlMessageSetDnsConfigurationResponse::CSuccessTrue = "true";
      char const * CrlMessageSetDnsConfigurationResponse::CSuccessFalse = "false";

      char const * CrlMessageSetDnsConfigurationResponse::CDnsNotSavedErrorMessage = "DNS not saved";

      CrlMessageSetDnsConfigurationResponse::~CrlMessageSetDnsConfigurationResponse (void)
      {
      }

      CrlMessageSetDnsConfigurationResponse::CrlMessageSetDnsConfigurationResponse(void)
       : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageSetDnsConfigurationResponse::validate (void)
      {
        bool result = AbsCrlMessageInternal::validate();

        // Validate required fields.
        if (result)
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
          }
        }
        return result;
      }

      CrlMessageSetDnsConfigurationResponse::TypeId CrlMessageSetDnsConfigurationResponse::GetTypeId (void)
      {
        return _getTypeId<CrlMessageSetDnsConfigurationResponse>();
      };

      CrlMessageSetDnsConfigurationResponse::TypeId CrlMessageSetDnsConfigurationResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
