/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageInternal.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSUCCMESSAGEINTERNAL_H_
# define _ABSUCCMESSAGEINTERNAL_H_

#include <gvr/communication/ucc/message/base/AbsUccMessage.h>

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
         * @brief This class is the base class for all internal ucc messages.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class AbsUccMessageInternal : public AbsUccMessage
        {
        public:
          static char const * COperationName; /** @brief Ucc Operation field name. */
          static char const * CMessageIdName; /** @brief Ucc Message Id field name. */
          static char const * CDeviceIdName;  /** @brief Ucc Device Id field name. */
          static char const * CPayloadName;   /** @brief Ucc Payload field name. */

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~AbsUccMessageInternal (void) = 0;

          /**
           * @brief Retrieves the current Message key identification, which in many protocols is the sequence number.
           *
           * @return The current Message key identification.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.KeyType
           *
           * @since 0.0.0
           */
          virtual KeyType getKey (void) const;

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
           * @brief Retrieves the current Message identification. Returns the same value as getKey.
           *
           * @return The current Message identification.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.IMessage.KeyType
           * @see gvr.communication.IMessage.getKey()
           *
           * @since 0.0.0
           */
          virtual std::string getMessageId (void) const;

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
           * @brief Sets the message id to which this message address.
           *
           * @param The message Id.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void setMessageId (char const *);

          /**
           * @brief Sets the message id to which this message address.
           *
           * @param The message Id.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void setMessageId (std::string const &);

          /**
           * @brief Generates a new message id to which this message address.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual void generateMessageId (void);

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
          AbsUccMessageInternal (char const *);

        protected:
          std::string const m_kOperation;
        };
      }
    }
  }

#endif
