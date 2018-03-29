/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileDownloadResponse.cpp
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {

      char const * UccMessageCloudFileDownloadResponse::CPayloadName = "payload";
      char const * UccMessageCloudFileDownloadResponse::CLocalUrlName = "localURL";
      char const * UccMessageCloudFileDownloadResponse::CCodeName = "code";
      char const * UccMessageCloudFileDownloadResponse::CMessageName = "message";

      char const * UccMessageCloudFileDownloadResponse::CUnsupportedDeviceCommandMessage = "Unsupported Device Command";
      const int    UccMessageCloudFileDownloadResponse::CUnsupportedDeviceCommandCode = 1;
      char const * UccMessageCloudFileDownloadResponse::CUnableToContactDeviceMessage = "Unable to contact device";
      const int    UccMessageCloudFileDownloadResponse::CUnableToContactDeviceCode = 2;
      char const * UccMessageCloudFileDownloadResponse::CUnableToExecuteCommandMessage = "Unable to execute command";
      const int    UccMessageCloudFileDownloadResponse::CUnableToExecuteCommandCode = 3;
      char const * UccMessageCloudFileDownloadResponse::CCannotExecuteAtThisTimeMessage = "Cannot execute at this time";
      const int    UccMessageCloudFileDownloadResponse::CCannotExecuteAtThisTimeCode = 4;
      char const * UccMessageCloudFileDownloadResponse::CInvalidPayloadMessage = "invalid payload";
      const int    UccMessageCloudFileDownloadResponse::CInvalidPayloadCode = 5;
      char const * UccMessageCloudFileDownloadResponse::CNotEnoughSpaceToPreformOperationMessage = "Not enough space to preform operation";
      const int    UccMessageCloudFileDownloadResponse::CNotEnoughSpaceToPreformOperationCode = 6;
      char const * UccMessageCloudFileDownloadResponse::CInvalidFileTypeMessage = "Invalid file type";
      const int    UccMessageCloudFileDownloadResponse::CInvalidFileTypeCode = 7;
      char const * UccMessageCloudFileDownloadResponse::CUnableToAccessFileMessage = "Unable to access file";
      const int    UccMessageCloudFileDownloadResponse::CUnableToAccessFileCode = 8;
      char const * UccMessageCloudFileDownloadResponse::CUnableToAccessURLMessage = "Unable to access URL";
      const int    UccMessageCloudFileDownloadResponse::CUnableToAccessURLCode = 9;
      char const * UccMessageCloudFileDownloadResponse::CCorruptedFileMessage = "Corrupted file";
      const int    UccMessageCloudFileDownloadResponse::CCorruptedFileCode = 12;

      UccMessageCloudFileDownloadResponse::~UccMessageCloudFileDownloadResponse (void)
      {
      }

      UccMessageCloudFileDownloadResponse::UccMessageCloudFileDownloadResponse(void)
       : AbsUccMessageCloudCommandResponse()
      {
      }

      bool UccMessageCloudFileDownloadResponse::validate (void)
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
            if (m_json.get(CSuccessName).asBoolean())
            {
              result = result && m_json.get(CPayloadName).isObject();
              if (result)
              {
                result = result && m_json.get(CPayloadName).get(CLocalUrlName).isString();
                result = result && (!m_json.get(CPayloadName).get(CLocalUrlName).asString().empty());
              }
            }
            else
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
