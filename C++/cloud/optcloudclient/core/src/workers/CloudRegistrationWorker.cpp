/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudRegistrationWorker.cpp
 * @author mvolker
 * @date Thursday, April 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <workers/CloudRegistrationWorker.h>

// Spot Messages...
# include <gvr/communication/spot/SpotMessageHardwareInfoResponse.h>
# include <gvr/communication/spot/SpotMessageSoftwareInfoResponse.h>
# include <gvr/communication/spot/SpotMessageDebianInfoResponse.h>
# include <gvr/communication/spot/SpotMessageOPTHwrInfo.h>
# include <gvr/communication/spot/SpotMessageOPTHwrInfoResponse.h>
# include <gvr/communication/spot/SpotMessageOptSwInfo.h>
# include <gvr/communication/spot/SpotMessageOptSwInfoResponse.h>

// Crl Messages...
# include <gvr/communication/crl/message/CrlMessageRegistration.h>
# include <gvr/communication/crl/message/CrlMessageRegistrationResponse.h>

// Ucc Messages...
# include <gvr/communication/ucc/message/UccMessageRegisterRequest.h>
# include <gvr/communication/ucc/message/UccMessageRegisterResponse.h>
# include <gvr/communication/ucc/message/UccMessageServerUpdate.h>
# include <gvr/communication/ucc/message/UccMessageServerUpdateResponse.h>
# include <gvr/communication/ucc/message/UccMessageConnect.h>
# include <gvr/communication/ucc/message/UccMessageConnectResponse.h>
# include <DeviceInformation.h>

# include <EncryptData.h>

# include <Poco/Timestamp.h>
# include <Poco/File.h>
# include <Poco/Util/PropertyFileConfiguration.h>
# include <Poco/Util/IniFileConfiguration.h>
# include <Poco/AutoPtr.h>

# include <gvrlog/log.h>
# include <gvr/communication/spot/Utils.h>

# include <sstream>
# include <fstream>
# include <iterator>

using Poco::Util::PropertyFileConfiguration;
using Poco::Util::AbstractConfiguration;
using Poco::Util::IniFileConfiguration;
using Poco::File;
using Poco::AutoPtr;

DECLARE_LOG_MODULE(core);


/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    CloudRegistrationWorker::CloudRegistrationWorker( UccSession &uccSession, SpotSession &spotSession, CrlSessionServer &crlSession, CloudStatus & cloudStatus, CloudConfigDeviceChecker & configChecker ):
        AbsCrlWorker("CloudRegistrationWorker", crlSession),
        m_uccSession(uccSession),
        m_spotSession(spotSession),
        m_crlSession(crlSession),
        m_cloudStatus(cloudStatus),
        m_configChecker(configChecker)
    {
      start();

      LOG_Debug(core, "Starting Cloud Registration Worker...");
    }

    CloudRegistrationWorker::~CloudRegistrationWorker()
    {
      stop();

      LOG_Debug(core, "Stopping Cloud Registration Worker...");
    }

    void CloudRegistrationWorker::sessionEvent( ICrlSessionListener::BufferType const & buffer, ICrlSessionListener::TypeId const & typeId, ICrlSessionListener::KeyType const & keyType)
    {
      LOG_Trace(core, "Enqueue a new CRL message notification.");

      enqueueNotification ( typeId, buffer );
    }

    void CloudRegistrationWorker::sessionEvent( ICrlSessionListener::StatusType status )
    {
      LOG_Trace(core, "Enqueue a new CRL session status notification.");

      enqueueNotification ( status );
    }

    void CloudRegistrationWorker::doSubscriptions( )
    {
      LOG_Debug(core, "Subscribing to CRL reply messages.");

      m_crlSession.subscribe(*this, CrlMessageRegistration::GetTypeId());
    }

    void CloudRegistrationWorker::doUnsubscriptions( )
    {
      LOG_Debug(core, "Un-subscribing to CRL request messages.");

      m_crlSession.unsubscribe(*this);
    }

    void CloudRegistrationWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(core, "Processing event message type notification.");

      if( CrlMessageRegistration::GetTypeId() == tId )
      {
        CrlMessageRegistration msg;
        if (msg.parse(data))
        {
          std::string registrationCode;
          std::string localId;

          {
            gvr::utils::Json json(msg.getPayload());
            registrationCode = json.get(CrlMessageRegistration::CRegistrationCodeName).asString();
            localId = json.get(CrlMessageRegistration::CLocalIdName).asString();
          }

          doRegistration(registrationCode,localId);
        }
        else
        {
          LOG_Warn(core, "Message received is a registration request invalid message. Discarded.");
        }
      }
      else
      {
        LOG_Warn(core, "Message received has unexpected type ID. Discarded.");
      }
    }

    void CloudRegistrationWorker::processNotification( StatusType status )
    {
      LOG_VDebug(core, "New status event has been received from CrlSession [%s].\n", m_crlSession.getStatusDescription().c_str());
    }

    void CloudRegistrationWorker::doRegistration( std::string& registrationCode, std::string& localId )
    {
      if( !populateAndSendUccRegistrationServerUpdateMessage() )
      {
        LOG_Error(core, "Error populating and send UccMessageServerUpdateMessage");
        sendGenericFailResponse();
        return;
      }

      UccMessageRegisterResponse msgUccResp;
      if( !populateAndSendUccRegisterRequestMessage(registrationCode, localId, msgUccResp) )
      {
        LOG_Error(core, "Error populating and send UccRegisterRequestMessage");
        sendGenericFailResponse();
        return;
      }

      CrlMessageRegistrationResponse msgCrlResponse;
      if( !populateAndSendCrlResponse(localId, msgUccResp, msgCrlResponse ) )
      {
        LOG_VCritical(core, "UccSession Error: [%s], sending a message.", m_uccSession.getErrorDescription().c_str());

        sendGenericFailResponse( );
        return;
      }
    }

    bool CloudRegistrationWorker::populateAndSendCrlResponse( std::string const & localId, const UccMessageRegisterResponse& msgUccResp, CrlMessageRegistrationResponse& msgCrlResponse )
    {
      gvr::utils::Json jsonCrlMsgRs(msgCrlResponse.getJson());
      gvr::utils::Json json(msgUccResp.getJson());

      if( json.get(UccMessageRegisterResponse::CSuccessName).asBoolean() )
      {
        if( !storeUccRegistrationResponseOk(json) )
        {
          LOG_Error(core, "UccSession Error: Error storing ucc registration response message");

          return false;
        }

        m_cloudStatus.setRegisteredDeviceId( json.get(UccMessageRegisterResponse::CConfigName).
                                                  get(UccMessageRegisterResponse::CDeviceIdName).asString(), localId );

        m_configChecker.restart();

        jsonCrlMsgRs.set(CrlMessageRegistrationResponse::CPayloadName).
                     set(CrlMessageRegistrationResponse::CSuccessName) = CrlMessageRegistrationResponse::CSuccessTrue;
      }
      else
      {
        std::string errorCode;
        std::string errorMessage;

        if (!json.get(UccMessageRegisterResponse::CErrorNumberName).isNull())
        {
          {
            std::ostringstream oss;
            oss << json.get(UccMessageRegisterResponse::CErrorNumberName).asInteger();
            errorCode = oss.str();
          }
          errorMessage = json.get(UccMessageRegisterResponse::CMessageName).asString();
          /*
           * TODO: we need to persist this information ...
           *
          json.get(UccMessageRegisterResponse::CCodeName).asString();
          */
        }
        else
        {
          std::string errorCode;
          {
            std::ostringstream oss;
            oss << json.get(UccMessageRegisterResponse::CUccCodeName).asInteger();
            errorCode = oss.str();
          }
          errorMessage = json.get(UccMessageRegisterResponse::CUccMessageName).asString();
        }

        m_cloudStatus.setUnregistered( errorCode, errorMessage );

        jsonCrlMsgRs.set(CrlMessageRegistrationResponse::CPayloadName).
                     set(CrlMessageRegistrationResponse::CSuccessName) = CrlMessageRegistrationResponse::CSuccessFalse;

        jsonCrlMsgRs.set(CrlMessageRegistrationResponse::CPayloadName).
                     set(CrlMessageRegistrationResponse::CErrorCodeName) = errorCode;

        jsonCrlMsgRs.set(CrlMessageRegistrationResponse::CPayloadName).
                     set(CrlMessageRegistrationResponse::CErrorMessageName) = errorMessage;
      }

      msgCrlResponse.setJson(jsonCrlMsgRs);
      if( !msgCrlResponse.validate() )
      {
        LOG_Error(core, "CrlSession Error: Error validation crl response message");

        std::string collectedInfo;
        msgCrlResponse.getJson().serialize(collectedInfo);
        LOG_VDebug(core, "CrlSession collected information in response is [\n%s\n].",collectedInfo.c_str());

        return false;
      }

      m_crlSession.send(msgCrlResponse);
      if( m_crlSession.getError() != CrlSession::CrlSessionError::CNoError )
      {
        LOG_VCritical(core, "CrlSession Error: [%s], sending a message.", m_crlSession.getErrorDescription().c_str());

        return false;
      }

      LOG_Debug(core, "The registration response has been sent to CRL-Entity.");

      return true;
    }

    bool CloudRegistrationWorker::storeUccRegistrationResponseOk( const gvr::utils::Json& json )
    {
      std::string pathCloudServerDataFile;

      m_cloudStatus.getPathCloudServerDataFile(pathCloudServerDataFile);

      try
      {
        EncryptData encryptData(m_spotSession);

        if( m_spotSession.getDevicePlatform() != SpotSession::eM3 )
        {
          if( !encryptData.generateKey() )
          {
            LOG_Error(core, "Fail to generate SPOT key to encrypt data.");
            return false;
          }
        }

        int gvrSiteId           = json.get(UccMessageRegisterResponse::CConfigName).get(UccMessageRegisterResponse::CGvrSiteIdName).asInteger();
        std::string deviceId    = json.get(UccMessageRegisterResponse::CConfigName).get(UccMessageRegisterResponse::CDeviceIdName).asString();
        std::string url         = json.get(UccMessageRegisterResponse::CConfigName).get(UccMessageRegisterResponse::CConnectName).get(UccMessageRegisterResponse::CUriName).asString();
        std::string certFormat  = json.get(UccMessageRegisterResponse::CConfigName).get(UccMessageRegisterResponse::CConnectName).get(UccMessageRegisterResponse::CCertFormatName).asString();
        std::string certPwd     = json.get(UccMessageRegisterResponse::CConfigName).get(UccMessageRegisterResponse::CConnectName).get(UccMessageRegisterResponse::CCertName).asString();
        std::string user        = json.get(UccMessageRegisterResponse::CConfigName).get(UccMessageRegisterResponse::CConnectName).get(UccMessageRegisterResponse::CUserIdName).asString();
        std::string pwd         = json.get(UccMessageRegisterResponse::CConfigName).get(UccMessageRegisterResponse::CConnectName).get(UccMessageRegisterResponse::CPwdName).asString();

        std::stringstream ss;

        ss << UccMessageRegisterResponse::CDeviceIdName   << " : " << deviceId    << std::endl;
        ss << UccMessageRegisterResponse::CUriName        << " : " << url         << std::endl;
        ss << UccMessageRegisterResponse::CCertFormatName << " : " << certFormat  << std::endl;
        ss << UccMessageRegisterResponse::CCertName       << " : " << certPwd     << std::endl;
        ss << UccMessageRegisterResponse::CUserIdName     << " : " << user        << std::endl;
        ss << UccMessageRegisterResponse::CPwdName        << " : " << pwd         << std::endl;
        ss << UccMessageRegisterResponse::CGvrSiteIdName  << " : " << gvrSiteId   << std::endl;

        std::string strDataToEncrypt(ss.str());


        std::vector<unsigned char> encryptedBuffer;
        if( m_spotSession.getDevicePlatform() != SpotSession::eM3 )
        {
          if( !encryptData.encrypt( strDataToEncrypt, encryptedBuffer) )
          {
            LOG_Error(core, "Fail to encrypt data.");
            return false;
          }
        }
        else
        {
          std::copy(strDataToEncrypt.begin(), strDataToEncrypt.end(), back_inserter(encryptedBuffer));
        }

        std::ofstream binaryFile(pathCloudServerDataFile.c_str(), std::ios::out | std::ofstream::binary | std::ofstream::trunc);

        if( !binaryFile.is_open() )
        {
          LOG_VError(core, "Could not create binary file [%s]\n", pathCloudServerDataFile.c_str());
          return false;
        }

        std::copy(encryptedBuffer.begin(), encryptedBuffer.end(), std::ostreambuf_iterator<char>(binaryFile));

        {
          std::string collectedInfo;
          json.serialize(collectedInfo);
          LOG_VDebug(core, "Collected information from UccMessageRegisterResponse is [\n%s\n].",collectedInfo.c_str());
        }

        return true;
      }
      catch(...)
      {
        LOG_VError(core, "CloudRegistrationWorker cannot create data file [%s] to store UccMessageRegisterResponse.", pathCloudServerDataFile.c_str());
      }

      return false;
    }

    bool CloudRegistrationWorker::populateAndSendUccRegistrationServerUpdateMessage( )
    {
      UccMessageServerUpdate msgUccMessageServerUpdate;

      std::string pathRegistrationServerDataFile;

      m_cloudStatus.getPathRegistrationServerDataFile(pathRegistrationServerDataFile);

      // Load configuration from .ini file type.
      AutoPtr<IniFileConfiguration> pConfig;
      try
      {
        pConfig = new IniFileConfiguration(pathRegistrationServerDataFile);

        std::string url         = pConfig->getString("url", "");
        std::string certFormat  = pConfig->getString("certFormat", "");
        std::string certPwd     = pConfig->getString("certPwd", "");
        std::string user        = pConfig->getString("user", "");
        std::string pwd         = pConfig->getString("pwd", "");
        std::string cert        = pConfig->getString("cert", "");

        std::string dnsServerName1;
        std::string dnsServerName2;
        m_cloudStatus.getDnsLocalConfiguration(dnsServerName1, dnsServerName2);

        msgUccMessageServerUpdate.generateMessageId();
        msgUccMessageServerUpdate.setDeviceId(m_cloudStatus.getDeviceId());

        gvr::utils::Json json(msgUccMessageServerUpdate.getJson());

        json.set(UccMessageServerUpdate::CPayloadName).set(UccMessageServerUpdate::CUrlName)        = url;
        json.set(UccMessageServerUpdate::CPayloadName).set(UccMessageServerUpdate::CDnsServer1Name) = dnsServerName1;
        json.set(UccMessageServerUpdate::CPayloadName).set(UccMessageServerUpdate::CDnsServer2Name) = dnsServerName2;
        json.set(UccMessageServerUpdate::CPayloadName).set(UccMessageServerUpdate::CCertName)       = cert;
        json.set(UccMessageServerUpdate::CPayloadName).set(UccMessageServerUpdate::CCertFormatName) = certFormat;
        json.set(UccMessageServerUpdate::CPayloadName).set(UccMessageServerUpdate::CCertPwdName)    = certPwd;
        json.set(UccMessageServerUpdate::CPayloadName).set(UccMessageServerUpdate::CUserName)       = user;
        json.set(UccMessageServerUpdate::CPayloadName).set(UccMessageServerUpdate::CPwdName)        = pwd;

        msgUccMessageServerUpdate.setJson(json);
        if( !msgUccMessageServerUpdate.validate() )
        {
          LOG_VCritical(core, "Error validation UccRegistrationServerUpdateMessage");
          return false;
        }

        {
          std::string data;
          msgUccMessageServerUpdate.getPayload().serialize(data);
          LOG_VDebug(core, "Sending the UccMessageServerUpdate data [\n%s\n].",data.c_str());
        }


        UccMessageServerUpdateResponse msgUccMessageServerUpdateResp;
        msgUccMessageServerUpdateResp.setMessageId(msgUccMessageServerUpdate.getMessageId());
        m_uccSession.sendSync(msgUccMessageServerUpdate, msgUccMessageServerUpdateResp);
        if( m_uccSession.getError() != UccSession::UccSessionError::CNoError )
        {
          LOG_VCritical(core, "UccSession Error: [%s], sending a message.", m_uccSession.getErrorDescription().c_str());
          sendGenericFailResponse( );
          return false;
        }

        return true;
      }
      catch(...)
      {
        LOG_VError(core, "CloudRegistrationWorker cannot open configuration file [%s] to populate UccRegistrationServerUpdateMessage.", pathRegistrationServerDataFile.c_str());
      }

      return false;
    }

    bool CloudRegistrationWorker::populateAndSendUccRegisterRequestMessage( const std::string& registrationCode, const std::string& localId, UccMessageRegisterResponse& msgUccResp )
    {
      UccMessageRegisterRequest msgUccRequest;
      gvr::utils::Json jsonRegInfo;
      if( !collectRegistrationInformation(jsonRegInfo, registrationCode, localId) )
      {
        LOG_Error(core, "Error recollecting spot information");
        sendGenericFailResponse( );
        return false;
      }

      {
        std::string collectedInfo;
        jsonRegInfo.serialize(collectedInfo);
        LOG_VDebug(core, "Collected information for UccMessageRegisterRequest is [\n%s\n].",collectedInfo.c_str());
      }

      msgUccRequest.setJson(jsonRegInfo);
      if( !msgUccRequest.validate() )
      {
        LOG_Error(core, "Error serializing ucc request message");
        sendGenericFailResponse( );
        return false;
      }

      m_uccSession.sendSync(msgUccRequest, msgUccResp);
      if( m_uccSession.getError() != UccSession::UccSessionError::CNoError )
      {
        LOG_VCritical(core, "UccSession Error: [%s], sending a message.", m_uccSession.getErrorDescription().c_str());

        sendGenericFailResponse( );
        return false;
      }

      return true;
    }

    void CloudRegistrationWorker::sendGenericFailResponse(  )
    {
      static char const * CInternalErrorCode = "-1";
      static char const * CInternalErrorMessage = "Internal PH Error";

      CrlMessageRegistrationResponse msgCrlResponse;
      gvr::utils::Json jsonCrlMsgRs(msgCrlResponse.getJson());

      jsonCrlMsgRs.set(CrlMessageRegistrationResponse::CPayloadName).
                   set(CrlMessageRegistrationResponse::CSuccessName) = CrlMessageRegistrationResponse::CSuccessFalse;

      jsonCrlMsgRs.set(CrlMessageRegistrationResponse::CPayloadName).
                   set(CrlMessageRegistrationResponse::CErrorCodeName) = CInternalErrorCode;

      jsonCrlMsgRs.set(CrlMessageRegistrationResponse::CPayloadName).
                   set(CrlMessageRegistrationResponse::CErrorMessageName) = CInternalErrorMessage;

      msgCrlResponse.setJson(jsonCrlMsgRs);
      if( !msgCrlResponse.validate() )
      {
        LOG_VCritical(core, "Error validation crl generic fail response message");
        return;
      }

      m_crlSession.send(msgCrlResponse);
      if( m_crlSession.getError() != CrlSession::CrlSessionError::CNoError )
      {
        LOG_VCritical(core, "CrlSession Error: [%s], sending a generic fail message response.", m_crlSession.getErrorDescription().c_str());
        return;
      }
    }

    bool CloudRegistrationWorker::collectRegistrationInformation( gvr::utils::Json & json, const std::string& registrationCode, const std::string& localId )
    {
      std::string dnsServerName1;
      std::string dnsServerName2;
      m_cloudStatus.getDnsLocalConfiguration(dnsServerName1, dnsServerName2);
      bool dnsSupported = (!dnsServerName1.empty()) || (!dnsServerName2.empty());

      DeviceInformation deviceInformation;
      return deviceInformation.collect(json,m_spotSession,localId,registrationCode, DeviceInformation::CSpotDefaultCertFormat, dnsSupported);
    }
  }
}
