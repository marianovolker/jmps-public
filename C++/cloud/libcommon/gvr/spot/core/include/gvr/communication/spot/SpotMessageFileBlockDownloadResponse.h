/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileBlockDownloadResponse.h
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEFILEBLOCKDOWNLOADRESPONSE_H_
# define _SPOTMESSAGEFILEBLOCKDOWNLOADRESPONSE_H_

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
       * @brief This class is the implementation for file block download message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageFileBlockDownloadResponse : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileBlockDownloadCommandId = 0x00;

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileBlockDownloadSubCommandId = 0x02;

          /**@brief fix message header size*/
          static const int CFileBlockDownloadResponseSize = 11 ;//size-appid-ssk-cmd-subcomd-ack-offset

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageFileBlockDownloadResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageFileBlockDownloadResponse(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageFileBlockDownloadResponse (void);

          /**
           * @brief Constructor by all fields.
           * @param ackCode, ack answer code
           * @since 0.0.0
           * */
          SpotMessageFileBlockDownloadResponse(unsigned char ackCode);

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
           * @brief Retrieves the file block download starting offset data field.
           * @return The file block download starting offset field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          const SizeType getStartingOffset() const;

          /**
           * @brief Sets the file block download starting offset field.
           * @param offset, file block download starting offset field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setStartingOffset(const SizeType offset);

        private:

          /** @brief info response data*/
          SizeType startingOffset;
      };
    }
  }
}

#endif
