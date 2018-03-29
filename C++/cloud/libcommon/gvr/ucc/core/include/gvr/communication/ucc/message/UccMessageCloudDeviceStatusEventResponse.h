/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceStatusEventResponse.h
 * @author mgrieco
 * @date Friday, Jul 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDDEVICESTATUSEVENTRESPONSE_H_
# define _UCCMESSAGECLOUDDEVICESTATUSEVENTRESPONSE_H_

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
        class UccMessageCloudDeviceStatusEventResponse : public AbsUccMessageCloudEventResponse
        {
        public:

          /**@brief Payload message field name*/
          static char const * CPayloadName;
          /**@brief Error Code message field name*/
          static char const * CErrorCodeName;
          /**@brief Error Message message field name*/
          static char const * CErrorMessageName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudDeviceStatusEventResponse (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudDeviceStatusEventResponse (void);

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
