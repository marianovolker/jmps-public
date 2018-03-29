/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageCloudStatusResponse.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLMESSAGECLOUDSTATUSRESPONSE_H_
# define _CRLMESSAGECLOUDSTATUSRESPONSE_H_

#include <gvr/communication/crl/message/base/AbsCrlMessageInternal.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Crl Domain
      namespace crl
      {
        /**
         * @brief This class is the client crl internal message status.
         * @see gvr.communication.crl.ICrlMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class CrlMessageCloudStatusResponse : public AbsCrlMessageInternal
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
           * @brief Payload field names.
           */
          static char const * CConnectionStatusName;
          static char const * CRegistrationStatusName;
          static char const * CErrorCodeName;
          static char const * CErrorMessageName;
          /**
           * @brief Payload field values.
           */
          static char const * CConnectionDisconnectedString;
          static char const * CConnectionConnectingString;
          static char const * CConnectionConnectedString;
          static char const * CRegistrationUnregisteredString;
          static char const * CRegistrationUnregisteringString;
          static char const * CRegistrationRegisteringString;
          static char const * CRegistrationRegisteredString;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~CrlMessageCloudStatusResponse (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          CrlMessageCloudStatusResponse (void);

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

          /**
           * @brief Following methods Retrieves the current instance Message class type identification.
           *
           * @return The current Message class type identification.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.TypeId
           *
           * @since 0.0.0
           */
          static  TypeId GetTypeId (void);
          virtual TypeId getTypeId (void) const;
        };
      }
    }
  }

#endif
