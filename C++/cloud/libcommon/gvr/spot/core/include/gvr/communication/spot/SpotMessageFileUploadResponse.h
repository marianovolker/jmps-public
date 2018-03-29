/*
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageFileUploadResponse.h
 * @author mgrieco
 * @date Friday, May 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGEFILEUPLOADRESPONSE_H_
# define _SPOTMESSAGEFILEUPLOADRESPONSE_H_

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
       * @brief This class is the implementation for file upload message response on Spot protocol.
       * @see gvr.communication.spot.ISpotMessage
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageFileUploadResponse : public AbsSpotMessageWithAppCatCmdID
      {
        public:

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileUploadCommandId = 0x00;

          /**@brief Spot Message Identification.*/
          static const unsigned char CFileUploadSubCommandId = 0x05;

          /**@brief fix message common size*/
          static const int CFileUploadCommonResponseSize = 7;//size-appid-ssk-cmd-subcomd-ack

          /**@brief fix message complete size*/
          static const int CFileUploadResponseSize = CFileUploadCommonResponseSize + 4 ;//filesize

          /**@brief SPOT accepted file upload, it can then begin.*/
          static const unsigned char CPackageUploadAccepted = 0x00;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageFileUploadResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief Default Constructor
           * @since 0.0.0
           * */
          SpotMessageFileUploadResponse(void);
          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          ~SpotMessageFileUploadResponse (void);

          /**
           * @brief Constructor by all fields.
           * @param ackCode, ack answer code
           * @since 0.0.0
           * */
          SpotMessageFileUploadResponse(unsigned char ackCode);

          /**
           * @brief Says if the requested package download can begin.
           * @return @b true If the certificate sent is OK and package download can begin.
           *         @b false Otherwise. The package download was not accepted and can not begin.
           * @see SpotMessagePackageDownload::CPackageDownloadAccepted
           */
          bool canUploadBegin();

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
           * @brief Retrieves the file Upload size data field.
           * @return The file Upload size field.
           * @exception Throws no exception.
           * @since 0.0.0
           */
          const SizeType getFileSize() const;

          /**
           * @brief Sets the file Upload size field.
           * @param offset, file Upload size field
           * @exception Throws no exception.
           * @since 0.0.0
           */
          void setFileSize(const SizeType size);

        private:

          /** @brief file size field*/
          SizeType fileSize;
      };
    }
  }
}

#endif
