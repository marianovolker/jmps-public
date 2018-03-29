/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageGetDnsConfigurationResponse.cpp
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageGetDnsConfigurationResponse.h>
#include <algorithm>
#include <cctype>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      char const * CrlMessageGetDnsConfigurationResponse::CMessageOperation = "get-dns-response";

      char const * CrlMessageGetDnsConfigurationResponse::CMessageServer1Name ="nameserver1";
      char const * CrlMessageGetDnsConfigurationResponse::CMessageServer2Name ="nameserver2";

      CrlMessageGetDnsConfigurationResponse::~CrlMessageGetDnsConfigurationResponse (void)
      {
      }

      CrlMessageGetDnsConfigurationResponse::CrlMessageGetDnsConfigurationResponse(void)
       : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageGetDnsConfigurationResponse::validate (void)
      {
        bool result = AbsCrlMessageInternal::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CPayloadName).isObject();
          if (result)
          {
            if (result && (!m_json.get(CPayloadName).get(CMessageServer1Name).isNull()))
            {
              result = result && m_json.get(CPayloadName).get(CMessageServer1Name).isString();
            }
            if (result && (!m_json.get(CPayloadName).get(CMessageServer2Name).isNull()))
            {
              result = result && m_json.get(CPayloadName).get(CMessageServer2Name).isString();
            }
          }
        }
        return result;
      }

      CrlMessageGetDnsConfigurationResponse::TypeId CrlMessageGetDnsConfigurationResponse::GetTypeId (void)
      {
        return _getTypeId<CrlMessageGetDnsConfigurationResponse>();
      };

      CrlMessageGetDnsConfigurationResponse::TypeId CrlMessageGetDnsConfigurationResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
