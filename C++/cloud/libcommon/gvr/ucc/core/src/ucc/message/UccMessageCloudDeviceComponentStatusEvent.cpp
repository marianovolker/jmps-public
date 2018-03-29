/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceComponentStatusEvent.cpp
 * @author mgrieco
 * @date Monday, Jun 29, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudDeviceComponentStatusEvent.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudDeviceComponentStatusEvent::CMessageOperation = "component-status-event";

      char const * UccMessageCloudDeviceComponentStatusEvent::CComponentName = "component";
      char const * UccMessageCloudDeviceComponentStatusEvent::CStatusName = "status";
      char const * UccMessageCloudDeviceComponentStatusEvent::CAttributeName = "attribute";

      char const * UccMessageCloudDeviceComponentStatusEvent::CapcStatusLegends[ UccMessageCloudDeviceComponentStatusEvent::eTotalStatuses ] =
      {
          "idle","in-use","down","in-warning","in-error","enabled","disabled","startup","download"
      };

      const char * UccMessageCloudDeviceComponentStatusEvent::getStatusLegend(EComponentStatus & eStatus)
      {
        return CapcStatusLegends[ eStatus ];
      }


      UccMessageCloudDeviceComponentStatusEvent::~UccMessageCloudDeviceComponentStatusEvent (void)
      {
      }

      UccMessageCloudDeviceComponentStatusEvent::UccMessageCloudDeviceComponentStatusEvent(void)
       : AbsUccMessageCloudEventRequest(CMessageOperation)
      {
      }

      bool UccMessageCloudDeviceComponentStatusEvent::validate (void)
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
            result = result && m_json.get(CPayloadName).get(CComponentName).isString();
            result = result && (!m_json.get(CPayloadName).get(CComponentName).asString().empty());

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

      UccMessageCloudDeviceComponentStatusEvent::TypeId UccMessageCloudDeviceComponentStatusEvent::GetTypeId (void)
      {
        return _getTypeId<UccMessageCloudDeviceComponentStatusEvent>();
      };

      UccMessageCloudDeviceComponentStatusEvent::TypeId UccMessageCloudDeviceComponentStatusEvent::getTypeId (void) const
      {
        return GetTypeId();
      }
    }
  }
}
