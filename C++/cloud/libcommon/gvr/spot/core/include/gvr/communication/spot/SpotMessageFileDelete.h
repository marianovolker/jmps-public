/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileDelete.h
 * @author mgrieco
 * @date Thursday, March 5, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEFILEDELETE_H_
# define _SPOTMESSAGEFILEDELETE_H_

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
       * @brief This class is the implementation for File Delete message request on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageFileDelete : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileDeleteCommandId = 0x00;

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileDeleteSubCommandId = 0x04;

          /**@brief fix message header size*/
          static const int CMessageHeaderSize = 6 ;//size-appid-ssk-cmd-subcomd
          /**@brief fix message size*/
          static const int CMessageSize = CMessageHeaderSize + 2;//header+filetype,fileid

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageFileDelete>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageFileDelete(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageFileDelete (void);

          /**
           * @brief Constructor by all fields.
           * @param type, file type field
           * @param id, file id field
           * @since 0.0.0
           * */
          SpotMessageFileDelete(unsigned char const  type, unsigned char const id);

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
           * @param package, file type field
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
           * @param package, file id field
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
