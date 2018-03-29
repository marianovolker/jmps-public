/*
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageLogin.h
 * @author mgrieco
 * @date Friday, December 22, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGELOGIN_H_
# define _SPOTMESSAGELOGIN_H_

# include <gvr/communication/spot/ISpotMessage.h>
# include <vector>

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
       * @brief This class is the implementation for Login message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageLogin : public ISpotMessage
      {
        public:

          /**@brief fix message size*/
          static const int CMessageSize = 9;

          /**@brief Spot Message Identification.*/
          static const unsigned char CLoginMessageId = 0x01;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageLogin>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageLogin(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageLogin (void);

          /**
           * @brief Constructor by all fields.
           * @param clientId, login client id request value
           * @param modulolen, login modulo length request value. Length of PMK RSA module (zero = no Key required).
           * @param pingTimeout, login ping timeout request value. 0=infinite seconds i.e.: no timeout.
           * @since 0.0.0
           * */
          SpotMessageLogin(unsigned char const clientId, int const moduloLen, unsigned char const pingTimeout);

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
           * @brief Sets the login client id request value.
           * @param id, login client id request value
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setClientId( unsigned  char id );

          /**
           * @brief Sets the login modulo length request value.
           * @param modulolen, login modulo length request value
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setModuloLen( int moduloLen );

          /**
           * @brief Sets the login ping timeout request value.
           * @param timeout, login ping timeout request value
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setPingTimeout( unsigned char timeout );

          /**
           * @brief Retrieves the client id request value.
           * @return The client id request value.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getClientId( void ) const;

          /**
           * @brief Retrieves the login modulo len request value.
           * @return The login modulo len request value.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          int getModuloLen( void ) const;

          /**
           * @brief Retrieves the login ping timeout request value.
           * @return The login ping timeout request value.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getPingTimeout( void ) const;

        private:
          /** @brief login clientId request field value*/
          unsigned char clientId;

          /** @brief login modulo length request field value*/
          int moduloLen;

          /** @brief login ping timeout request field value*/
          unsigned char pingTimeout;
      };
    }
  }
}

#endif
