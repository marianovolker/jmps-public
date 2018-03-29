/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudInstall.h
 * @author gparis
 * @date Monday, April 13, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDINSTALL_H_
# define _UCCMESSAGECLOUDINSTALL_H_

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
         * @brief This class is the server ucc cloud message install response.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-04-13
         * @version 0.0.0, 2015-04-13
         * @author gparis
         */
        class UccMessageCloudInstall : public AbsUccMessageCloudCommandRequest
        {
        public:
          /**@brief fix message operation*/
          static char const * CMessageOperation;
          static char const * CFileTypeName;
          static char const * CTrackingNumberName;
          static char const * CUrlName;
          static char const * CInstallWindowName;
          static char const * CStartTimeName;
          static char const * CEndTimeName;

          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudInstall (void);

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudInstall (void);

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

#endif // _UCCMESSAGECLOUDINSTALL_H_

