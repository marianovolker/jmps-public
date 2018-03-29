/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageSetDnsConfiguration.cpp
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageSetDnsConfiguration.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      char const * CrlMessageSetDnsConfiguration::CMessageOperation = "set-dns";
      char const * CrlMessageSetDnsConfiguration::CMessageServer1Name ="nameserver1";
      char const * CrlMessageSetDnsConfiguration::CMessageServer2Name ="nameserver2";

      CrlMessageSetDnsConfiguration::~CrlMessageSetDnsConfiguration (void)
      {
      }

      CrlMessageSetDnsConfiguration::CrlMessageSetDnsConfiguration(void)
       : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageSetDnsConfiguration::validate (void)
      {
        bool result = AbsCrlMessageInternal::validate();

        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CPayloadName).isObject();
          if(result)
          {
            result = result && m_json.get(CPayloadName).get(CMessageServer1Name).isString();
            if(result)
            {
              result = result && !m_json.get(CPayloadName).get(CMessageServer1Name).isEmpty();
              if (result && (!m_json.get(CPayloadName).get(CMessageServer2Name).isNull()))
              {
                result = result && m_json.get(CPayloadName).get(CMessageServer2Name).isString();
                result = result && !m_json.get(CPayloadName).get(CMessageServer2Name).isEmpty();
              }
            }
          }
        }
        return result;
      }

      CrlMessageSetDnsConfiguration::TypeId CrlMessageSetDnsConfiguration::GetTypeId (void)
      {
        return _getTypeId<CrlMessageSetDnsConfiguration>();
      };

      CrlMessageSetDnsConfiguration::TypeId CrlMessageSetDnsConfiguration::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
