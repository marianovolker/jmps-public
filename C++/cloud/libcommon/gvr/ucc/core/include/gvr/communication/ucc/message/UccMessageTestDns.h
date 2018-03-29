/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageTestDns.h
 * @author mgrieco
 * @date Monday, Jun 15, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGETESTDNS_H_
# define _UCCMESSAGETESTDNS_H_

#include <gvr/communication/ucc/message/base/AbsUccMessageInternal.h>

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
         * @brief This class is the server ucc cloud message test dns request.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-04-28
         * @version 0.0.0, 2015-04-28
         * @author mgrieco
         */
        class UccMessageTestDns : public AbsUccMessageInternal
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /** @brief Server name field name.*/
          static char const * CServerName;
          /** @brief Domain field name.*/
          static char const * CDomainName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageTestDns (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageTestDns (void);

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
