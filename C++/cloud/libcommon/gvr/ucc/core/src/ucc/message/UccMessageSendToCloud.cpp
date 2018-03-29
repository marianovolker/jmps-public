/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageSendToCloud.cpp
 * @author mgrieco
 * @date Thursday, May 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageSendToCloud.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageSendToCloud::CMessageOperation = "sendToCloud";

      char const * UccMessageSendToCloud::CUrlName = "url";
      char const * UccMessageSendToCloud::CPathName = "path";
      char const * UccMessageSendToCloud::CFileSizeName = "fileSize";
      char const * UccMessageSendToCloud::CTrackingNumberName = "trackingNumber";
      char const * UccMessageSendToCloud::CChecksumName = "checksum";
      char const * UccMessageSendToCloud::CFileTypeName = "fileType";
      char const * UccMessageSendToCloud::CCompressionTypeName = "compressionType";

      UccMessageSendToCloud::~UccMessageSendToCloud (void)
      {
      }

      UccMessageSendToCloud::UccMessageSendToCloud(void) : AbsUccMessageInternal(CMessageOperation)
      {
      }

      bool UccMessageSendToCloud::validate (void)
      {
        bool result = AbsUccMessageInternal::validate();
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
            result = result && (!m_json.get(CPayloadName).get(CUrlName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CPathName).isString();
            result = result && (!m_json.get(CPayloadName).get(CPathName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CFileSizeName).isNumber();

            if(!m_json.get(CPayloadName).get(CTrackingNumberName).isNull())
            {
              result = result && m_json.get(CPayloadName).get(CTrackingNumberName).isString();
            }

            result = result && m_json.get(CPayloadName).get(CChecksumName).isString();
            result = result && (!m_json.get(CPayloadName).get(CChecksumName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CFileTypeName).isString();
            result = result && (!m_json.get(CPayloadName).get(CFileTypeName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CCompressionTypeName).isString();
            result = result && (!m_json.get(CPayloadName).get(CCompressionTypeName).asString().empty());
            }
        }
        return result;
      }

      UccMessageSendToCloud::TypeId UccMessageSendToCloud::GetTypeId (void)
      {
        return _getTypeId<UccMessageSendToCloud>();
      };

      UccMessageSendToCloud::TypeId UccMessageSendToCloud::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
