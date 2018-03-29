/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudInstall.cpp
 * @author gparis
 * @date Monday, Monday, April 13, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudInstall.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudInstall::CMessageOperation = "install-cmd";

      char const * UccMessageCloudInstall::CFileTypeName = "fileType";
      char const * UccMessageCloudInstall::CTrackingNumberName = "trackingNumber";
      char const * UccMessageCloudInstall::CUrlName = "localURL";
      char const * UccMessageCloudInstall::CInstallWindowName = "installWindow";
      char const * UccMessageCloudInstall::CStartTimeName = "startTime";
      char const * UccMessageCloudInstall::CEndTimeName = "endTime";

      UccMessageCloudInstall::~UccMessageCloudInstall (void)
      {
      }

      UccMessageCloudInstall::UccMessageCloudInstall(void)
       : AbsUccMessageCloudCommandRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudInstall::validate (void)
      {
        bool result = AbsUccMessageCloudCommandRequest::validate();
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
            result = result && m_json.get(CPayloadName).get(CFileTypeName).isString();
            // TODO: Check with Business Analyst!!!
            // result = result && (!m_json.get(CPayloadName).get(CFileTypeName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CTrackingNumberName).isString();
            // TODO: Check with Business Analyst!!!
            // result = result && (!m_json.get(CPayloadName).get(CTrackingNumberName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CUrlName).isString();
            // TODO: Check with Business Analyst!!!
            // result = result && (!m_json.get(CPayloadName).get(CUrlName).asString().empty());

            if (result && (!m_json.get(CPayloadName).get(CInstallWindowName).isNull()))
            {
              result = result && m_json.get(CPayloadName).get(CInstallWindowName).isObject();
              if (result)
              {
                result = result && m_json.get(CPayloadName).get(CStartTimeName).isString();
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CPayloadName).get(CStartTimeName).asString().empty());

                result = result && m_json.get(CPayloadName).get(CEndTimeName).isString();
                // TODO: Check with Business Analyst!!!
                // result = result && (!m_json.get(CPayloadName).get(CEndTimeName).asString().empty());
              }
            }
          }
        }
        return result;
      }

      UccMessageCloudInstall::TypeId UccMessageCloudInstall::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudInstall>();
      };

      UccMessageCloudInstall::TypeId UccMessageCloudInstall::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
