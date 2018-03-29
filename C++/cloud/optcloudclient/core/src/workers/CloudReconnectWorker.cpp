/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudReconnectWorker.cpp
 * @author mgrieco
 * @date Monday, Jun 08, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <workers/CloudReconnectWorker.h>

#include <gvr/communication/ucc/message/UccMessageCloudReconnect.h>
#include <gvr/communication/ucc/message/UccMessageCloudReconnectResponse.h>
#include <gvr/communication/ucc/message/UccMessageConnect.h>
#include <gvr/communication/ucc/message/UccMessageConnectResponse.h>
#include <gvr/communication/ucc/message/UccMessageRegisterResponse.h>

#include <Poco/Util/PropertyFileConfiguration.h>
#include <EncryptData.h>
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
    CloudReconnectWorker::CloudReconnectWorker( UccSession & uccSession, SpotSession & spotSession, CloudStatus & cloudStatus):
        AbsOptCloudClientWorker("CloudReconnectWorker"), m_pUccSession(&uccSession), m_pSpotSession(&spotSession), m_cloudStatus(cloudStatus)
    {
      start();
      LOG_Debug(core, "Starting CloudReconnectWorker...");
    }

    CloudReconnectWorker::~CloudReconnectWorker()
    {
      stop();
      LOG_Debug(core, "Stopping CloudReconnectWorker...");
    }

    void CloudReconnectWorker::sessionEvent( IUccSessionListener::BufferType const & buffer, IUccSessionListener::TypeId const & typeId, IUccSessionListener::KeyType const & keyType)
    {
      LOG_Debug(core, "Enqueue a new message notification...");
      enqueueNotification ( typeId, buffer );
    }

    void CloudReconnectWorker::sessionEvent( IUccSessionListener::StatusType status )
    {
      LOG_Debug(core, "Enqueue a new status notification...");
      enqueueNotification ( status );
    }

    void CloudReconnectWorker::doSubscriptions( )
    {
      // Here we need to subscribe to a specific event reconnect that will received from the ucc session.
      LOG_Debug(core, "Subscribe to all type messages...");
      m_pUccSession->subscribe(*this,gvr::communication::ucc::UccMessageCloudReconnect::GetTypeId());
    }

    void CloudReconnectWorker::doUnsubscriptions( )
    {
      // Here we need to unsubscribe to all events.
      LOG_Debug(core, "Un-subscribe to all type messages...");
      m_pUccSession->unsubscribe(*this);
    }

    void CloudReconnectWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(core, "Process event message type notification ...");
      IUccSessionListener::TypeId typeId(tId);

      // Here we need to verify if the notification correspond to uccReconnectRequest.
      if( typeId == gvr::communication::ucc::UccMessageCloudReconnect::GetTypeId() )
      {
        doReconnect(data);
      }
    }

    void CloudReconnectWorker::processNotification( StatusType status )
    {
      LOG_VNotice(core, "New status event has been received from UccSession [%s].\n", m_pUccSession->getStatusDescription().c_str());
    }

    void CloudReconnectWorker::doReconnect ( const AbsOptCloudClientWorker::BufferType& data )
    {
      gvr::communication::ucc::UccMessageCloudReconnect uccCloudReconnect;
      if (uccCloudReconnect.parse(data))
      {
        std::string jsonMessage;
        {
          uccCloudReconnect.getJson().serialize(jsonMessage);
          LOG_VNotice(core, "Received a reconnect command from the UCC [\n%s\n].\n", jsonMessage.c_str());
        }

        {
          std::string decryptedData;
          bool replaceOldEncryptedDataFile = false;
          bool decryptedDataOK = getDecryptedConnectionData(decryptedData);
          bool reconectStartOk = decryptedDataOK && (m_cloudStatus.getDeviceId().compare(uccCloudReconnect.getDeviceId()) == 0);
          sendReconnectResponse(uccCloudReconnect.getMessageId(), reconectStartOk);
          if(reconectStartOk)
          {
            LOG_VNotice(core, "Decrypted connection data Ok, doing a connect..\n", jsonMessage.c_str());
            doConnect(decryptedData);
          }
        }
      }
      else
      {
        LOG_VError(core, "UccMessageCloudReconnect Error [%s].\n", "Message parsing failed for reset command failed.");
      }
    }

    void CloudReconnectWorker::doConnect ( const std::string& strDecryptedData )
    {
      bool doConnect = false;
      gvr::communication::ucc::UccMessageConnect msgUccMessageConnect;
      {
        msgUccMessageConnect.setDeviceId(m_cloudStatus.getDeviceId());
        msgUccMessageConnect.generateMessageId();

        // Load configuration from .ini file type.
        std::istringstream is(strDecryptedData);
        Poco::AutoPtr<Poco::Util::PropertyFileConfiguration> pConfig= new Poco::Util::PropertyFileConfiguration(is);
        std::string url         = pConfig->getString(gvr::communication::ucc::UccMessageRegisterResponse::CUriName       , "");
        std::string certFormat  = pConfig->getString(gvr::communication::ucc::UccMessageRegisterResponse::CCertFormatName, "");
        std::string certType    = pConfig->getString(gvr::communication::ucc::UccMessageRegisterResponse::CCertName      , ""); // Please check, maybe is wrong ...
        std::string user        = pConfig->getString(gvr::communication::ucc::UccMessageRegisterResponse::CUserIdName    , "");
        std::string pwd         = pConfig->getString(gvr::communication::ucc::UccMessageRegisterResponse::CPwdName       , "");
        {

          std::string dnsServerName1;
          std::string dnsServerName2;
          m_cloudStatus.getDnsLocalConfiguration(dnsServerName1, dnsServerName2);

          gvr::utils::Json payload;
          payload.set(gvr::communication::ucc::UccMessageConnect::CUriName)             = url;
          payload.set(gvr::communication::ucc::UccMessageConnect::CNameServer1Name)     = dnsServerName1;
          payload.set(gvr::communication::ucc::UccMessageConnect::CNameServer2Name)     = dnsServerName2;
          payload.set(gvr::communication::ucc::UccMessageConnect::CCertFormatName)      = certFormat;
          payload.set(gvr::communication::ucc::UccMessageConnect::CCertTypeName)        = certType;
          payload.set(gvr::communication::ucc::UccMessageConnect::CUseridName)          = user;
          payload.set(gvr::communication::ucc::UccMessageConnect::CPwdName)             = pwd;

          int reconnectionDelayValue;
          m_cloudStatus.getReconnectionDelayValue(reconnectionDelayValue);
          payload.set(gvr::communication::ucc::UccMessageConnect::CConnectionDelayName) = reconnectionDelayValue;

          {
            std::string collectedInfo;
            payload.serialize(collectedInfo);
            LOG_VDebug(core, "Collected information for UccMessageConnect is [\n%s\n].",collectedInfo.c_str());
          }
          msgUccMessageConnect.setPayload(payload);

          gvr::communication::ucc::UccMessageConnectResponse msgUccMessageConnectResp;
          msgUccMessageConnectResp.setMessageId(msgUccMessageConnect.getMessageId());
          m_pUccSession->sendSync(msgUccMessageConnect, msgUccMessageConnectResp);
          if( m_pUccSession->getError() != gvr::communication::ucc::UccSession::UccSessionError::CNoError )
          {
            m_cloudStatus.setDisconnected();
            LOG_VCritical(core, "UccSession Error: [%s], sending a message.", m_pUccSession->getErrorDescription().c_str());
          }
          else
          {
            gvr::utils::Json json(msgUccMessageConnectResp.getPayload());
            if (json.get(gvr::communication::ucc::UccMessageConnectResponse::CSuccessName).asBoolean())
            {
              LOG_Notice(core, "UccMessageConnect successful.");
              m_cloudStatus.setConnected();
            }
            else
            {
              std::string errorCode(json.get(gvr::communication::ucc::UccMessageConnectResponse::CCodeName).asString());
              std::string errorMessage(json.get(gvr::communication::ucc::UccMessageConnectResponse::CMessageName).asString());
              LOG_VError(core, "UccMessageConnect Error [%s].\n", errorMessage.c_str());
              m_cloudStatus.setDisconnected(errorCode,errorMessage);
            }
          }
        }
      }
    }

    void CloudReconnectWorker::sendReconnectResponse(const std::string & messageId, const bool reconnectStartOk)
    {
     gvr::communication::ucc::UccMessageCloudReconnectResponse uccCloudReconnectResponse;
     uccCloudReconnectResponse.setMessageId(messageId);

     if (reconnectStartOk)
     {
       uccCloudReconnectResponse.setSuccess(true);
     }
     else
     {
       uccCloudReconnectResponse.setSuccess(false);
       gvr::utils::Json jsonSnd = uccCloudReconnectResponse.getJson();
       jsonSnd.set(UccMessageCloudReconnectResponse::CCodeName, UccMessageCloudReconnectResponse::CInvalidEncryptedDataCode);
       jsonSnd.set(UccMessageCloudReconnectResponse::CMessageName, UccMessageCloudReconnectResponse::CInvalidEncryptedDatamessage);
       uccCloudReconnectResponse.setJson(jsonSnd);
     }

     {
       std::string jsonMessage;
       uccCloudReconnectResponse.getJson().serialize(jsonMessage);
       LOG_VNotice(core, "Sending a reconnect command response to the UCC [\n%s\n].\n", jsonMessage.c_str());
     }
     m_pUccSession->send(uccCloudReconnectResponse);
    }

    void CloudReconnectWorker::replaceOldEncryptedConnectionData(const std::string& oldEncryptedDataFile, const std::string& newEncryptedDataFile)
    {
      if (remove(oldEncryptedDataFile.c_str()) != 0)
      {
        LOG_VError(core, "Encrypted data file remove failed\n");
      }
      else
      {
        rename(newEncryptedDataFile.c_str(), oldEncryptedDataFile.c_str());
        LOG_Notice(core, "Old encrypted data file replaced.\n");
      }
    }

    bool CloudReconnectWorker::getDecryptedConnectionData(std::string& decryptedData)
    {
      bool newEncryptedDataFileUsed = false;

      std::string encryptedDataFile;
      std::string newEncryptedDataFile;

      m_cloudStatus.getPathCloudServerDataFile(encryptedDataFile);
      m_cloudStatus.getPathNewCloudServerDataFile(newEncryptedDataFile);
      std::ifstream binaryFile(newEncryptedDataFile.c_str(), std::ifstream::binary);
      if (binaryFile.is_open())
      {
        LOG_VNotice(core, "Using new encrypted data to reconnect.\n");
        binaryFile.close();
        newEncryptedDataFileUsed = true;
      }
      else
      {
        LOG_VNotice(core, "Using old encrypted data to reconnect.\n");
        std::ifstream binaryFile(encryptedDataFile.c_str(), std::ifstream::binary);
        if (binaryFile.is_open())
        {
         binaryFile.close();
        }
        else
        {
         LOG_VError(core, "Could not open encrypted binary file [%s]\n", encryptedDataFile.c_str());
         return false;
        }
      }

      bool decryptOk = false;
      EncryptData encryptData(*m_pSpotSession);
      if(newEncryptedDataFileUsed)
      {
        decryptOk = encryptData.decryptConnectionData(newEncryptedDataFile, decryptedData);
        if(decryptOk)
        {
          replaceOldEncryptedConnectionData(encryptedDataFile, newEncryptedDataFile);
        }
      }
      else
      {
        decryptOk = encryptData.decryptConnectionData(encryptedDataFile, decryptedData);
      }
      return decryptOk;
    }
  }
}
