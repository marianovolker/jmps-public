/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageOPTHwrInfoResponse.h
 * @author Guillermo Paris
 * @date Thursday, May 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef SPOTMESSAGEOPTHWRINFORESPONSE_H_
#define SPOTMESSAGEOPTHWRINFORESPONSE_H_

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
 * @brief This class is the implementation of a handler of spot OPT hardware info response messages.
 *
 * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
 * @since 0.0.0, 2015-05-14
 * @version 0.0.0, 2015-05-14
 * @author Guillermo Paris
 */
class SpotMessageOPTHwrInfoResponse : public AbsSpotMessageWithAppCatCmdID
{
  public:

    /** @brief Item identification codes */
    enum ItemID
    {
      ePRINTER_SERIALNUMBER       = 0x01, ///< @brief Printer serial number.
      ePRINTER_DEVTYPE            = 0x02, ///< @brief Printer Type ( thermal or legacy dot-matrix ).
      ePRINTER_DEVFUNCTIONS       = 0x03, ///< @brief Supported graphic, bar code, functions & features.
      ePRINTER_DEVMODEL           = 0x04, ///< @brief Printer model description.
      eIOMODULE_SERIALNUMBER      = 0x10, ///< @brief I/O module serial number.
      eIOMODULE_DEVTYPE           = 0x11, ///< @brief I/O module Type.
      eIOMODULE_DEVFUNCTIONS      = 0x12, ///< @brief I/O Supported functions.
      eIOMODULE_DEVMODEL          = 0x13, ///< @brief I/O module model description.

      eOTI_CONTACTLESS_READER_SERIALNUMBER = 0x20, ///< @brief Contactless Reader serial number.
    };

    /** @brief Printer type codes */
    enum PrinterType
    {
      eUnknown, eThermal, eDotMatrix
    };

   /**
    * @type   DeviceInfoMap
    * @brief  Return type when requesting all the information about a specific device.
    * @see    SpotMessageOPTHwrInfoResponse::getAllData()
    */
    typedef std::map<std::string, std::string>   DeviceInfoMap;


    /**@brief default message size. @note minimum message size corresponding to only one item. */
    static const ISpotMessage::SizeType CMinMessageSize = 11;

    /** @brief max allowed item size. @note Just used as a prevention in case of data corruption. */
    static const ISpotMessage::SizeType CMaxItemLength = 512;

    /**@brief Spot Message Identification, command category.*/
    static const unsigned char COPTHwrInfoCategoryId = 0x10;
    /**@brief Spot Message Identification, command code.*/
    static const unsigned char COPTHwrInfoCommandId = 0x02;

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
    static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageOPTHwrInfoResponse>(); };
    virtual TypeId getTypeId (void) const { return GetTypeId(); };

    /**
     * @brief default constructor
     */
    SpotMessageOPTHwrInfoResponse(void);

   /**
    * @brief Destructor
    * @since 0.0.0
    */
    virtual ~SpotMessageOPTHwrInfoResponse (void);


   /**
    * @brief Gets the total number of the items present in the last response.
    * @note  The returned number of items will be zero in parse() method has not previously been called.
    *
    * @return The number of items present in the last response as a result of a parse()
    *
    * @see SpotMessageOPTHwrInfo::parse()
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
    * @brief Gets the printer serial number.
    *
    * @param [Out] @b sOut The serial number of the printer to be returned.
    *
    * @return @b true if everything went OK collecting the serial number from the response.
    *         @b false if an error blocked to collect the info.
    */
    bool getPrinterSerialNumber (std::string & sOut) const ;

   /**
    * @brief Gets the contactless reader serial number.
    *
    * @param [Out] @b sOut The serial number of the contactless reader to be returned.
    *
    * @return @b true if everything went OK collecting the serial number from the response.
    *         @b false if an error blocked to collect the info.
    */
    bool getContactlessSerialNumber (std::string & sOut) const ;

   /**
    * @brief Gets the I/O module serial number.
    *
    * @param [Out] @b sOut The I/O module number of the printer to be returned.
    *
    * @return @b true if everything went OK collecting the serial number from the response.
    *         @b false if an error blocked to collect the info.
    */
    bool getIOModuleSerialNumber (std::string & sOut) const ;

   /*
    * @brief Gets all the details (items) of the printer info as a map.
    *
    * @param [Out] map is the returned map<string(field_name),string(field_value)>.
    * @return @b true if everything went OK collecting items from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getPrinterInfo (std::string & sDeviceName, DeviceInfoMap & map) const ;

   /*
    * @brief Gets all the details (items) of the contactless reader info as a map.
    * @note Currently there is only one item for this device (serial number).
    *       This method is provided for future expansion / Pinpad Hardware Info compatibility.
    *       If you need to retrieve just the serial number, then call
    *       getContactlessSerialNumber() instead of this.
    *
    * @param [Out] map is the returned map<string(field_name),string(field_value)>.
    * @return @b true if everything went OK collecting items from the response.
    *         @b false if an error blocked to collect all the info.
    *
    * @see SpotMessageOPTHwrInfoResponse::getContactlessSerialNumber()
    */
    bool getContactlessReaderInfo (std::string & sDeviceName, DeviceInfoMap & map) const ;

   /*
    * @brief Gets all the details (items) of the display info as a map.
    *
    * @param [Out] map is the returned map<string(field_name),string(field_value)>.
    * @return @b true if everything went OK collecting items from the response.
    *         @b false if an error blocked to collect all the info.
    */
    bool getIOModuleInfo (std::string & sDeviceName, DeviceInfoMap & map) const ;

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
    * @brief Gets the sub-item integer contained in only one byte.
    *
    * @param iTagID is the "sub-item unique code" or hardware info tag ID.
    * @param [Out] @b iOut The requested sub-item, returned as a signed integer.
    *
    */
    bool getOneByteIntegerSubItem(int iTagID, int & iOut) const ;

   /**
    * @brief Gets the sub-item integer (4 bytes) represented as a decimal output string.
    *
    * @param iTagID is the "sub-item unique code" or hardware info tag ID.
    * @param [Out] @b sOut The requested sub-item, a decimal integer, returned as a string.
    *
    */
    bool getFourByteDecimalIntegerSubItem(int iTagID, std::string & sOut) const ;

   /**
    * @brief Gets the sub-item integer (1 byte) represented as an hex output string.
    *
    * @param iTagID is the "sub-item unique code" or hardware info tag ID.
    * @param [Out] @b sOut The requested sub-item value returned as a one byte integer C hex string.
    *
    */
    bool getOneByteSubItemCHexNumberAsString(int iTagID, std::string & sOut) const ;

   /**
    * @brief Gets the sub-item integer (4 bytes) represented as an hex output string.
    *
    * @param iTagID is the "sub-item unique code" or hardware info tag ID.
    * @param [Out] @b sOut The requested sub-item value returned as a four byte C hex string.
    *
    */
    bool getFourByteSubItemCHexNumberAsString(int iTagID, std::string & sOut) const ;

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


    static const char * CpcPrinter;            ///< @brief Label "printer".
    static const char * CpcContactlessReader;  ///< @brief Label "contactless reader".
    static const char * CpcIOModule;           ///< @brief Label "I/O module".

    static const char * CpcSerialNo;           ///< @brief Label "serial number".
    static const char * CpcType;               ///< @brief Label "type".
    static const char * CpcModel;              ///< @brief Label "model".
    static const char * CpcFunctions;          ///< @brief Label "functions".

    static const char * CpcUnknown;            ///< @brief Label "unknown".
    static const char * CpcThermalPrinter;     ///< @brief Label "thermal printer".
    static const char * CpcDotMatrixPrinter;   ///< @brief Label "dot matrix printer".

    TagLengthValueVector  m_TLVvector;
};


} // namespace gvr

} // namespace communication

} // namespace spot

#endif /* SPOTMESSAGEOPTHWRINFORESPONSE_H_ */

