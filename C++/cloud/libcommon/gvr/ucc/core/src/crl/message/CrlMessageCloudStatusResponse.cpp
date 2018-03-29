/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageCloudStatusResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/crl/message/CrlMessageCloudStatusResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Crl Domain
    namespace crl
    {
      char const * CrlMessageCloudStatusResponse::CMessageOperation = "cloud-status-response";


      char const * CrlMessageCloudStatusResponse::CConnectionStatusName = "connection-status";
      char const * CrlMessageCloudStatusResponse::CRegistrationStatusName = "registration-status";
      char const * CrlMessageCloudStatusResponse::CErrorCodeName = "error-code";
      char const * CrlMessageCloudStatusResponse::CErrorMessageName = "error-message";

      char const * CrlMessageCloudStatusResponse::CConnectionDisconnectedString = "Disconnected";
      char const * CrlMessageCloudStatusResponse::CConnectionConnectingString = "Connecting";
      char const * CrlMessageCloudStatusResponse::CConnectionConnectedString = "Connected";
      char const * CrlMessageCloudStatusResponse::CRegistrationUnregisteredString = "Unregistered";
      char const * CrlMessageCloudStatusResponse::CRegistrationUnregisteringString = "Unregistering";
      char const * CrlMessageCloudStatusResponse::CRegistrationRegisteringString = "Registering";
      char const * CrlMessageCloudStatusResponse::CRegistrationRegisteredString = "Registered";

      CrlMessageCloudStatusResponse::~CrlMessageCloudStatusResponse (void)
      {
      }

      CrlMessageCloudStatusResponse::CrlMessageCloudStatusResponse(void)
       : AbsCrlMessageInternal(CMessageOperation)
      {
      }

      bool CrlMessageCloudStatusResponse::validate (void)
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
              result = result && m_json.get(CPayloadName).get(CConnectionStatusName).isString();
              // TODO: Check with Business Analyst!!!
              // if (result)
              // {
              //   std::string connection = m_json.get(CPayloadName).get(CConnectionStatusName).asString();
              //   result = ((connection.compare(CConnectionDisconnectedString)==0) ||
              //             (connection.compare(CConnectionConnectingString)==0) ||
              //             (connection.compare(CConnectionConnectedString)==0));
              // }

              result = result && m_json.get(CPayloadName).get(CRegistrationStatusName).isString();
              // TODO: Check with Business Analyst!!!
              // if (result)
              // {
              //   std::string registration = m_json.get(CPayloadName).get(CRegistrationStatusName).asString();
              //   result = ((registration.compare(CRegistrationUnregisteredString)==0) ||
              //             (registration.compare(CRegistrationUnregisteringString)==0) ||
              //             (registration.compare(CRegistrationRegisteringString)==0) ||
              //             (registration.compare(CRegistrationRegisteredString)==0));
              // }

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
          }
        }
        return result;
      }

      CrlMessageCloudStatusResponse::TypeId CrlMessageCloudStatusResponse::GetTypeId (void)
      {
        return _getTypeId<CrlMessageCloudStatusResponse>();
      };

      CrlMessageCloudStatusResponse::TypeId CrlMessageCloudStatusResponse::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
