/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageVGDFileTransferResponse.h
 * @author Guillermo Paris
 * @date Monday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEVGDFILETRANSFERRESPONSE_H_
#define SPOTMESSAGEVGDFILETRANSFERRESPONSE_H_


#include <gvr/communication/spot/AbsSpotMessageWithAppCatCmdID.h>

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
 * @brief This class is the handler implementation of vanguard spot file/block upload/download response messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-03-16
 * @version 0.0.0, 2015-03-16
 * @author Guillermo Paris
 */
class SpotMessageVGDFileTransferResponse : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /** @brief Minimum Message size. @note This message has variable size. */
    static const SizeType  CMinMessageSize = 6;

    /**@brief Spot Message Identification, file transfer category code.*/
    static const unsigned char CVGDFileTransferCategoryId = 0x10;

    /**@brief Spot Message Identification, download file command code.*/
    static const unsigned char CVGDFileDownloadTransferCommandId = 0x01;

    /**@brief Spot Message Identification, download block command code.*/
    static const unsigned char CVGDBlockDownloadTransferCommandId = 0x02;

    /**@brief Spot Message Identification, upload file command code.*/
    static const unsigned char CVGDFileUploadTransferCommandId = 0x03;

    /**@brief Spot Message Identification, upload block command code.*/
    static const unsigned char CVGDBlockUploadTransferCommandId = 0x04;

    /** @brief Item File Size word. Offset inside message. */
    static const ISpotMessage::SizeType CFileSizeOffset = 0x06;

    /** @brief Item File Block Data. Offset inside message. */
    static const ISpotMessage::SizeType CFileDataOffset = 0x06;

    /**@brief Block Size - maximum .*/
    static const ISpotMessage::SizeType CMaxBlockSize = 65528;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageVGDFileTransferResponse>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };


   /**
    * @brief default Constructor
    */
    SpotMessageVGDFileTransferResponse (void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessageVGDFileTransferResponse (void);

   /**
    * @brief Serializes the hardware info request message to a byte vector.
    * @see IMessage::serialize()
    */
    BufferType serialize (void) const;

   /**
    * @brief Parses the given buffer expecting to have a SPOT PackageBlock Info Request message inside it.
    * @brief This parsing of a request is only used for tests.
    * @see IMessage::parse()
    */
    bool parse (BufferType const &);

   /**
    * @brief Says if the requested package download can begin.
    * @return @b true If the certificate sent is OK and package download can begin.
    *         @b false Otherwise. The package download was not accepted and can not begin.
    * @see SpotMessagePackageDownload::CPackageDownloadAccepted
    */
    bool canDownloadBegin();

   /**
    * @brief Says if the block upload/download was finished OK.
    * @return @b true If the block transfer was finished OK,  @b false otherwise.
    *
    * @see SpotMessagePackageDownload::CPackageDownloadAccepted
    */
    bool wasBlockTransferOK();

    /**
    * @brief Gets the command code (type of operation).
    * @return @b byte The operation command code ranging from CVGDFileDownloadTransferCommandId (0x01)
    *                 to CVGDBlockUploadTransferCommandId (0x04).
    */
    unsigned char getOperationCommand (void) const;

   /**
    * @brief Sets the command code (type of operation).
    * @param @b bCommandCode is the command code ranging from CVGDFileDownloadTransferCommandId (0x01)
    *           to CVGDBlockUploadTransferCommandId (0x04).
    */
    void setOperationCommand (unsigned char bCommandCode);

   /**
    * @brief Gets the file length.
    * @return The file length to be uploaded.
    */
    ISpotMessage::SizeType  getFileLength () const;

   /**
    * @brief Gets the block length and block data.
    * @param [Out] @b length is the received block length of the file chunk being upload.
    * @return The byte pointer to the block data. These bytes are constant because belong to
    *           the internal block buffer whose owner is this message.
    */
    const unsigned char* getBlock(ISpotMessage::SizeType & length);

    /**
     * @brief Implements the inherited method. getKey.
     *
     * @return The unique SPOT command key that uniquely identifies the SPOT command/reply
     *         associated with this message.
     *
     * @see IMessage::getKey()
     */
     virtual KeyType getKey (void) const;


  private:

    /**@brief  block upload/download stage finished OK.*/
    static const unsigned char CBlockTransferOK = 0x02;


    /** @brief File length. */
    ISpotMessage::SizeType  m_uFileLength;

    /** @brief Block length. */
    ISpotMessage::SizeType  m_uBlockLength;

    /** @brief Block data. */
    unsigned char *   m_pbBlockData;
};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* SPOTMESSAGEVGDFILETRANSFERRESPONSE_H_ */

