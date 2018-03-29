/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileDownloadStatusEvent.cpp
 * @author mgrieco
 * @date Monday, Apt 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadStatusEvent.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudFileDownloadStatusEvent::CMessageOperation = "file-download-event";

      char const * UccMessageCloudFileDownloadStatusEvent::CFileTypeName = "fileType";
      char const * UccMessageCloudFileDownloadStatusEvent::CTrackingNumberName = "trackingNumber";
      char const * UccMessageCloudFileDownloadStatusEvent::CStatusName = "status";
      char const * UccMessageCloudFileDownloadStatusEvent::CMessageName = "message";

      const char* UccMessageCloudFileDownloadStatusEvent::CapcStatusLegends[ UccMessageCloudFileDownloadStatusEvent::eTotalStatuses ] =
      {
        "start", "in-progress", "transfer-failure", "failure", "complete"
      };

      const char * UccMessageCloudFileDownloadStatusEvent::getStatusLegend (UccMessageCloudFileDownloadStatusEvent::EDownloadEventStatus eStatus)
      {
        return CapcStatusLegends[ eStatus ];
      }


      UccMessageCloudFileDownloadStatusEvent::~UccMessageCloudFileDownloadStatusEvent (void)
      {
      }

      UccMessageCloudFileDownloadStatusEvent::UccMessageCloudFileDownloadStatusEvent(void)
       : AbsUccMessageCloudEventRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudFileDownloadStatusEvent::validate (void)
      {
        bool result = AbsUccMessageCloudEventRequest::validate();

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
            result = result && (!m_json.get(CPayloadName).get(CFileTypeName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CTrackingNumberName).isString();
            result = result && (!m_json.get(CPayloadName).get(CTrackingNumberName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CStatusName).isString();
            result = result && (!m_json.get(CPayloadName).get(CStatusName).asString().empty());
            if(result && !m_json.get(CPayloadName).get(CMessageName).isNull())
            {
              result = result && m_json.get(CPayloadName).get(CMessageName).isString();
              result = result && (!m_json.get(CPayloadName).get(CMessageName).asString().empty());
            }
          }
        }
        return result;
      }

      UccMessageCloudFileDownloadStatusEvent::TypeId UccMessageCloudFileDownloadStatusEvent::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudFileDownloadStatusEvent>();
      };

      UccMessageCloudFileDownloadStatusEvent::TypeId UccMessageCloudFileDownloadStatusEvent::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
