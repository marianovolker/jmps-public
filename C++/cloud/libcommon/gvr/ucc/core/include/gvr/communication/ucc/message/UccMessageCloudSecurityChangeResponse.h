/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudSecurityChangeResponse.h
 * @author mgrieco
 * @date Wednesday, May 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDSECURITYCHANGERESPONSE_H_
# define _UCCMESSAGECLOUDSECURITYCHANGERESPONSE_H_

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
         * @brief This class is the client ucc cloud security change message response.
         * @see gvr.communication.ucc.IUccMessage
         */
        class UccMessageCloudSecurityChangeResponse : public AbsUccMessageCloudCommandResponse
        {
        public:
          /**
           * @brief Field names.
           */
          static char const * CPayloadName;
          static char const * CCodeName;
          static char const * CMessageName;


          /**
           * @brief Possible errors.
           */
          static const char   CUnsupportedDeviceCommandDataCode;
          static const char * CUnsupportedDeviceCommandDataMessage;

          static const char   CUnableToContactDeviceDataCode;
          static const char * CUnableToContactDeviceDataMessage;

          static const char   CUnableToExecuteCommandDataCode;
          static const char * CUnableToExecuteCommandDataMessage;

          static const char   CCannotExecuteAtThisTimeDataCode;
          static const char * CCannotExecuteAtThisTimeDataMessage;

          static const char   CInvalidPayloadDataCode;
          static const char * CInvalidPayloadDataMessage;

          static const char   CNotEnoughSpaceToPerformOperationDataCode;
          static const char * CNotEnoughSpaceToPerformOperationDataMessage;

          static const char   CInvalidFileTypeDataCode;
          static const char * CInvalidFileTypeDataMessage;

          static const char   CInvalidCertificateFormatDataCode;
          static const char * CInvalidCertificateFormatDataMessage;

          static const char   CCorruptedCertificateDataCode;
          static const char * CCorruptedCertificateDataMessage;


          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudSecurityChangeResponse (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudSecurityChangeResponse (void);

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
