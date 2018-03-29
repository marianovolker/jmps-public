/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileDownloadResponse.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDFILEDOWNLOADRESPONSE_H_
# define _UCCMESSAGECLOUDFILEDOWNLOADRESPONSE_H_

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
         * @brief This class is the client ucc cloud message reset.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class UccMessageCloudFileDownloadResponse : public AbsUccMessageCloudCommandResponse
        {
        public:
          /**
           * @brief Field names.
           */
          static char const * CPayloadName;
          static char const * CLocalUrlName;
          static char const * CCodeName;
          static char const * CMessageName;

          static char const * CUnsupportedDeviceCommandMessage;
          static const int    CUnsupportedDeviceCommandCode;
          static char const * CUnableToContactDeviceMessage;
          static const int    CUnableToContactDeviceCode;
          static char const * CUnableToExecuteCommandMessage;
          static const int    CUnableToExecuteCommandCode;
          static char const * CCannotExecuteAtThisTimeMessage;
          static const int    CCannotExecuteAtThisTimeCode;
          static char const * CInvalidPayloadMessage;
          static const int    CInvalidPayloadCode;
          static char const * CNotEnoughSpaceToPreformOperationMessage;
          static const int    CNotEnoughSpaceToPreformOperationCode;
          static char const * CInvalidFileTypeMessage;
          static const int    CInvalidFileTypeCode;
          static char const * CUnableToAccessFileMessage;
          static const int    CUnableToAccessFileCode;
          static char const * CUnableToAccessURLMessage;
          static const int    CUnableToAccessURLCode;
          static char const * CCorruptedFileMessage;
          static const int    CCorruptedFileCode;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudFileDownloadResponse (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudFileDownloadResponse (void);

          /**
           * @brief validate the Message fields.
           *
           * @return True if parse process is successful. False otherwise.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual bool validate (void);
        };
      }
    }
  }

#endif
