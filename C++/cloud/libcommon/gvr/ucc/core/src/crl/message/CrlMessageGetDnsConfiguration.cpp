/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageGetDnsConfiguration.cpp
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageGetDnsConfiguration.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      char const * CrlMessageGetDnsConfiguration::CMessageOperation = "get-dns";

      CrlMessageGetDnsConfiguration::~CrlMessageGetDnsConfiguration (void)
      {
      }

      CrlMessageGetDnsConfiguration::CrlMessageGetDnsConfiguration(void)
       : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageGetDnsConfiguration::validate (void)
      {
        bool result = AbsCrlMessageInternal::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CPayloadName).isObject();
        }
        return result;
      }

      CrlMessageGetDnsConfiguration::TypeId CrlMessageGetDnsConfiguration::GetTypeId (void)
      {
        return _getTypeId<CrlMessageGetDnsConfiguration>();
      };

      CrlMessageGetDnsConfiguration::TypeId CrlMessageGetDnsConfiguration::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
