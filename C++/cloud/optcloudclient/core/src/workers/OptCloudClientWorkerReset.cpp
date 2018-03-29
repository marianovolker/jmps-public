/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file OptCloudClientWorkerReset.cpp
 * @author mvolker
 * @date Monday, March 30, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <workers/OptCloudClientWorkerReset.h>

#include <gvr/communication/spot/SpotMessageReset.h>
#include <gvr/communication/ucc/message/UccMessageCloudReset.h>
#include <gvr/communication/ucc/message/UccMessageCloudResetResponse.h>

#include <gvrlog/log.h>

DECLARE_LOG_MODULE(core);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {
    OptCloudClientWorkerReset::OptCloudClientWorkerReset( UccSession & uccSession, SpotSession & spotSession ):
        AbsOptCloudClientWorker("OptCloudClientWorkerReset"),
        m_pUccSession(&uccSession), m_pSpotSession(&spotSession)
    {
      start();

      LOG_Debug(core, "Starting Opt Cloud Client Worker...");
    }

    OptCloudClientWorkerReset::~OptCloudClientWorkerReset()
    {
      stop();

      LOG_Debug(core, "Stopping Opt Cloud Client Worker...");
    }

    void OptCloudClientWorkerReset::sessionEvent( IUccSessionListener::BufferType const & buffer, IUccSessionListener::TypeId const & typeId, IUccSessionListener::KeyType const & keyType)
    {
      LOG_Debug(core, "Enqueue a new message notification...");

      enqueueNotification ( typeId, buffer );
    }

    void OptCloudClientWorkerReset::sessionEvent( IUccSessionListener::StatusType status )
    {
      LOG_Debug(core, "Enqueue a new status notification...");

      enqueueNotification ( status );
    }

    void OptCloudClientWorkerReset::doSubscriptions( )
    {
      // Here we need to subscribe to a specific event reset that will received from the uccsession.
      LOG_Debug(core, "Subscribe to all type messages...");

      m_pUccSession->subscribe(*this,gvr::communication::ucc::UccMessageCloudReset::GetTypeId());
    }

    void OptCloudClientWorkerReset::doUnsubscriptions( )
    {
      // Here we need to unsubscribe to all events.
      LOG_Debug(core, "Un-subscribe to all type messages...");

      m_pUccSession->unsubscribe(*this);
    }

    void OptCloudClientWorkerReset::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(core, "Process event message type notification ...");

      IUccSessionListener::TypeId typeId(tId);

      // Here we need to verify if the notification correspond to uccResetRequest.
      if( typeId == gvr::communication::ucc::UccMessageCloudReset::GetTypeId() )
      {
        doReset(data);
      }
    }

    void OptCloudClientWorkerReset::processNotification( StatusType status )
    {
      LOG_VNotice(core, "New status event has been received from UccSession [%s].\n", m_pUccSession->getStatusDescription().c_str());
    }

    void OptCloudClientWorkerReset::doReset ( const AbsOptCloudClientWorker::BufferType& data )
    {
      gvr::communication::ucc::UccMessageCloudReset uccCloudReset;
      if (uccCloudReset.parse(data))
      {
        {
          std::string jsonMessage;
          {
            uccCloudReset.getJson().serialize(jsonMessage);
            LOG_VNotice(core, "Received a reset command from the UCC [\n%s\n].\n", jsonMessage.c_str());
          }

          jsonMessage.clear();

          {
            gvr::communication::ucc::UccMessageCloudResetResponse uccCloudResetResponse;
            uccCloudResetResponse.setMessageId(uccCloudReset.getMessageId());
            uccCloudResetResponse.setSuccess(true);

            {
              uccCloudResetResponse.getJson().serialize(jsonMessage);
              LOG_VNotice(core, "Sending a reset command response to the UCC [\n%s\n].\n", jsonMessage.c_str());
            }

            m_pUccSession->send(uccCloudResetResponse);
          }
        }

        {
          const SpotChannel & channel = dynamic_cast<const SpotChannel &>(m_pSpotSession->getChannel());

          LOG_VNotice(core, "Sending a reset command to SPOT [%s:%d].\n", channel.getIp().c_str(), channel.getPort());

          gvr::communication::spot::SpotMessageReset msgReset;

          m_pSpotSession->send(msgReset);
          if( m_pSpotSession->getError() != gvr::communication::spot::SpotSession::SpotSessionError::CNoError )
          {
            LOG_VError(core, "SpotSession Error [%s].\n", m_pSpotSession->getErrorDescription().c_str());
          }
        }
      }
      else
      {
        LOG_VError(core, "UccMessageCloudReset Error [%s].\n", "Message parsing failed for reset command failed.");
      }
    }
  }
}
