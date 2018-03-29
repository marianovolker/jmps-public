/*
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePingResponse.h
 * @author mgrieco
 * @date Friday, December 29, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEPINGRESPONSE_H_
# define _SPOTMESSAGEPINGRESPONSE_H_

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
       * @brief This class is the implementation for Ping message response on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessagePingResponse : public ISpotMessage
      {
        private:
          /** @brief acknowledge response code*/
          unsigned char ackCode;

        public:

          /**@brief default message size*/
          static const int CMessageSize = 6;

          /**@brief Spot Message Identification.*/
          static const unsigned char CPingMessageId = 0x03;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessagePingResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           */
          SpotMessagePingResponse(void);

          /**
           * Constructor giving the acknowledge value.
           * @param the acknowledge value for the Ping Response.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          SpotMessagePingResponse(unsigned char);

          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessagePingResponse (void);

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
           * @brief Retrieves the acknowledge response code.
           * @return The acknowledge response code.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getAckCode( void ) const;

          /**
           * @brief Sets the acknowledge response code.
           * @param ackcode. The acknowledge response code.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setAckCode( unsigned char const );
      };
    }
  }
}

#endif
