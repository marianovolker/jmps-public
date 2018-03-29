/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file DeviceInformation.cpp
 * @author mvolker
 * @date Monday, June 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <DeviceInformation.h>

#include <gvrlog/log.h>

#include <gvr/communication/ucc/message/UccMessageCloudDeviceConfigChange.h>
#include <gvr/communication/ucc/message/UccMessageRegisterRequest.h>

#include <gvr/communication/spot/SpotMessageHardwareInfo.h>
#include <gvr/communication/spot/SpotMessageHardwareInfoResponse.h>
#include <gvr/communication/spot/SpotMessageOPTHwrInfo.h>
#include <gvr/communication/spot/SpotMessageOPTHwrInfoResponse.h>
#include <gvr/communication/spot/SpotMessageOptSwInfo.h>
#include <gvr/communication/spot/SpotMessageOptSwInfoResponse.h>

#include <workers/DebianInfoCollectorWorker.h>

DECLARE_LOG_MODULE(core);

using namespace gvr::communication::ucc;
using namespace gvr::communication::spot;

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    char const * DeviceInformation::CEmptyString             = "";
    char const * DeviceInformation::CUnmodifiedDeviceType    = CEmptyString;
    char const * DeviceInformation::CUnmodifiedManufacturer  = CEmptyString;
    char const * DeviceInformation::CUnmodifiedModel         = CEmptyString;
    char const * DeviceInformation::CUnmodifiedSerialNbr     = CEmptyString;
    char const * DeviceInformation::CUnmodifiedLocalId       = CEmptyString;

    char const * DeviceInformation::CUnregisteredDeviceId    = CEmptyString;

    bool const   DeviceInformation::CSpotDefaultDNSSupported = true;
    char const * DeviceInformation::CSpotDefaultCertFormat   = "P12";
    char const * DeviceInformation::CSpotDefaultManufacturer = "Gilbarco Veeder-Root";
    char const * DeviceInformation::CSpotDefaultModel        = CEmptyString;           // Default Model for Spot is empty.
    char const * DeviceInformation::CSpotDefaultSerialNbr    = CEmptyString;           // Default SerialNbr for Spot is empty.
    char const * DeviceInformation::CSpotDefaultDeviceType   = CEmptyString;           // Default DeviceType for Spot is empty.

    char const * DeviceInformation::CRegisterDeviceName      = UccMessageRegisterRequest::CRegisterDeviceName;
    char const * DeviceInformation::CRegistrationCodeName    = UccMessageRegisterRequest::CRegistrationCodeName;
    char const * DeviceInformation::CCertFormatName          = UccMessageRegisterRequest::CCertFormatName;
    char const * DeviceInformation::CDnsSupportedName        = UccMessageRegisterRequest::CDnsSupportedName;
    char const * DeviceInformation::CDeviceConfigName        = UccMessageRegisterRequest::CDeviceConfigName;

    char const * DeviceInformation::CDeviceTypeName          = UccMessageRegisterRequest::CDeviceTypeName;
    char const * DeviceInformation::CLocalIdName             = UccMessageRegisterRequest::CLocalIdName;
    char const * DeviceInformation::CAssetInfoName           = UccMessageRegisterRequest::CAssetInfoName;
    char const * DeviceInformation::CManufacturerName        = UccMessageRegisterRequest::CManufacturerName;
    char const * DeviceInformation::CModelName               = UccMessageRegisterRequest::CModelName;
    char const * DeviceInformation::CSerialNbrName           = UccMessageRegisterRequest::CSerialNbrName;
    char const * DeviceInformation::CFirmwareName            = UccMessageRegisterRequest::CFirmwareName;
    char const * DeviceInformation::CComponentName           = UccMessageRegisterRequest::CComponentName;
    char const * DeviceInformation::CNameName                = UccMessageRegisterRequest::CNameName;
    char const * DeviceInformation::CAtributesName           = UccMessageRegisterRequest::CAtributesName;
    char const * DeviceInformation::CEventsName              = UccMessageRegisterRequest::CEventsName;
    char const * DeviceInformation::COperationName           = UccMessageRegisterRequest::COperationName;
    char const * DeviceInformation::CPayloadName             = UccMessageRegisterRequest::CPayloadName;
    char const * DeviceInformation::CCmdsName                = UccMessageRegisterRequest::CCmdsName;
    char const * DeviceInformation::CRequestName             = UccMessageRegisterRequest::CRequestName;
    char const * DeviceInformation::CResponseName            = UccMessageRegisterRequest::CResponseName;
    char const * DeviceInformation::CAssociatedName          = UccMessageRegisterRequest::CAssociatedName;
    char const * DeviceInformation::CProxiedName             = UccMessageRegisterRequest::CProxiedName;

    char const * DeviceInformation::CDeviceIdName            = UccMessageCloudDeviceConfigChange::CDeviceIdName;
    char const * DeviceInformation::CRemoveName              = UccMessageCloudDeviceConfigChange::CRemoveName;

    //////////
    char const * DeviceInformation::CSpotDefaultHarcodedEventsAndCommnads = ""
    "{"
    "  \"events\": ["
    "    {"
    "      \"operation\": \"cloud-device-config-change-event\","
    "      \"payload\":   \"http://schemas.gilbarco.com/json/event/v1/cloud-device-config-change-event.json\""
    "    },"
    "    {"
    "      \"operation\": \"alert-event\","
    "      \"payload\":   \"http://schemas.gilbarco.com/json/event/v1/alert-event.json\""
    "    },"
    "    {"
    "      \"operation\": \"component-alert-event\","
    "      \"payload\":   \"http://schemas.gilbarco.com/json/event/v1/component-alert-event.json\""
    "    },"
    "    {"
    "      \"operation\": \"component-status-event\","
    "      \"payload\":   \"http://schemas.gilbarco.com/json/event/v1/component-status-event.json\""
    "    },"
    "    {"
    "      \"operation\": \"status-event\","
    "      \"payload\":   \"http://schemas.gilbarco.com/json/event/v1/status-event.json\""
    "    },"
    "    {"
    "      \"operation\": \"file-download-event\","
    "      \"payload\":   \"http://schemas.gilbarco.com/json/event/v1/file-download-event.json\""
    "    },"
    "    {"
    "      \"operation\": \"file-upload-event\","
    "      \"payload\":   \"http://schemas.gilbarco.com/json/event/v1/file-upload-event.json\""
    "    },"
    "    {"
    "      \"operation\": \"install-event\","
    "      \"payload\":   \"http://schemas.gilbarco.com/json/event/v1/install-event.json\""
    "    }"
    "  ],"
    "  \"cmds\": ["
    "    {"
    "      \"operation\": \"heartbeat-cmd\","
    "      \"request\":   \"{}\","
    "      \"response\":  \"{}\""
    "    },"
    "    {"
    "      \"operation\": \"reconnect-cmd\","
    "      \"request\":   \"{}\","
    "      \"response\":  \"{}\""
    "    },"
    "    {"
    "      \"operation\": \"security-change-cmd\","
    "      \"request\":   \"http://schemas.gilbarco.com/json/command/v1/security-change-cmd-request.json\","
    "      \"response\":  \"{}\""
    "    },"
    "    {"
    "      \"operation\": \"unregister-cmd\","
    "      \"request\":   \"http://schemas.gilbarco.com/json/command/v1/unregister-cmd-request.json\","
    "      \"response\":  \"{}\""
    "    },"
    "    {"
    "      \"operation\": \"file-download-cmd\","
    "      \"request\":   \"http://schemas.gilbarco.com/json/command/v1/file-download-cmd-request.json\","
    "      \"response\":  \"http://schemas.gilbarco.com/json/command/v1/file-download-cmd-response.json\""
    "    },"
    "    {"
    "      \"operation\": \"file-upload-cmd\","
    "      \"request\":   \"http://schemas.gilbarco.com/json/command/v1/file-upload-cmd-request.json\","
    "      \"response\":  \"{}\""
    "    },"
    "    {"
    "      \"operation\": \"file-upload-schedule-cmd\","
    "      \"request\":   \"http://schemas.gilbarco.com/json/command/v1/file-upload-schedule-cmd-request.json\","
    "      \"response\":  \"{}\""
    "    },"
    "    {"
    "      \"operation\": \"install-cmd\","
    "      \"request\":   \"http://schemas.gilbarco.com/json/command/v1/install-cmd-request.json\","
    "      \"response\":  \"{}\""
    "    },"
    "    {"
    "      \"operation\": \"reset-cmd\","
    "      \"request\":   \"{}\","
    "      \"response\":  \"{}\""
    "    }"
    "  ]"
    "}"
    "";
    //////////

    DeviceInformation::~DeviceInformation (void)
    {
    }

    DeviceInformation::DeviceInformation (void)
    {
    }

    bool DeviceInformation::collect (gvr::utils::Json & json, SpotSession & session,
                                                              std::string const & localId, std::string const & registrationCode,
                                                              std::string const & certFormat,
                                                              bool const & dnsSupported,
                                                              gvr::utils::Json const & associated)
    {
      static bool const forceForDeviceNewRegistration = true;
      bool result = false;
      gvr::utils::Json jsonEventsAndcommands;
      
      if (jsonEventsAndcommands.parse(CSpotDefaultHarcodedEventsAndCommnads) == true)
      {
        gvr::utils::Json jsonDeviceConfig;
        if (listSpotConfig(jsonDeviceConfig,forceForDeviceNewRegistration,
                                            jsonEventsAndcommands.get(CEventsName),
                                            jsonEventsAndcommands.get(CCmdsName),
                                            associated,localId,
                                            CSpotDefaultManufacturer,CSpotDefaultModel,
                                            CSpotDefaultSerialNbr,CSpotDefaultDeviceType,
                                            session) == true)
        {
          populateRegisterDevice(json,registrationCode,certFormat,dnsSupported);
          json.set(CDeviceConfigName) = jsonDeviceConfig;
          result = true;
        }
      }
      else
      {
        LOG_Critical(core,"DeviceInformation::collect - Events and commands Parse Error.");
      }
      return result;
    }

    bool DeviceInformation::collect (gvr::utils::Json & json, SpotSession & session,
                                                              std::string const & localId,
                                                              gvr::utils::Json const & associated)
    {
      static bool const forceForDeviceConfigUpdate = false;
      bool result = false;
      gvr::utils::Json jsonEventsAndcommands;
      
      if (jsonEventsAndcommands.parse(CSpotDefaultHarcodedEventsAndCommnads) == true)
      {
        gvr::utils::Json jsonDeviceConfig;
        result = listSpotConfig(json,forceForDeviceConfigUpdate,
                                     jsonEventsAndcommands.get(CEventsName),
                                     jsonEventsAndcommands.get(CCmdsName),
                                     associated,localId,
                                     CUnmodifiedManufacturer,CUnmodifiedModel,
                                     CUnmodifiedSerialNbr,CUnmodifiedDeviceType,
                                     session);
      }
      else
      {
        LOG_Critical(core,"DeviceInformation::collect - Events and commands Parse Error.");
      }
      return result;
    }

    bool DeviceInformation::pushAssociatedDeviceInfo (gvr::utils::Json & json, bool force,
                                                                               std::string const & deviceId,
                                                                               TextInfoMap & events,
                                                                               CommandsMap & commands,
                                                                               TextInfoMap & firmware, ComponentMap & components,
                                                                               std::string const & localId,
                                                                               std::string const & manufacturer,
                                                                               std::string const & model,
                                                                               std::string const & serial,
                                                                               std::string const & deviceType)
    {
      bool result = false;

      if (json.isNull() || json.isArray())
      {
        gvr::utils::Json jsonItem;

        if (force)
        {
          listDeviceInfo(jsonItem,force,
                                  deviceType,
                                  localId,
                                  manufacturer,
                                  model,serial,
                                  firmware,components,
                                  events,commands);

          if (jsonItem.isEmpty() == false)
          {
            jsonItem.set(CProxiedName) = true;
          }
        }
        else if (deviceId.empty() == false)
        {
          listDeviceInfo(jsonItem,force,
                                  deviceType,
                                  localId,
                                  manufacturer,
                                  model,serial,
                                  firmware,components,
                                  events,commands);

          if (jsonItem.isEmpty() == false)
          {
            jsonItem.set(CDeviceIdName) = deviceId;
          }
          else
          {
            jsonItem.set(CRemoveName) = true;
            jsonItem.set(CDeviceIdName) = deviceId;
          }
        }

        if (jsonItem.isEmpty() == false)
        {
          json.push_back(jsonItem);
          result = true;
        }
      }
      return result;
    }

    void DeviceInformation::populateRegisterDevice (gvr::utils::Json & json, std::string const & registrationCode,
                                                                             std::string const & certFormat, bool dnsSupported)
    {
      json.set(CRegisterDeviceName)
          .set(CRegistrationCodeName) = registrationCode;
      json.set(CRegisterDeviceName)
          .set(CCertFormatName)       = certFormat;
      json.set(CRegisterDeviceName)
          .set(CDnsSupportedName)     = dnsSupported;
    }

    bool DeviceInformation::listSpotConfig (gvr::utils::Json & json, bool force,
                                                                     gvr::utils::Json const & events,
                                                                     gvr::utils::Json const & commands,
                                                                     gvr::utils::Json const & associated,
                                                                     std::string const & localId,
                                                                     std::string const & manufacturer,
                                                                     std::string const & model,
                                                                     std::string const & serial,
                                                                     std::string const & deviceType,
                                                                     SpotSession & session)
    {
      bool result = false;

      std::string  deviceTypeAux(deviceType);
      TextInfoMap  firmware;
      ComponentMap components;

      if (collectFirmwareAndComponentsInfo(force,deviceTypeAux,firmware,components,session) == true)
      {
        TextInfoMap eventsDummy;
        CommandsMap commandsDummy;

        listDeviceInfo(json,force,
                            deviceTypeAux,
                            localId,
                            manufacturer,
                            model,serial,
                            firmware,components,
                            eventsDummy,commandsDummy);

        if (events.isEmpty() == false)
        {
          json.set(CEventsName) = events;
        }

        if (commands.isEmpty() == false)
        {
          json.set(CCmdsName) = commands;
        }

        if (associated.isEmpty() == false)
        {
          json.set(CAssociatedName) = associated;
        }

        result = true;
      }

      return result;
    }

    void DeviceInformation::listFirmwareInfo (gvr::utils::Json & json, TextInfoMap & firmware)
    {
      TextInfoMapIterator itr = firmware.begin();
      TextInfoMapIterator iend = firmware.end();

      while (itr != iend)
      {
        json.set(itr->first) = itr->second;
        ++itr;
      }
    }

    void DeviceInformation::listComponentsInfo (gvr::utils::Json & json, ComponentMap & components)
    {
      ComponentMapIterator itr  = components.begin();
      ComponentMapIterator iend = components.end();

      while (itr != iend)
      {
        std::string const & name       = itr->first;
        bool                remove     = itr->second.empty();
        TextInfoMap const & attributes = itr->second;

        gvr::utils::Json jsonComponent;

        jsonComponent.set(CNameName) = name;

        if (remove)
        {
          jsonComponent.set(CRemoveName) = true;
        }
        else
        {
          TextInfoMapIterator itr_attr = attributes.begin();
          TextInfoMapIterator iend_attr = attributes.end();

          while (itr_attr != iend_attr)
          {
            jsonComponent.set(CAtributesName).set(itr_attr->first) = itr_attr->second;
            ++itr_attr;
          }
        }

        json.push_back(jsonComponent);

        ++itr;
      }
    }

    void DeviceInformation::populateAssetInfo (gvr::utils::Json & json, bool force,
                                                                        std::string const & manufacturer,
                                                                        std::string const & model, std::string const & serial,
                                                                        TextInfoMap & firmware, ComponentMap & components)
    {
      // Manufacturer
      if (force || (manufacturer.empty() == false))
      {
        json.set(CAssetInfoName)
            .set(CManufacturerName) = manufacturer;
      }

      //Model
      if (model.empty() == false)
      {
        json.set(CAssetInfoName)
            .set(CModelName) = model;
      }

      //SerialNbr
      if (serial.empty() == false)
      {
        json.set(CAssetInfoName)
            .set(CSerialNbrName) = serial;
      }

      //Firmware
      {
        gvr::utils::Json jsonAux;
        listFirmwareInfo(jsonAux,firmware);
        if (jsonAux.isEmpty() == false)
        {
          json.set(CAssetInfoName).set(CFirmwareName) = jsonAux;
        }
      }

      //Components
      {
        gvr::utils::Json jsonAux;
        listComponentsInfo(jsonAux,components);
        if (jsonAux.isEmpty() == false)
        {
          json.get(CAssetInfoName).set(CComponentName) = jsonAux;
        }
      }

      if (json.get(CAssetInfoName).isEmpty())
      {
        if (force)
        {
          json.set(CAssetInfoName);
        }
        else
        {
          json.remove(CAssetInfoName);
        }
      }
    }

    void DeviceInformation::populateEvents (gvr::utils::Json & json, bool force, TextInfoMap & events)
    {
      TextInfoMapIterator itr = events.begin();
      TextInfoMapIterator iend = events.end();

      while (itr != iend)
      {
        gvr::utils::Json jsonItem;
        jsonItem.set(COperationName) = itr->first;
        if (force || (itr->second.empty() == false))
        {
          jsonItem.set(CPayloadName) = itr->second;
        }
        else
        {
          jsonItem.set(CRemoveName) = true;
        }
        json.set(CEventsName).push_back(jsonItem);
        ++itr;
      }

      if (json.get(CEventsName).isEmpty())
      {
        if (force)
        {
          json.set(CEventsName);
        }
        else
        {
          json.remove(CEventsName);
        }
      }
    }

    void DeviceInformation::populateCommands (gvr::utils::Json & json, bool force, CommandsMap & commands)
    {
      CommandsMapIterator itr  = commands.begin();
      CommandsMapIterator iend = commands.end();

      while (itr != iend)
      {
        gvr::utils::Json jsonItem;
        jsonItem.set(COperationName) = itr->first;
        if (force)
        {
          jsonItem.set(CRequestName) = itr->second.first;
          jsonItem.set(CResponseName) = itr->second.second;
        }
        else if (itr->second.first.empty() && itr->second.second.empty())
        {
          jsonItem.set(CRemoveName) = true;
        }
        else
        {
          if (itr->second.first.empty() == false)
          {
            jsonItem.set(CRequestName) = itr->second.first;
          }

          if (itr->second.second.empty() == false)
          {
            jsonItem.set(CResponseName) = itr->second.second;
          }
        }
        json.set(CCmdsName).push_back(jsonItem);
        ++itr;
      }

      if (json.get(CCmdsName).isEmpty())
      {
        if (force)
        {
          json.set(CCmdsName);
        }
        else
        {
          json.remove(CCmdsName);
        }
      }
    }

    void DeviceInformation::listDeviceInfo (gvr::utils::Json & json, bool force,
                                                                     std::string const & deviceType,
                                                                     std::string const & localId,
                                                                     std::string const & manufacturer,
                                                                     std::string const & model, std::string const & serial,
                                                                     TextInfoMap & firmware, ComponentMap & components,
                                                                     TextInfoMap & events,
                                                                     CommandsMap & commands)
    {
      // deviceType
      if (force || (deviceType.empty() == false))
      {
        json.set(CDeviceTypeName) = deviceType;
      }

      // localId
      if (force || (localId.empty() == false))
      {
        json.set(CLocalIdName) = localId;
      }

      populateAssetInfo(json,force,
                             manufacturer,
                             model,serial,
                             firmware,components);

      populateEvents(json,force,
                          events);

      populateCommands(json,force,
                          commands);
    }

    bool DeviceInformation::collectFirmwareAndComponentsInfo (bool force,
                                                              std::string  & deviceType,
                                                              TextInfoMap  & firmware,
                                                              ComponentMap & components,
                                                              SpotSession  & session)
    {
      static SpotSession::TimeMillisecondsType const CCommonTimeout = 1000; // one second of timeout...

      SpotMessageHardwareInfo msgHwrRequest;
      SpotMessageHardwareInfoResponse msgHwrReply;
      session.sendSync(msgHwrRequest, msgHwrReply, CCommonTimeout);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        LOG_VCritical(core, "SpotSession Error: [%s], sending SpotMessageHardwareInfo message.", session.getErrorDescription().c_str());
        return false;
      }

      SpotMessageSoftwareInfo msgPkgRequest;
      SpotMessageSoftwareInfoResponse msgPkgReply;
      session.sendSync(msgPkgRequest, msgPkgReply, CCommonTimeout);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        LOG_VCritical(core, "SpotSession Error: [%s], sending SpotMessageSoftwareInfo message.", session.getErrorDescription().c_str());
        return false;
      }

      SpotMessageOPTHwrInfo msgOptHwrRequest;
      SpotMessageOPTHwrInfoResponse msgOptHwrReply;
      session.sendSync(msgOptHwrRequest, msgOptHwrReply, CCommonTimeout);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        LOG_VCritical(core, "SpotSession Error: [%s], sending SpotMessageOPTHwrInfo message.", session.getErrorDescription().c_str());
        return false;
      }

      SpotMessageOptSwInfo msgOptSwInfoRequest;
      SpotMessageOptSwInfoResponse msgOptSwInfoReply;
      session.sendSync(msgOptSwInfoRequest, msgOptSwInfoReply, CCommonTimeout);
      if( session.getError() != SpotSession::SpotSessionError::CNoError )
      {
        LOG_VCritical(core, "SpotSession Error: [%s], sending SpotMessageOptSwInfo message.", session.getErrorDescription().c_str());
        return false;
      }

      std::string  sDeblabel, sCRLabel, sKPLabel, sDYLabel, sPRLabel, sCLRLabel, sIOMLabel, sOptPkgLabel, sPkgLabel;
      TextInfoMap mapDebInfo, mapCR, mapKP, mapDY, mapPR, mapCLR, mapIOM, mapOptFirmware, mapFirmware;

      {
        DebianInfoCollectorWorker debinfwrk(session);
        if (debinfwrk.collectInformation(sDeblabel,mapDebInfo) == false)
        {
          LOG_VCritical(core, "SpotSession Error: [%s], collecting debian packages list.", session.getErrorDescription().c_str());
          return false;
        }
      }

      msgHwrReply.getCardReaderInfo(sCRLabel, mapCR);
      msgHwrReply.getKeypadInfo(sKPLabel, mapKP);
      msgHwrReply.getDisplayInfo(sDYLabel, mapDY);
      msgOptHwrReply.getPrinterInfo(sPRLabel, mapPR);
      msgOptHwrReply.getContactlessReaderInfo(sCLRLabel, mapCLR);
      msgOptHwrReply.getIOModuleInfo(sIOMLabel, mapIOM);
      msgOptSwInfoReply.getAllOptSoftwareDescription(sOptPkgLabel, mapOptFirmware);
      msgPkgReply.getAllSoftwareDescription(sPkgLabel, mapFirmware);

      // Append OptFirmware to current Firmware map ...
      mapFirmware.insert(mapOptFirmware.begin(), mapOptFirmware.end());

      /////////////////////////////////////////////////////////
      // TODO: Review. We are not using the spot serial number.
      //               The serial number required by cloud is
      //               the one provided externally in the
      //               pre-registration.
      // msgHwrReply.getSerialNumber( sSpotSerialNbr );
      /////////////////////////////////////////////////////////

      if (force && deviceType.empty())
      {
        msgHwrReply.getSpotModel(deviceType);
      }

      LOG_Debug(core, "All information has been collected.");

      firmware.clear();
      components.clear();

      firmware.swap(mapFirmware);

      components.insert( StrTextInfoMapPair( sCRLabel , mapCR      ) );
      components.insert( StrTextInfoMapPair( sKPLabel , mapKP      ) );
      components.insert( StrTextInfoMapPair( sDYLabel , mapDY      ) );
      components.insert( StrTextInfoMapPair( sDeblabel, mapDebInfo ) );
      components.insert( StrTextInfoMapPair( sPRLabel , mapPR      ) );
      components.insert( StrTextInfoMapPair( sCLRLabel, mapCLR     ) );
      components.insert( StrTextInfoMapPair( sIOMLabel, mapIOM     ) );

      return true;
    }
  }
}

