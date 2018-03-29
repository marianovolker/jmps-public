/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceConfigChange.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDDEVICECONFIGCHANGE_H_
# define _UCCMESSAGECLOUDDEVICECONFIGCHANGE_H_

#include <gvr/communication/ucc/message/base/AbsUccMessageCloudEventRequest.h>

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
        class UccMessageCloudDeviceConfigChange : public AbsUccMessageCloudEventRequest
        {
        public:
          static char const * CMessageOperation;

          static char const * CDeviceTypeName;
          static char const * CLocalIdName;
          static char const * CAssetInfoName;
          static char const * CManufacturerName;
          static char const * CModelName;
          static char const * CSerialNbrName;
          static char const * CFirmwareName;
          static char const * CComponentName;
          static char const * CNameName;
          static char const * CRemoveName;
          static char const * CAtributesName;
          static char const * CEventsName;
          static char const * COperationName;
          static char const * CPayloadName;
          static char const * CCmdsName;
          static char const * CRequestName;
          static char const * CResponseName;
          static char const * CAssociatedName;
          // TODO: Check with Business Analyst!!!
          //static char const * CProxiedName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudDeviceConfigChange (void);

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
          UccMessageCloudDeviceConfigChange (void);

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
