/**
 * Copyright © 2014 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudClientApplicationConfiguration.cpp
 * @author gparis
 * @date Tuesday, September 16, 2014
 * @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
 */

#include "CloudClientApplicationConfiguration.h"

/**
 * Global Immutable definitions
 * ============================
 *
 * @brief This section is intended for global constants.
 *
 */

  const char* DEFAULT_FOO_NAME = "FOO for everyone.";


/**
 * Default Configuration Values
 * ============================
 *
 * @brief This section is intended for default values of the application configuration.
 *
 */

  const char*  CloudClientApplicationConfiguration::mst_CpcFooName = DEFAULT_FOO_NAME;

  //Device registration configuration keys
const std::string CloudClientApplicationConfiguration::CDeviceRegistrationRequestConfigKey = "DeviceRegistrationRequest";
const std::string CloudClientApplicationConfiguration::CRegisterDeviceConfigKey = "registerDevice";
const std::string CloudClientApplicationConfiguration::CRegistrationCodeConfigKey = "registrationCode";
const std::string CloudClientApplicationConfiguration::CCertFormatConfigKey = "certFormat";
const std::string CloudClientApplicationConfiguration::CDnsSupportedConfigKey = "dnsSupported";
const std::string CloudClientApplicationConfiguration::CDeviceConfigConfigKey = "deviceConfig";
const std::string CloudClientApplicationConfiguration::CDeviceTypeConfigKey = "deviceType";
const std::string CloudClientApplicationConfiguration::CLocalIdConfigKey = "localId";
const std::string CloudClientApplicationConfiguration::CAssetInfoConfigKey = "assetInfo";
const std::string CloudClientApplicationConfiguration::CManufacturerConfigKey = "manufacturer";
const std::string CloudClientApplicationConfiguration::CModelConfigKey = "model";
const std::string CloudClientApplicationConfiguration::CSerialNbrConfigKey = "serialNbr";
const std::string CloudClientApplicationConfiguration::CFirmwareConfigKey = "firmware";
const std::string CloudClientApplicationConfiguration::CComponentsConfigKey = "components";
const std::string CloudClientApplicationConfiguration::CEventsKey = "events";
const std::string CloudClientApplicationConfiguration::CEventNameConfigKey = "eventName";
const std::string CloudClientApplicationConfiguration::CPayloadConfigKey = "payload";
const std::string CloudClientApplicationConfiguration::CMSDSConfigKey = "cmds";
const std::string CloudClientApplicationConfiguration::CCmdConfigKey = "cmd";
const std::string CloudClientApplicationConfiguration::CCmdNameConfigKey = "cmdName";
const std::string CloudClientApplicationConfiguration::CCmdRequestConfigKey = "request";
const std::string CloudClientApplicationConfiguration::CCmdResponseConfigKey = "response";
const std::string CloudClientApplicationConfiguration::CAssociatedConfigKey = "associated";



  /** Default Constructor */
CloudClientApplicationConfiguration::CloudClientApplicationConfiguration()
  : m_iFoo(DEFAULT_FOO_VALUE), m_iLineParamQty(0),
    m_pcFooName(DEFAULT_FOO_NAME), m_apcLineParameters(NULL)
{
}


CloudClientApplicationConfiguration::CloudClientApplicationConfiguration(int nParams, const char* *apcParameters)
  : m_iFoo(0), m_iLineParamQty(0),
    m_pcFooName(NULL), m_apcLineParameters(NULL)
{
  setCommandLineParameters(nParams, apcParameters);
}


const char** CloudClientApplicationConfiguration::getCommandLineParameters(int& nParams)
{
  nParams = m_iLineParamQty;
  return m_apcLineParameters;
}

const char* CloudClientApplicationConfiguration::getCommandLineParameter(int nPos)
{
  if( nPos >= 0 && nPos < m_iLineParamQty )
  {
    return m_apcLineParameters[nPos];
  }

  return NULL;
}

const char* CloudClientApplicationConfiguration::getFoo(int& iFoo)
{
  iFoo = m_iFoo;
  return m_pcFooName;
}

const char* CloudClientApplicationConfiguration::getDefaultFoo(int& iFoo)
{
  iFoo = mst_CiFoo;
  return mst_CpcFooName;
}

void CloudClientApplicationConfiguration::setCommandLineParameters(int nParams, const char* *apcParameters)
{
  if( nParams > 0 && apcParameters != NULL )
  {
    m_iLineParamQty = nParams;
    m_apcLineParameters = apcParameters;
  }
}


void CloudClientApplicationConfiguration::setCommandLineParameter(int nPos, const char* pcParameter)
{
  if( nPos >= 0 && nPos < m_iLineParamQty )
  {
    m_apcLineParameters[nPos] = pcParameter;
  }
}


void CloudClientApplicationConfiguration::setFoo(int iFoo, const char* pcFooName)
{
  m_iFoo = iFoo;
  m_pcFooName = pcFooName;
}

bool CloudClientApplicationConfiguration::loadSiteRegistrationConfig(const char* filename)
{
    using boost::property_tree::ptree;

    printf("\nDEBUG: loadSiteRegistrationConfig. Reading xml data. (%s) and Checking the mandatories fields.\n", filename);
    try
    {
		read_xml(filename, m_siteRegistrationDataTree);

		printf(" getting DeviceRegistrationRequest\n");
		ptree request = m_siteRegistrationDataTree.get_child(CDeviceRegistrationRequestConfigKey.c_str());

		//LOAD DEVICE DATA
		ptree device = request.get_child(CRegisterDeviceConfigKey.c_str());

		std::string data = device.get<std::string>(CRegistrationCodeConfigKey.c_str());
		printf(" registerDevice:%s. %s\n", CRegistrationCodeConfigKey.c_str(), data.c_str());

		data = device.get<std::string>(CCertFormatConfigKey.c_str());
		printf(" registerDevice:certFormat. %s\n", data.c_str());

		data = device.get<std::string>(CDnsSupportedConfigKey.c_str());
		printf(" registerDevice:dnsSupported. %s\n", data.c_str());

		//LOAD CONFIG DATA
		ptree config = request.get_child(CDeviceConfigConfigKey.c_str());
		data = config.get<std::string>(CDeviceTypeConfigKey.c_str());
		printf(" deviceConfig:deviceType. %s\n", data.c_str());

		data = config.get<std::string>(CLocalIdConfigKey.c_str());
		printf(" deviceConfig:localId. %s\n", data.c_str());
			//LOAD ASSET INFO IN CONFIG DATA
		ptree assetInfo = config.get_child(CAssetInfoConfigKey.c_str());
		data = assetInfo.get<std::string>(CManufacturerConfigKey.c_str());
		printf(" deviceConfig:assetInfo:manufacturer. %s\n", data.c_str());

		data = assetInfo.get<std::string>(CModelConfigKey.c_str());
		printf(" deviceConfig:assetInfo:model. %s\n", data.c_str());
		data = assetInfo.get<std::string>(CSerialNbrConfigKey.c_str());
		printf(" deviceConfig:assetInfo:serialNbr. %s\n", data.c_str());
		data = assetInfo.get<std::string>(CFirmwareConfigKey.c_str());
		printf(" deviceConfig:assetInfo:firmware. %s\n", data.c_str());
		data = assetInfo.get<std::string>(CComponentsConfigKey.c_str());
		printf(" deviceConfig:assetInfo:components. %s\n", data.c_str());

		ptree events = config.get_child(CEventsKey.c_str());
		data = events.get_child("event").get<std::string>(CEventNameConfigKey.c_str()); //TODO manage array of events
		printf(" deviceConfig:assetInfo:events:event:eventName. %s\n", data.c_str());
		data = events.get_child("event").get<std::string>(CPayloadConfigKey.c_str());
		printf(" deviceConfig:assetInfo:events:event:payload. %s\n", data.c_str());

		ptree cmds = config.get_child(CMSDSConfigKey.c_str());
		ptree cmdKey = cmds.get_child(CCmdConfigKey.c_str());
		data = cmdKey.get<std::string>(CCmdNameConfigKey.c_str());
		printf(" deviceConfig:assetInfo:cmds:cmd:cmdName. %s\n", data.c_str());
		data = cmdKey.get<std::string>(CCmdRequestConfigKey.c_str());
		printf(" deviceConfig:assetInfo:cmds:cmd:request. %s\n", data.c_str());
		data = cmdKey.get<std::string>(CCmdResponseConfigKey.c_str());
		printf(" deviceConfig:assetInfo:cmds:cmd:response. %s\n", data.c_str());

		ptree associated = config.get_child(CAssociatedConfigKey.c_str());
    }
    catch (std::exception& e)
    {
      std::cerr << "\nERROR LoadSiteRegistrationConfig exception: " << e.what() << "\n";
      return false;
    }
    return true;
}

