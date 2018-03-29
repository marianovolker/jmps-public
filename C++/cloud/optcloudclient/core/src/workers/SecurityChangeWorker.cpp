/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SecurityChangeWorker.cpp
 * @author mvolker
 * @date Monday, June 8, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <workers/SecurityChangeWorker.h>

# include <gvr/communication/ucc/message/UccMessageCloudSecurityChange.h>
# include <gvr/communication/ucc/message/UccMessageCloudSecurityChangeResponse.h>

# include <gvrlog/log.h>
# include <EncryptData.h>

# include <algorithm>
# include <string>
# include <sstream>
# include <fstream>

DECLARE_LOG_MODULE(core);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    SecurityChangeWorker::SecurityChangeWorker(UccSession & uccSession, SpotSession &spotSession, CloudStatus & cloudStatus)
     : AbsOptCloudClientWorker("SecurityChangeWorker"),
        m_uccSession(uccSession),
        m_spotSession(spotSession),
        m_cloudStatus(cloudStatus)
    {
      start();

      LOG_Debug(core, "Starting Cloud Security Change Worker...");
    }

    SecurityChangeWorker::~SecurityChangeWorker()
    {
      stop();

      LOG_Debug(core, "Stopping Cloud Security Change Worker...");
    }

    void SecurityChangeWorker::sessionEvent( IUccSessionListener::BufferType const & buffer, IUccSessionListener::TypeId const & typeId, IUccSessionListener::KeyType const & keyType)
    {
      LOG_Trace(core, "Enqueue a new UCC message notification.");

      enqueueNotification ( typeId, buffer );
    }

    void SecurityChangeWorker::sessionEvent( IUccSessionListener::StatusType status )
    {
      LOG_Trace(core, "Enqueue a new UCC session status notification.");

      enqueueNotification ( status );
    }

    void SecurityChangeWorker::doSubscriptions( )
    {
      LOG_Debug(core, "Subscribing to UCC reply messages.");

      m_uccSession.subscribe(*this, UccMessageCloudSecurityChange::GetTypeId());
    }

    void SecurityChangeWorker::doUnsubscriptions( )
    {
      LOG_Debug(core, "Un-subscribing to UCC request messages.");

      m_uccSession.unsubscribe(*this);
    }

    void SecurityChangeWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(core, "Processing event message type notification.");

      UccMessageCloudSecurityChangeResponse msgResponse;


      if( UccMessageCloudSecurityChange::GetTypeId() == tId )
      {
        UccMessageCloudSecurityChange msgRequest;
        if( msgRequest.parse(data) )
        {
          // TODO: delete this log before to launch to release...
          {
            std::string jsonMessage;
            msgRequest.getJson().serialize(jsonMessage);
            LOG_VNotice(core, "Received a security change command from UCC [\n%s\n].\n", jsonMessage.c_str());
          }

          gvr::utils::Json json(msgRequest.getPayload());
          msgResponse.setMessageId(msgRequest.getMessageId());


          std::string sCertFormat = json.get(UccMessageCloudSecurityChange::CCertFormatName).asString();
          std::transform(sCertFormat.begin(), sCertFormat.end(), sCertFormat.begin(), ::toupper);
          if( sCertFormat == UccMessageCloudSecurityChange::CCertFormatP12 )
          {
            if( storeNewCertificatesInformation( json ) )
            {
              msgResponse.setSuccess(true);
            }
            else
            {
              msgResponse.setSuccess(false);
              gvr::utils::Json jsonSnd = msgResponse.getJson();
              jsonSnd.set(UccMessageCloudSecurityChangeResponse::CCodeName, UccMessageCloudSecurityChangeResponse::CCorruptedCertificateDataCode);
              jsonSnd.set(UccMessageCloudSecurityChangeResponse::CMessageName, UccMessageCloudSecurityChangeResponse::CCorruptedCertificateDataMessage);
              msgResponse.setJson(jsonSnd);
            }
          }
          else
          {
            msgResponse.setSuccess(false);
            gvr::utils::Json jsonSnd = msgResponse.getJson();
            jsonSnd.set(UccMessageCloudSecurityChangeResponse::CCodeName, UccMessageCloudSecurityChangeResponse::CInvalidCertificateFormatDataCode);
            jsonSnd.set(UccMessageCloudSecurityChangeResponse::CMessageName, UccMessageCloudSecurityChangeResponse::CInvalidCertificateFormatDataMessage);
            msgResponse.setJson(jsonSnd);
          }
        }
        else
        {
          LOG_Warn(core, "Message received is a security-change request invalid message. Discarded.");

          msgResponse.generateMessageId();
          msgResponse.setSuccess(false);
          gvr::utils::Json jsonSnd = msgResponse.getJson();
          jsonSnd.set(UccMessageCloudSecurityChangeResponse::CCodeName, UccMessageCloudSecurityChangeResponse::CInvalidPayloadDataCode);
          jsonSnd.set(UccMessageCloudSecurityChangeResponse::CMessageName, UccMessageCloudSecurityChangeResponse::CInvalidPayloadDataMessage);
          msgResponse.setJson(jsonSnd);
        }
      }
      else
      {
        LOG_Warn(core, "Message received has unexpected type ID. Discarded.");

        msgResponse.generateMessageId();
        msgResponse.setSuccess(false);
        gvr::utils::Json jsonSnd = msgResponse.getJson();
        jsonSnd.set(UccMessageCloudSecurityChangeResponse::CCodeName, UccMessageCloudSecurityChangeResponse::CUnsupportedDeviceCommandDataCode);
        jsonSnd.set(UccMessageCloudSecurityChangeResponse::CMessageName, UccMessageCloudSecurityChangeResponse::CUnsupportedDeviceCommandDataMessage);
        msgResponse.setJson(jsonSnd);
      }



      if( m_uccSession.send(msgResponse) == UccSession::UccSessionError::CNoError )
      {
        // TODO: delete this log before to launch to release...
        {
          std::string jsonMessage;
          msgResponse.getJson().serialize(jsonMessage);
          LOG_VNotice(core, "Sending a config change command response to UCC [\n%s\n].\n", jsonMessage.c_str());
        }
      }
      else
      {
        LOG_Error(core, "Fail to send a message to UCC.");
      }
    }

    void SecurityChangeWorker::processNotification( StatusType status )
    {
      LOG_VDebug(core, "CloudSecurityChangeWorker - New status event has been received from uccSession [%s].\n", m_uccSession.getStatusDescription().c_str());
    }


    bool SecurityChangeWorker::storeNewCertificatesInformation( const gvr::utils::Json& json )
    {
      std::string pathCloudServerDataNewFile;

      m_cloudStatus.getPathNewCloudServerDataFile(pathCloudServerDataNewFile);

      try
      {
        EncryptData encryptData(m_spotSession);

        std::string url         = json.get(UccMessageCloudSecurityChange::CUriName).asString();
        std::string certFormat  = json.get(UccMessageCloudSecurityChange::CCertFormatName).asString();
        std::string certPwd     = json.get(UccMessageCloudSecurityChange::CCertName).asString();
        std::string pwd         = json.get(UccMessageCloudSecurityChange::CPwdName).asString();

        std::stringstream ss;

        ss << UccMessageCloudSecurityChange::CUriName        << " : " << url         << std::endl;
        ss << UccMessageCloudSecurityChange::CCertFormatName << " : " << certFormat  << std::endl;
        ss << UccMessageCloudSecurityChange::CCertName       << " : " << certPwd     << std::endl;
        ss << UccMessageCloudSecurityChange::CPwdName        << " : " << pwd         << std::endl;

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

        std::ofstream binaryFile(pathCloudServerDataNewFile.c_str(), std::ios::out | std::ofstream::binary | std::ofstream::trunc);

        if( !binaryFile.is_open() )
        {
          LOG_VError(core, "Could not create binary file [%s]\n", pathCloudServerDataNewFile.c_str());
          return false;
        }

        std::copy(encryptedBuffer.begin(), encryptedBuffer.end(), std::ostreambuf_iterator<char>(binaryFile));

        return true;
      }
      catch(...)
      {
        LOG_VError(core, "SecurityChangeWorker cannot create data file [%s] to store UccMessageCloudSecurityChange.", pathCloudServerDataNewFile.c_str());
      }

      return false;
    }
  }
}
