/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileUpload.h
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEFILEUPLOAD_H_
# define _SPOTMESSAGEFILEUPLOAD_H_

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
       * @brief This class is the implementation for File Upload message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageFileUpload : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileUploadCommandId = 0x00;

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileUploadSubCommandId = 0x05;

          /**@brief fix message header size*/
          static const int CMessageHeaderSize = 6 ;
          /**@brief fix message size*/
          static const int CMessageCommonSize = CMessageHeaderSize + 1 + 1;//header,filetype,fileid.

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageFileUpload>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageFileUpload(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageFileUpload (void);

          /**
           * @brief Constructor by all fields.
           * @param type, file type field
           * @param id, file id field
           * @since 0.0.0
           * */
          SpotMessageFileUpload(unsigned char const  type, unsigned char const id);

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
           * @brief Sets the file type field.
           * @type package, file type field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setFileType( unsigned char const type);

          /**
           * @brief Retrieves the file typea data field.
           * @return The file type field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getFileType( void ) const;
          /**
           * @brief Retrieves the file id field.
           * @return The file id field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getFileId( void ) const;

          /**
           * @brief Sets the file id field.
           * @id package, file id field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setFileId( unsigned char const id);

        private:
          /** @brief file type field*/
          unsigned char fileType;
          /** @brief file id field*/
          unsigned char fileId;
      };
    }
  }
}

#endif
