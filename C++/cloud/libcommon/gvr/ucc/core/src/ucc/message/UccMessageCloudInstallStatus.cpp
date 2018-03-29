/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudInstallStatus.cpp
 * @author gparis
 * @date Tuesday, April 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudInstallStatus.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {


      char const * UccMessageCloudInstallStatus::CMessageOperation = "install-event";

      const char * UccMessageCloudInstallStatus::CapcStatuses[] =
      {
        "start", "in-progress", "complete", "failure"
      };

      char const * UccMessageCloudInstallStatus::CFileTypeName = "fileType";
      char const * UccMessageCloudInstallStatus::CTrackingNumberName = "trackingNumber";
      char const * UccMessageCloudInstallStatus::CRollBackLabel = "rollback";
      char const * UccMessageCloudInstallStatus::CTrue = "true";
      char const * UccMessageCloudInstallStatus::CFalse = "false";
      char const * UccMessageCloudInstallStatus::CStatusLabel = "status";
      char const * UccMessageCloudInstallStatus::CStatusOK = "complete";
      char const * UccMessageCloudInstallStatus::CStatusError = "failure";
      char const * UccMessageCloudInstallStatus::CMessageLabel = "message";

      UccMessageCloudInstallStatus::~UccMessageCloudInstallStatus (void)
      {
      }

      UccMessageCloudInstallStatus::UccMessageCloudInstallStatus(void)
       : AbsUccMessageCloudEventRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudInstallStatus::validate (void)
      {
        bool result = AbsUccMessageCloudEventRequest::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CMessageIdName).isString();
          result = result && !m_json.get(CMessageIdName).asString().empty();

          result = result && m_json.get(CDeviceIdName).isString();
          result = result && !m_json.get(CDeviceIdName).asString().empty();

          result = result && m_json.get(CPayloadName).isObject();
          if (result)
          {
            result = result && m_json.get(CPayloadName).get(CFileTypeName).isString();
            result = result && !m_json.get(CPayloadName).get(CFileTypeName).asString().empty();

            result = result && m_json.get(CPayloadName).get(CTrackingNumberName).isString();
            result = result && !m_json.get(CPayloadName).get(CTrackingNumberName).asString().empty();

            if (result && ! m_json.get(CPayloadName).get(CRollBackLabel).isNull()) // optional field
            {
              result = result && m_json.get(CPayloadName).get(CRollBackLabel).isBoolean();
            }

            result = result && m_json.get(CPayloadName).get(CStatusLabel).isString();
            result = result && !m_json.get(CPayloadName).get(CStatusLabel).asString().empty();

            if ( result && m_json.get(CPayloadName).get(CStatusLabel).asString() != CStatusOK )
            {
              // message must be present as a non null string if status != "success"
              result = result && m_json.get(CPayloadName).get(CMessageLabel).isString();
              result = result && !m_json.get(CPayloadName).get(CMessageLabel).asString().empty();
            }
          }
        }
        return result;
      }

      UccMessageCloudInstallStatus::TypeId UccMessageCloudInstallStatus::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudInstallStatus>();
      };

      UccMessageCloudInstallStatus::TypeId UccMessageCloudInstallStatus::getTypeId (void) const
      {
        return GetTypeId();
      }

      void UccMessageCloudInstallStatus::setStatus(EStatus e)
      {
        getJson().set(AbsUccMessageCloudEventRequest::CPayloadName).set(CStatusLabel, CapcStatuses[e]);
      }

      void UccMessageCloudInstallStatus::setError(const std::string & sError)
      {
        getJson().set(AbsUccMessageCloudEventRequest::CPayloadName).set(CStatusLabel, CapcStatuses[eFailure]);
        getJson().set(AbsUccMessageCloudEventRequest::CPayloadName).set(CMessageLabel, sError);
      }


    }
  }
}
