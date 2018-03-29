/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileUploadStatusEvent.cpp
 * @author mgrieco
 * @date Wednesday, May 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudFileUploadStatusEvent.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudFileUploadStatusEvent::CMessageOperation = "file-upload-event";

      char const * UccMessageCloudFileUploadStatusEvent::CFileCategoryName = "fileCategory";
      char const * UccMessageCloudFileUploadStatusEvent::CFileTypeName = "fileType";
      char const * UccMessageCloudFileUploadStatusEvent::CFileIdName = "fileId";
      char const * UccMessageCloudFileUploadStatusEvent::CTrackingNumberName = "trackingNumber";
      char const * UccMessageCloudFileUploadStatusEvent::CStatusName = "status";
      char const * UccMessageCloudFileUploadStatusEvent::CFileCompressionName = "fileCompression";
      char const * UccMessageCloudFileUploadStatusEvent::CMessageName = "message";

      char const * UccMessageCloudFileUploadStatusEvent::CapcStatusLegends[ UccMessageCloudFileUploadStatusEvent::eTotalStatuses ] =
      {
        "start", "in-progress", "transfer-failure", "failure", "complete"
      };

      const char * UccMessageCloudFileUploadStatusEvent::getStatusLegend(UccMessageCloudFileUploadStatusEvent::EUploadEventStatus eStatus)
      {
        return CapcStatusLegends[ eStatus ];
      }


      UccMessageCloudFileUploadStatusEvent::~UccMessageCloudFileUploadStatusEvent (void)
      {
      }

      UccMessageCloudFileUploadStatusEvent::UccMessageCloudFileUploadStatusEvent(void)
       : AbsUccMessageCloudEventRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudFileUploadStatusEvent::validate (void)
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
            result = result && m_json.get(CPayloadName).get(CFileCategoryName).isString();
            result = result && (!m_json.get(CPayloadName).get(CFileCategoryName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CFileTypeName).isString();
            result = result && (!m_json.get(CPayloadName).get(CFileTypeName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CFileIdName).isString();
            result = result && (!m_json.get(CPayloadName).get(CFileIdName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CTrackingNumberName).isString();
            result = result && (!m_json.get(CPayloadName).get(CTrackingNumberName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CStatusName).isString();
            result = result && (!m_json.get(CPayloadName).get(CStatusName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CFileCompressionName).isString();
            result = result && (!m_json.get(CPayloadName).get(CFileCompressionName).asString().empty());

            if(result && !m_json.get(CPayloadName).get(CMessageName).isNull())
            {
              result = result && m_json.get(CPayloadName).get(CMessageName).isString();
              result = result && (!m_json.get(CPayloadName).get(CMessageName).asString().empty());
            }
          }
        }
        return result;
      }

      UccMessageCloudFileUploadStatusEvent::TypeId UccMessageCloudFileUploadStatusEvent::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudFileUploadStatusEvent>();
      };

      UccMessageCloudFileUploadStatusEvent::TypeId UccMessageCloudFileUploadStatusEvent::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
