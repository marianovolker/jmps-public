/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageSendToCloudStatusEvent.h
 * @author mgrieco
 * @date Thursday, May 28, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGESENDTOCLOUDSTATUSEVENT_H_
# define _UCCMESSAGESENDTOCLOUDSTATUSEVENT_H_

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
         * @brief This class handles the UCC file upload status event ("sendToCloud-status").
         * @see gvr.communication.ucc.IUccMessage
         */
        class UccMessageSendToCloudStatusEvent : public AbsUccMessageInternal
        {
        public:

          /**@brief default valid status value*/
          enum ESendToCloudEventStatus
          {
            eStart = 0, eInProgress, eTransferFailure, eComplete, eTotalStatuses
          };

          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
           * @brief Message Constants Field names.
           */
          static char const * CStatusName;
          static char const * CFileSizeName;
          static char const * CCurrentSizeName;
          static char const * CTrackingNumberName;
          static char const * CFileIdName;
          static char const * CCodeName;
          static char const * CMessageName;


          /**
           * @brief Constructor.
           */
          UccMessageSendToCloudStatusEvent (void);

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageSendToCloudStatusEvent (void);

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
           * @brief Following methods retrieve the current instance Message class type identification.
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

          /**
           * @brief Retrieves the corresponding legend for the given event status
           * @param the status to get the legend
           * @return the legend
           * */
          static const char * getStatusLegend (ESendToCloudEventStatus);


        private:

          static const char* CStatusLegends[ eTotalStatuses ]; ///<@brief Legends for event status field

        };
      }
    }
  }

#endif
