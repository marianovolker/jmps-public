/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceStatusEventResponse.cpp
 * @author mgrieco
 * @date Friday, Jul 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudDeviceStatusEventResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudDeviceStatusEventResponse::CErrorCodeName = "code";
      char const * UccMessageCloudDeviceStatusEventResponse::CErrorMessageName = "message";

      UccMessageCloudDeviceStatusEventResponse::~UccMessageCloudDeviceStatusEventResponse (void)
      {
      }

      UccMessageCloudDeviceStatusEventResponse::UccMessageCloudDeviceStatusEventResponse(void)
       : AbsUccMessageCloudEventResponse()
      {
      }

      bool UccMessageCloudDeviceStatusEventResponse::validate (void)
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
