/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageRegistration.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageRegistration.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      char const * CrlMessageRegistration::CMessageOperation = "registration";

      char const * CrlMessageRegistration::CRegistrationCodeName = "registrationCode";
      char const * CrlMessageRegistration::CLocalIdName = "localId";

      CrlMessageRegistration::~CrlMessageRegistration (void)
      {
      }

      CrlMessageRegistration::CrlMessageRegistration(void)
       : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageRegistration::validate (void)
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
              result = result && m_json.get(CPayloadName).get(CRegistrationCodeName).isString();

              result = result && m_json.get(CPayloadName).get(CLocalIdName).isString();
            }
          }
        }
        return result;
      }

      CrlMessageRegistration::TypeId CrlMessageRegistration::GetTypeId (void)
      {
        return _getTypeId<CrlMessageRegistration>();
      };

      CrlMessageRegistration::TypeId CrlMessageRegistration::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
