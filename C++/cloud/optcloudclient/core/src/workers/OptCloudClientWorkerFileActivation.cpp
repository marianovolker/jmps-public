/**
* Copyright © 2015 Gilbarco Inc.
* Confidential and Proprietary.
*
* @file OptCloudClientWorkerFileActivation.cpp
* @author gparis
* @date Monday, April 14, 2015
* @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
*/

#include <sys/stat.h>
#include <cstdio>
#include <fstream>

#include <gvr/communication/spot/SpotMessagePackageActivation.h>
#include <gvr/communication/spot/SpotMessagePackageActivationResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudInstall.h>
#include <gvr/communication/ucc/message/UccMessageCloudInstallResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudInstallStatus.h>
#include <gvr/communication/spot/SPOTFileDownloadFlow.h>

#include <gvrlog/log.h>

#include <workers/OptCloudClientWorkerFileActivation.h>

DECLARE_LOG_MODULE(worker);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {


    const char * OptCloudClientWorkerFileActivation::CActivationMarkTempFilename = "activation-packages.tmp";


    OptCloudClientWorkerFileActivation::OptCloudClientWorkerFileActivation( UccSession & uccSession, SpotSession & spotSession ):
        AbsOptCloudClientWorker("OptCloudClientWorkerFileActivation"),
        m_pUccSession(&uccSession), m_pSpotSession(&spotSession)
    {
      start();

      LOG_Debug(worker, "Starting OPT cloud client worker for file activation.");
    }

    OptCloudClientWorkerFileActivation::~OptCloudClientWorkerFileActivation()
    {
      stop();

      LOG_Debug(worker, "Stopping OPT cloud client worker for file activation.");
    }

    void OptCloudClientWorkerFileActivation::sessionEvent( IUccSessionListener::BufferType const & buffer, IUccSessionListener::TypeId const & typeId, IUccSessionListener::KeyType const & keyType)
    {
      LOG_Debug(worker, "Enqueue a new message notification for file activation.");

      enqueueNotification ( typeId, buffer );
    }

    void OptCloudClientWorkerFileActivation::sessionEvent( IUccSessionListener::StatusType status )
    {
      LOG_Debug(worker, "Enqueue a new status notification for file activation.");

      enqueueNotification ( status );
    }

    void OptCloudClientWorkerFileActivation::doSubscriptions( )
    {
      // Here we need to subscribe to a specific event reset that will received from the uccsession.
      LOG_Debug(worker, "Subscribe to file install UCC message type.");

      m_pUccSession->subscribe(*this, UccMessageCloudInstall::GetTypeId());
    }

    void OptCloudClientWorkerFileActivation::doUnsubscriptions( )
    {
      // Here we need to unsubscribe to all events.
      LOG_Debug(worker, "Un-subscribe to file install UCC message type.");

      m_pUccSession->unsubscribe(*this);
    }

    void OptCloudClientWorkerFileActivation::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Debug(worker, "Process UCC request message type notification ...");

      IUccSessionListener::TypeId typeId(tId);

      // Here we need to verify if the notification correspond to uccResetRequest.
      if( typeId == gvr::communication::ucc::UccMessageCloudInstall::GetTypeId() )
      {
        doActivation(data);
      }
    }

    void OptCloudClientWorkerFileActivation::processNotification( StatusType status )
    {
      LOG_VNotice(worker, "New status event has been received from UccSession [%s].\n", m_pUccSession->getStatusDescription().c_str());
    }

    void OptCloudClientWorkerFileActivation::doActivation ( const AbsOptCloudClientWorker::BufferType& data )
    {
      bool                            blParseOK;
      std::string                     sJson, sMsgId, sDeviceId, sFileType, sTrackingNbr, sFolder;
      UccMessageCloudInstall          uccCloudInstall;
      UccMessageCloudInstallResponse  uccCloudInstallResponse;

      blParseOK    = uccCloudInstall.parse(data);

      sMsgId       = uccCloudInstall.getMessageId();
      sDeviceId    = uccCloudInstall.getJson().get(AbsUccMessageCloudCommandRequest::CDeviceIdName).asString();
      sFileType    = uccCloudInstall.getJson().get(AbsUccMessageCloudCommandRequest::CPayloadName).get(UccMessageCloudInstall::CFileTypeName).asString();
      sTrackingNbr = uccCloudInstall.getJson().get(AbsUccMessageCloudCommandRequest::CPayloadName).get(UccMessageCloudInstall::CTrackingNumberName).asString();

      sFolder = uccCloudInstall.getJson().get(AbsUccMessageCloudCommandRequest::CPayloadName).get(UccMessageCloudInstall::CUrlName).asString();
      sFolder = sFolder.substr(sFolder.find("/"));
      LOG_VTrace(worker, "Selected RELEASE FOLDER [\n%s\n].\n", sFolder.c_str());

      uccCloudInstallResponse.setMessageId( sMsgId );
      uccCloudInstallResponse.getJson().set(AbsUccMessageCloudCommandRequest::CDeviceIdName, sDeviceId);
      uccCloudInstallResponse.getJson().set(AbsUccMessageCloudCommandRequest::CPayloadName).set(UccMessageCloudInstall::CFileTypeName, sFileType);
      uccCloudInstallResponse.getJson().get(AbsUccMessageCloudCommandRequest::CPayloadName).set(UccMessageCloudInstall::CTrackingNumberName, sTrackingNbr);

      uccCloudInstall.getJson().serialize(sJson);
      LOG_VNotice(worker, "Received an install command from the UCC [\n%s\n].\n", sJson.c_str());

      if ( blParseOK )
      {
        uccCloudInstallResponse.setSuccess(true);
        uccCloudInstallResponse.getJson().serialize(sJson);
        LOG_VTrace(worker, "Sending an OK install command response to the UCC [\n%s\n].\n", sJson.c_str());

        m_pUccSession->send(uccCloudInstallResponse);
      }
      else // could not parse the command, send an error reply saying "invalid payload".
      {
        const char * pcData = reinterpret_cast<const char*>(&data.front());
        uccCloudInstallResponse.setSuccess(false);
        uccCloudInstallResponse.setError(UccMessageCloudInstallResponse::eInvalidPayload);

        LOG_VError(worker, "UccMessageCloudActivation Error %s\n[%s].\n",
                           "Message parsing for install command failed.",
                           pcData + CBinaryHeaderSize);
        m_pUccSession->send(uccCloudInstallResponse);
        return;
      }

      // A temporary file is created into the filesystem, for persistence reasons.
      {
        std::ofstream tempFile(CActivationMarkTempFilename, std::ios_base::out | std::ios_base::trunc);

        tempFile << AbsUccMessageCloudCommandRequest::CDeviceIdName << " = " << sDeviceId << std::endl;
        tempFile << UccMessageCloudInstall::CFileTypeName << " = " << sFileType << std::endl;
        tempFile << UccMessageCloudInstall::CTrackingNumberName << " = " << sTrackingNbr << std::endl;
      }

      UccMessageCloudInstallStatus          uccCloudInstallStatusEvent;
      uccCloudInstallStatusEvent.generateMessageId();
      uccCloudInstallStatusEvent.setDeviceId(sDeviceId);

      {
        uccCloudInstallStatusEvent.getJson().set(AbsUccMessageCloudCommandRequest::CPayloadName).set(UccMessageCloudInstallStatus::CFileTypeName, sFileType);
        uccCloudInstallStatusEvent.getJson().get(AbsUccMessageCloudCommandRequest::CPayloadName).set(UccMessageCloudInstallStatus::CTrackingNumberName, sTrackingNbr);
     }

      uccCloudInstallStatusEvent.setStatus(UccMessageCloudInstallStatus::eStart);
      sJson.clear();
      uccCloudInstallStatusEvent.getJson().serialize(sJson);
      LOG_VTrace(worker, "Sending a start install event to the cloud [\n%s\n].\n", sJson.c_str());
      m_pUccSession->send(uccCloudInstallStatusEvent);

      const SpotChannel & channel = dynamic_cast<const SpotChannel &>(m_pSpotSession->getChannel());
      LOG_VTrace(worker, "Sending an activation command to SPOT [%s:%d].\n", channel.getIp().c_str(), channel.getPort());


      // Send the resources files.
      {
        SPOTFileDownload downloadFlow(*m_pSpotSession);
        LOG_VNotice(worker, "Downloading resources to SPOT from %s.", sFolder.c_str());
        if( !downloadFlow.downloadResources(sFolder) )
        {
          uccCloudInstallStatusEvent.setError("error communicating to SPOT");
          if( m_pSpotSession->getError() != SpotSession::SpotSessionError::CNoError )
          {
            LOG_VError(worker, "SpotSession Error [%s].\n%s", m_pSpotSession->getErrorDescription().c_str(), "Resources file download could not be sent or replied.\n");
          }
          m_pUccSession->send(uccCloudInstallStatusEvent);
          remove( CActivationMarkTempFilename );
          return;
        }
      }

      SpotMessagePackageActivation          msgActivation;
      SpotMessagePackageActivationResponse  msgActivationReply;

      msgActivation.setVgdPlatform( m_pSpotSession->getDevicePlatform() == SpotSession::eM7 );
      unsigned int uerror = m_pSpotSession->sendSync(msgActivation, msgActivationReply, CSendActivationTimeout);
      if( uerror != 0 ) // error sending/receiving messages
      {
        uccCloudInstallStatusEvent.setError("error communicating to SPOT");

        if( m_pSpotSession->getError() != SpotSession::SpotSessionError::CNoError )
        {
          LOG_VError(worker, "SpotSession Error [%s].\n%s",
                     m_pSpotSession->getErrorDescription().c_str(),
                     "Activation message could not be sent or replied.\n");
        }

        m_pUccSession->send(uccCloudInstallStatusEvent);

        // The temporary file is removed from the filesystem.
        remove( CActivationMarkTempFilename );
        return;
      }
      else if( msgActivationReply.hasError() ) // error activating packages
      {
        // The temporary file is removed from the filesystem because the activation will not be performed.
        remove( CActivationMarkTempFilename );

        unsigned char bAck = msgActivationReply.getAckCode();
        LOG_VError(worker, "Ack = 0x%02X when tried to activate packages.\n", bAck);

        if( bAck == SpotMessagePackageActivationResponse::CNoPackagesForUpdate ) // nothing to activate
        {
          uccCloudInstallStatusEvent.setError("a previous download operation was not performed");
          LOG_Notice(worker, "Package activation: no application packages to activate.\n");
        }
        else // other errors with packages
        {
          uccCloudInstallStatusEvent.setError("errors replied to the SPOT activation command");
          LOG_Critical(worker, "Package activation failed! Installation end with errors.\n");
        }

        m_pUccSession->send(uccCloudInstallStatusEvent);
        return;
      }

      // Now we can confirm the installation ("activation") was sent and reply received OK.
      // Then a confirmation event "in-progress" will be sent after the good rebooting of  the unit,
      // next time this application is running.
      uccCloudInstallStatusEvent.getJson().serialize(sJson);
      LOG_VNotice(worker, "Sending an OK install command response to the UCC [\n%s\n].\n", sJson.c_str());
      uccCloudInstallStatusEvent.setStatus(UccMessageCloudInstallStatus::eInProgress);
      m_pUccSession->send(uccCloudInstallStatusEvent);
    }

    bool OptCloudClientWorkerFileActivation::hasBeenPackageActivationPerformed()
    {
      struct stat   statBuffer;
      return ( stat(CActivationMarkTempFilename, &statBuffer) == 0 );
    }
    void OptCloudClientWorkerFileActivation::notifyCloudInstallationFinished() const
    {
      notifyCloudInstallationFinished(*m_pUccSession);
    }

    bool OptCloudClientWorkerFileActivation::notifyCloudInstallationFinished(UccSession & uccSession)
    {
      bool result = false;

      // The following fields are retrieved from a temporary file
      std::string sDummy1, sDummy2;
      std::string sDeviceId;
      std::string sFileType;
      std::string sTrackingNbr;
      {
        std::ifstream tempFile(CActivationMarkTempFilename);

        tempFile >> sDummy1 >> sDummy2 >> sDeviceId;
        tempFile >> sDummy1 >> sDummy2 >> sFileType;
        tempFile >> sDummy1 >> sDummy2 >> sTrackingNbr;
      }

      std::string      sJson;
      gvr::utils::Json jsonPayload;
      UccMessageCloudInstallStatus   uccCloudInstallStatusEvent;

      uccCloudInstallStatusEvent.generateMessageId();
      uccCloudInstallStatusEvent.setDeviceId(sDeviceId);
      jsonPayload.set(UccMessageCloudInstallStatus::CFileTypeName, sFileType);
      jsonPayload.set(UccMessageCloudInstallStatus::CTrackingNumberName, sTrackingNbr);
      jsonPayload.set(UccMessageCloudInstallStatus::CStatusLabel, UccMessageCloudInstallStatus::CStatusOK);
      uccCloudInstallStatusEvent.setPayload(jsonPayload);

      uccCloudInstallStatusEvent.getJson().serialize(sJson);
      LOG_VDebug(worker, "Sending an install OK event reply to the UCC [\n%s\n].\n", sJson.c_str());

      if( uccSession.send(uccCloudInstallStatusEvent) == UccSession::UccSessionError::CNoError )
      {
        // The temporary file is removed from the filesystem.
        remove( CActivationMarkTempFilename );
        result = true;
      }

      return result;
    }
  }
}
