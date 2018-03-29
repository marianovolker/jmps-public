/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageStatus.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageStatus.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageStatus::CMessageOperation = "status";

      char const * UccMessageStatus::COperativeName = "operative";
      char const * UccMessageStatus::CConnectionName = "connection";
      char const * UccMessageStatus::CRegistrationName = "registration";

      char const * UccMessageStatus::CConnectionDisconnectedString = "Disconnected";
      char const * UccMessageStatus::CConnectionConnectingString = "Connecting";
      char const * UccMessageStatus::CConnectionConnectedString = "Connected";
      char const * UccMessageStatus::CRegistrationDeactivatedString = "Deactivated";
      char const * UccMessageStatus::CRegistrationActivatedString = "Activated";

      UccMessageStatus::~UccMessageStatus (void)
      {
      }

      UccMessageStatus::UccMessageStatus(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageStatus::validate (void)
      {
        bool result = AbsUccMessageInternal::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CMessageIdName).isString();
          result = result && (!m_json.get(CMessageIdName).asString().empty());

          result = result && m_json.get(CDeviceIdName).isString();
          result = result && (!m_json.get(CDeviceIdName).asString().empty());

          if (result && (!m_json.get(CPayloadName).isNull()))
          {
            result = result && m_json.get(CPayloadName).isObject();
            if (result)
            {
              result = result && m_json.get(CPayloadName).get(COperativeName).isBoolean();

              result = result && m_json.get(CPayloadName).get(CConnectionName).isString();
              // TODO: Check with Business Analyst!!!
              // if (result)
              // {
              //   std::string connection = m_json.get(CPayloadName).get(CConnectionName).asString();
              //   result = ((connection.compare(CConnectionDisconnectedString)==0) ||
              //             (connection.compare(CConnectionConnectingString)==0) ||
              //             (connection.compare(CConnectionConnectedString)==0));
              // }

              result = result && m_json.get(CPayloadName).get(CRegistrationName).isString();
              // TODO: Check with Business Analyst!!!
              // if (result)
              // {
              //   std::string registration = m_json.get(CPayloadName).get(CRegistrationName).asString();
              //   result = ((registration.compare(CRegistrationDeactivatedString)==0) ||
              //             (registration.compare(CRegistrationActivatedString)==0));
              // }
            }
          }
        }
        return result;
      }

      UccMessageStatus::TypeId UccMessageStatus::GetTypeId (void)
      {
        return _getTypeId<UccMessageStatus>();
      };

      UccMessageStatus::TypeId UccMessageStatus::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
