/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileUploadStatusEventResponse.cpp
 * @author mgrieco
 * @date Wednesday, May 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudFileUploadStatusEventResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudFileUploadStatusEventResponse::CCodeName = "code";
      char const * UccMessageCloudFileUploadStatusEventResponse::CMessageName = "message";

      UccMessageCloudFileUploadStatusEventResponse::~UccMessageCloudFileUploadStatusEventResponse (void)
      {
      }

      UccMessageCloudFileUploadStatusEventResponse::UccMessageCloudFileUploadStatusEventResponse(void)
       : AbsUccMessageCloudEventResponse()
      {
      }

      bool UccMessageCloudFileUploadStatusEventResponse::validate (void)
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
