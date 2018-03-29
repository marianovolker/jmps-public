/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileDeleteResponse.h
 * @author mgrieco
 * @date Friday, March 6, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEFILEDELETERESPONSE_H_
# define _SPOTMESSAGEFILEDELETERESPONSE_H_

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
       * @brief This class is the implementation for file delete message response on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageFileDeleteResponse : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileDeleteCommandId = 0x00;

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileDeleteSubCommandId = 0x04;

          /**@brief fix message header size*/
          static const int CFileDeleteResponseSize = 7 ;//size-appid-ssk-cmd-subcomd-ack

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageFileDeleteResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageFileDeleteResponse(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageFileDeleteResponse (void);

          /**
           * @brief Constructor by all fields.
           * @param ackCode, ack answer code
           * @since 0.0.0
           * */
          SpotMessageFileDeleteResponse(unsigned char ackCode);

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

        private:

      };
    }
  }
}

#endif
