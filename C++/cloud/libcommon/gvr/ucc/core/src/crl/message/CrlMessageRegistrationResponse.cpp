/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageRegistrationResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageRegistrationResponse.h>
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
      char const * CrlMessageRegistrationResponse::CMessageOperation = "registration-response";

      char const * CrlMessageRegistrationResponse::CSuccessName = "success";
      char const * CrlMessageRegistrationResponse::CErrorCodeName = "error-code";
      char const * CrlMessageRegistrationResponse::CErrorMessageName = "error-message";

      char const * CrlMessageRegistrationResponse::CSuccessTrue = "true";
      char const * CrlMessageRegistrationResponse::CSuccessFalse = "false";

      CrlMessageRegistrationResponse::~CrlMessageRegistrationResponse (void)
      {
      }

      CrlMessageRegistrationResponse::CrlMessageRegistrationResponse(void)
       : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageRegistrationResponse::validate (void)
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
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CPayloadName).get(CErrorCodeName).asString().empty());
                }

                if (result && (!m_json.get(CPayloadName).get(CErrorMessageName).isNull()))
                {
                  result = result && m_json.get(CPayloadName).get(CErrorMessageName).isString();
                }
              }
/*
              result = result && m_json.get(CPayloadName).get(CSuccessName).isBoolean();

              if (result && (m_json.get(CPayloadName).get(CSuccessName).asBoolean() == false))
              {
                if (result && (!m_json.get(CPayloadName).get(CErrorCodeName).isNull()))
                {
                  result = result && m_json.get(CPayloadName).get(CErrorCodeName).isString();
                  // TODO: Check with Business Analyst!!!
                  // result = result && (!m_json.get(CPayloadName).get(CErrorCodeName).asString().empty());
                }

                if (result && (!m_json.get(CPayloadName).get(CErrorMessageName).isNull()))
                {
                  result = result && m_json.get(CPayloadName).get(CErrorMessageName).isString();
                }
              }
*/
            }
          }
        }
        return result;
      }

      CrlMessageRegistrationResponse::TypeId CrlMessageRegistrationResponse::GetTypeId (void)
      {
        return _getTypeId<CrlMessageRegistrationResponse>();
      };

      CrlMessageRegistrationResponse::TypeId CrlMessageRegistrationResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
