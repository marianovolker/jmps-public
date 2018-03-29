/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudReconnectResponse.cpp
 * @author mgrieco
 * @date Monday, Jun 08, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudReconnectResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudReconnectResponse::CCodeName = "code";
      char const * UccMessageCloudReconnectResponse::CMessageName = "message";
      char const * UccMessageCloudReconnectResponse::CInvalidEncryptedDatamessage = "Unable to execute command - Invalid connection encrypted data";

      UccMessageCloudReconnectResponse::~UccMessageCloudReconnectResponse (void)
      {
      }

      UccMessageCloudReconnectResponse::UccMessageCloudReconnectResponse(void)
       : AbsUccMessageCloudCommandResponse()
      {
      }

      bool UccMessageCloudReconnectResponse::validate (void)
      {
        bool result = AbsUccMessageCloudCommandResponse::validate();
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
              result = result && m_json.get(CCodeName).isString();
              result = result && m_json.get(CMessageName).isString();
            }
          }
        }
        return result;
      }
    }
  }
}
