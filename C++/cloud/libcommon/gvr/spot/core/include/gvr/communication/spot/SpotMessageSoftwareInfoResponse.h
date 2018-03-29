/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageSoftwareInfoResponse.h
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGESOFTWAREINFORESPONSE_H_
#define SPOTMESSAGESOFTWAREINFORESPONSE_H_

#include <string>
#include <map>
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
class SpotMessageSoftwareInfoResponse : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /** @brief Item identification codes */
    enum ItemID
    {
      eFirstID                              =  0x02,  ///< @brief Just a marker for the first ID.

      eSPOT_OS_VERSION                      =  0x02,  ///< @brief SPOT Operating System Version.
      eSWF_PKG1                             =  0x03,  ///< @brief Software Package #1 Version (ROMfs1 version).
      eSWF_PKG2                             =  0x04,  ///< @brief Software Package #2 Version (ROMfs2 version).
      eSWF_PKG3                             =  0x05,  ///< @brief Software Package #3 Version (ROMfs3 version).
      eSWF_PKG4                             =  0x06,  ///< @brief Software Package #4 Version (ROMfs4 version).
      eSWF_PKG5                             =  0x07,  ///< @brief Software Package #5 Version (ROMfs5 version).
      eSWF_PKG6                             =  0x08,  ///< @brief Software Package #6 Version (ROMfs6 version).
      eSWF_PKG7                             =  0x09,  ///< @brief Software Package #7 Version (ROMfs7 version).
      eSWF_PKG8                             =  0x0A,  ///< @brief Software Package #8 Version (ROMfs8 version).
      eFWR_CR_VER                           =  0x0B,  ///< @brief Firmware Version of the Card Reader.
      eFWR_KP_VER                           =  0x0C,  ///< @brief Firmware Version of the Keypad
      eFWR_SEC_VER                          =  0x0D,  ///< @brief Firmware Version of the Security Module.
      eM7_SPOT_SYS_BUILD_VERSION            =  0x0F,  ///< @brief "Build version"
      eM7_CR_FWR_VERSION                    =  0x10,  ///< @brief "Version of the firmware of the Card reader"
      eM7_SPOT_M7_FW_VERSION                =  0x13,  ///< @brief "Vanguard PCI string identify"
      eM7_SPOT_SYS_PCI_APPLICATION_VERSION  =  0x14,  ///< @brief "Vanguard PCI APPLICATION version"

      eLastID                               =  0x14   ///< @brief Just a marker for the last ID.
    };


    struct ItemIDLabel
    {
      int           itemID;
      const char *  itemLabel;
    };

   /**
    * @type   AllInfoMap
    * @brief  Return type when requesting all software/firmware information.
    * @see    SpotMessageSoftwareInfoResponse::getAllData()
    */
    typedef std::map<std::string, std::string>   SoftwareInfoMap;

    /**@brief default message size*/
    static const int CMinMessageSize = 11; // minimum message size corresponding to only one item.

    /** @brief max allowed item size. @note Just used as a prevention in case of data corruption. */
    static const ISpotMessage::SizeType CMaxItemLength = 512;

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CSoftwareInfoCategoryId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CSoftwareInfoCommandId = 0x03;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageSoftwareInfoResponse>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };

    /**
     * @brief default constructor
     */
    SpotMessageSoftwareInfoResponse(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessageSoftwareInfoResponse (void);


   /**
    * @brief Gets the total number of the items present in the last response.
    * @note  The returned number of items will be zero in parse() method has not previously been called.
    *
    * @return The number of items present in the last response as a result of a parse()
    *
    * @see SpotMessageSoftwareInfo::parse()
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
                                       unsigned int & iTagID, ISpotMessage::SizeType & length) const;

   /*
    * @brief Gets all the software info as a map.
    *
    * @param [Out] @b sLabel is the map label legend to be used in reports.
    * @param [Out] @b map is the returned map<string(field_name),string(field_value)>.
    * @return @b true if everything went OK collecting items from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getAllSoftwareDescription (std::string & sLabel, SoftwareInfoMap & map ) const ;

   /**
    * @brief Gets the card reader firmware label and version.
    *
    * @param [Out] @b sLabel      The label legend to be used in reports.
    * @param [Out] @b sFwrDescrip The card reader firmware version.
    *
    * @return @b true if everything went OK collecting the info from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getCardReaderFirmwareVersion (std::string & sLabel, std::string & sFwrDescrip) const ;

   /**
    * @brief Gets the keypad firmware label and version.
    *
    * @param [Out] @b sLabel      The label legend to be used in reports.
    * @param [Out] @b sFwrDescrip The keypad firmware version.
    *
    * @return @b true if everything went OK collecting the info from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getKeypadFirmwareVersion (std::string & sLabel, std::string & sFwrDescrip) const ;

   /**
    * @brief Gets the security module firmware label and version.
    *
    * @param [Out] @b sLabel      The label legend to be used in reports.
    * @param [Out] @b sFwrDescrip The security module firmware version.
    *
    * @return @b true if everything went OK collecting the info from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getSecurityModuleFirmwareVersion (std::string & sLabel, std::string & sFwrDescrip) const ;

   /**
    * @brief Serializes the hardware info response message (only used for tests).
    * @note This method serializes just a typical hard-coded reply for unit testing.
    *       This is because a SPOT response message is never sent to SPOT !!
    * @see IMessage::serialize()
    */
    ISpotMessage::BufferType serialize (void) const;

   /**
    * @brief Parses the given buffer expecting to have a SPOT Software Info Response message inside it.
    * @see IMessage::parse()
    */
    bool parse (ISpotMessage::BufferType const &);


  private:

   /**
    * @brief Gets the item description/details as an output string.
    *
    * @param iTagID is the "sub-item unique code" or hardware info tag ID.
    * @param [Out] @b sOut The sub-item description/details to be returned.
    * @param maxLength (optional) If supplied (not null), clips the output text to be no larger than this length.
    *
    */
    bool getSubItemDescription(int iTagID, std::string & sOut, ISpotMessage::SizeType maxLength = 0) const ;

    /**
     * @brief Gets the item label as an output string.
     *
     * @param iTagID is the "sub-item unique code" or hardware info tag ID.
     * @param [Out] @b sOut The sub-item label to be returned.
     *
     */
    bool getSubItemLabel(int iTagID, std::string & sOut) const ;

    static const char *                   CpcFirmware;    ///< @brief Label "firmware" .
    static const char *                   CpcSecurity;    ///< @brief Label "security" .
    static const char *                   CpcKeypad;      ///< @brief Label "keypad".
    static const char *                   CpcCardReader;  ///< @brief Label "card reader".

    static const ISpotMessage::SizeType   CMaxItems = 16; ///< @brief Max. number of requested items.
    static const ItemIDLabel              aCItemLabels[CMaxItems];  ///< @brief Items IDs and its labels.


    TagLengthValueVector  m_TLVvector;
};


} // namespace gvr

} // namespace communication

} // namespace spot

#endif /* SPOTMESSAGESOFTWAREINFORESPONSE_H_ */

