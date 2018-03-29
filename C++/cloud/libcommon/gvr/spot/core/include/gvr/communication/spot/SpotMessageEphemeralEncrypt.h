/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageEphemeralEncrypt.h
 * @author mgrieco
 * @date Tuesday, March 31, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEEPHEMERALENCRYPT_H_
# define _SPOTMESSAGEEPHEMERALENCRYPT_H_

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
       * @brief This class is the implementation for new ephemeral key message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageEphemeralEncrypt : public ISpotMessage
      {
        public:

          /**@brief fix message size*/
          static const int CMinimumMessageSize = 7;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageEphemeralEncrypt>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @param decrypt to indicate if a command is used as decrypt command.
           * @since 0.0.0
           * */
          SpotMessageEphemeralEncrypt( bool decrypt = false );

          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageEphemeralEncrypt (void);

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
          bool parse (const BufferType &);

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
           * @brief Retrieves the encrypt/decrypt data message field.
           * @return The encrypt/decrypt data message field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          const BufferType& getData( void ) const;

          /**
           * @brief Sets the encrypt/decrypt data message field.
           * @param data. The encrypt/decrypt data message field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setData( const BufferType& data );

        private:
          /** @brief data to encrypt field*/
          BufferType m_data;

          /** @brief decrypt boolean value field*/
          bool m_decrypt;
      };
    }
  }
}

#endif
