/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageTestDns.cpp
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageTestDns.h>
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
      char const * CrlMessageTestDns::CMessageOperation = "test-dns";
      char const * CrlMessageTestDns::CMessageServerName ="nameserver";
      char const * CrlMessageTestDns::CMessageDomainName ="domain-name";

      CrlMessageTestDns::~CrlMessageTestDns (void)
      {
      }

      CrlMessageTestDns::CrlMessageTestDns(void) : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageTestDns::validate (void)
      {
        bool result = AbsCrlMessageInternal::validate();

        // Validate required fields.
        if (result)
        {
          result = result && (!m_json.get(CPayloadName).isNull());
          if(result)
          {
            result = result && m_json.get(CPayloadName).isObject();
            if(result)
            {
              result = result && m_json.get(CPayloadName).get(CMessageServerName).isString();
              if(result)
              {
                result = result && !m_json.get(CPayloadName).get(CMessageServerName).isEmpty();
                if (result && (!m_json.get(CPayloadName).get(CMessageDomainName).isNull()))
                {
                  result = result && m_json.get(CPayloadName).get(CMessageDomainName).isString();
                  result = result && !m_json.get(CPayloadName).get(CMessageDomainName).isEmpty();
                }
              }
            }
          }
        }
        return result;
      }

      CrlMessageTestDns::TypeId CrlMessageTestDns::GetTypeId (void)
      {
        return _getTypeId<CrlMessageTestDns>();
      };

      CrlMessageTestDns::TypeId CrlMessageTestDns::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
