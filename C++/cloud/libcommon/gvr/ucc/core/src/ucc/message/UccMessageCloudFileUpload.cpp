/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileUpload.cpp
 * @author mgrieco
 * @date Wednesday, May 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudFileUpload.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudFileUpload::CMessageOperation = "file-upload-cmd";

      char const * UccMessageCloudFileUpload::CFileCategoryName = "fileCategory";
      char const * UccMessageCloudFileUpload::CFileTypeName = "fileType";
      char const * UccMessageCloudFileUpload::CTrackingNumberName = "trackingNumber";
      char const * UccMessageCloudFileUpload::CUrlName = "url";
      char const * UccMessageCloudFileUpload::CDateRangeName = "dateRange";
      char const * UccMessageCloudFileUpload::CDateRangeFromDateName = "fromDate";
      char const * UccMessageCloudFileUpload::CDateRangeToDateName = "toDate";

      UccMessageCloudFileUpload::~UccMessageCloudFileUpload (void)
      {
      }

      UccMessageCloudFileUpload::UccMessageCloudFileUpload(void)
       : AbsUccMessageCloudCommandRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudFileUpload::validate (void)
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
            result = result && m_json.get(CPayloadName).get(CFileCategoryName).isString();
            result = result && (!m_json.get(CPayloadName).get(CFileCategoryName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CFileTypeName).isString();
            result = result && (!m_json.get(CPayloadName).get(CFileTypeName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CTrackingNumberName).isString();

            result = result && m_json.get(CPayloadName).get(CUrlName).isString();

            if( m_json.get(CPayloadName).get(CDateRangeName).isObject())
            {
              result = result && m_json.get(CPayloadName).get(CDateRangeName).get(CDateRangeFromDateName).isString();
              result = result && m_json.get(CPayloadName).get(CDateRangeName).get(CDateRangeToDateName).isString();
            }
          }
        }
        return result;
      }

      UccMessageCloudFileUpload::TypeId UccMessageCloudFileUpload::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudFileUpload>();
      };

      UccMessageCloudFileUpload::TypeId UccMessageCloudFileUpload::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
