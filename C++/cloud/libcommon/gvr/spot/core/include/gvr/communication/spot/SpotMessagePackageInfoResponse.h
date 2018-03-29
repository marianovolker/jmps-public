/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageInfoResponse.h
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEPACKAGEINFORESPONSE_H_
#define SPOTMESSAGEPACKAGEINFORESPONSE_H_

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
 * @brief This class is the implementation of a handler of spot hardware info response messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-02-10
 * @version 0.0.0, 2015-02-10
 * @author Guillermo Paris
 */
class SpotMessagePackageInfoResponse : public AbsSpotMessageWithAppCatCmdID
{
  public:

    class Certificate
    {
      public:
        static const ISpotMessage::SizeType CLength = 248;

        Certificate();
        ~Certificate();

        inline const unsigned char* getData() const { return pbData; }
        void setData(const unsigned char*);


      private:
        unsigned char * pbData;
    };


    /**@brief default message size*/
    static const int CMinMessageSize = 11; // minimum message size corresponding to only one item.

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CPackageInfoCategoryId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CPackageInfoCommandId = 0x03;

    /** @brief Item Quantity byte. Offset inside message. */
    static const ISpotMessage::SizeType  CMessageItemQtyOffset = 0x07;
    /** @brief Item Data. Offset inside message. */
    static const ISpotMessage::SizeType  CMessageItemDataOffset = 0x08;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessagePackageInfoResponse>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };

    /**
     * @brief default constructor
     */
    SpotMessagePackageInfoResponse(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessagePackageInfoResponse (void);


    /**
     * @brief Gets the total number of the items present in the last response.
     * @note  The returned number of items will be zero in parse() method has not previously been called.
     *
     * @return The number of items present in the last response as a result of a parse()
     *
     * @see SpotMessagePackageInfo::parse()
     * @see TagLengthValueVector
     */
     unsigned int getItemQty (void);

    /**
     * @brief Gets a read only pointer to an item binary data of the response, as a byte array.
     *
     * @param @b [In]  UINT index  : The linear position of the item into the reply message.
     * @param @b [Out] UINT length : The binary data length of the selected item.
     *
     * @return The pointer to the selected data (byte* ), or NULL if there is not item data or the
     *         index is out of range.
     *
     * @see TagLengthValueVector
     */
     const unsigned char * getItemData (ISpotMessage::SizeType uIndex,
                                        ISpotMessage::SizeType uOffset) const;

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

     void parseSourceData(unsigned int nItems, const unsigned char* pSourceData);

     std::vector<Certificate>  m_Certificates;
};


} // namespace gvr

} // namespace communication

} // namespace spot

#endif /* SPOTMESSAGEPACKAGEINFORESPONSE_H_ */

