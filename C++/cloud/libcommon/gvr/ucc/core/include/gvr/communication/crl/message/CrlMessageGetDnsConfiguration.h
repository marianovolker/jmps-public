/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageGetDnsConfiguration.h
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLMESSAGEGETDNSCONFIGURATION_H_
# define _CRLMESSAGEGETDNSCONFIGURATION_H_

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
         */
        class CrlMessageGetDnsConfiguration : public AbsCrlMessageInternal
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~CrlMessageGetDnsConfiguration (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          CrlMessageGetDnsConfiguration (void);

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
