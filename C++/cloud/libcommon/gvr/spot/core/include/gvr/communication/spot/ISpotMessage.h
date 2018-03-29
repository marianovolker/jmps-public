/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file ISpotMessage.h
 * @author mgrieco
 * @date Friday, December 16, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _ISPOTMESSAGE_H_
# define _ISPOTMESSAGE_H_

# include <gvr/communication/IMessage.h>
# include <gvr/communication/spot/SpotMessageSpec.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Communication Domain
    namespace communication
    {
      /// @brief Spot Domain
      namespace spot
      {
        /**
         * @brief This class is the base interface class for all spot messages.
         * @see gvr.communication.MessageSpec
         * @since 0.0.0, 2014-12-16
         * @version 0.0.0, 2014-12-16
         * @author mgrieco
         */
        class ISpotMessage : public IMessage<SpotMessageSpec>
        {
          public:
            /**SPOT Applications Id for request*/
            static const unsigned char CMessageRequestSpotAppId;

            /** PINPAD Application Id for request, */
            static const unsigned char CMessageRequestPinpadAppId;
            /** SETUP Application Id for request. */
            static const unsigned char CMessageRequestSetupAppId;
            /** OPT Application Id for request. */
            static const unsigned char CMessageRequestOPTAppId;

            /**SPOT Applications Id for answers*/
            static const unsigned char CMessageResponseSpotAppId;
            /** PINPAD Application Id for answers. */
            static const unsigned char CMessageResponsePinpadAppId;
            /** SETUP Id for answers. */
            static const unsigned char CMessageResponseSetupAppId;
            /** OPT Application Id for answers. */
            static const unsigned char CMessageResponseOPTAppId;


            /**@brief Spot Message Identification fr.*/
            static const unsigned char CPinpadExtendedMessageCmdId;

            /**@brief Spot Message Identification for Pinpad Extended.*/
            static const unsigned char CEphemeralKeySubCmdId;


            /**@brief Spot Message Identification for New Ephemeral Key.*/
            static const unsigned char CNewEphemeralKeySubSubCmdId;
            /**@brief Spot Message Identification for Ephemeral encrypt.*/
            static const unsigned char CEphemeralEncryptSubCmdId;
            /**@brief Spot Message Identification for New Ephemeral decrypt.*/
            static const unsigned char CEphemeralDecryptSubCmdId;


            /**@brief No encryption message default value**/
            static const unsigned char CDefaulNoEncryptionValue;

            /**@brief the default ack code for No errors on Spot Messages*/
            static const unsigned char CErrorCodeNoError;
            /**@brief the default ack code for Download Complete*/
            static const unsigned char CErrorCodeDownloadComplete;
            /**@brief the default ack code for Debian Intermediate Block*/
            static const unsigned char CErrorCodeDebianIntermediateBlock;
            /**@brief the default ack code for User Already Logged*/
            static const unsigned char CErrorCodeUserAlreadyLogged;
            /**@brief the default ack code for debian activation ok*/
            static const unsigned char CErrorCodeDebianActivation;

            //Redefine types that are nor derived from an interface.
            typedef IMessage<SpotMessageSpec>::Spec       Spec; /// @brief The specification to which this ISpotMessage interface belongs.
            typedef IMessage<SpotMessageSpec>::KeyType    KeyType; /// @brief The the identification key type for the message which pairs request to responses (A.K.A. in some protocols as Sequence Identification Field).
            typedef IMessage<SpotMessageSpec>::BufferType BufferType;/// @brief The buffer container type for the serialized representation of the message.
            typedef IMessage<SpotMessageSpec>::SizeType   SizeType;/// @brief The size type for any size measurable field.
            typedef IMessage<SpotMessageSpec>::TypeId     TypeId;/// @brief The Message class type identification. A unique identifier value which represents a message class.


            //Getters common to all spot messages!

            /**
            * @brief Retrieves the spot application id for the message.
            * @return The message spot application id.
            * @exception Throws no exception.
            * @since 0.0.0
            */
            virtual unsigned char getSpotAppId (void) const = 0;

            /**
            * @brief Retrieves the spot application ssk field (encryption type) for the message.
            * @return The message spot ssk message field.
            * @exception Throws no exception.
            * @since 0.0.0
            */
            virtual unsigned char getSsk(void) const = 0;

            /**
            * @brief Retrieves the spot application command id for the message.
            * @return The message spot command id.
            * @exception Throws no exception.
            * @since 0.0.0
            */
            virtual unsigned char getCommand(void) const = 0;

            /* Not implemented
            virtual unsigned char getSK(void) = 0;
            virtual unsigned char getCrc32(void) = 0;
            virtual unsigned char getRnd(void) = 0;
            virtual unsigned char getDum(void) = 0;
            virtual unsigned char getCrc16(void) = 0;
            */

            /**
            * @brief Destructor
            * @since 0.0.0
            * */
            virtual ~ISpotMessage (void);
        };
      }
    }
  }

#endif
