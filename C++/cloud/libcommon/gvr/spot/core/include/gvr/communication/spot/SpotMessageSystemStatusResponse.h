/*
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageSystemStatusResponse.h
 * @author mgrieco
 * @date Friday, December 29, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTMESSAGESYSTEMSTATUSRESPONSE_H_
# define _SPOTMESSAGESYSTEMSTATUSRESPONSE_H_

# include <gvr/communication/spot/AbsSpotMessageWithAppCatCmdID.h>
# include <vector>
# include <iostream>

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
       * @brief This class is the implementation for System Status message response on Spot protocol.
       * @see gvr.communication.spot.AbsSpotMessageWithAppCatCmdID
       * @since 0.0.0
       * @author mgrieco
       */
      class SpotMessageSystemStatusResponse : public AbsSpotMessageWithAppCatCmdID
      {

          ///< @brief The pinpad subsystem device indexes are listed in the following enum:
          enum  EPinpadSubsystemIndexes
          {
            ePinpad_Main, ePinpad_SecurityModule, ePinpad_Display,
            ePinpad_Keyboard, ePinpad_CardReader, ePinpad_GEMCore,
            eTotalPinpadSubsystems
          };

          ///< @brief The OPT subsystem device indexes are listed in the following enum:
          enum EOPTSubsystemIndexes
          {
            eOPT_Printer, eOPT_IO_Module, eOPT_IO_NotBatt_Input1,
            eOPT_Contactless, eOPT_IO_NotBatt_Input2, eOPT_IO_Batt_Input4,
            eTotalOPTSubsystems
          };

          ///< @brief Possible SPOT device statuses to cloud reporting.
          enum EStatuses
          {
            eIdle, eInUse, eDown, eWarning, eError, eStartup, eTotalStatuses
            //eStatusOK, eStatusWarning, eStatusError, eTotalStatuses
          };

          static const char * CapcStatuses[eTotalStatuses];


        public:

          /**
           * @type   DeviceStatusMap
           * @brief  Return type when requesting all the status information about a specific device.
           * @see    SpotMessageSystemStatusResponse::getItemData()
           */
          typedef std::map<std::string, std::string>   DeviceStatusMap;

          /** @brief Item Data. */
          struct ItemType
          {
            unsigned char deviceCode;
            unsigned char status;
            unsigned char statusExt;
            unsigned char errorCode;

            ItemType()
            {
              deviceCode = 0x00;
              status     = 0x00;
              statusExt  = 0x00;
              errorCode  = 0x00;
            }

            bool isNull(){ return deviceCode == 0 && status == 0 && statusExt == 0 && errorCode == 0; }
          };

          ///< @brief The pinpad subsystem device codes are listed in the following table:
          enum  EPinpadSubsystemTypes
          {
            eSPOT_SYS_MAIN            = 0x01, // The whole system
            eSPOT_SYS_SECURITYMODULE  = 0x02, // Security module subsystem
            eSPOT_SYS_DISPLAY         = 0x04, // Display subsystem
            eSPOT_SYS_KEYBOARD        = 0x08, // Keyboard subsystem
            eSPOT_SYS_CARDREADER      = 0x10, // Card reader subsystem
            eSPOT_SYS_GEMCORE         = 0x20  // SAM interface-module subsystem
          };

          ///< @brief The OPT subsystem device codes are listed in the following table:
          enum EOPTSubsystemTypes
          {
            eOPT_PRINTER              = 0x01, // Printer
            eOPT_IO_MODULE            = 0x02, // I/O module
            eOPT_IO_NOTBATT_INPUT1    = 0x04, // Not batterized input # 1
            eOPT_CONTACTLESS          = 0x08, // GCM (OTI contactless reader)
            eOPT_IO_NOTBATT_INPUT2    = 0x10, // Not batterized input # 2
            eOPT_IO_BATT_INPUT4       = 0x20  // Batterized input # 4
          };


          /**@brief default message size*/
          static const int CMinMessageSize = CHeaderSize + 2; // minimum message size, header size plus ackcode size plus num_entries size.

          /**@brief Spot Message Identification, command category.*/
          static const unsigned char CSystemStatusResponseCommandId = 0x10;

          /**@brief Spot Message Identification, command code.*/
          static const unsigned char CSystemStatusResponseSubCommandId = 0x01;

          /** @brief Item Quantity byte. Offset inside message. */
          static const ISpotMessage::SizeType CMessageItemQtyOffset = 0x07;

          /** @brief Item Data. Offset inside message. */
          static const ISpotMessage::SizeType CMessageItemDataOffset = 0x08;

          static const char * CpcStatusLabel;

          static const char * CpcDeviceLabel;

          static const char * CpcStateLabel;

          static const char * CpcStateExLabel;

          static const char * CpcErrorLabel;

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
          static  TypeId GetTypeId (void) { return _getTypeId<SpotMessageSystemStatusResponse>(); };
          virtual TypeId getTypeId (void) const { return GetTypeId(); };

          /**
           * @brief default constructor
           * @since 0.0.0
           */
          SpotMessageSystemStatusResponse(void);

          /**
           * @brief Destructor.
           * @since 0.0.0
           */
          virtual ~SpotMessageSystemStatusResponse();

          /**
           * @brief Gets the total number of the items present in the last response.
           * @note  The returned number of items will be zero in parse() method has not previously been called.
           *
           * @return The number of items present in the last response as a result of a parse()
           *
           * @see SpotMessageHardwareInfo::parse()
           * @see ItemType
           */
          unsigned int getItemQty (void) const;

          /**
           * @brief Gets one item data of the response.
           *
           * @param @b UINT index  : The linear position of the item into the reply message.
           *
           * @return The selected item, or NULL item structure if the
           *         index is out of range.
           *
           * @see ItemType
           */
          ItemType getItemData ( unsigned int uIndex ) const;

          /**
           * @brief Gets one item data of the response.
           *
           * @param @b [In]  iItemId : The item ID to search into the reply message.
           * @param @b [Out] iIndex  : The linear position of the item into the reply message.
           *
           * @return The requested item, or a NULL item structure if not found.
           *
           * @see ItemType
           */
          ItemType getItemById ( int iItemId, int & iIndex ) const;

          /*
           * @brief Gets all the details (items) of the display status as two maps.
           * @param [Out] @b sDeviceName The device name ( "display" ).
           * @param [Out] @b map is the returned summary map<string,string> describing device name and status.
           * @param [Out] @b attrMap is the returned detail map<string,string> with attributes (status, statusEx, error).
           * @return @b true if everything went OK collecting items from the response.
           *         @b false if an error blocked to collect all the info.
           */
          bool getDisplayInfo (std::string & sDeviceName, DeviceStatusMap & map, DeviceStatusMap & attrMap) const ;

          /*
           * @brief Gets all the details (items) of the keypad status as two maps.
           * @param [Out] @b sDeviceName The device name ( "keypad" ).
           * @param [Out] @b map is the returned summary map<string,string> describing device name and status.
           * @param [Out] @b attrMap is the returned detail map<string,string> with attributes (status, statusEx, error).
           * @return @b true if everything went OK collecting items from the response.
           *         @b false if an error blocked to collect all the info.
           */
          bool getKeypadInfo (std::string & sDeviceName, DeviceStatusMap & map, DeviceStatusMap & attrMap) const ;

          /*
           * @brief Gets all the details (items) of the card reader status as two maps.
           * @param [Out] @b sDeviceName The device name ( "card reader" ).
           * @param [Out] @b map is the returned summary map<string,string> describing device name and status.
           * @param [Out] @b attrMap is the returned detail map<string,string> with attributes (status, statusEx, error).
           * @return @b true if everything went OK collecting items from the response.
           *         @b false if an error blocked to collect all the info.
           */
          bool getCardReaderInfo (std::string & sDeviceName, DeviceStatusMap & map, DeviceStatusMap & attrMap) const ;

          /*
           * @brief Gets all the details (items) of the printer status as two maps.
           * @param [Out] @b sDeviceName The device name ( "printer" ).
           * @param [Out] @b map is the returned summary map<string,string> describing device name and status.
           * @param [Out] @b attrMap is the returned detail map<string,string> with attributes (status, statusEx, error).
           * @return @b true if everything went OK collecting items from the response.
           *         @b false if an error blocked to collect all the info.
           */
          bool getPrinterInfo (std::string & sDeviceName, DeviceStatusMap & map, DeviceStatusMap & attrMap) const ;

          /**
           * @brief Serializes the System Status response message (only used for tests).
           * @note This method serializes just a typical hard-coded reply for unit testing.
           *       This is because a SPOT response message is never sent to SPOT !!
           * @see IMessage::serialize()
           */
          ISpotMessage::BufferType serialize (void) const;

          /**
           * @brief Parses the given buffer expecting to have a SPOT System Status Response message inside it.
           * @see IMessage::parse()
           */
          bool parse ( ISpotMessage::BufferType const & );


        private:

          /*
           * @brief Gets all the details (items) of the requested device status as two maps.
           * @param [In]  @b bReplyAppId the response application ID (currently 0x81 or 0x84)
           * @param [In]  @b bDeviceId the Id of the requested device (currently 1, 2, 4, 8, 16 or 32)
           * @param [Out] @b sDeviceName The requested device name.
           * @param [Out] @b map is the returned summary map<string,string> describing device name and status.
           * @param [Out] @b attrMap is the returned detail map<string,string> with attributes (status, statusEx, error).
           * @return @b true if everything went OK collecting items from the response.
           *         @b false if an error blocked to collect all the info.
           */
          bool getDeviceInfo ( unsigned char bReplyAppId, unsigned char bDeviceId, std::string & sDeviceName,
                               DeviceStatusMap & map, DeviceStatusMap & attrMap) const ;

          static const char * CapcPinpadSubsystemLabels[eTotalPinpadSubsystems];
          static const char * CapcOPTSubsystemLabels[eTotalOPTSubsystems];

          std::vector<ItemType> m_items;
      };

    }
  }
}

#endif
