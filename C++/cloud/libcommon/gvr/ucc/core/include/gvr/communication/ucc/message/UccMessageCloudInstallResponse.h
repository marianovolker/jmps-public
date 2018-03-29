/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudInstall.h
 * @author gparis
 * @date Monday, April 13, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDINSTALLRESPONSE_H_
# define _UCCMESSAGECLOUDINSTALLRESPONSE_H_

#include <gvr/communication/ucc/message/base/AbsUccMessageCloudCommandResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Ucc Domain
    namespace ucc
    {


        /**
         * @brief This class is the client ucc cloud message install response.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-04-13
         * @version 0.0.0, 2015-04-13
         * @author gparis
         */
      class UccMessageCloudInstallResponse : public AbsUccMessageCloudCommandResponse
      {
        public:

          enum ErrorCode
          {
            eOK = 0, eUnsupportedDeviceCommand, eUnableToContactDevice, eUnableToExecuteCommand,
            eCannotExecuteThisTime, eInvalidPayload, eNoRoomForOperation,
            eInvalidFileType, eUnableToAccessFile, eUnableToAccessURL,
            eCodeArrayLength
          };

          /**
           * @brief Field names.
           */
          static char const * CPayloadName;
          static char const * CLocalUrlName;
          static char const * CCodeName;
          static char const * CMessageName;

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudInstallResponse (void);

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudInstallResponse (void);

          /**
           * @brief Generates the Json fields to the corresponding error code and description fields.
           *
           * @param @b e : ErrorCode is the enumerated error code value to set in Json.
           */
          void setError (ErrorCode e);

          /**
           * @brief validate the Message fields.
           * @brief validate the Message fields.
           *
           * @return True if parse process is successful. False otherwise.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual bool validate (void);


        private:

          static const char *  CapcErrorLegends[eCodeArrayLength]; ///< @brief Legends for the error codes.
      };


    }
  }
}

#endif // _UCCMESSAGECLOUDINSTALLRESPONSE_H_

