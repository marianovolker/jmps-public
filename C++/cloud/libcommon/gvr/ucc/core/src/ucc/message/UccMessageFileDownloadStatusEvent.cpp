/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageFileDownloadStatusEvent.cpp
 * @author gparis
 * @date Wednesday, May 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageFileDownloadStatusEvent.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageFileDownloadStatusEvent::CMessageOperation = "receiveFromCloud-status";

      char const * UccMessageFileDownloadStatusEvent::CStatusName = "status";
      char const * UccMessageFileDownloadStatusEvent::CFileSizeName = "fileSize";
      char const * UccMessageFileDownloadStatusEvent::CCurrentSizeName = "currentSize";
      char const * UccMessageFileDownloadStatusEvent::CTrackingNumberName = "trackingNumber";
      char const * UccMessageFileDownloadStatusEvent::CCodeName = "code";
      char const * UccMessageFileDownloadStatusEvent::CMessageName = "message";

      const char* UccMessageFileDownloadStatusEvent::CapcStatusLegends[ UccMessageFileDownloadStatusEvent::eTotalStatuses ] =
      {
        "start", "in-progress", "transfer-failure", "complete"
      };

      const char * UccMessageFileDownloadStatusEvent::getStatusLegend (UccMessageFileDownloadStatusEvent::EDownloadEventStatus eStatus)
      {
        return CapcStatusLegends[ eStatus ];
      }

      UccMessageFileDownloadStatusEvent::UccMessageFileDownloadStatusEvent(void)
       : AbsUccMessageInternal(CMessageOperation)
      {
      }

      UccMessageFileDownloadStatusEvent::~UccMessageFileDownloadStatusEvent (void)
      {
      }

      bool UccMessageFileDownloadStatusEvent::validate (void)
      {
        bool result = AbsUccMessageInternal::validate();
        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(COperationName).isString();
          result = result && ( m_json.get(COperationName).asString().compare(CMessageOperation) == 0);

          result = result && m_json.get(CMessageIdName).isString();
          result = result && (!m_json.get(CMessageIdName).asString().empty());

          result = result && m_json.get(CDeviceIdName).isString();
          result = result && (!m_json.get(CDeviceIdName).asString().empty());

          result = result && m_json.get(CPayloadName).isObject();
          if (result)
          {
            result = result && m_json.get(CPayloadName).get(CStatusName).isString();
            result = result && (!m_json.get(CPayloadName).get(CStatusName).asString().empty());

            result = result && m_json.get(CPayloadName).get(CFileSizeName).isNumber();

            result = result && m_json.get(CPayloadName).get(CCurrentSizeName).isNumber();

            result = result && m_json.get(CPayloadName).get(CTrackingNumberName).isString();
            result = result && (!m_json.get(CPayloadName).get(CTrackingNumberName).asString().empty());

            // Optional error field validations.

            if ( result && ! m_json.get(CPayloadName).get(CCodeName).isNull() )
            {
              result = result && m_json.get(CPayloadName).get(CCodeName).isNumber();
            }

            if ( result && ! m_json.get(CPayloadName).get(CMessageName).isNull() )
            {
              result = result && m_json.get(CPayloadName).get(CMessageName).isString();
            }
          }
        }
        return result;
      }

      UccMessageFileDownloadStatusEvent::TypeId UccMessageFileDownloadStatusEvent::GetTypeId (void)
      {
        return _getTypeId<UccMessageFileDownloadStatusEvent>();
      };

      UccMessageFileDownloadStatusEvent::TypeId UccMessageFileDownloadStatusEvent::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
