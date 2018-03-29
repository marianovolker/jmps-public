/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file UccMessageCloudInstallStatus.h
 * @author gparis
 * @date Tuesday, April 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _UCCMESSAGECLOUDINSTALLSTATUS_H_
# define _UCCMESSAGECLOUDINSTALLSTATUS_H_

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
         * @brief This class is the server ucc cloud installation status message response.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-04-14
         * @version 0.0.0, 2015-04-14
         * @author gparis
         */
        class UccMessageCloudInstallStatus : public AbsUccMessageCloudEventRequest
        {
        public:

          /**
           * @brief The enumeration of the diferent values of the field "status".
           */
          enum EStatus
          {
            eStart, eInProgress, eComplete, eFailure, eTotalStatuses
          };

          static const char * CapcStatuses[eTotalStatuses];///< @brief The string labels corresponding to the different statuses.

          /**@brief fix message operation*/
          static char const * CMessageOperation;

          /**@brief message labels and fix values.*/
          static char const * CFileTypeName;
          static char const * CTrackingNumberName;
          static char const * CRollBackLabel;
          static char const * CTrue;
          static char const * CFalse;
          static char const * CStatusLabel;
          static char const * CStatusOK;
          static char const * CStatusError;
          static char const * CMessageLabel;


          /**
           * @brief Constructor.
           *
           * @param Reference to the operation constant string.
           * @param Reference to the Structure header.
           */
          UccMessageCloudInstallStatus (void);


          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~UccMessageCloudInstallStatus (void);


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
           * @brief Sets the payload status field according to the enumerated value.
           * @param @b e The status enumeration value whose string value is to be set in "status" field.
           */
          void setStatus(EStatus e);

          /**
           * @brief Sets the payload status field to "failure" and also the given error message.
           * @param @b sError the error message to be set.
           */
          void setError(const std::string & sError);


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

#endif // _UCCMESSAGECLOUDINSTALLSTATUS_H_

