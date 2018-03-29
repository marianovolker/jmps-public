/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileDownload.h
 * @author mgrieco
 * @date Thursday, March 5, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEFILEDOWNLOAD_H_
# define _SPOTMESSAGEFILEDOWNLOAD_H_

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
       * @brief This class is the implementation for File Download message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageFileDownload : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileDownloadCommandId = 0x00;

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileDownloadSubCommandId = 0x01;

          /**@brief fix message header size*/
          static const int CMessageHeaderSize = 6 ;
          /**@brief fix message size*/
          static const int CMessageCommonSize = CMessageHeaderSize + 1 + 1 + 4 + 4 + 1 + 16;//header,filetype,fileid,filesize,crcdata,descript.

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageFileDownload>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageFileDownload(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageFileDownload (void);

          /**
           * @brief Constructor by all fields.
           * @param type, file type field
           * @param id, file id field
           * @since 0.0.0
           * */
          SpotMessageFileDownload(unsigned char const  type, unsigned char const id);

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

          /**
           * @brief Retrieves the file size field.
           * @return The file size field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          SizeType getFileSize( void ) const;

          /**
           * @brief Sets the file size field.
           * @param size, file size field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setFileSize( SizeType const size);

          /**
           * @brief Retrieves the file crc field.
           * @return The file crc field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          BufferType getCrcData( void ) const;

          /**
           * @brief Sets the file crc field.
           * @param crc, file crc field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setCrcData( BufferType const crc);

          /**
           * @brief Retrieves the file AuthenType field.
           * @return The file AuthenType field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          unsigned char getAuthenType( void ) const;
          /**
           * @brief Sets the file AuthenType field.
           * @param type, file AuthenType field
           * @exception Throws no exception.
           * @since 0.0.0
           */

          void setAuthenType( unsigned char const type);

          /**
           * @brief Retrieves the file description field.
           * @return The file description field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          BufferType getSecretAuthenticator( void ) const;
          /**
           * @brief Sets the file authenticator field.
           * @param authenticator, file authenticator field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setSecretAuthenticator( BufferType const authenticator);

          /**
           * @brief Retrieves the file description field.
           * @return The file description field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          std::string getDescription( void ) const;
          /**
           * @brief Sets the file description field.
           * @param description, file description field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setDescription( std::string const description);

        private:
          /** @brief file type field*/
          unsigned char fileType;
          /** @brief file id field*/
          unsigned char fileId;
          /** @brief file size field*/
          SizeType fileSize;
          /** @brief file crc32 data field*/
          BufferType crcData;
          /** @brief file authentication type field*/
          unsigned char authenType;
          /** @brief file secret authenticator field*/
          BufferType secretAuthenticator;
          /** @brief file id field*/
          std::string fileDescription;
      };
    }
  }
}

#endif
