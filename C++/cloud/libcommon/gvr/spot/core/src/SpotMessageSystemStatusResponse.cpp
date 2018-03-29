/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotMessageSystemStatusResponse.cpp
 * @author mgrieco
 * @date Friday, December 29, 2014
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <sstream>
#include <iomanip>
#include <gvr/communication/spot/SpotMessageSystemStatusResponse.h>

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace communication
  {
    /// @brief Spot Domain
    namespace spot
    {


      const char * SpotMessageSystemStatusResponse::CapcStatuses[eTotalStatuses] =
      {
        "idle", "in-use", "down", "warning", "error", "startup"
        //"Ok", "warning", "error"
      };
      const char * SpotMessageSystemStatusResponse::CapcPinpadSubsystemLabels[eTotalPinpadSubsystems] =
      {
        "main system", "security module", "display", "keypad", "card reader", "SAM interface"
      };
      const char * SpotMessageSystemStatusResponse::CapcOPTSubsystemLabels[eTotalOPTSubsystems] =
      {
        "printer", "I/O module", "Not batterized input 1", "OTI", "Not batterized input 2", "Batterized input 4"
      };

      const char * SpotMessageSystemStatusResponse::CpcStatusLabel = "status";

      const char * SpotMessageSystemStatusResponse::CpcDeviceLabel = "device";

      const char * SpotMessageSystemStatusResponse::CpcStateLabel = "state";

      const char * SpotMessageSystemStatusResponse::CpcStateExLabel = "stateEx";

      const char * SpotMessageSystemStatusResponse::CpcErrorLabel = "error";


      SpotMessageSystemStatusResponse::SpotMessageSystemStatusResponse(void)
        : AbsSpotMessageWithAppCatCmdID (true, CMessageResponsePinpadAppId, CSystemStatusResponseCommandId, CSystemStatusResponseSubCommandId)
      {
      }

      SpotMessageSystemStatusResponse::~SpotMessageSystemStatusResponse()
      {
      }

      unsigned int SpotMessageSystemStatusResponse::getItemQty( void ) const
      {
        return m_items.size();
      }

      SpotMessageSystemStatusResponse::ItemType SpotMessageSystemStatusResponse::getItemData( unsigned int uIndex ) const
      {
        SpotMessageSystemStatusResponse::ItemType item;

        if( uIndex < m_items.size() )
        {
          item = m_items[uIndex];
        }

        return item;
      }

      SpotMessageSystemStatusResponse::ItemType SpotMessageSystemStatusResponse::getItemById ( int iItemId, int & iIndex ) const
      {
        SpotMessageSystemStatusResponse::ItemType nullItem;

        for(int i = 0; i < m_items.size(); i++)
        {
          if( iItemId == m_items[i].deviceCode )
          {
            iIndex = i;
            return m_items[i];
          }
        }

        iIndex = -1;
        return nullItem;
      }

      bool SpotMessageSystemStatusResponse::getDisplayInfo ( std::string & sDeviceName,
                                                             DeviceStatusMap & map,
                                                             DeviceStatusMap & attrMap  ) const
      {
        return getDeviceInfo( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                              eSPOT_SYS_DISPLAY, sDeviceName, map, attrMap );

      }

      bool SpotMessageSystemStatusResponse::getKeypadInfo ( std::string & sDeviceName,
                                                            DeviceStatusMap & map,
                                                            DeviceStatusMap & attrMap  ) const
      {
        return getDeviceInfo( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                              eSPOT_SYS_KEYBOARD, sDeviceName, map, attrMap );

      }

      bool SpotMessageSystemStatusResponse::getCardReaderInfo ( std::string & sDeviceName,
                                                                DeviceStatusMap & map,
                                                                DeviceStatusMap & attrMap ) const
      {
        return getDeviceInfo( ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId,
                              eSPOT_SYS_CARDREADER, sDeviceName, map, attrMap );
      }

      bool SpotMessageSystemStatusResponse::getPrinterInfo ( std::string & sDeviceName,
                                                             DeviceStatusMap & map,
                                                             DeviceStatusMap & attrMap) const
      {
        return getDeviceInfo( ISpotMessageWithAppCatCmdID::CMessageResponseOPTAppId,
                              eOPT_PRINTER, sDeviceName, map, attrMap );
      }

      SpotMessageSystemStatusResponse::BufferType SpotMessageSystemStatusResponse::serialize (void) const
      {
        BufferType retBuffer( AbsSpotMessageWithAppCatCmdID::serialize() );

        retBuffer.push_back( static_cast<unsigned char>(m_items.size()) );

        for( unsigned int i = 0; i < m_items.size(); i++ )
        {
          retBuffer.push_back( m_items[i].deviceCode );
          retBuffer.push_back( m_items[i].status     );
          retBuffer.push_back( m_items[i].statusExt  );
          retBuffer.push_back( m_items[i].errorCode  );
        }

        return retBuffer;
      }

      bool SpotMessageSystemStatusResponse::parse(BufferType const & buff)
      {
        if( buff.size() <= CMinMessageSize )
        {
          return false;
        }

        if( buff[CMessageItemQtyOffset] == 0 )
        {
          return false; // This response has no item codes, which is only allowed in requests, but not in a reply message.
        }

        if( buff[CMessageAppIdOffset] == ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId ||
            buff[CMessageAppIdOffset] == ISpotMessageWithAppCatCmdID::CMessageResponseOPTAppId )
        {
          setAppId( buff[CMessageAppIdOffset] );
        }
        else
        {
          return false; // not an Application ID having system status notification (Pinpad or OPT).
        }

        // do the rest of the parsing
        bool blRet = AbsSpotMessageWithAppCatCmdID::parse(buff); // Length and AckCode are get by this method.

        if( !blRet )
        {
          std::cout << "Bad header parsed 1!!!\n";
          return false;
        }

        unsigned int nItems = buff[CMessageItemQtyOffset];

        //std::cout << std::dec << "SpotMessageSystemStatusResponse::parse() - Length: " << size() << " bytes. ACK = " << (getAckCode()==0?("ok"):("fail")) << " . Items: " << nItems << std::endl;

        if( getAckCode() != CMessageResponseAckOK )
        {
          return true;
        }

        unsigned int offset = CMessageItemDataOffset;
        for( unsigned int i = 0; i < nItems; i++ )
        {
          SpotMessageSystemStatusResponse::ItemType item;

          item.deviceCode = buff[ (offset    ) ];
          item.status     = buff[ (offset + 1) ];
          item.statusExt  = buff[ (offset + 2) ];
          item.errorCode  = buff[ (offset + 3) ];

          offset += 4;
          m_items.push_back(item);
        }

        blRet = ( m_items.size() != 0 ); // If there is at least one item is a successfully message response.

        if( !blRet )
        {
          std::cout << "SpotMessageSystemStatusResponse::parse() - FAILED !!\n";
          return false;
        }

        setSerializedSize( CMinMessageSize + 4 * m_items.size() );

        return true;
      }


      bool SpotMessageSystemStatusResponse::getDeviceInfo
      (
        unsigned char bReplyAppId,
        unsigned char bDeviceId,
        std::string & sDeviceName,
        DeviceStatusMap & map,
        DeviceStatusMap & attrMap  ) const
      {
        if( getSpotAppId() != bReplyAppId )
          return false; // bad reply application Id

        int idx = -1;
        SpotMessageSystemStatusResponse::ItemType item = getItemById ( bDeviceId, idx );
        if( idx < 0) return false; // Info for the request device was not queried or received.

        const char* pcStatus = CapcStatuses[eIdle];

        // Business rules are different for each particular device
        if( bReplyAppId == ISpotMessageWithAppCatCmdID::CMessageResponsePinpadAppId )
        {
          switch(bDeviceId)
          {
            case eSPOT_SYS_MAIN:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[ePinpad_Main];
              // TODO: implement rule
              break;

            case eSPOT_SYS_SECURITYMODULE:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[ePinpad_SecurityModule];
              // TODO: implement rule
              break;

            case eSPOT_SYS_DISPLAY:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[ePinpad_Display];
              if( item.errorCode != 0 )
                pcStatus = CapcStatuses[eError];
              else if( item.status != 0x00 )
                pcStatus = CapcStatuses[eDown];
              break;

            case eSPOT_SYS_KEYBOARD:
            case eSPOT_SYS_CARDREADER:
              idx = ( bDeviceId == eSPOT_SYS_KEYBOARD ? ePinpad_Keyboard : ePinpad_CardReader);
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[idx];
              if( item.errorCode != 0 || item.status == 0x03 || item.status == 0x04 )
                pcStatus = CapcStatuses[eError];
              else if( item.status == 0x06 )
                pcStatus = CapcStatuses[eDown];
              else if( item.status == 0x05 )
                pcStatus = CapcStatuses[eStartup];
              else if( item.status == 0x02 )
                pcStatus = CapcStatuses[eInUse];
              break;

            case eSPOT_SYS_GEMCORE:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[ePinpad_GEMCore];
              // TODO: implement rule
              break;

            default:
              return false; // unknown device Id
          }
        }
        else if( bReplyAppId == ISpotMessageWithAppCatCmdID::CMessageResponseOPTAppId )
        {
          switch(bDeviceId)
          {
            case eOPT_PRINTER:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[eOPT_Printer];
              if( item.errorCode != 0 || item.status == 0x01 )
                pcStatus = CapcStatuses[eError];
              else if( item.status == 0x04 )
                pcStatus = CapcStatuses[eStartup];
              else if( item.status == 0x03 )
                pcStatus = CapcStatuses[eInUse];
              break;

            case eOPT_IO_MODULE:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[eOPT_IO_Module];
              // TODO: implement rule
              break;

            case eOPT_IO_NOTBATT_INPUT1:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[eOPT_IO_NotBatt_Input1];
              // TODO: implement rule
              break;

            case eOPT_CONTACTLESS:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[eOPT_Contactless];
              // TODO: implement rule
              break;

            case eOPT_IO_NOTBATT_INPUT2:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[eOPT_IO_NotBatt_Input2];
              // TODO: implement rule
              break;

            case eOPT_IO_BATT_INPUT4:
              map[CpcDeviceLabel] = CapcPinpadSubsystemLabels[eOPT_IO_Batt_Input4];
              // TODO: implement rule
              break;

            default:
              return false; // unknown device Id
          }

        }

        map[CpcStatusLabel] = pcStatus;

        std::stringstream strStream;
        strStream << "0x" << std::hex << std::setw(2) << std::setfill('0');
        strStream << static_cast<unsigned int>(item.status);
        attrMap[CpcStateLabel] = strStream.str();

        strStream.str("");
        strStream << "0x" << static_cast<unsigned int>(item.statusExt);
        attrMap[CpcStateExLabel] = strStream.str();

        strStream.str("");
        strStream << "0x" << static_cast<unsigned int>(item.errorCode);
        attrMap[CpcErrorLabel] = strStream.str();

        return true;
      }


    }
  }
}
