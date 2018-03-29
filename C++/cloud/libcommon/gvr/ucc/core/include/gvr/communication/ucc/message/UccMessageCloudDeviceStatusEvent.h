/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudDeviceStatusEvent.h
 * @author mgrieco
 * @date Friday, Jul 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDDEVICESTATUSEVENT_H_
# define _UCCMESSAGECLOUDDEVICESTATUSEVENT_H_

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
         * @brief This class is the server ucc cloud device component message event request.
         * @see gvr.communication.ucc.IUccMessage
         */
        class UccMessageCloudDeviceStatusEvent : public AbsUccMessageCloudEventRequest
        {
        public:


          /**
           * @brief This enumeration contains the default components status.
           * */
          enum EComponentStatus
          {
            eIdle=0,eInUse, eDown, eInWarning, eInError, eStartup, eTotalStatuses
          };


          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
           * @brief Message Constants Field names.
           */
          /**@brief Status message field name*/
          static char const * CStatusName;
          /**@brief Atttribute message field name*/
          static char const * CAttributeName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudDeviceStatusEvent (void);

          /**
           * @brief Constructor.
           * */
          UccMessageCloudDeviceStatusEvent (void);

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
          static  TypeId      GetTypeId (void);
          virtual TypeId      getTypeId (void) const;

          /**
           * @brief Returns the legend for a given status.
           * @param The given status.
           * @return The status legend for a the given status
           * */
          static const char * getStatusLegend(EComponentStatus &);


        private:

          static const char* CapcStatusLegends[ eTotalStatuses ]; ///<@brief Legends for status field

        };
      }
    }
  }

#endif
