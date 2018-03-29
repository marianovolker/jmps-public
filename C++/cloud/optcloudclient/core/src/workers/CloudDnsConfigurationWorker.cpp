/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudDnsConfigurationWorker.cpp
 * @author mgrieco
 * @date Wednesday, Jun 17, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <workers/CloudDnsConfigurationWorker.h>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <gvrlog/log.h>
DECLARE_LOG_MODULE(core);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    CloudDnsConfigurationWorker::CloudDnsConfigurationWorker( UccSession &uccSession, CrlSessionServer &crlSession, CloudStatus & cloudStatus ):
        AbsCrlWorker("CloudDnsConfigurationWorker", crlSession),
        m_uccSession(uccSession),
        m_crlSession(crlSession),
        m_cloudStatus(cloudStatus)
    {
      start();
      LOG_Debug(core, "Starting Cloud DnsConfiguration Worker...");
    }

    CloudDnsConfigurationWorker::~CloudDnsConfigurationWorker()
    {
      stop();
      LOG_Debug(core, "Stopping Cloud DnsConfiguration Worker...");
    }

    void CloudDnsConfigurationWorker::sessionEvent( ICrlSessionListener::BufferType const & buffer, ICrlSessionListener::TypeId const & typeId, ICrlSessionListener::KeyType const & keyType)
    {
      LOG_Trace(core, "Enqueue a new CRL message notification.");
      enqueueNotification ( typeId, buffer );
    }

    void CloudDnsConfigurationWorker::sessionEvent( ICrlSessionListener::StatusType status )
    {
      LOG_Trace(core, "Enqueue a new CRL session status notification.");
      enqueueNotification ( status );
    }

    void CloudDnsConfigurationWorker::doSubscriptions( )
    {
      LOG_Debug(core, "Subscribing to CRL reply messages.");

      m_crlSession.subscribe(*this, CrlMessageGetDnsConfiguration::GetTypeId());
      m_crlSession.subscribe(*this, CrlMessageSetDnsConfiguration::GetTypeId());
      m_crlSession.subscribe(*this, CrlMessageTestDns::GetTypeId());
    }

    void CloudDnsConfigurationWorker::doUnsubscriptions( )
    {
      LOG_Debug(core, "Un-subscribing to CRL request messages.");
      m_crlSession.unsubscribe(*this);
    }

    void CloudDnsConfigurationWorker::doGetDnsConfiguration(CrlMessageGetDnsConfiguration & msg)
    {
      std::string jsonMessage;
      {
        msg.getJson().serialize(jsonMessage);
        LOG_VNotice(core, "Received a get dns configuration command from the UCC [\n%s\n].\n", jsonMessage.c_str());
      }
      jsonMessage.clear();

      {
        CrlMessageGetDnsConfigurationResponse crlGetDnsConfigurationResponse;
        gvr::utils::Json payload;

        std::string dnsServerName1;
        std::string dnsServerName2;
        m_cloudStatus.getDnsLocalConfiguration(dnsServerName1, dnsServerName2);

        payload.set(CrlMessageGetDnsConfigurationResponse::CMessageServer1Name, dnsServerName1);
        payload.set(CrlMessageGetDnsConfigurationResponse::CMessageServer2Name, dnsServerName2);

        {
          std::string data;
          payload.serialize(data);
          LOG_VDebug(core, "Data for get dns configuration response is [\n%s\n].",data.c_str());
        }
        crlGetDnsConfigurationResponse.setPayload(payload);
        this->m_crlSession.send(crlGetDnsConfigurationResponse);
      }
    }

    void CloudDnsConfigurationWorker::doSetDnsConfiguration(CrlMessageSetDnsConfiguration & msg)
    {
      std::string jsonMessage;
      {
        msg.getJson().serialize(jsonMessage);
        LOG_VNotice(core, "Received a set dns configuration command from the CRL [\n%s\n].\n", jsonMessage.c_str());
      }

      jsonMessage.clear();
      {
        CrlMessageSetDnsConfigurationResponse crlSetDnsConfigurationResponse;
        gvr::utils::Json payload;

        std::string dnsConfigFilename;
        m_cloudStatus.getPathDeviceDnsConfiguration(dnsConfigFilename);
        ofstream dnsConfigFile (dnsConfigFilename.c_str());
        if (dnsConfigFile.is_open())
        {
          dnsConfigFile << msg.getPayload().get(CrlMessageSetDnsConfiguration::CMessageServer1Name).asString() << std::endl;
          dnsConfigFile << msg.getPayload().get(CrlMessageSetDnsConfiguration::CMessageServer2Name).asString() << std::endl;
          payload.set(CrlMessageSetDnsConfigurationResponse::CSuccessName) = CrlMessageSetDnsConfigurationResponse::CSuccessTrue;
          LOG_VNotice(core, "Dns config file saved[%s].\n", dnsConfigFilename.c_str());
        }
        else
        {
          payload.set(CrlMessageSetDnsConfigurationResponse::CSuccessName) = CrlMessageSetDnsConfigurationResponse::CSuccessFalse;
          payload.set(CrlMessageSetDnsConfigurationResponse::CErrorCodeName) = CrlMessageSetDnsConfigurationResponse::CDnsNotSavedErrorCode;
          payload.set(CrlMessageSetDnsConfigurationResponse::CErrorMessageName) = CrlMessageSetDnsConfigurationResponse::CDnsNotSavedErrorMessage;
          LOG_VNotice(core, "Dns config file not saved [%s].\n", dnsConfigFilename.c_str());
        }
        crlSetDnsConfigurationResponse.setPayload(payload);

        {
          crlSetDnsConfigurationResponse.getJson().serialize(jsonMessage);
          LOG_VNotice(core, "Sending a set dns configuration command response to the CRL [\n%s\n].\n", jsonMessage.c_str());
        }
        m_crlSession.send(crlSetDnsConfigurationResponse);
      }
    }

    void CloudDnsConfigurationWorker::doTestDnsConfiguration(CrlMessageTestDns & msg)
    {
      std::string jsonMessage;
      {
        msg.getJson().serialize(jsonMessage);
        LOG_VNotice(core, "Received a Test dns configuration command from the CRL [\n%s\n].\n", jsonMessage.c_str());
      }
      jsonMessage.clear();
      {
        CrlMessageTestDnsResponse crlTestDnsConfigurationResponse;
        gvr::utils::Json payload;

        UccMessageTestDns uccDnsTest;
        uccDnsTest.setDeviceId(m_cloudStatus.getDeviceId());
        uccDnsTest.generateMessageId();
        payload.set(UccMessageTestDns::CServerName) = msg.getPayload().get(CrlMessageTestDns::CMessageServerName);
        payload.set(UccMessageTestDns::CDomainName) = msg.getPayload().get(CrlMessageTestDns::CMessageDomainName);
        uccDnsTest.setPayload(payload);

        UccMessageTestDnsResponse uccDnsTestRs;
        uccDnsTestRs.setMessageId(uccDnsTest.getMessageId());
        m_uccSession.sendSync(uccDnsTest, uccDnsTestRs);

        payload.clear();
        if( m_uccSession.getError() != UccSession::UccSessionError::CNoError )
        {
          LOG_VCritical(core, "UccSession Error: [%s], sending a message to test dns.", m_uccSession.getErrorDescription().c_str());
          payload.set(CrlMessageTestDnsResponse::CSuccessName) = CrlMessageTestDnsResponse::CSuccessFalse;
          payload.set(CrlMessageTestDnsResponse::CErrorCodeName) = m_uccSession.getError();
          payload.set(CrlMessageTestDnsResponse::CErrorMessageName) = m_uccSession.getErrorDescription();
        }
        else
        {

          uccDnsTestRs.getJson().serialize(jsonMessage);
          LOG_VDebug(core, "Received information for ucc test dns is [\n%s\n].",jsonMessage.c_str());


          payload.set(CrlMessageTestDnsResponse::CSuccessName) = uccDnsTestRs.getPayload().get(UccMessageTestDnsResponse::CSuccessName);
          if(uccDnsTestRs.getPayload().get(UccMessageTestDnsResponse::CSuccessName).asBoolean())
          {
            payload.set(CrlMessageTestDnsResponse::CHostIpName) = uccDnsTestRs.getPayload().get(UccMessageTestDnsResponse::CHostIpName);
          }
          else
          {
            payload.set(CrlMessageTestDnsResponse::CErrorCodeName) = uccDnsTestRs.getPayload().get(UccMessageTestDnsResponse::CErrorCodeName);
            payload.set(CrlMessageTestDnsResponse::CErrorMessageName) = uccDnsTestRs.getPayload().get(UccMessageTestDnsResponse::CErrorMessageName);
          }
        }

        jsonMessage.clear();
        crlTestDnsConfigurationResponse.setPayload(payload);
        {
          crlTestDnsConfigurationResponse.getJson().serialize(jsonMessage);
          LOG_VNotice(core, "Sending a Test dns configuration command response to the CRL [\n%s\n].\n", jsonMessage.c_str());
        }
        m_crlSession.send(crlTestDnsConfigurationResponse);
      }
    }

    void CloudDnsConfigurationWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(core, "Processing event message type notification.");

      if( CrlMessageGetDnsConfiguration::GetTypeId() == tId )
      {
        CrlMessageGetDnsConfiguration msg;
        if (msg.parse(data))
        {
          doGetDnsConfiguration(msg);
        }
        else
        {
          LOG_Warn(core, "Message received is a crl get dns configuration command request invalid message. Discarded.");
        }
      }
      else if( CrlMessageSetDnsConfiguration::GetTypeId() == tId )
      {
        CrlMessageSetDnsConfiguration msg;
        if (msg.parse(data))
        {
          doSetDnsConfiguration(msg);
        }
        else
        {
          LOG_Warn(core, "Message received is a crl set dns configuration command request invalid message. Discarded.");
        }
      }
      else if( CrlMessageTestDns::GetTypeId() == tId )
      {
        CrlMessageTestDns msg;
        if (msg.parse(data))
        {
          doTestDnsConfiguration(msg);
        }
        else
        {
          LOG_Warn(core, "Message received is a crl test dns request invalid message. Discarded.");
        }
      }
      else
      {
        LOG_Warn(core, "Message received has unexpected type ID. Discarded.");
      }
    }

    void CloudDnsConfigurationWorker::processNotification( StatusType status )
    {
      LOG_VDebug(core, "New status event has been received from CrlSession [%s].\n", m_crlSession.getStatusDescription().c_str());
    }
  }
}
