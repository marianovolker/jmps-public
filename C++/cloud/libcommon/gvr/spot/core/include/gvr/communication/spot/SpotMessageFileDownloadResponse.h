/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileDownloadResponse.h
 * @author mgrieco
 * @date Wednesday, February 18, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEFILEDOWNLOADRESPONSE_H_
# define _SPOTMESSAGEFILEDOWNLOADRESPONSE_H_

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
       * @brief This class is the implementation for file download message response on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageFileDownloadResponse : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileDownloadCommandId = 0x00;

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileDownloadSubCommandId = 0x01;

          /**@brief fix message common size*/
          static const int CFileDownloadCommonResponseSize = 7;//size-appid-ssk-cmd-subcomd-ack

          /**@brief fix message complete size*/
          static const int CFileDownloadResponseSize = 11 ;//coomon +offset

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageFileDownloadResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageFileDownloadResponse(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageFileDownloadResponse (void);

          /**
           * @brief Constructor by all fields.
           * @param ackCode, ack answer code
           * @since 0.0.0
           * */
          SpotMessageFileDownloadResponse(unsigned char ackCode);

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
           * @brief Retrieves the file download starting offset data field.
           * @return The file download starting offset field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          const SizeType getStartingOffset() const;

          /**
           * @brief Sets the file download starting offset field.
           * @param offset, file download starting offset field
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
