/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file IUccMessage.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _IUCCMESSAGE_H_
# define _IUCCMESSAGE_H_

# include <gvr/communication/IMessage.h>
# include <gvr/communication/ucc/UccMessageSpec.h>
# include <string>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Utils Domain
    namespace utils
    {
      class Json;
      class AbsJson;
    }

    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Ucc Domain
      namespace ucc
      {
        /**
         * @brief This class is the base interface class for all ucc messages.
         * @see gvr.communication.IMessage
         * @see gvr.communication.ucc.UccMessageSpec
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class IUccMessage : public IMessage<UccMessageSpec>
        {
        public:
          //Redefine types that are nor derived from an interface.
          typedef IMessage<UccMessageSpec>::Spec       Spec; /// @brief The specification to which this ISpotMessage interface belongs.
          typedef IMessage<UccMessageSpec>::KeyType    KeyType; /// @brief The the identification key type for the message which pairs request to responses (A.K.A. in some protocols as Sequence Identification Field).
          typedef IMessage<UccMessageSpec>::BufferType BufferType; /// @brief The buffer container type for the serialized representation of the message.
          typedef IMessage<UccMessageSpec>::SizeType   SizeType; /// @brief The size type for any size measurable field.
          typedef IMessage<UccMessageSpec>::TypeId     TypeId; /// @brief The Message class type identification. A unique identifier value which represents a message class.
          typedef gvr::utils::Json                     JsonType; /// @brief The Message Json field;
          typedef gvr::utils::AbsJson                  AbsJsonType; /// @brief The Message Json field;

          /**
           * @brief Internal flag constant values.
           */
          static unsigned char const CInteralFlagInternalMessage;

          /**
           * @brief validate the Message fields.
           *
           * @return True if parse process is successful. False otherwise.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual bool validate (void) = 0;

          /**
           * @brief Retrieves the Message Internal flag value.
           *
           * @return The current Message internal flag.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual unsigned char getInternalFlag (void) const = 0;

          /**
           * @brief Check if message is an ucc internal message.
           *
           * @return True if message is an ucc internal message.
           *
           * @exception Throws no exception.
           *
           * @since 0.0.0
           */
          virtual bool isInternalMessage (void) const = 0;

          /**
           * @brief Retrieves the Json field.
           *
           * @since 0.0.0
           **/
          virtual AbsJsonType const & getJson (void) const = 0;

          /**
           * @brief Retrieves the Json field.
           *
           * @since 0.0.0
           **/
          virtual AbsJsonType & getJson (void) = 0;

          /**
           * @brief Sets the Json field.
           *
           * @param Json field content.
           *
           * @since 0.0.0
           **/
          virtual void setJson (AbsJsonType const &) = 0;

          /**
          * @brief Destructor
          * @since 0.0.0
          * */
          virtual ~IUccMessage (void) = 0;
        };
      }
    }
  }

#endif
