/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudReconnect.h
 * @author mgrieco
 * @date Monday, Jun 08, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDRECONNECT_H_
# define _UCCMESSAGECLOUDRECONNECT_H_

#include <gvr/communication/ucc/message/base/AbsUccMessageCloudCommandRequest.h>

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
         * @brief This class is the server ucc reconnect change message request.
         * @see gvr.communication.ucc.IUccMessage
         */
        class UccMessageCloudReconnect : public AbsUccMessageCloudCommandRequest
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudReconnect (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudReconnect (void);

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
