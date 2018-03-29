/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudReconnectResponse.h
 * @author mgrieco
 * @date Monday, Jun 08, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDRECONNECTRESPONSE_H_
# define _UCCMESSAGECLOUDRECONNECTRESPONSE_H_

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
         * @brief This class is the client ucc cloud reconnect message response.
         * @see gvr.communication.ucc.IUccMessage
         */
        class UccMessageCloudReconnectResponse : public AbsUccMessageCloudCommandResponse
        {
        public:

          static char const * CPayloadName; /** @brief Payload message field name. */
          static char const * CCodeName;/** @brief Error Code message field name. */
          static char const * CMessageName;/** @brief Error description message field name. */


          static const char   CInvalidEncryptedDataCode = 0x03; /** @brief Invalid encrypted data code error*/
          static char const * CInvalidEncryptedDatamessage; /** @brief Invalid encrypted data description error*/

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudReconnectResponse (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudReconnectResponse (void);

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
