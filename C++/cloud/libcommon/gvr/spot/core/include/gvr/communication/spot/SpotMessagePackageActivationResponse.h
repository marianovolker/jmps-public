/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageActivationResponse.h
 * @author mgrieco
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEPACKAGEACTIVATIONRESPONSE_H_
# define _SPOTMESSAGEPACKAGEACTIVATIONRESPONSE_H_

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
       * @brief This class is the implementation for Package Activation response message on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessagePackageActivationResponse : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification m3/m5.*/
          static const unsigned char CPackageActivationCommandId = 0x10;

          /**@brief Spot Message Identification m7.*/
          static const unsigned char CVgdPackageActivationCommandId = 0x60;

          /**@brief Spot Message Identification.*/
          static const unsigned char CPackageActivationSubCommandId = 0x04;

          /**@brief fix message header size*/
          static const int CMessageResponseCommonSize = 7 ;//size-appid-ssk-cmd-subcomd-ack-

          /** @brief Void package/debian list for activation */
          static const unsigned char CNoPackagesForUpdate = 0x1B ;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessagePackageActivationResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessagePackageActivationResponse(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessagePackageActivationResponse (void);

          /**
           * @brief Constructor by all fields.
           * @param package, activation package name field
           * @since 0.0.0
           * */
          SpotMessagePackageActivationResponse(unsigned char ackCode);

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
           * @brief Overrides the inherited method getCommand according with the use of the message.
           * @see IMessage::parse()
           */
          unsigned char getCommand( void ) const;

          /**
           * @brief Retrieves the activation response data field.
           * @return The activation response data field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          const std::string getInfoResponseData() const;

          /**
           * @brief Sets the activation response data field.
           * @param infoResponseData, activation response data field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setInfoResponseData(const std::string& infoResponseData);

          /**
           * @brief Sets the vgd platform field.
           * @param vgd, vgd platform field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setVgdPlatform(bool vgd);

          /**
           * @brief Retrieves the vgd platform field.
           * @return The vgd platform field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          bool getVgdPlatform( void ) const;

          /**
           * @brief Retrieves the error condition of the last package activation.
           * @return @b true if the ACKCode == 0, @b false otherwise.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          bool hasError( void ) const;


        private:

          /** @brief info response data*/
          std::string infoResponseData;

          /** @brief activation vanguard platform option indicator*/
          bool vgdPlatform;
      };
    }
  }
}

#endif
