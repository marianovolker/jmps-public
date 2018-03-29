/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageNewEphemeralKeyResponse.h
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGENEWEPHEMERALKEYRESPONSE_H_
# define _SPOTMESSAGENEWEPHEMERALKEYRESPONSE_H_

# include <gvr/communication/spot/ISpotMessage.h>
# include <vector>
# include <iostream>

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
       * @brief This class is the implementation for new ephemeral key message response on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageNewEphemeralKeyResponse : public ISpotMessage
      {
        private:
          /** @brief acknowledge response code*/
          unsigned char m_ackCode;

          /** @brief in case of error this field will be filled with the specific error.*/
          unsigned char m_specificError;

        public:

          /**@brief default message size*/
          static const int CMessageSize = 8;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageNewEphemeralKeyResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           */
          SpotMessageNewEphemeralKeyResponse(void);

          /**
           * Constructor giving the acknowledge value.
           * @param the acknowledge value for the new ephemeral key Response.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          SpotMessageNewEphemeralKeyResponse(unsigned char);

          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageNewEphemeralKeyResponse (void);

          /**
           * @brief Implements the inherited method. getKey.
           * @see IMessage::getKey()
           */
          KeyType getKey (void) const;

          /**
           * @brief Implements the inherited method. size.
           * @see IMessage::size()
           */
          SizeType size (void) const;

          /**
           * @brief Implements the inherited method. serialize.
           * @see IMessage::serialize()
           */
          BufferType serialize (void) const;

          /**
           * @brief Implements the inherited method. parse.
           * @see IMessage::parse()
           */
          bool parse (BufferType const &);

          /**
           * @brief Check if the message is valid.
           * @see IMessage::valid()
           */
          bool valid () {return (m_ackCode==0x00);}

          /**
           * @brief Retrieves the spot application id.
           * @return The spot application id.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getSpotAppId (void) const;

          /**
           * @brief Retrieves the ssk message field.
           * @return The ssk message field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getSsk (void) const;

          /**
           * @brief Retrieves the command message field.
           * @return The command message field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getCommand(void) const;

          /**
           * @brief Retrieves the subcommand message field.
           * @return The subcommand one message field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getSubCommandOne() const;
          /**
           * @brief Retrieves the subcommand message field.
           * @return The subcommand two message field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getSubCommandTwo() const;

          /**
           * @brief Retrieves the acknowledge response code.
           * @return The acknowledge response code.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getAckCode( void ) const;

          /**
           * @brief Retrieves the specific error code in case of error.
           * @return Error code in case of error.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getSpecificError( void ) const;
      };
    }
  }
}

#endif
