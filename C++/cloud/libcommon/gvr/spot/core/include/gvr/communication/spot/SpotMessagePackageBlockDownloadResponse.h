/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageBlockDownloadResponse.h
 * @author Guillermo Paris
 * @date Monday, March 02, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEPACKAGEBLOCKDOWNLOADRESPONSE_H_
#define SPOTMESSAGEPACKAGEBLOCKDOWNLOADRESPONSE_H_

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
 * @brief This class is the implementation handler of spot package download response messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-03-02
 * @version 0.0.0, 2015-03-02
 * @author Guillermo Paris
 */
class SpotMessagePackageBlockDownloadResponse : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /**@brief default message size*/
    static const int CMinMessageSize = 7; // constant message size.

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CPackageBlockDownloadCategoryId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CPackageBlockDownloadCommandId = 0x02;

    /**@brief Block was received OK, another can be sent.*/
    static const unsigned char CBlockDownloadedOK = 0x00; // not 0x02, that is resource block OK;

    /** @brief Item File Offset. Index inside message. */
    static const ISpotMessage::SizeType  CMessageDependencyLengthIndex = 0x07;

    /** @brief Item File Offset. Index inside message. */
    static const ISpotMessage::SizeType  CMessageDependenciesIndex = 0x09;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessagePackageBlockDownloadResponse>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };


    /**
     * @brief default constructor
     */
    SpotMessagePackageBlockDownloadResponse(void);

    /**
     * @brief Destructor
     * @since 0.0.0
     */
     virtual ~SpotMessagePackageBlockDownloadResponse (void);

     /**
      * @brief Says if the last block sent was properly received OK.
      * @return @b true If the last block sent was received OK and another block can be sent.
      *         @b false Otherwise. Another block can not be sent and whole download must be restarted/resumed.
      * @see SpotMessagePackageBlockDownload::CBlockDownloadedOK
      */
     bool wasBlockReceivedOK();


    /**
     * @brief Gets the total length of raw data (characters) belonging to the debian dependency strings
     *        after download operation. Returns zero if there is not broken debian
     *        packages dependencies after the download.
     *
     * @return The raw data size of the debian dependency strings after download operation.
     */
     ISpotMessage::SizeType  getDebianDependenciesTotalLength (void) const;

   /**
    * @brief Gets the raw data of the debian dependency strings after download operation.
    *        Returns zero if there is not broken debian packages dependencies after the download.
    *
    * @param [Out] @b unsigned int [OPTIONAL] The raw data size reference of the debian dependency strings.
    *                              This is an optional pointer where this value is to be put.
    * @return The raw data (characters) of the debian dependency strings after download operation.
    *         A NULL pointer is returned if there is not broken debian dependencies.
    */
     const char*    getDebianDependencies (ISpotMessage::SizeType * puTotalLength = NULL) const;

    /**
     * @brief Serializes the hardware info response message (only used for tests).
     * @note This method serializes just a typical hard-coded reply for unit testing.
     *       This is because a SPOT response message is never sent to SPOT !!
     * @see IMessage::serialize()
     */
     ISpotMessage::BufferType serialize (void) const;

    /**
     * @brief Parses the given buffer expecting to have a SPOT PackageBlock Info Response message inside it.
     * @see IMessage::parse()
     */
     bool parse (ISpotMessage::BufferType const &);


  private:

     /** @brief Debian package dependency strings length. */
     ISpotMessage::SizeType  m_uDebianDependencyStringLength;

     /** @brief Debian package dependency strings. Raw data. */
     char *                  m_pcDebianDependencyStringData;
};


} // namespace gvr

} // namespace communication

} // namespace spot

#endif /* SPOTMESSAGEPACKAGEBLOCKDOWNLOADRESPONSE_H_ */

