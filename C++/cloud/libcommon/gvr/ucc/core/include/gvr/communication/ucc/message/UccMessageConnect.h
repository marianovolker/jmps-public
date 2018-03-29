/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageConnect.h
 * @author mgrieco
 * @date Monday, Apr 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECONNECT_H_
# define _UCCMESSAGECONNECT_H_

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
         * @brief This class is the server ucc cloud message connect request.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-04-28
         * @version 0.0.0, 2015-04-28
         * @author mgrieco
         */
        class UccMessageConnect : public AbsUccMessageInternal
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
           * @brief Field names.
           */
          static char const * CUriName;
          static char const * CNameServer1Name; /**@brief Dns server 1 field name */
          static char const * CNameServer2Name; /**@brief Dns server 2 field name */
          static char const * CCertFormatName;
          static char const * CCertTypeName;
          static char const * CUseridName;
          static char const * CPwdName;
          static char const * CConnectionDelayName; /**@brief Connection delay field name for reconnection case */
          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageConnect (void);

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageConnect (void);

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
