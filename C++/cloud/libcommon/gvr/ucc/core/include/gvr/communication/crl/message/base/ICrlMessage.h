/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ICrlMessage.h
 * @author pcantarini
 * @date Monday, Mars 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ICRLMESSAGE_H_
# define _ICRLMESSAGE_H_

# include <gvr/communication/IMessage.h>
# include <gvr/communication/crl/CrlMessageSpec.h>
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
      /// @brief Crl Domain
      namespace crl
      {
        /**
         * @brief This class is the base interface class for all crl messages.
         * @see gvr.communication.IMessage
         * @see gvr.communication.crl.CrlMessageSpec
         * @since 0.0.0, 2015-03-02
         * @version 0.0.0, 2015-03-02
         * @author pcantarini
         */
        class ICrlMessage : public IMessage<CrlMessageSpec>
        {
        public:
          //Redefine types that are nor derived from an interface.
          typedef IMessage<CrlMessageSpec>::Spec       Spec; /// @brief The specification to which this ISpotMessage interface belongs.
          typedef IMessage<CrlMessageSpec>::KeyType    KeyType; /// @brief The the identification key type for the message which pairs request to responses (A.K.A. in some protocols as Sequence Identification Field).
          typedef IMessage<CrlMessageSpec>::BufferType BufferType; /// @brief The buffer container type for the serialized representation of the message.
          typedef IMessage<CrlMessageSpec>::SizeType   SizeType; /// @brief The size type for any size measurable field.
          typedef IMessage<CrlMessageSpec>::TypeId     TypeId; /// @brief The Message class type identification. A unique identifier value which represents a message class.
          typedef gvr::utils::Json                     JsonType; /// @brief The Message Json field;
          typedef gvr::utils::AbsJson                  AbsJsonType; /// @brief The Message Json field;

          /**
           * @brief Internal flag constant values.
           */
          static unsigned char const CInteralFlagInternalMessage;

          /**
           * @brief validate the Message fields.
           *
           * @return True if parse process is scrlessful. False otherwise.
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
           * @brief Check if message is an crl internal message.
           *
           * @return True if message is an crl internal message.
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
          virtual ~ICrlMessage (void) = 0;
        };
      }
    }
  }

#endif
