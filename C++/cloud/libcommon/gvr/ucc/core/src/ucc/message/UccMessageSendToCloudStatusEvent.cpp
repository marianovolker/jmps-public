/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageSendToCloudStatusEvent.cpp
 * @author mgrieco
 * @date Thursday, May 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageSendToCloudStatusEvent.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageSendToCloudStatusEvent::CMessageOperation = "sendToCloud-status";

      char const * UccMessageSendToCloudStatusEvent::CStatusName = "status";
      char const * UccMessageSendToCloudStatusEvent::CFileSizeName = "fileSize";
      char const * UccMessageSendToCloudStatusEvent::CCurrentSizeName = "currentSize";
      char const * UccMessageSendToCloudStatusEvent::CTrackingNumberName = "trackingNumber";
      char const * UccMessageSendToCloudStatusEvent::CFileIdName = "fileId";
      char const * UccMessageSendToCloudStatusEvent::CCodeName = "code";
      char const * UccMessageSendToCloudStatusEvent::CMessageName = "message";


      const char * UccMessageSendToCloudStatusEvent::CStatusLegends[ UccMessageSendToCloudStatusEvent::eTotalStatuses ] =
      {
        "start", "in-progress", "transfer-failure", "complete"
      };


      UccMessageSendToCloudStatusEvent::UccMessageSendToCloudStatusEvent(void) : AbsUccMessageInternal(CMessageOperation)
      {
      }

      UccMessageSendToCloudStatusEvent::~UccMessageSendToCloudStatusEvent (void)
      {
      }

      bool UccMessageSendToCloudStatusEvent::validate (void)
      {
        bool result = AbsUccMessageInternal::validate();

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

            if ( result && ! m_json.get(CPayloadName).get(CFileIdName).isNull() )
            {
              result = result && m_json.get(CPayloadName).get(CFileIdName).isString();
              result = result && (!m_json.get(CPayloadName).get(CFileIdName).asString().empty());
            }

            if ( result && ! m_json.get(CPayloadName).get(CCodeName).isNull() )
            {
              result = result && m_json.get(CPayloadName).get(CCodeName).isNumber();
            }

            if ( result && ! m_json.get(CPayloadName).get(CMessageName).isNull() )
            {
              result = result && m_json.get(CPayloadName).get(CMessageName).isString();
              result = result && ! m_json.get(CPayloadName).get(CMessageName).asString().empty();
            }
          }
        }
        return result;
      }

      UccMessageSendToCloudStatusEvent::TypeId UccMessageSendToCloudStatusEvent::GetTypeId (void)
      {
        return _getTypeId<UccMessageSendToCloudStatusEvent>();
      };

      UccMessageSendToCloudStatusEvent::TypeId UccMessageSendToCloudStatusEvent::getTypeId (void) const
      {
        return GetTypeId();
      }

      const char * UccMessageSendToCloudStatusEvent::getStatusLegend (UccMessageSendToCloudStatusEvent::ESendToCloudEventStatus eStatus)
      {
        return CStatusLegends[ eStatus ];
      }
    }
  }
}
