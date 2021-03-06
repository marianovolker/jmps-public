/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageServerUpdate.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGESERVERUPDATE_H_
# define _UCCMESSAGESERVERUPDATE_H_

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
         * @brief This class is the client ucc internal message status.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class UccMessageServerUpdate : public AbsUccMessageInternal
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
           * @brief Payload field names.
           */
          static char const * CUrlName;
          static char const * CDnsServer1Name; /**@brief Dns Server1 Name field name * */
          static char const * CDnsServer2Name; /**@brief Dns Server2 Name field name * */
          static char const * CCertName;
          static char const * CCertFormatName;
          static char const * CCertPwdName;
          static char const * CUserName;
          static char const * CPwdName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageServerUpdate (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageServerUpdate (void);

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
