/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file AbsUccMessageCloud.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ABSUCCMESSAGECLOUD_H_
# define _ABSUCCMESSAGECLOUD_H_

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
         * @brief This class is the base class for all cloud ucc messages.
         * @see gvr.communication.ucc.IUccMessage
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class AbsUccMessageCloud : public AbsUccMessage
        {
        public:
          static char const * CMessageIdName; /** @brief Ucc Cloud Message Id field name. */

          /**
           * @brief Internal flag constant values.
           */
          static unsigned char const CInteralFlagCloudMessage;

          /**
           * @brief Destructor
           * @since 0.0.0
           */
          virtual ~AbsUccMessageCloud (void) = 0;

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
           * @brief Following method retrieves the current instance Message class type identification.
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

        protected:
          /**
           * @brief Constructor.
           * 
           * @param Reference to the operation constant string.
           */
          AbsUccMessageCloud (void);
        };
      }
    }
  }

#endif
