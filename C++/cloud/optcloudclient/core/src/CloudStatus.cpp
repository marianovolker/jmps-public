/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CrlManager.cpp
 * @author mvolker
 * @date Tuesday, April 14, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <CloudStatus.h>

#include <gvrlog/log.h>

#include <fstream>

#include <Poco/Util/PropertyFileConfiguration.h>

DECLARE_LOG_MODULE(core);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Cloud Domain
  namespace cloud
  {
    char const * CloudStatus::CDefaultDeviceId = "0";
    char const * CloudStatus::CDefaultLocalId = "0";
    char const * CloudStatus::CUnregisteredDeviceId = CloudStatus::CDefaultDeviceId;
    char const * CloudStatus::CUnregisteredLocalId = CloudStatus::CDefaultLocalId;
    char const * CloudStatus::CRegistrationErrorCodeNoError = "0";
    char const * CloudStatus::CConnectionErrorCodeNoError = "0";
    char const * CloudStatus::CEmptyString = "";
    char const * CloudStatus::CPathDeviceRegisterInfoFieldName = "PathDeviceRegisterInfo";
    char const * CloudStatus::CPathDeviceRegisterInfoFieldValueDefault = "./deviceRegisterInfo.ini";
    char const * CloudStatus::CDeviceIdFieldName = "DeviceId";
    char const * CloudStatus::CLocalIdFieldName = "LocalId";

    char const * CloudStatus::CDefaultReconnectionDelayValue = "60";

    CloudStatus::~CloudStatus (void)
    {
    }

    CloudStatus::CloudStatus (Poco::AutoPtr<Poco::Util::IniFileConfiguration> initConfigurationFile)
     : m_deviceId(CUnregisteredDeviceId),
       m_localId(CUnregisteredLocalId),
       m_RegistrationErrorCode(CRegistrationErrorCodeNoError),
       m_RegistrationErrorMessage(CEmptyString),
       m_connected(false),
       m_ConnectionErrorCode(CConnectionErrorCodeNoError),
       m_ConnectionErrorMessage(CEmptyString),
       m_initConfigurationFile(initConfigurationFile),
       m_operative(false)
    {
      private_unsafeLoadDeviceId(m_deviceId,m_localId);
    }

    std::string CloudStatus::getDeviceId  (void) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);
      return m_deviceId;
    }

    std::string CloudStatus::getLocalId  (void) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);
      return m_localId;
    }

    bool        CloudStatus::isRegistered (void) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);
      return ((m_deviceId.empty() == false) && (m_deviceId.compare(CUnregisteredDeviceId) != 0));
    }

    bool        CloudStatus::isConnected  (void) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);
      return m_connected;
    }

    bool        CloudStatus::isOperative  (void) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);
      return m_operative;
    }

    void CloudStatus::getRegistrationError (std::string & errorCode, std::string & errorMessage) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);
      errorCode = m_RegistrationErrorCode;
      errorMessage = m_RegistrationErrorMessage;
    }

    void CloudStatus::getConnectionError (std::string & errorCode, std::string & errorMessage) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);
      errorCode = m_ConnectionErrorCode;
      errorMessage = m_ConnectionErrorMessage;
    }

    void CloudStatus::getPathRegistrationServerDataFile (std::string & pathRegistrationServerDataFile) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);

      if( !m_initConfigurationFile.isNull() )
      {
        pathRegistrationServerDataFile = m_initConfigurationFile->getString("PathRegistrationServerData", "./registration_server_data.ini");
      }
    }

    void CloudStatus::getPathCloudServerDataFile (std::string & pathCloudServerDataFile) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);

      if( !m_initConfigurationFile.isNull() )
      {
        pathCloudServerDataFile = m_initConfigurationFile->getString("PathCloudServerData", "./cloud_server_data.ini");
      }
    }

    void CloudStatus::getPathNewCloudServerDataFile (std::string & pathNewCloudServerDataFile) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);

      if( !m_initConfigurationFile.isNull() )
      {
        pathNewCloudServerDataFile = m_initConfigurationFile->getString("PathNewCloudServerData", "./new_cloud_server_data.ini");
      }
    }

    void CloudStatus::getPathDeviceDnsConfiguration (std::string & pathDeviceDnsConfigurationFile) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);

      if( !m_initConfigurationFile.isNull() )
      {
        pathDeviceDnsConfigurationFile = m_initConfigurationFile->getString("PathDeviceDnsConfiguration", "./deviceDnsConfigurationInfo.ini");
      }
    }

    void CloudStatus::getReconnectionDelayValue (int & reConnectionDelayValue) const
    {
      ScopedLock lck(const_cast<CloudStatus*>(this)->m_mtx);

      if( !m_initConfigurationFile.isNull() )
      {
        reConnectionDelayValue = strtoul(m_initConfigurationFile->getString("ReConnectionDelayValue", CDefaultReconnectionDelayValue).c_str(), 0, 10);
      }
    }

    void CloudStatus::setRegisteredDeviceId (std::string const & deviceId)
    {
      ScopedLock lck(m_mtx);
      m_deviceId = deviceId;
      m_RegistrationErrorCode = CRegistrationErrorCodeNoError;
      m_RegistrationErrorMessage = CEmptyString;
      private_unsafeSaveDeviceId(m_deviceId,m_localId);
    }

    void CloudStatus::setRegisteredDeviceId (std::string const & deviceId, std::string const & localId)
    {
      ScopedLock lck(m_mtx);
      m_deviceId = deviceId;
      m_localId = localId;
      m_RegistrationErrorCode = CRegistrationErrorCodeNoError;
      m_RegistrationErrorMessage = CEmptyString;
      private_unsafeSaveDeviceId(m_deviceId,m_localId);
    }

    void CloudStatus::setUnregistered (void)
    {
      ScopedLock lck(m_mtx);
      m_deviceId = CUnregisteredDeviceId;
      private_unsafeSaveDeviceId(m_deviceId,m_localId);
    }

    void CloudStatus::setUnregistered (std::string const & errorCode, std::string const & errorMessage = CEmptyString)
    {
      ScopedLock lck(m_mtx);
      m_deviceId = CUnregisteredDeviceId;
      m_RegistrationErrorCode = errorCode;
      m_RegistrationErrorMessage = errorMessage;
      private_unsafeSaveDeviceId(m_deviceId,m_localId);
    }

    void CloudStatus::setConnected (void)
    {
      ScopedLock lck(m_mtx);
      m_connected = true;
      m_ConnectionErrorCode = CConnectionErrorCodeNoError;
      m_ConnectionErrorMessage = CEmptyString;
    }

    void CloudStatus::setDisconnected (void)
    {
      ScopedLock lck(m_mtx);
      m_connected = false;
    }

    void CloudStatus::setDisconnected (std::string const & errorCode, std::string const & errorMessage = CEmptyString)
    {
      ScopedLock lck(m_mtx);
      m_connected = false;
      m_ConnectionErrorCode = errorCode;
      m_ConnectionErrorMessage = errorMessage;
    }

    bool CloudStatus::testAndSetConnected (bool const & connected = true)
    {
      bool result = false;
      {
        ScopedLock lck(m_mtx);
        result = m_connected;
        m_connected = connected;
      }
      return result;
    }

    bool CloudStatus::testAndSetDisconnected (std::string const & errorCode, std::string const & errorMessage = CEmptyString)
    {
      bool result = false;
      {
        ScopedLock lck(m_mtx);
        result = m_connected;
        m_connected = false;
        m_ConnectionErrorCode = errorCode;
        m_ConnectionErrorMessage = errorMessage;
      }
      return result;
    }

    void CloudStatus::setOperative (bool operative)
    {
      ScopedLock lck(m_mtx);
      m_operative = operative;
    }

    void CloudStatus::private_unsafeSaveDeviceId (std::string const & deviceId, std::string const & localId) const
    {
      if( !m_initConfigurationFile.isNull() )
      {
        std::string pathDeviceRegisterInfo = m_initConfigurationFile->getString(CPathDeviceRegisterInfoFieldName,CPathDeviceRegisterInfoFieldValueDefault);
        if (pathDeviceRegisterInfo.empty() == false)
        {
          try
          {
            // Load configuration from .ini file type.
            Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConfig = new Poco::Util::PropertyFileConfiguration();
            if( !pConfig.isNull() )
            {
              pConfig->setString(CDeviceIdFieldName,deviceId);
              pConfig->setString(CLocalIdFieldName,localId);
              pConfig->save(pathDeviceRegisterInfo);
            }
          }
          catch(...)
          {
            LOG_VError(core, "CloudStatus cannot open configuration file [%s] to save Device Register Info.", pathDeviceRegisterInfo.c_str());
          }
        }
        else
        {
          LOG_Error(core, "CloudStatus cannot open missing configuration file name to save Device Register Info.");
        }
      }
    }

    void CloudStatus::private_unsafeLoadDeviceId (std::string & deviceId, std::string & localId) const
    {
      if( !m_initConfigurationFile.isNull() )
      {
        std::string pathDeviceRegisterInfo = m_initConfigurationFile->getString(CPathDeviceRegisterInfoFieldName,CPathDeviceRegisterInfoFieldValueDefault);
        if (pathDeviceRegisterInfo.empty() == false)
        {
          try
          {
            // Load configuration from .ini file type.
            Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConfig = new Poco::Util::PropertyFileConfiguration(pathDeviceRegisterInfo);
            if( !pConfig.isNull() )
            {
              deviceId = pConfig->getString(CDeviceIdFieldName,CUnregisteredDeviceId);
              localId = pConfig->getString(CLocalIdFieldName,CUnregisteredLocalId);
            }
          }
          catch(...)
          {
            LOG_VError(core, "CloudStatus cannot open configuration file [%s] to load Device Register Info.", pathDeviceRegisterInfo.c_str());
          }
        }
        else
        {
          LOG_Error(core, "CloudStatus cannot open missing configuration file name to load Device Register Info.");
        }
      }
    }

    void CloudStatus::getDnsLocalConfiguration(std::string & dnsServerName1, std::string & dnsServerName2) const
    {
      dnsServerName1 = "";
      dnsServerName2 = "";

      std::string dnsConfigFileName;
      getPathDeviceDnsConfiguration(dnsConfigFileName);
      std::ifstream dnsFile(dnsConfigFileName.c_str());
      if (dnsFile.is_open())
      {
        if (getline(dnsFile, dnsServerName1))
        {
          dnsServerName1.erase(std::remove(dnsServerName1.begin(), dnsServerName1.end(), '\n'), dnsServerName1.end());
          if (getline(dnsFile, dnsServerName2))
          {
            dnsServerName2.erase(std::remove(dnsServerName2.begin(), dnsServerName2.end(), '\n'), dnsServerName2.end());
          }
        }
      }
    }

  }
}
