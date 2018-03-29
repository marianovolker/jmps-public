/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageDebianInfoResponse.h
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEDEBIANINFORESPONSE_H_
# define _SPOTMESSAGEDEBIANINFORESPONSE_H_

#include <gvr/communication/spot/AbsSpotMessageWithAppCatCmdID.h>
# include <gvr/communication/spot/ISpotMessage.h>
# include <vector>
# include <sstream>

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
       * @brief This class is the implementation for Debian Info message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageDebianInfoResponse : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CDebianInfoCommandId = 0x10;

          /**@brief Spot Message Identification.*/
          static const unsigned char CDebianInfoSubCommandId = 0x05;

          /**@brief fix message header size*/
          static const int CMessageResponseCommonSize = 7 ;//size-appid-ssk-cmd-subcomd-ack-

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageDebianInfoResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageDebianInfoResponse(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageDebianInfoResponse (void);

          /**
           * @brief Constructor by all fields.
           * @param package, debian package name field
           * @since 0.0.0
           * */
          SpotMessageDebianInfoResponse(unsigned char ackCode);

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
           * @brief Retrieves the debian package info data field.
           * @return The debian package info field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          const std::string getInfoResponseData() const;

          /**
           * @brief Sets the debian package info field.
           * @param infoResponseData, debian package info field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setInfoResponseData(const std::string& infoResponseData);

        private:

          /** @brief info response data*/
          std::string infoResponseData;
      };
    }
  }
}

#endif
