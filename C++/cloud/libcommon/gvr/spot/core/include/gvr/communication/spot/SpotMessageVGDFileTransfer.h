/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageVGDFileTransfer.h
 * @author Guillermo Paris
 * @date Monday, March 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEVGDFILETRANSFER_H_
#define SPOTMESSAGEVGDFILETRANSFER_H_


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
 * @brief This class is the handler implementation of vanguard spot file/block upload/download request messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-03-16
 * @version 0.0.0, 2015-03-16
 * @author Guillermo Paris
 */
class SpotMessageVGDFileTransfer : public AbsSpotMessageWithAppCatCmdID
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

    /**@brief Block Size - maximum .*/
    static const ISpotMessage::SizeType CMaxBlockSize = 65528;

    /** @brief Item File Size word. Offset inside message. */
    static const ISpotMessage::SizeType CFileSizeOffset = 0x06;

    /** @brief Item File Block Data. Offset inside message. */
    static const ISpotMessage::SizeType CFileDataOffset = 0x06;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageVGDFileTransfer>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };


   /**
    * @brief default Constructor
    */
    SpotMessageVGDFileTransfer (void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessageVGDFileTransfer (void);

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
    * @brief Sets the file length.
    * @param @b length is the file length to transfer (upload or  download).
    */
    void setFileLength (ISpotMessage::SizeType length);

   /**
    * @brief Sets the block length and data, and also set the total message length for the current operation.
    * @param @b length is the desired block length of the file chunk being download.
    * @param @b pBlockData is the byte pointer to the block data.
    */
    void setBlock(ISpotMessage::SizeType length, const unsigned char* pbBlockData);


  private:

    /** @brief Block length. */
    ISpotMessage::SizeType  m_uFileLength;

    /** @brief Block length. */
    ISpotMessage::SizeType  m_uBlockLength; // TODO: See if this field is really required.

    /** @brief Reference to block data. */
    const unsigned char *   m_pbBlockData;
};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* SPOTMESSAGEVGDFILETRANSFER_H_ */

