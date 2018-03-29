/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file DeviceInformation.h
 * @author mvolker
 * @date Monday, June 22, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _DEVICE_INFORMATION_H_
#define _DEVICE_INFORMATION_H_

# include <string>
# include <map>

# include <gvr/communication/spot/SpotSession.h>
# include <gvr/utils/Json.h>

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class DeviceInformation
      {
        public:
          typedef std::pair<std::string,std::string> StrStrPair;
          typedef std::map<std::string,std::string>  TextInfoMap;
          typedef std::map<std::string,TextInfoMap>  ComponentMap;
          typedef std::map<std::string,StrStrPair>   CommandsMap;

          static char const * CUnmodifiedDeviceType;
          static char const * CUnmodifiedManufacturer;
          static char const * CUnmodifiedModel;
          static char const * CUnmodifiedSerialNbr;
          static char const * CUnmodifiedLocalId;

          static char const * CUnregisteredDeviceId;

          static bool const   CSpotDefaultDNSSupported;
          static char const * CSpotDefaultCertFormat;

          /**
           * @brief default destructor.
           */
          virtual ~DeviceInformation (void);

          /**
           * @brief default constructor.
           */
          DeviceInformation (void);

          bool collect (gvr::utils::Json & json, gvr::communication::spot::SpotSession & session,
                                                 std::string const & localId, std::string const & registrationCode,
                                                 std::string const & certFormat = CSpotDefaultCertFormat,
                                                 bool const & dnsSupported = CSpotDefaultDNSSupported,
                                                 gvr::utils::Json const & associated = gvr::utils::Json());

          bool collect (gvr::utils::Json & json, gvr::communication::spot::SpotSession & session,
                                                 std::string const & localId,
                                                 gvr::utils::Json const & associated = gvr::utils::Json());

          bool pushAssociatedDeviceInfo (gvr::utils::Json & json, bool force,
                                                                  std::string const & deviceId,
                                                                  TextInfoMap & events,
                                                                  CommandsMap & commands,
                                                                  TextInfoMap & firmware, ComponentMap & components,
                                                                  std::string const & localId,
                                                                  std::string const & manufacturer,
                                                                  std::string const & model,
                                                                  std::string const & serial,
                                                                  std::string const & deviceType);

        protected:
          typedef std::pair<std::string,TextInfoMap> StrTextInfoMapPair;
          typedef TextInfoMap::const_iterator        TextInfoMapIterator;
          typedef ComponentMap::const_iterator       ComponentMapIterator;
          typedef CommandsMap::const_iterator        CommandsMapIterator;

          static char const * CEmptyString;

          static char const * CSpotDefaultManufacturer;
          static char const * CSpotDefaultModel;
          static char const * CSpotDefaultSerialNbr;
          static char const * CSpotDefaultDeviceType;

          static char const * CRegisterDeviceName;
          static char const * CRegistrationCodeName;
          static char const * CCertFormatName;
          static char const * CDnsSupportedName;
          static char const * CDeviceConfigName;

          static char const * CDeviceTypeName;
          static char const * CLocalIdName;
          static char const * CAssetInfoName;
          static char const * CManufacturerName;
          static char const * CModelName;
          static char const * CSerialNbrName;
          static char const * CFirmwareName;
          static char const * CComponentName;
          static char const * CNameName;
          static char const * CAtributesName;
          static char const * CEventsName;
          static char const * COperationName;
          static char const * CPayloadName;
          static char const * CCmdsName;
          static char const * CRequestName;
          static char const * CResponseName;
          static char const * CAssociatedName;
          static char const * CProxiedName;

          static char const * CDeviceIdName;
          static char const * CRemoveName;

          //////////
          static char const * CSpotDefaultHarcodedEventsAndCommnads;
          //////////

          void populateRegisterDevice (gvr::utils::Json & json, std::string const & registrationCode,
                                                                std::string const & certFormat, bool dnsSupported);

          bool listSpotConfig (gvr::utils::Json & json, bool force,
                                                        gvr::utils::Json const & events,
                                                        gvr::utils::Json const & commands,
                                                        gvr::utils::Json const & associated,
                                                        std::string const & localId,
                                                        std::string const & manufacturer,
                                                        std::string const & model,
                                                        std::string const & serial,
                                                        std::string const & deviceType,
                                                        gvr::communication::spot::SpotSession & session);

          void listFirmwareInfo (gvr::utils::Json & json, TextInfoMap & firmware);

          void listComponentsInfo (gvr::utils::Json & json, ComponentMap & components);

          void populateAssetInfo (gvr::utils::Json & json, bool force,
                                                                std::string const & manufacturer,
                                                                std::string const & model, std::string const & serial,
                                                                TextInfoMap & firmware, ComponentMap & components);

          void populateEvents (gvr::utils::Json & json, bool force, TextInfoMap & events);

          void populateCommands (gvr::utils::Json & json, bool force, CommandsMap & commands);

          void listDeviceInfo (gvr::utils::Json & json, bool force,
                                                        std::string const & deviceType,
                                                        std::string const & localId,
                                                        std::string const & manufacturer,
                                                        std::string const & model, std::string const & serial,
                                                        TextInfoMap & firmware, ComponentMap & components,
                                                        TextInfoMap & events,
                                                        CommandsMap & commands);

          bool collectFirmwareAndComponentsInfo (bool force,
                                                 std::string  & deviceType,
                                                 TextInfoMap  & firmware,
                                                 ComponentMap & components,
                                                 gvr::communication::spot::SpotSession & session);

      };
    }
  }

#endif
