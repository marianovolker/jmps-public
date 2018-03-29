/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageFileDownloadStatusEvent.h
 * @author gparis
 * @date Wednesday, May 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGEFILEDOWNLOADSTATUSEVENT_H_
# define _UCCMESSAGEFILEDOWNLOADSTATUSEVENT_H_

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
         * @brief This class handles the UCC file download status event ("receiveFromCloud-status").
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-05-27
         * @version 0.0.0, 2015-05-27
         * @author gparis
         */
        class UccMessageFileDownloadStatusEvent : public AbsUccMessageInternal
        {
        public:

        enum EDownloadEventStatus
        {
          eStart = 0, eInProgress, eTransferFailure, eComplete, eTotalStatuses
        };

          /**@brief fix message operation*/
          static char const * CMessageOperation;

          static char const * CStatusName;
          static char const * CFileSizeName;
          static char const * CCurrentSizeName;
          static char const * CTrackingNumberName;
          static char const * CCodeName;
          static char const * CMessageName;


          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageFileDownloadStatusEvent (void);

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageFileDownloadStatusEvent (void);

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

          static const char * getStatusLegend (EDownloadEventStatus);


        private:

          static const char* CapcStatusLegends[ eTotalStatuses ]; ///<@brief Legends for status field
        };
      }
    }
  }

#endif
