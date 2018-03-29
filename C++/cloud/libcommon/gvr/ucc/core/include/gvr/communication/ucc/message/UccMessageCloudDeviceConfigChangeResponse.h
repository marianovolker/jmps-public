/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceConfigChangeResponse.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDDEVICECONFIGCHANGERESPONSE_H_
# define _UCCMESSAGECLOUDDEVICECONFIGCHANGERESPONSE_H_

#include <gvr/communication/ucc/message/base/AbsUccMessageCloudEventResponse.h>

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
         * @brief This class is the ucc message any.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class UccMessageCloudDeviceConfigChangeResponse : public AbsUccMessageCloudEventResponse
        {
        public:
          static char const * CSuccessName;
          static char const * CAssociatedIdName;
          static char const * CDeviceIdName;
          static char const * CDeviceTypeName;
          static char const * CLocalIdName;
          static char const * CCloudStateName;
          // TODO: Check with Business Analyst!!!
          //static char const * CFileUploadUrlName;
          static char const * CCodeName;
          static char const * CMessageName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudDeviceConfigChangeResponse (void);

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
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudDeviceConfigChangeResponse (void);

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
          virtual TypeId getTypeId (void) const;
        };
      }
    }
  }

#endif
