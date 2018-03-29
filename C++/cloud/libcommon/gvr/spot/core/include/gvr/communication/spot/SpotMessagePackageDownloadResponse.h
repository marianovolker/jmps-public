/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageDownloadResponse.h
 * @author Guillermo Paris
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEPACKAGEDOWNLOADRESPONSE_H_
#define SPOTMESSAGEPACKAGEDOWNLOADRESPONSE_H_

#include <gvr/communication/spot/TagLengthValueVector.h>
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
 * @brief This class is the handler implementation of spot package download response messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-03-02
 * @version 0.0.0, 2015-03-02
 * @author Guillermo Paris
 */
class SpotMessagePackageDownloadResponse : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /**@brief default message size*/
    static const int CMessageSize = 11; // constant message size.

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CPackageDownloadCategoryId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CPackageDownloadCommandId = 0x01;

    /**@brief Certificate OK, package download can begin.*/
    static const unsigned char CPackageDownloadAccepted = 0x00;

    /** @brief Item File Offset. Index inside message. */
    static const ISpotMessage::SizeType  CMessageFileOffsetIndex = 0x07;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessagePackageDownloadResponse>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };

    /**
     * @brief default constructor
     */
    SpotMessagePackageDownloadResponse(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessagePackageDownloadResponse (void);

    /**
     * @brief Says if the requested package download can begin.
     * @return @b true If the certificate sent is OK and package download can begin.
     *         @b false Otherwise. The package download was not accepted and can not begin.
     * @see SpotMessagePackageDownload::CPackageDownloadAccepted
     */
    bool canDownloadBegin();

   /**
    * @brief Gets the initial offset present in the response when resume download has been requested.
    * @note  Only used in resume download operations. Returns zero for restart downloading operations.
    *
    * @return The offset into the file data to resume a previous aborted download operation.
    *
    * @see SpotMessagePackageDownload::setResumeMode()
    */
    ISpotMessage::SizeType    getInitialOffset (void);

    /**
     * @brief Serializes the hardware info response message (only used for tests).
     * @note This method serializes just a typical hard-coded reply for unit testing.
     *       This is because a SPOT response message is never sent to SPOT !!
     * @see IMessage::serialize()
     */
     ISpotMessage::BufferType serialize (void) const;

    /**
     * @brief Parses the given buffer expecting to have a SPOT Package Info Response message inside it.
     * @see IMessage::parse()
     */
     bool parse (ISpotMessage::BufferType const &);


  private:

     ISpotMessage::SizeType  m_uInitialOffset; ///< @brief The initial offset for resume download operations.
};


} // namespace gvr

} // namespace communication

} // namespace spot

#endif /* SPOTMESSAGEPACKAGEDOWNLOADRESPONSE_H_ */

