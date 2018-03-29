/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageConnectResponse.h
 * @author mgrieco
 * @date Monday, Apr 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECONNECTRESPONSE_H_
# define _UCCMESSAGECONNECTRESPONSE_H_

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
         * @brief This class is the client ucc cloud message connect response.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-04-28
         * @version 0.0.0, 2015-04-28
         * @author mgrieco
         */
        class UccMessageConnectResponse : public AbsUccMessageInternal
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
           * @brief Field names.
           */
          static char const * CPayloadName;
          static char const * CSuccessName;
          static char const * CCodeName;
          static char const * CMessageName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageConnectResponse (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageConnectResponse (void);

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
