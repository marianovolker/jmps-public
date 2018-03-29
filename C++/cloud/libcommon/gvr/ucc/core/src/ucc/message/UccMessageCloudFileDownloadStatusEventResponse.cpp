/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileDownloadStatusEventResponse.cpp
 * @author mgrieco
 * @date Monday, Apt 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadStatusEventResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudFileDownloadStatusEventResponse::CCodeName = "code";
      char const * UccMessageCloudFileDownloadStatusEventResponse::CMessageName = "message";

      UccMessageCloudFileDownloadStatusEventResponse::~UccMessageCloudFileDownloadStatusEventResponse (void)
      {
      }

      UccMessageCloudFileDownloadStatusEventResponse::UccMessageCloudFileDownloadStatusEventResponse(void)
       : AbsUccMessageCloudEventResponse()
      {
      }

      bool UccMessageCloudFileDownloadStatusEventResponse::validate (void)
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
