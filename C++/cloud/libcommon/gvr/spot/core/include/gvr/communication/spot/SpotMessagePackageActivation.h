/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageActivation.h
 * @author mgrieco
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEPACKAGEACTIVATION_H_
# define _SPOTMESSAGEPACKAGEACTIVATION_H_

#include <gvr/communication/spot/AbsSpotMessageWithAppCatCmdID.h>
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
       * @brief This class is the implementation for Package Activation message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessagePackageActivation : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification m3/m5.*/
          static const unsigned char CPackageActivationCommandId = 0x10;

          /**@brief Spot Message Identification m7.*/
          static const unsigned char CVgdPackageActivationCommandId = 0x60;

          /**@brief Spot Message Identification.*/
          static const unsigned char CPackageActivationSubCommandId = 0x04;

          /**@brief fix message header size*/
          static const int CMessageHeaderSize = 6 ;
          /**@brief Default Package Activation pakage time size */
          static const int CPackageActivationTimeSize = 16;
          /**@brief Default Package Activation pakage name size */
          static const int CPackageActivationNameSize = 80;
          /**@brief fix message size*/
          static const int CMessageCommonSize = CMessageHeaderSize + CPackageActivationTimeSize;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessagePackageActivation>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessagePackageActivation(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessagePackageActivation (void);

          /**
           * @brief Constructor by package name field.
           * @param package, activation package field
           * @since 0.0.0
           * */
          SpotMessagePackageActivation(std::string const package);

          /**
           * @brief Constructor by all fields.
           * @param package, activation package field
           * @since 0.0.0
           * */
          SpotMessagePackageActivation(bool const vgd, std::string const package);

          /**
           * @brief Overrides the inherited method. size.
           * @see IMessage::size()
           */
          SizeType size (void) const;

          /**
           * @brief Overrides the inherited method. serialize.
           * @see IMessage::serialize()
           */
          BufferType serialize (void) const;

          /**
           * @brief Overrides the inherited method. parse.
           * @see IMessage::parse()
           */
          bool parse (BufferType const &);

          /**
           * @brief Overrides the inherited method getSpotAppId according with the use of the message.
           * @see IMessage::parse()
           */
          unsigned char getSpotAppId( void ) const;

          /**
           * @brief Overrides the inherited method getSpotCommand according with the use of the message.
           * @see IMessage::parse()
           */
          unsigned char getCommand( void ) const;

          /**
           * @brief Sets the package name field.
           * @param name, package name field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setPackageActivationName( std::string const name );


          /**
           * @brief Retrieves the package name field.
           * @return The package name field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          std::string getPackageActivationName( void ) const;

          /**
           * @brief Sets the activation time field.
           * @param time, activation time field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setPackageActivationTime( std::string const time );

          /**
           * @brief Retrieves the activation time field.
           * @return The activation time field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          std::string getPackageActivationTime( void ) const;

          /**
           * @brief Sets the vgd platform field.
           * @param vgd, vgd platform field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setVgdPlatform(bool const vgd);
          /**
           * @brief Retrieves the vgd platform field.
           * @return The vgd platform field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          bool getVgdPlatform( void ) const;

        private:
          /** @brief activation package name field*/
          std::string packageName;

          /** @brief activation time field. See Spot Protocol Programmer guide on activation message description*/
          std::string activationTime;

          /** @brief activation vanguard platform option indicator*/
          bool vgdPlatform;
      };
    }
  }
}

#endif
