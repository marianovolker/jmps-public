/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudRegistrationStatusWorker.cpp
 * @author mvolker
 * @date Thursday, April 16, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <workers/CloudRegistrationStatusWorker.h>
# include <gvr/communication/crl/message/CrlMessageCloudStatus.h>
# include <gvr/communication/crl/message/CrlMessageCloudStatusResponse.h>

# include <gvrlog/log.h>

DECLARE_LOG_MODULE(core);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    CloudRegistrationStatusWorker::CloudRegistrationStatusWorker(CrlSessionServer & crlSession, CloudStatus & cloudStatus)
     : AbsCrlWorker("CloudRegistrationStatusWorker", crlSession),
        m_crlSession(crlSession),
        m_cloudStatus(cloudStatus)
    {
      start();

      LOG_Debug(core, "Starting Cloud Registration Status Worker...");
    }

    CloudRegistrationStatusWorker::~CloudRegistrationStatusWorker()
    {
      stop();

      LOG_Debug(core, "Stopping Cloud Registration Status Worker...");
    }

    void CloudRegistrationStatusWorker::sessionEvent( ICrlSessionListener::BufferType const & buffer, ICrlSessionListener::TypeId const & typeId, ICrlSessionListener::KeyType const & keyType)
    {
      LOG_Trace(core, "Enqueue a new CRL message notification.");

      enqueueNotification ( typeId, buffer );
    }

    void CloudRegistrationStatusWorker::sessionEvent( ICrlSessionListener::StatusType status )
    {
      LOG_Trace(core, "Enqueue a new CRL session status notification.");

      enqueueNotification ( status );
    }

    void CloudRegistrationStatusWorker::doSubscriptions( )
    {
      LOG_Debug(core, "Subscribing to CRL reply messages.");

      m_crlSession.subscribe(*this, CrlMessageCloudStatus::GetTypeId());
    }

    void CloudRegistrationStatusWorker::doUnsubscriptions( )
    {
      LOG_Debug(core, "Un-subscribing to CRL request messages.");

      m_crlSession.unsubscribe(*this);
    }

    void CloudRegistrationStatusWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(core, "Processing event message type notification.");

      if (CrlMessageCloudStatus::GetTypeId() == tId)
      {
        CrlMessageCloudStatus msg;
        if (msg.parse(data))
        {
          CrlMessageCloudStatusResponse msgRs;
          if (m_cloudStatus.isRegistered() == false)
          {
            gvr::utils::Json json;
            json.set(CrlMessageCloudStatusResponse::CRegistrationStatusName) = CrlMessageCloudStatusResponse::CRegistrationUnregisteredString;
            json.set(CrlMessageCloudStatusResponse::CConnectionStatusName) = CrlMessageCloudStatusResponse::CConnectionDisconnectedString;
            {
              std::string errorCode;
              std::string errorMessage;
              m_cloudStatus.getRegistrationError(errorCode,errorMessage);
              if (errorCode.compare(CloudStatus::CRegistrationErrorCodeNoError) != 0)
              {
                json.set(CrlMessageCloudStatusResponse::CErrorCodeName) = errorCode;
                json.set(CrlMessageCloudStatusResponse::CErrorMessageName) = errorMessage;
              }
            }
            msgRs.setPayload(json);
          }
          else if (m_cloudStatus.isConnected() == false)
          {
            gvr::utils::Json json;
            json.set(CrlMessageCloudStatusResponse::CRegistrationStatusName) = CrlMessageCloudStatusResponse::CRegistrationRegisteredString;
            json.set(CrlMessageCloudStatusResponse::CConnectionStatusName) = CrlMessageCloudStatusResponse::CConnectionDisconnectedString;
            {
              std::string errorCode;
              std::string errorMessage;
              m_cloudStatus.getConnectionError(errorCode,errorMessage);
              if (errorCode.compare(CloudStatus::CConnectionErrorCodeNoError) != 0)
              {
                json.set(CrlMessageCloudStatusResponse::CErrorCodeName) = errorCode;
                json.set(CrlMessageCloudStatusResponse::CErrorMessageName) = errorMessage;
              }
            }
            msgRs.setPayload(json);
          }
          else
          {
            gvr::utils::Json json;
            json.set(CrlMessageCloudStatusResponse::CRegistrationStatusName) = CrlMessageCloudStatusResponse::CRegistrationRegisteredString;
            json.set(CrlMessageCloudStatusResponse::CConnectionStatusName) = CrlMessageCloudStatusResponse::CConnectionConnectedString;
            msgRs.setPayload(json);
          }
          LOG_Debug(core, "Sending cloud-status-response message.");
          m_crlSession.send(msgRs);
        }
        else
        {
          LOG_Warn(core, "Message received is a cloud-status request invalid message. Discarded.");
        }
      }
      else
      {
        LOG_Warn(core, "Message received has unexpected type ID. Discarded.");
      }
    }

    void CloudRegistrationStatusWorker::processNotification( StatusType status )
    {
      LOG_VDebug(core, "New status event has been received from CrlSession [%s].\n", m_crlSession.getStatusDescription().c_str());
    }
  }
}
