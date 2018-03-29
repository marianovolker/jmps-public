/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileBlockUploadResponse.h
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEFILEBLOCKUPLOADRESPONSE_H_
# define _SPOTMESSAGEFILEBLOCKUPLOADRESPONSE_H_

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
       * @brief This class is the implementation for file block Upload message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageFileBlockUploadResponse : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileBlockUploadCommandId = 0x00;

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileBlockUploadSubCommandId = 0x06;

          /**@brief fix message header size*/
          static const int CFileBlockUploadCommonResponseSize = 7 ;//size-appid-ssk-cmd-subcomd-ack

          /**@brief fix message header size*/
          static const int CFileBlockUploadCommonOkResponseSize = CFileBlockUploadCommonResponseSize + 4 + 4 ;//size-appid-ssk-cmd-subcomd-ack-offset-size

          /**@brief Block was received OK, another can be requested.*/
          static const unsigned char CBlockUploadedOK = 0x00;

          /**@brief The last block has arrived. No more blocks can't be requested.*/
          static const unsigned char CLastBlock = 0x02;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageFileBlockUploadResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageFileBlockUploadResponse(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageFileBlockUploadResponse (void);

          /**
           * @brief Constructor by all fields.
           * @param ackCode, ack answer code
           * @since 0.0.0
           * */
          SpotMessageFileBlockUploadResponse(unsigned char ackCode);

          /**
           * @brief Says if the last block requested was properly received OK.
           * @return @b true If the last block sent was received OK and another block can be retrieved.
           *         @b false Otherwise. Another block can not be received and whole upload must be restarted/resumed.
           * @see SpotMessagePackageBlockDownload::CBlockDownloadedOK
           */
          bool wasBlockReceivedOK();

          /**
           * @brief Says if the last block received is effectively the last block of the file.
           * @return @b true If the last block received is the last one of the file requested.
           *         @b false Otherwise. Another block can not be received and whole upload must be restarted/resumed.
           * @see SpotMessagePackageBlockDownload::CBlockDownloadedOK
           */
          bool isLastBlock();

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
           * @brief Retrieves the next block offset field.
           * @return The next block offset field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          const SizeType getStartingOffset() const;

          /**
           * @brief Sets the next block offset field.
           * @param offset, the next block offset field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setStartingOffset(const SizeType offset);

          /**
           * @brief Retrieves the file block data field.
           * @return The file block data field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          const BufferType & getBlockData( void ) const;

          /**
           * @brief Sets the file block data field.
           * @id package, file block data field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setBlockData( BufferType const data);


        private:

          /** @brief file block next offset field*/
          SizeType nextBlockOffset;

          /** @brief file block data field*/
          BufferType blockData;
      };
    }
  }
}

#endif
