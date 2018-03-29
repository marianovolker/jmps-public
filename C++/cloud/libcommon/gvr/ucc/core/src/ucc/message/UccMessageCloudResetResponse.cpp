/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudResetResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudResetResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudResetResponse::CCodeName = "code";
      char const * UccMessageCloudResetResponse::CMessageName = "message";

      UccMessageCloudResetResponse::~UccMessageCloudResetResponse (void)
      {
      }

      UccMessageCloudResetResponse::UccMessageCloudResetResponse(void)
       : AbsUccMessageCloudCommandResponse()
      {
      }

      bool UccMessageCloudResetResponse::validate (void)
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
