/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileUploadStatusEventResponse.h
 * @author mgrieco
 * @date Wednesday, May 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDFILEUPLOADSTATUSEVENTRESPONSE_H_
# define _UCCMESSAGECLOUDFILEUPLOADSTATUSEVENTRESPONSE_H_

#include <gvr/communication/ucc/message/base/AbsUccMessageCloudEventResponse.h>

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
         * @brief This class is the client ucc cloud file upload message event response.
         * @see gvr.communication.ucc.IUccMessage
         */
        class UccMessageCloudFileUploadStatusEventResponse : public AbsUccMessageCloudEventResponse
        {
        public:

          /**
           * @brief Message Constants Field names.
           */
          static char const * CPayloadName;
          static char const * CCodeName;
          static char const * CMessageName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudFileUploadStatusEventResponse (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudFileUploadStatusEventResponse (void);

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
