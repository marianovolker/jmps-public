/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceStatusEvent.cpp
 * @author mgrieco
 * @date Friday, Jul 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudDeviceStatusEvent.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudDeviceStatusEvent::CMessageOperation = "status-event";

      char const * UccMessageCloudDeviceStatusEvent::CStatusName = "status";
      char const * UccMessageCloudDeviceStatusEvent::CAttributeName = "attribute";

      char const * UccMessageCloudDeviceStatusEvent::CapcStatusLegends[ UccMessageCloudDeviceStatusEvent::eTotalStatuses ] =
      {
          "idle","in-use","down","in-warning","in-error","startup"
      };

      const char * UccMessageCloudDeviceStatusEvent::getStatusLegend(EComponentStatus & eStatus)
      {
        return CapcStatusLegends[ eStatus ];
      }


      UccMessageCloudDeviceStatusEvent::~UccMessageCloudDeviceStatusEvent (void)
      {
      }

      UccMessageCloudDeviceStatusEvent::UccMessageCloudDeviceStatusEvent(void)
       : AbsUccMessageCloudEventRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudDeviceStatusEvent::validate (void)
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
            result = result && m_json.get(CPayloadName).get(CStatusName).isString();
            result = result && (!m_json.get(CPayloadName).get(CStatusName).asString().empty());

            if( !m_json.get(CPayloadName).get(CAttributeName).isNull() )
            {
              result = result && m_json.get(CPayloadName).get(CAttributeName).isObject();
              result = result && (m_json.get(CPayloadName).get(CAttributeName).length()>0);
            }
          }
        }
        return result;
      }

      UccMessageCloudDeviceStatusEvent::TypeId UccMessageCloudDeviceStatusEvent::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudDeviceStatusEvent>();
      };

      UccMessageCloudDeviceStatusEvent::TypeId UccMessageCloudDeviceStatusEvent::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
