/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileBlockDownload.h
 * @author mgrieco
 * @date Thursday, March 5, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEFILEBLOCKDOWNLOAD_H_
# define _SPOTMESSAGEFILEBLOCKDOWNLOAD_H_

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
       * @brief This class is the implementation for File Block Download message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageFileBlockDownload : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileBlockDownloadCommandId = 0x00;

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileBlockDownloadSubCommandId = 0x02;

          /**@brief fix message header size*/
          static const int CMessageHeaderSize = 6 ;
          /**@brief fix message size*/
          static const int CMessageCommonSize = CMessageHeaderSize + 4 + 2;//header,offset,size.

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageFileBlockDownload>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageFileBlockDownload(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageFileBlockDownload (void);

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
           * @brief Retrieves the file block data field.
           * @return The file block data field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          BufferType getBlockData( void ) const;

          /**
           * @brief Sets the file block data field.
           * @id package, file block data field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setBlockData( BufferType const data);

          /**
           * @brief Retrieves the block offset field.
           * @return The block offset field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          SizeType getBlockOffset( void ) const;

          /**
           * @brief Sets the block offset field.
           * @param offset, block offset field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setBlockOffset( SizeType const offset);

        private:
          /** @brief file block offset field*/
          SizeType blockOffset;
          /** @brief file block data field*/
          BufferType blockData;
      };
    }
  }
}

#endif
