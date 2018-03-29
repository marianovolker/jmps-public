/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageHardwareInfoResponse.h
 * @author Guillermo Paris
 * @date Tuesday, February 10, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEHARDWAREINFORESPONSE_H_
#define SPOTMESSAGEHARDWAREINFORESPONSE_H_

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
class SpotMessageHardwareInfoResponse : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /** @brief Item identification codes */
    enum ItemID
    {
      eSPOT_SERIALNUMBER          = 0x01, ///< @brief Main board serial number.
      eSECURITY_HWRID             = 0x11, ///< @brief SPOT Hardware Release ID.
      eKEYBOARD_SERIALNUMBER      = 0x20, ///< @brief Keyboard serial number.
      eKEYBOARD_HWRID             = 0x21, ///< @brief Keyboard Hardware release ID.
      eKEYBOARD_DEVTYPE           = 0x22, ///< @brief Keyboard Device Type.
      eCARDREADER_SERIALNUMBER    = 0x30, ///< @brief Card Reader serial number.
      eCARDREADER_HWRID           = 0x31, ///< @brief Card Reader Hardware release ID.
      eCARDREADER_DEVTYPE         = 0x32, ///< @brief Card Reader Device Type.
      eCARDREADER_DEVFUNCTION     = 0x33, ///< @brief Track supported (read/write functions).
      eCARDREADER_DEVMODEL        = 0x34, ///< @brief Card Reader Model description.
      eDISPLAY_DEVTYPE            = 0x40, ///< @brief Display Type.
      eDISPLAY_DEVFUNCTION        = 0x41, ///< @brief Supported graphic functions/formats.
      eDISPLAY_DEVMODEL           = 0x42, ///< @brief Display model description.
      eDISPLAY_DEVHWRCONFIG       = 0x43  ///< @brief Display Resolution
    };

   /**
    * @type   DeviceInfoMap
    * @brief  Return type when requesting all the information about a specific device.
    * @see    SpotMessageHardwareInfoResponse::getAllData()
    */
    typedef std::map<std::string, std::string>   DeviceInfoMap;


    /**@brief default message size. @note minimum message size corresponding to only one item. */
    static const ISpotMessage::SizeType CMinMessageSize = 11;

    /** @brief max allowed item size. @note Just used as a prevention in case of data corruption. */
    static const ISpotMessage::SizeType CMaxItemLength = 512;

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char CHardwareInfoCategoryId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char CHardwareInfoCommandId = 0x02;

    /** @brief Item Quantity byte. Offset inside message. */
    static const ISpotMessage::SizeType CMessageItemQtyOffset = 0x07;
    /** @brief Item Data. Offset inside message. */
    static const ISpotMessage::SizeType CMessageItemDataOffset = 0x08;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageHardwareInfoResponse>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };

    /**
     * @brief default constructor
     */
    SpotMessageHardwareInfoResponse(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessageHardwareInfoResponse (void);


   /**
    * @brief Gets the total number of the items present in the last response.
    * @note  The returned number of items will be zero in parse() method has not previously been called.
    *
    * @return The number of items present in the last response as a result of a parse()
    *
    * @see SpotMessageHardwareInfo::parse()
    * @see TagLengthValueVector
    */
    unsigned int getItemQty (void);

   /**
    * @brief Gets a read only pointer to an item binary data of the response, as a byte array.
    *
    * @param @b [In]  UINT index  : The linear position of the item into the reply message.
    * @param @b [Out] UINT uTagID : The Tag ID of the selected item.
    * @param @b [Out] UINT length : The binary data length of the selected item.
    *
    * @return The pointer to the selected data (byte* ), or NULL if there is not item data or the
    *         index is out of range.
    *
    * @see TagLengthValueVector
    */
    const unsigned char * getItemData (ISpotMessage::SizeType   uIndex,
                                       unsigned int &           uTagID,
                                       ISpotMessage::SizeType & length) const;

   /**
    * @brief Gets the SPOT serial number.
    *
    * @param [Out] @b sOut The serial number of the SPOT (PPN) to be returned.
    *
    * @return @b true if everything went OK collecting the serial number from the response.
    *         @b false if an error blocked to collect the info.
    */
    bool getSerialNumber (std::string & sOut) const ;

   /**
    * @brief Gets the SPOT Hardware release ID.
    *
    * @param [Out] @b sOut The hex string representation of the two bytes hardware ID.
    *
    * @return @b true if everything went OK collecting the info from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getHardwareId (std::string & sOut) const ;

   /**
    * @brief Gets the SPOT model.
    * @note This model identification is based on SPOT display model description.
    *
    * @param [Out] @b sOut The SPOT model: M3, M5 or M7.
    *
    */
    bool getSpotModel (std::string & sOut) const ;

   /*
    * @brief Gets all the details (items) of the keyboard info as a map.
    *
    * @param [Out] map is the returned map<string(field_name),string(field_value)>.
    * @return @b true if everything went OK collecting items from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getKeypadInfo (std::string & sDeviceName, DeviceInfoMap & map) const ;

   /*
    * @brief Gets all the details (items) of the card reader info as a map.
    *
    * @param [Out] map is the returned map<string(field_name),string(field_value)>.
    * @return @b true if everything went OK collecting items from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getCardReaderInfo (std::string & sDeviceName, DeviceInfoMap & map) const ;

   /*
    * @brief Gets all the details (items) of the display info as a map.
    *
    * @param [Out] map is the returned map<string(field_name),string(field_value)>.
    * @return @b true if everything went OK collecting items from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getDisplayInfo (std::string & sDeviceName, DeviceInfoMap & map) const ;

   /**
    * @brief Serializes the hardware info response message (only used for tests).
    * @note This method serializes just a typical hard-coded reply for unit testing.
    *       This is because a SPOT response message is never sent to SPOT !!
    * @see IMessage::serialize()
    */
    ISpotMessage::BufferType serialize (void) const;

   /**
    * @brief Parses the given buffer expecting to have a SPOT Hardware Info Response message inside it.
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
    * @brief Gets the sub-item integer (2 bytes) represented as a decimal output string.
    *
    * @param iTagID is the "sub-item unique code" or hardware info tag ID.
    * @param [Out] @b sOut The requested sub-item, a decimal integer, returned as a string.
    *
    */
    bool getTwoByteDecimalIntegerSubItem(int iTagID, std::string & sOut) const ;

   /**
    * @brief Gets the sub-item integer (1 byte) represented as an hex output string.
    *
    * @param iTagID is the "sub-item unique code" or hardware info tag ID.
    * @param [Out] @b sOut The requested sub-item value returned as a one byte integer C hex string.
    *
    */
    bool getOneByteSubItemCHexNumberAsString(int iTagID, std::string & sOut) const ;

   /**
    * @brief Gets the sub-item integer (2 bytes) represented as an hex output string.
    *
    * @param iTagID is the "sub-item unique code" or hardware info tag ID.
    * @param [Out] @b sOut The requested sub-item value returned as a two byte integer C hex string.
    *
    */
    bool getTwoByteSubItemCHexNumberAsString(int iTagID, std::string & sOut) const ;

   /**
    * @brief Gets the sub-item (many bytes) represented as an hex output string.
    *
    * @param [In]  @b iTagID is the "sub-item unique code" or hardware info tag ID.
    * @param [Out] @b sOut The requested sub-item value returned as an hex string.
    * @param [In]  @b szSeparator is the optional separator between hex byte representations.
    *                 If this parameter is not supplied (NULL) then a one blank space is assumed.
    *
    */
    bool getSubItemBytesAsDelimitedHexString(int iTagID, std::string & sOut,
                                             const char* szSeparator = NULL) const ;


    static const char * CpcKeypad;            ///< @brief Label "keypad".
    static const char * CpcCardReader;        ///< @brief Label "card reader".
    static const char * CpcDisplay;           ///< @brief Label "display".

    static const char * CpcSerialNo;          ///< @brief Label "serial number".
    static const char * CpcHwrID;             ///< @brief Label "hardware ID".
    static const char * CpcHwrConf;           ///< @brief Label "hardware configuration".
    static const char * CpcType;              ///< @brief Label "type".
    static const char * CpcModel;             ///< @brief Label "model".
    static const char * CpcFunctions;         ///< @brief Label "functions".
    static const char * CpcXresolution;       ///< @brief Label "X resolution".
    static const char * CpcYresolution;       ///< @brief Label "Y resolution".
    static const char * CpcColorDepth;        ///< @brief Label "color depth".

    static const char * CpcDisplayClueForM3;  ///< @brief Display Clue for M3 detection, "MCF5" .
    static const char * CpcDisplayClueForM5;  ///< @brief Display Clue for M5 detection, "OMAP" .
    static const char * CpcDisplayClue1ForM7; ///< @brief Display Clue #1 for M7 detection, "UPM" .
    static const char * CpcDisplayClue2ForM7; ///< @brief Display Clue #2 for M7 detection, "LCD module".

    static const char * CpcM3;                ///< @brief SPOT model is M3 , "M3" .
    static const char * CpcM5;                ///< @brief SPOT model is M3 , "M5" .
    static const char * CpcM7;                ///< @brief SPOT model is M3 , "M7" .
    static const char * CpcUnknown;           ///< @brief SPOT model is unknown , "unknown".

    TagLengthValueVector  m_TLVvector;
};


} // namespace gvr

} // namespace communication

} // namespace spot

#endif /* SPOTMESSAGEHARDWAREINFORESPONSE_H_ */

