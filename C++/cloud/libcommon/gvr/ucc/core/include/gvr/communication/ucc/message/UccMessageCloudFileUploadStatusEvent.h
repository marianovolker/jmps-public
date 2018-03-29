/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudFileUploadStatusEvent.h
 * @author mgrieco
 * @date Wednesday, May 27, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDFILEUPLOADSTATUSEVENT_H_
# define _UCCMESSAGECLOUDFILEUPLOADSTATUSEVENT_H_

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
         * @brief This class is the server ucc cloud file upload message event request.
         * @see gvr.communication.ucc.IUccMessage
         */
        class UccMessageCloudFileUploadStatusEvent : public AbsUccMessageCloudEventRequest
        {
        public:

          enum EUploadEventStatus
          {
            eStart = 0, eInProgress, eTransferFailure, eFailure, eComplete, eTotalStatuses
          };


          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**
           * @brief Message Constants Field names.
           */
          static char const * CFileCategoryName;
          static char const * CFileTypeName;
          static char const * CFileIdName;
          static char const * CTrackingNumberName;
          static char const * CStatusName;
          static char const * CFileCompressionName;
          static char const * CMessageName;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudFileUploadStatusEvent (void);

          /**
           * @brief Constructor.
           * */
          UccMessageCloudFileUploadStatusEvent (void);

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
          static const char * getStatusLegend (EUploadEventStatus);


        private:

          static const char* CapcStatusLegends[ eTotalStatuses ]; ///<@brief Legends for status field

        };
      }
    }
  }

#endif
