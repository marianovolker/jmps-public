/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageCloudEventResponse.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSUCCMESSAGECLOUDEVENTRESPONSE_H_
# define _ABSUCCMESSAGECLOUDEVENTRESPONSE_H_

#include <gvr/communication/ucc/message/base/AbsUccMessageCloud.h>

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
         * @brief This class is the base class for all cloud event response ucc messages.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class AbsUccMessageCloudEventResponse : public AbsUccMessageCloud
        {
        public:
          static char const * CSuccessName; /** @brief Ucc Cloud Event Response Success field name. */

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~AbsUccMessageCloudEventResponse (void) = 0;

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
           * @brief Resets message header to the expected values if missing.
           *
           * @param force boolean value which forces reseting messageId and deviceId. Default value is false.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void resetHeader (bool);

          /**
           * @brief Retrieves the success status of this message.
           *
           * @return The device Id.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual bool getSuccess  (void) const;

          /**
           * @brief Sets the success status of this message.
           *
           * @param The success status.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void setSuccess (bool);

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

        protected:
          /**
           * @brief Constructor.
           * 
           * @param Reference to the operation constant string.
           */
          AbsUccMessageCloudEventResponse (void);
        };
      }
    }
  }

#endif
