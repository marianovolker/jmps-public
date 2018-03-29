/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudSecurityChangeResponse.cpp
 * @author mgrieco
 * @date Wednesday, May 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <gvr/communication/ucc/message/UccMessageCloudSecurityChangeResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {
      char const * UccMessageCloudSecurityChangeResponse::CCodeName = "code";
      char const * UccMessageCloudSecurityChangeResponse::CMessageName = "message";


      /**
       * @brief Possible errors.
       */
      const char   UccMessageCloudSecurityChangeResponse::CUnsupportedDeviceCommandDataCode             = 1;
      const char * UccMessageCloudSecurityChangeResponse::CUnsupportedDeviceCommandDataMessage          = "Unsupported Device Command";

      const char   UccMessageCloudSecurityChangeResponse::CUnableToContactDeviceDataCode                = 2;
      const char * UccMessageCloudSecurityChangeResponse::CUnableToContactDeviceDataMessage             = "Unable to contact device";

      const char   UccMessageCloudSecurityChangeResponse::CUnableToExecuteCommandDataCode               = 3;
      const char * UccMessageCloudSecurityChangeResponse::CUnableToExecuteCommandDataMessage            = "Unable to execute command";

      const char   UccMessageCloudSecurityChangeResponse::CCannotExecuteAtThisTimeDataCode              = 4;
      const char * UccMessageCloudSecurityChangeResponse::CCannotExecuteAtThisTimeDataMessage           = "Cannot execute at this time";

      const char   UccMessageCloudSecurityChangeResponse::CInvalidPayloadDataCode                       = 5;
      const char * UccMessageCloudSecurityChangeResponse::CInvalidPayloadDataMessage                    = "Invalid payload";

      const char   UccMessageCloudSecurityChangeResponse::CNotEnoughSpaceToPerformOperationDataCode     = 6;
      const char * UccMessageCloudSecurityChangeResponse::CNotEnoughSpaceToPerformOperationDataMessage  = "Not enough space to preform operation";

      const char   UccMessageCloudSecurityChangeResponse::CInvalidFileTypeDataCode                      = 7;
      const char * UccMessageCloudSecurityChangeResponse::CInvalidFileTypeDataMessage                   = "Invalid file type";

      const char   UccMessageCloudSecurityChangeResponse::CInvalidCertificateFormatDataCode             = 10;
      const char * UccMessageCloudSecurityChangeResponse::CInvalidCertificateFormatDataMessage          = "Invalid certificate format";

      const char   UccMessageCloudSecurityChangeResponse::CCorruptedCertificateDataCode                 = 11;
      const char * UccMessageCloudSecurityChangeResponse::CCorruptedCertificateDataMessage              = "Corrupted certificate";



      UccMessageCloudSecurityChangeResponse::~UccMessageCloudSecurityChangeResponse (void)
      {
      }

      UccMessageCloudSecurityChangeResponse::UccMessageCloudSecurityChangeResponse(void)
       : AbsUccMessageCloudCommandResponse()
      {
      }

      bool UccMessageCloudSecurityChangeResponse::validate (void)
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
