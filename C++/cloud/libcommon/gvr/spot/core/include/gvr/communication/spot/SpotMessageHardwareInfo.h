/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageHardwareInfo.h
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEHARDWAREINFO_H_
#define SPOTMESSAGEHARDWAREINFO_H_


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
 * @brief This class is the implementation of a handler of spot hardware info request messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-02-10
 * @version 0.0.0, 2015-02-10
 * @author Guillermo Paris
 */
class SpotMessageHardwareInfo : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /**@brief default message size*/
    static const int CMinMessageSize = 7; // minimum message size corresponding to only one item.

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CHardwareInfoCommandId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CHardwareInfoSubCommandId = 0x02;

    /** @brief Item Quantity byte. Offset inside message. */
    static const ISpotMessage::SizeType CMessageItemQtyOffset = 0x06;
    /** @brief Item Data. Offset inside message. */
    static const ISpotMessage::SizeType CMessageItemDataOffset = 0x07;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageHardwareInfo>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };


    /**
     * @brief default Constructor
     */
    SpotMessageHardwareInfo(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessageHardwareInfo (void);
    /**
     * @brief Serializes the hardware info request message to a byte vector.
     * @see IMessage::serialize()
     */
     BufferType serialize (void) const;

    /**
     * @brief Parses the given buffer expecting to have a SPOT Hardware Info Request message inside it.
     * @brief This parsing of a request is only used for tests.
     * @see IMessage::parse()
     */
     bool parse (BufferType const &);

    /**
     * @brief Sets all the item codes to be requested.
     * @param @b uQty Item quantity. Number of items to be set.
     * @param @b pbItemCodes : The byte array containing the item codes.
     *
     * @return @b true if the items has been copied successfully to the internal vector.
     *         @b false otherwise.
     */
     bool setItems(unsigned int uQty, const unsigned char* pbItemCodes);


  private:

     /**@brief Item request codes .*/
     std::vector<unsigned char>  m_ItemVector;
};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* SPOTMESSAGEHARDWAREINFO_H_ */

