/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudInstallResponse.cpp
 * @author gparis
 * @date Monday, April 13, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudInstallResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudInstallResponse::CCodeName = "code";
      char const * UccMessageCloudInstallResponse::CMessageName = "message";

      const char * UccMessageCloudInstallResponse::CapcErrorLegends[eCodeArrayLength] =
      {
        "OK", "Unsupported Device Command.", "Unable to contact device.", "Unable to execute command.",
        "Cannot execute at this time.", "Invalid payload.", "Not enough space to preform operation.",
        "Invalid file type.", "Unable to access file.", "Unable to access URL."
      };

      UccMessageCloudInstallResponse::~UccMessageCloudInstallResponse (void)
      {
      }

      UccMessageCloudInstallResponse::UccMessageCloudInstallResponse(void)
       : AbsUccMessageCloudCommandResponse()
      {
      }

      void UccMessageCloudInstallResponse::setError (ErrorCode e)
      {
        m_json.set(CCodeName,    e);
        m_json.set(CMessageName, CapcErrorLegends[e]);
      }


      bool UccMessageCloudInstallResponse::validate (void)
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
            if (! m_json.get(CSuccessName).asBoolean() ) // In case of errors there are 2 more fields to parse.
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
