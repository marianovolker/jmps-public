/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileDownload.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudFileDownload.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudFileDownload::CMessageOperation = "file-download-cmd";

      char const * UccMessageCloudFileDownload::CFileTypeName = "fileType";
      char const * UccMessageCloudFileDownload::CTrackingNumberName = "trackingNumber";
      char const * UccMessageCloudFileDownload::CUrlName = "url";
      char const * UccMessageCloudFileDownload::CFileSizeName = "fileSize";
      char const * UccMessageCloudFileDownload::CChecksumTypeName = "checksumType";
      char const * UccMessageCloudFileDownload::CChecksumName = "checksum";

      UccMessageCloudFileDownload::~UccMessageCloudFileDownload (void)
      {
      }

      UccMessageCloudFileDownload::UccMessageCloudFileDownload(void)
       : AbsUccMessageCloudCommandRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudFileDownload::validate (void)
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

            result = result && m_json.get(CPayloadName).get(CFileSizeName).isNumber();

            result = result && m_json.get(CPayloadName).get(CChecksumTypeName).isString();
            // TODO: Check with Business Analyst!!!
            // result = result && (!m_json.get(CPayloadName).get(CChecksumTypeName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CChecksumName).isString();
            // TODO: Check with Business Analyst!!!
            // result = result && (!m_json.get(CPayloadName).get(CChecksumName).asString().empty());
          }
        }
        return result;
      }

      UccMessageCloudFileDownload::TypeId UccMessageCloudFileDownload::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudFileDownload>();
      };

      UccMessageCloudFileDownload::TypeId UccMessageCloudFileDownload::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
