/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudStatusWorker.cpp
 * @author mvolker
 * @date Thursday, April 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

# include <workers/CloudStatusWorker.h>

# include <gvr/communication/ucc/message/UccMessageStatus.h>

# include <gvrlog/log.h>

DECLARE_LOG_MODULE(core);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    CloudStatusWorker::CloudStatusWorker(UccSession & uccSession, CloudStatus & cloudStatus, CloudConfigDeviceChecker & cloudConfigDeviceChecker)
     : AbsOptCloudClientWorker("CloudStatusWorker"),
        m_uccSession(uccSession),
        m_cloudStatus(cloudStatus),
        m_cloudConfigDeviceChecker(cloudConfigDeviceChecker)
    {
      start();

      LOG_Debug(core, "Starting Cloud Status Worker...");
    }

    CloudStatusWorker::~CloudStatusWorker()
    {
      stop();

      LOG_Debug(core, "Stopping Cloud Status Worker...");
    }

    void CloudStatusWorker::sessionEvent( IUccSessionListener::BufferType const & buffer, IUccSessionListener::TypeId const & typeId, IUccSessionListener::KeyType const & keyType)
    {
      LOG_Trace(core, "Enqueue a new UCC message notification.");

      enqueueNotification ( typeId, buffer );
    }

    void CloudStatusWorker::sessionEvent( IUccSessionListener::StatusType status )
    {
      LOG_Trace(core, "Enqueue a new UCC session status notification.");

      enqueueNotification ( status );
    }

    void CloudStatusWorker::doSubscriptions( )
    {
      LOG_Debug(core, "Subscribing to UCC reply messages.");

      m_uccSession.subscribe(*this, UccMessageStatus::GetTypeId());
    }

    void CloudStatusWorker::doUnsubscriptions( )
    {
      LOG_Debug(core, "Un-subscribing to UCC request messages.");

      m_uccSession.unsubscribe(*this);
    }

    void CloudStatusWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(core, "Processing event message type notification.");

      if( UccMessageStatus::GetTypeId() == tId )
      {
        UccMessageStatus msg;
        if( msg.parse(data) )
        {
          gvr::utils::Json json(msg.getPayload());

          // Operative Status.
          m_cloudStatus.setOperative(json.get(UccMessageStatus::COperativeName).asBoolean());

          // Connecting Status.
          if( json.get(UccMessageStatus::CConnectionName).asString() == UccMessageStatus::CConnectionDisconnectedString )
          {
            if (m_cloudStatus.testAndSetConnected(false) == true)
            {
              m_cloudConfigDeviceChecker.restart();
            }
            else
            {
              LOG_VDebug(core, "Discarded - Already disconnected.");
            }
          }
          else if( json.get(UccMessageStatus::CConnectionName).asString() == UccMessageStatus::CConnectionConnectingString )
          {
            // TODO: Remove. Connecting should not trigger any thing.
            //m_cloudStatus.setDisconnected();
            LOG_VDebug(core, "Discarded - Do not trigger any action while connecting.");
          }
          else if( json.get(UccMessageStatus::CConnectionName).asString() == UccMessageStatus::CConnectionConnectedString )
          {
            if (m_cloudStatus.testAndSetConnected(true) == false)
            {
              m_cloudConfigDeviceChecker.restart();
            }
            else
            {
              LOG_VDebug(core, "Discarded - Already connected.");
            }
          }
/*
          // Registration Status.
          if( json.get(UccMessageStatus::CRegistrationName).asString() == UccMessageStatus::CRegistrationDeactivatedString )
          {
            m_cloudStatus.setUnregistered();
            check = 0;
          }
          else if( json.get(UccMessageStatus::CRegistrationName).asString() == UccMessageStatus::CRegistrationActivatedString )
          {
            if (m_cloudStatus.isRegistered() == false)
            {
              if ((msg.getDeviceId().empty() == false) ||
                  (msg.getDeviceId().compare(CloudStatus::CUnregisteredDeviceId) != 0))
              {
                m_cloudStatus.setRegisteredDeviceId(msg.getDeviceId());
                check = 1;
              }
              else
              {
                LOG_VError(core, "Sent registration activated with deviceId [\n%s\n].",msg.getDeviceId().c_str());
              }
            }
            else
            {
              LOG_VDebug(core, "Discarded - Already registered.");
            }
          }
*/
          {
            std::string collectedInfo;
            json.serialize(collectedInfo);
            LOG_VDebug(core, "Collected information for UccMessageStatus is [\n%s\n].",collectedInfo.c_str());
          }
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

    void CloudStatusWorker::processNotification( StatusType status )
    {
      LOG_VDebug(core, "CloudStatusWorker - New status event has been received from uccSession [%s].\n", m_uccSession.getStatusDescription().c_str());
      if (status == gvr::communication::ucc::UccSession::UccSessionStatus::CStatusSessionStarted)
      {
        if (m_cloudStatus.isRegistered())
        {
          LOG_Debug(core, "CloudStatusWorker - ConfigDeviceChecker being restarted.");
          m_cloudStatus.setDisconnected();
          m_cloudConfigDeviceChecker.restart();
          LOG_Debug(core, "CloudStatusWorker - ConfigDeviceChecker restarted.");
        }
      }
      else if (status == gvr::communication::ucc::UccSession::UccSessionStatus::CStatusSessionStopped)
      {
        m_cloudStatus.setDisconnected();
        m_cloudConfigDeviceChecker.stop();
      }
    }
  }
}
