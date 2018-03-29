/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessagePackageInfo.h
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEPACKAGEINFO_H_
#define SPOTMESSAGEPACKAGEINFO_H_


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
class SpotMessagePackageInfo : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /** @brief message size @note This message is always 7 bytes long. */
    static const ISpotMessage::SizeType  CMessageSize = 7;

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CPackageInfoCommandId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CPackageInfoSubCommandId = 0x03;

    /**@brief Set Selected - Current active packages.*/
    static const unsigned char CActivePackages = 0x00;
    /**@brief Set Selected - New downloaded packages..*/
    static const unsigned char CNewPackages = 0x01;
    /** @brief Item Set selection byte. Offset inside message. */
    static const ISpotMessage::SizeType CSetSelectionByteOffset = 0x06;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessagePackageInfo>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };


    /**
     * @brief default Constructor
     */
    SpotMessagePackageInfo(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessagePackageInfo (void);
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
     * @brief Select the package set whose information is to be requested.
     * @param @b bSelectionByte @b 0 means current active packages.
     *                          @b 1 means new downloaded ones, not active yet.
     *                          Other values are silently ignored.
     */
     void selectPackageSet(unsigned char bSelectionByte);


  private:

     /** @brief Package selector code. */
     unsigned char  m_bPackageSelector;
};


} // namespace spot

} // namespace communication

} // namespace gvr


#endif /* SPOTMESSAGEPACKAGEINFO_H_ */

