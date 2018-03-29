/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceComponentStatusEventResponse.cpp
 * @author mgrieco
 * @date Monday, Jun 29, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudDeviceComponentStatusEventResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudDeviceComponentStatusEventResponse::CErrorCodeName = "code";
      char const * UccMessageCloudDeviceComponentStatusEventResponse::CErrorMessageName = "message";

      UccMessageCloudDeviceComponentStatusEventResponse::~UccMessageCloudDeviceComponentStatusEventResponse (void)
      {
      }

      UccMessageCloudDeviceComponentStatusEventResponse::UccMessageCloudDeviceComponentStatusEventResponse(void)
       : AbsUccMessageCloudEventResponse()
      {
      }

      bool UccMessageCloudDeviceComponentStatusEventResponse::validate (void)
      {
        bool result = AbsUccMessageCloudEventResponse::validate();

        // Validate required fields.
        if (result)
        {
          result = result && m_json.get(CMessageIdName).isString();
          result = result && (!m_json.get(CMessageIdName).asString().empty());

          result = result && m_json.get(CSuccessName).isBoolean();
          if (result)
          {
            if (!m_json.get(CSuccessName).asBoolean())
            {
              result = result && m_json.get(CErrorCodeName).isString();
              result = result && m_json.get(CErrorMessageName).isString();
            }
          }
        }
        return result;
      }
    }
  }
}
