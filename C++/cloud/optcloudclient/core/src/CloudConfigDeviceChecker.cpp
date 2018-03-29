/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudConfigDeviceChecker.cpp
 * @author mvolker
 * @date Tuesday, April 14, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <CloudConfigDeviceChecker.h>

#include <gvrlog/log.h>

#include <DeviceInformation.h>
#include <gvr/communication/ucc/message/UccMessageConnect.h>
#include <gvr/communication/ucc/message/UccMessageConnectResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudDeviceConfigChange.h>
#include <gvr/communication/ucc/message/UccMessageCloudDeviceConfigChangeResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudInstallStatus.h>
#include <gvr/communication/ucc/message/UccMessageRegisterResponse.h>

#include <workers/OptCloudClientWorkerFileActivation.h>

#include <EncryptData.h>

#include <Poco/Util/PropertyFileConfiguration.h>
#include <Poco/AutoPtr.h>

#include <sys/stat.h>
#include <fstream>

DECLARE_LOG_MODULE(core);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Cloud Domain
  namespace cloud
  {
    CloudConfigDeviceChecker::~CloudConfigDeviceChecker (void)
    {
      private_safe_stopCheckCycle();
    }

    CloudConfigDeviceChecker::CloudConfigDeviceChecker (TimeMillisecondsType milliseconds, UccSession & uccSession, gvr::communication::spot::SpotSession & spotSession, CloudStatus & cloudStatus)
     : m_mtx(),
       m_checking(false),
       m_cycleTimeout(milliseconds),
       m_timer(0,0),
       m_uccSession(uccSession),
       m_spotSession(spotSession),
       m_cloudStatus(cloudStatus)
    {
    }

    bool CloudConfigDeviceChecker::isChecking  (void) const
    {
      ScopedLock guard(const_cast<CloudConfigDeviceChecker*>(this)->m_mtx);
      return m_checking;
    }

    void CloudConfigDeviceChecker::start (void)
    {
      private_safe_startCheckCycle();
    }

    void CloudConfigDeviceChecker::stop (void)
    {
      private_safe_stopCheckCycle();
    }

    void CloudConfigDeviceChecker::restart (void)
    {
      private_safe_stopCheckCycle();
      private_safe_startCheckCycle();
    }

    void CloudConfigDeviceChecker::private_safe_startCheckCycle (void)
    {
      ScopedLock guard(m_mtx);
      if (m_checking == false)
      {
        m_checking = true;
        m_timer.stop();
        m_timer.start(Poco::TimerCallback<CloudConfigDeviceChecker>(*this,&CloudConfigDeviceChecker::private_safe_callbackForTimer));
      }
    }

    void CloudConfigDeviceChecker::private_safe_stopCheckCycle (void)
    {
      ScopedLock guard(m_mtx);
      if (m_checking == true)
      {
        m_timer.restart(0);
        m_checking = false;
      }
    }

    bool CloudConfigDeviceChecker::private_unsafe_doCheck (void)
    {
      static char const * CLocalId = "";
      bool result = false;
      if (m_cloudStatus.isRegistered())
      {
        if (m_cloudStatus.isConnected() == false)
        {
          bool doConnect = false;
          UccMessageConnect msgUccMessageConnect;
          {
            std::string pathCloudServerDataFile;
            m_cloudStatus.getPathCloudServerDataFile(pathCloudServerDataFile);
            if (pathCloudServerDataFile.empty() == false)
            {
              msgUccMessageConnect.setDeviceId(m_cloudStatus.getDeviceId());
              msgUccMessageConnect.generateMessageId();
              try
              {
                std::string strDecryptedData;
                {
                  EncryptData encryptData(m_spotSession);
                  encryptData.decryptConnectionData(pathCloudServerDataFile,strDecryptedData);
                }

                if (strDecryptedData.empty() == false)
                {
                  std::istringstream is(strDecryptedData);

                  // Load configuration from .ini file type.
                  Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConfig= new Poco::Util::PropertyFileConfiguration(is);

                  std::string url         = pConfig->getString(UccMessageRegisterResponse::CUriName       , "");
                  std::string certFormat  = pConfig->getString(UccMessageRegisterResponse::CCertFormatName, "");
                  std::string certType    = pConfig->getString(UccMessageRegisterResponse::CCertName      , ""); // Please check, maybe is wrong ...
                  std::string user        = pConfig->getString(UccMessageRegisterResponse::CUserIdName    , "");
                  std::string pwd         = pConfig->getString(UccMessageRegisterResponse::CPwdName       , "");

                  {
                    std::string dnsServerName1;
                    std::string dnsServerName2;
                    m_cloudStatus.getDnsLocalConfiguration(dnsServerName1, dnsServerName2);

                    gvr::utils::Json payload;
                    payload.set(UccMessageConnect::CUriName)         = url;
                    payload.set(UccMessageConnect::CNameServer1Name) = dnsServerName1;
                    payload.set(UccMessageConnect::CNameServer2Name) = dnsServerName2;
                    payload.set(UccMessageConnect::CCertFormatName)  = certFormat;
                    payload.set(UccMessageConnect::CCertTypeName)    = certType;
                    payload.set(UccMessageConnect::CUseridName)      = user;
                    payload.set(UccMessageConnect::CPwdName)         = pwd;

                    {
                      std::string collectedInfo;
                      payload.serialize(collectedInfo);
                      LOG_VDebug(core, "Collected information for UccMessageConnect is [\n%s\n].",collectedInfo.c_str());
                    }
                    msgUccMessageConnect.setPayload(payload);
                    doConnect = true;
                  }
                }
              }
              catch(...)
              {
                LOG_VError(core, "CloudConfigDeviceChecker cannot open configuration file [%s] to populate UccMessageConnect.", pathCloudServerDataFile.c_str());
              }
            }
            else
            {
              LOG_Error(core, "CloudConfigDeviceChecker cannot open missing configuration file name to populate UccMessageConnect.");
            }
          }

          if (doConnect && (m_cloudStatus.isConnected() == false)) // Ask again to narrow the network race condition.
          {
            UccMessageConnectResponse msgUccMessageConnectResp;
            msgUccMessageConnectResp.setMessageId(msgUccMessageConnect.getMessageId());
            m_uccSession.sendSync(msgUccMessageConnect, msgUccMessageConnectResp);
            if( m_uccSession.getError() != UccSession::UccSessionError::CNoError )
            {
              m_cloudStatus.setDisconnected();
              LOG_VCritical(core, "UccSession Error: [%s], sending a message [UccMessageConnect].", m_uccSession.getErrorDescription().c_str());
            }
            else
            {
              {
                gvr::utils::Json payload = msgUccMessageConnectResp.getJson();
                std::string reveivedInfo;
                payload.serialize(reveivedInfo);
                LOG_VDebug(core, "Received information for UccMessageConnectResponse is [\n%s\n].",reveivedInfo.c_str());
              }

              gvr::utils::Json json(msgUccMessageConnectResp.getPayload());
              if (json.get(UccMessageConnectResponse::CSuccessName).asBoolean())
              {
                LOG_Debug(core, "UccMessageConnect has been received OK by the UCC.");
                // Connect response does not indicate connection status. It only means the UCC received and process the message.
              }
              else
              {
                std::string errorCode(json.get(UccMessageConnectResponse::CCodeName).asString());
                std::string errorMessage(json.get(UccMessageConnectResponse::CMessageName).asString());
                m_cloudStatus.setDisconnected(errorCode,errorMessage);
              }
            }
          }
        }
        else
        {
          if ((private_hasBeenPackageActivationPerformed() == false) ||
              (private_notifyCloudInstallationFinished()))
          {
            bool collected = false;
            UccMessageCloudDeviceConfigChange msgUccDeviceConfigChange;
            {
              gvr::utils::Json payload;
              gvr::cloud::DeviceInformation deviceInfo;
              if( deviceInfo.collect(payload,m_spotSession,m_cloudStatus.getLocalId()) )
              {
                msgUccDeviceConfigChange.setPayload(payload);
                collected = true;
              }
            }

            if (collected)
            {
              UccMessageCloudDeviceConfigChangeResponse msgUccDeviceConfigChangeResp;
              msgUccDeviceConfigChange.setDeviceId(m_cloudStatus.getDeviceId());
              msgUccDeviceConfigChange.generateMessageId();
              msgUccDeviceConfigChangeResp.setMessageId(msgUccDeviceConfigChange.getMessageId());

              {
                gvr::utils::Json payload = msgUccDeviceConfigChange.getPayload();
                std::string collectedInfo;
                payload.serialize(collectedInfo);
                LOG_VDebug(core, "Collected information for UccMessageCloudDeviceConfigChange is [\n%s\n].",collectedInfo.c_str());
              }

              m_uccSession.sendSync(msgUccDeviceConfigChange,msgUccDeviceConfigChangeResp);

              if (m_uccSession.getError() != UccSession::UccSessionError::CNoError)
              {
                LOG_VCritical(core, "UccSession Error: [%s], sending a message [UccMessageCloudDeviceConfigChange].", m_uccSession.getErrorDescription().c_str());
              }
              else
              {
                {
                  gvr::utils::Json payload = msgUccDeviceConfigChangeResp.getJson();
                  std::string receivedInfo;
                  payload.serialize(receivedInfo);
                  LOG_VDebug(core, "Received information for UccMessageCloudDeviceConfigChangeResponse is [\n%s\n].",receivedInfo.c_str());
                }

                result = true;
              }
            }
          }
        }
      }
      return result;
    }

    void CloudConfigDeviceChecker::private_unsafe_pauseTimer (void)
    {
      m_timer.restart(0);
    }

    void CloudConfigDeviceChecker::private_unsafe_resumeTimer (TimeMillisecondsType milliseconds)
    {
      m_timer.restart(milliseconds);
    }

    void CloudConfigDeviceChecker::private_unsafe_resetTimerForCycle (void)
    {
      private_unsafe_resumeTimer(m_cycleTimeout);
    }

    void CloudConfigDeviceChecker::private_safe_callbackForTimer (Poco::Timer & timer)
    {
      ScopedLock guard(m_mtx);
      LOG_Debug(core, "CloudConfigDeviceChecker::private_unsafe_doCheck - Calling...");
      if (private_unsafe_doCheck() == false)
      {
        LOG_Debug(core, "CloudConfigDeviceChecker::private_unsafe_doCheck - Failed.");
        private_unsafe_resetTimerForCycle();
        LOG_Debug(core, "CloudConfigDeviceChecker::private_unsafe_resetTimerForCycle - Done.");
      }
      else
      {
        LOG_Debug(core, "CloudConfigDeviceChecker::private_unsafe_doCheck - Succeeded.");
        m_checking = false;
        private_unsafe_pauseTimer();
      }
    }

    bool CloudConfigDeviceChecker::private_hasBeenPackageActivationPerformed (void) const
    {
      return OptCloudClientWorkerFileActivation::hasBeenPackageActivationPerformed();
    }

    bool CloudConfigDeviceChecker::private_notifyCloudInstallationFinished (void) const
    {
      return OptCloudClientWorkerFileActivation::notifyCloudInstallationFinished(m_uccSession);
    }
  }
}
