/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageDownload.h
 * @author Guillermo Paris
 * @date Monday, March 2, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEPACKAGEDOWNLOAD_H_
#define SPOTMESSAGEPACKAGEDOWNLOAD_H_


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
 * @brief This class is the handler implementation of spot package download request messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-03-02
 * @version 0.0.0, 2015-03-02
 * @author Guillermo Paris
 */
class SpotMessagePackageDownload : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /** @brief Message size @note This message is always 260 bytes long. */
    static const ISpotMessage::SizeType  CMessageSize = 260;

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CPackageDownloadCommandId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CPackageDownloadSubCommandId = 0x01;

    /** @brief Certificate size @note This certificate is always 248 bytes long. */
    static const ISpotMessage::SizeType  CCertificateSize = 248;

    /**@brief Command - only Verify package.*/
    static const unsigned char CVerify = 0x00;
    /**@brief Command - Save package into SPOT.*/
    static const unsigned char CSave = 0x01;
    /**@brief Option - only Restart a fresh download of the package.*/
    static const unsigned char CRestart = 0x00;
    /**@brief Option - Resume aborted package download.*/
    static const unsigned char CResume = 0x01;

    /** @brief Item File Size dword. Offset inside message. */
    static const ISpotMessage::SizeType CFileSizeOffset = 0x06;

    /** @brief Item Certificate block 248 bytes. Offset inside message. */
    static const ISpotMessage::SizeType CCertificateOffset = 0x0A;

    /** @brief Item Command Verify or Save byte. Offset inside message. */
    static const ISpotMessage::SizeType CCommandOffset = 0x0102;

    /** @brief Item option Restart or Resume byte. Offset inside message. */
    static const ISpotMessage::SizeType COptionOffset = 0x0103;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessagePackageDownload>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };


   /**
    * @brief default Constructor
    */
    SpotMessagePackageDownload(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessagePackageDownload (void);

   /**
    * @brief Serializes the hardware info request message to a byte vector.
    * @see IMessage::serialize()
    */
    BufferType serialize (void) const;

   /**
    * @brief Parses the given buffer expecting to have a SPOT Package Info Request message inside it.
    * @brief This parsing of a request is only used for tests.
    * @see IMessage::parse()
    */
    bool parse (BufferType const &);

   /**
    * @brief Set package verification as the current operation.
    */
    void setVerifyOperation(void);

   /**
    * @brief Set package downloading as the current operation.
    * @note  This is set by default after message construction.
    */
    void setDownloadOperation(void);

   /**
    * @brief Set starting from the beginning as the current mode.
    * @note  This is set by default after message construction.
    */
    void setRestartMode(void);

   /**
    * @brief Set resume downloading as the current operation.
    */
    void setResumeMode(void);

   /**
    * @brief Sets the file length the current operation.
    * @param @b length is the total file length of the file to download.
    */
    void setFileLength(ISpotMessage::SizeType length);

   /**
    * @brief Sets the certificate data reference for the current operation.
    *
    * @param @b pCertificateData is the byte pointer to the certificate data (248 bytes).
    *
    */
    void setCertificateData(const unsigned char* pbCertificateData);


  private:

   /**
    * @brief Command verify or save.
    * @note  Set to CSave (download) by construction.
    */
    unsigned char           m_bCommand;

   /**
    * @brief Option restart again or resume.
    * @note  Set to CRestart by construction.
    */
    unsigned char           m_bOption;

    /** @brief File length. */
    ISpotMessage::SizeType  m_uFileLength;

    /** @brief Reference to file certificate data. */
    const unsigned char *   m_pbCertificate;
};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* SPOTMESSAGEPACKAGEDOWNLOAD_H_ */

