/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageFileDownload.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageFileDownload.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageFileDownload::CMessageOperation = "receiveFromCloud";

      char const * UccMessageFileDownload::CUrlName = "url";
      char const * UccMessageFileDownload::CPathName = "path";
      char const * UccMessageFileDownload::CFileSizeName = "fileSize";
      char const * UccMessageFileDownload::CTrackingNumberName = "trackingNumber";
      char const * UccMessageFileDownload::CChecksumTypeName = "checksumType";
      char const * UccMessageFileDownload::CChecksumName = "checksum";

      UccMessageFileDownload::~UccMessageFileDownload (void)
      {
      }

      UccMessageFileDownload::UccMessageFileDownload(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageFileDownload::validate (void)
      {
        bool result = AbsUccMessageInternal::validate();
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
            result = result && m_json.get(CPayloadName).get(CUrlName).isString();
            // TODO: Check with Business Analyst!!!
            // result = result && (!m_json.get(CPayloadName).get(CUrlName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CPathName).isString();
            // TODO: Check with Business Analyst!!!
            // result = result && (!m_json.get(CPayloadName).get(CPathName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CFileSizeName).isNumber();

            result = result && m_json.get(CPayloadName).get(CTrackingNumberName).isString();
            // TODO: Check with Business Analyst!!!
            // result = result && (!m_json.get(CPayloadName).get(CTrackingNumberName).asString().empty());

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

      UccMessageFileDownload::TypeId UccMessageFileDownload::GetTypeId (void)
      {
        return _getTypeId<UccMessageFileDownload>();
      };

      UccMessageFileDownload::TypeId UccMessageFileDownload::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
