/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageCloudCommandRequest.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSUCCMESSAGECLOUDCOMMANDREQUEST_H_
# define _ABSUCCMESSAGECLOUDCOMMANDREQUEST_H_

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
         * @brief This class is the base class for all cloud command request ucc messages.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class AbsUccMessageCloudCommandRequest : public AbsUccMessageCloud
        {
        public:
          static char const * COperationName; /** @brief Ucc Cloud Command Request Operation field name. */
          static char const * CDeviceIdName;  /** @brief Ucc Cloud Command Request Device Id field name. */
          static char const * CPayloadName;   /** @brief Ucc Cloud Command Request Payload field name. */

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~AbsUccMessageCloudCommandRequest (void) = 0;

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
           * @brief Gets the expected message operation for this message.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getOperationExpected (void) const;

          /**
           * @brief Retrieves the Message Operation.
           *
           * @return The current Message operation.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getOperation (void) const;

          /**
           * @brief Retrieves the device Id to which this message address.
           *
           * @return The device Id.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual std::string getDeviceId  (void) const;

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
           * @brief Sets the device Id to which this message address.
           *
           * @param The device Id.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void setDeviceId (char const *);

          /**
           * @brief Sets the device Id to which this message address.
           *
           * @param The device Id.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void setDeviceId (std::string const &);

          /**
           * @brief Retrieves the payload field inside the  Json field.
           *
           * @since 0.0.0
           **/
          virtual JsonType const getPayload (void) const;

          /**
           * @brief Sets the payload field inside the Json field.
           *
           * @param Json field content.
           *
           * @since 0.0.0
           **/
          virtual void setPayload (AbsJsonType const &);

          /**
           * @brief Removes the payload field inside the Json field.
           *
           * @since 0.0.0
           **/
          virtual void removePayload (void);

        protected:
          /**
           * @brief Constructor.
           * 
           * @param Reference to the operation constant string.
           */
          AbsUccMessageCloudCommandRequest (char const *);

        protected:
          std::string const m_kOperation;
        };
      }
    }
  }

#endif
