/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlMessageTestDnsResponse.h
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CRLMESSAGETESTDNSRESPONSE_H_
# define _CRLMESSAGETESTDNSRESPONSE_H_

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
        class CrlMessageTestDnsResponse : public AbsCrlMessageInternal
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /** @brief Success field name. */
          static char const * CSuccessName;
          /** @brief Host ip field name. */
          static char const * CHostIpName;
          /** @brief Error Code field name. */
          static char const * CErrorCodeName;
          /** @brief Error Message field name. */
          static char const * CErrorMessageName;

          /** @brief Success field true string value. */
          static char const * CSuccessTrue;
          /** @brief Success field false string value. */
          static char const * CSuccessFalse;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~CrlMessageTestDnsResponse (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          CrlMessageTestDnsResponse (void);

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
