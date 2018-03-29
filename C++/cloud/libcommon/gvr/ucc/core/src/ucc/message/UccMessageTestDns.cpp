/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageTestDns.cpp
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageTestDns.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageTestDns::CMessageOperation = "testDNS";

      char const * UccMessageTestDns::CServerName = "nameserver";
      char const * UccMessageTestDns::CDomainName = "domain-name";


      UccMessageTestDns::~UccMessageTestDns (void)
      {
      }

      UccMessageTestDns::UccMessageTestDns(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageTestDns::validate (void)
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
            result = result && m_json.get(CPayloadName).get(CServerName).isString();
            result = result && (!m_json.get(CPayloadName).get(CServerName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CDomainName).isString();
            result = result && (!m_json.get(CPayloadName).get(CDomainName).asString().empty());
          }
        }
        return result;
      }

      UccMessageTestDns::TypeId UccMessageTestDns::GetTypeId (void)
      {
        return _getTypeId<UccMessageTestDns>();
      };

      UccMessageTestDns::TypeId UccMessageTestDns::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
