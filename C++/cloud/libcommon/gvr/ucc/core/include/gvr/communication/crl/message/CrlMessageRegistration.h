/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageRegistration.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLMESSAGEREGISTRATION_H_
# define _CRLMESSAGEREGISTRATION_H_

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
        class CrlMessageRegistration : public AbsCrlMessageInternal
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
           * @brief Payload field names.
           */
          static char const * CRegistrationCodeName;
          static char const * CLocalIdName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~CrlMessageRegistration (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          CrlMessageRegistration (void);

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
