/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageBlockDownload.h
 * @author Guillermo Paris
 * @date Monday, March 2, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEPACKAGEBLOCKDOWNLOAD_H_
#define SPOTMESSAGEPACKAGEBLOCKDOWNLOAD_H_


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
 * @brief This class is the handler implementation of spot package block download request messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-03-02
 * @version 0.0.0, 2015-03-02
 * @author Guillermo Paris
 */
class SpotMessagePackageBlockDownload : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /** @brief Minimum Message size. @note This message has variable size. */
    static const ISpotMessage::SizeType  CMinMessageSize = 8;

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CPackageDownloadCategoryId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CPackageBlockDownloadCommandId = 0x02;

    /**@brief Block Size - maximum .*/
    static const ISpotMessage::SizeType CMaxBlockSize = 2048;
    /**@brief Block Size - maximum for m7 (Vanguard) devices.*/
    static const ISpotMessage::SizeType CMaxBlockSizeVgd = 8192;
    /**@brief Block Size - maximum for debian packages.*/
    static const ISpotMessage::SizeType CMaxBlockSizeDebian = 8192;

    /** @brief Item Block Size word. Offset inside message. */
    static const ISpotMessage::SizeType CBlockSizeOffset = 0x06;

    /** @brief Item Block Data. Offset inside message. */
    static const ISpotMessage::SizeType CBlockDataOffset = 0x08;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessagePackageBlockDownload>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };


   /**
    * @brief default Constructor
    */
    SpotMessagePackageBlockDownload(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessagePackageBlockDownload (void);

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
    * @brief Sets the block length and data, and also set the total message length for the current operation.
    * @param @b length is the desired block length of the file chunk being download.
    * @param @b pBlockData is the byte pointer to the block data. This is NULL ONLY IN THE LAST MESSAGE.
    */
    void setBlock(ISpotMessage::SizeType length, const unsigned char* pbBlockData = NULL);


  private:

    /** @brief Block length. */
    ISpotMessage::SizeType  m_uBlockLength;

    /** @brief Reference to block data. */
    const unsigned char *   m_pbBlockData;
};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* SPOTMESSAGEPACKAGEBLOCKDOWNLOAD_H_ */

