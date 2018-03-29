/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotReleaseDownloadWorker.cpp
 * @author mgrieco
 * @date Tuesday, April 07, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */


#include <Poco/Path.h>
#include <Poco/File.h>
#include <Poco/Crypto/DigestEngine.h>

#include <sstream>
#include <fstream>

#include <gvr/communication/ucc/message/UccMessageCloudFileDownload.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadStatusEventResponse.h>
#include <gvr/communication/ucc/message/UccMessageFileDownload.h>
#include <gvr/communication/ucc/message/UccMessageFileDownloadResponse.h>
#include <gvr/communication/ucc/message/UccMessageFileDownloadStatusEvent.h>


#include <gvrlog/log.h>
#include <workers/SpotReleaseDownloadWorker.h>

DECLARE_LOG_MODULE(worker);

///////////////////////////////////////////////////////////////
//TODO: Move to a more suitable place.
#include <sys/statvfs.h>
namespace
{
  unsigned long int getAvailableSpace(const char* path)
  {
    struct statvfs stat;
    if (statvfs(path, &stat) != 0)
    {
      // An error has been found.
      return 0;
    }
    // The available size is computed with: f_bsize * f_bavail
    return stat.f_bsize * stat.f_bavail;
  }
}
///////////////////////////////////////////////////////////////

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {


    using namespace gvr::communication::ucc;
    using namespace gvr::communication::spot;

    typedef Poco::TimerCallback<SpotReleaseDownloadWorker> InProgressCallback;
    typedef Poco::Timer               Timer;
    typedef Poco::FastMutex           Mutex;
    typedef Poco::ScopedLock<Mutex>   ScopeLock; ///< @brief Automatic scope life mutex.

    char const * SpotReleaseDownloadWorker::CSODA_MD5    = "MD5";
    char const * SpotReleaseDownloadWorker::CSODA_SHA1   = "SHA 1";
    char const * SpotReleaseDownloadWorker::CSODA_SHA2   = "SHA 2";
    char const * SpotReleaseDownloadWorker::CSODA_SHA256 = "SHA 256";
    char const * SpotReleaseDownloadWorker::CSODA_SHA512 = "SHA 512";
    char const * SpotReleaseDownloadWorker::CSupportedChecksumTypeBySODA = SpotReleaseDownloadWorker::CSODA_SHA256;

    char const * SpotReleaseDownloadWorker::CMD5    = "MD5";
    char const * SpotReleaseDownloadWorker::CSHA1   = "SHA1";
    char const * SpotReleaseDownloadWorker::CSHA256 = "SHA256";
    char const * SpotReleaseDownloadWorker::CSHA512 = "SHA512";

    const char* SpotReleaseDownloadWorker::CpcPHError = "Internal PH error. ";
    const char* SpotReleaseDownloadWorker::CSwBundle = "software-bundle";

    const char* SpotReleaseDownloadWorker::CapcEventLegends[ SpotReleaseDownloadWorker::eTotalEvents ] =
    {
      "eDownloadRequest",
      "eRcvFileFromCloudReplyOk",
      "eRcvFileFromCloudReplyError",
      "eRcvFileFromCloudStatusComplete",
      "eRcvFileFromCloudStatusError",
      "eChecksumOK",
      "eChecksumError",
      "eDecompressOk",
      "eDecompressError",
      "eSPOTDownloadOk",
      "eSPOTDownloadError",
      "eOtherFatalError"
    };

    const char* SpotReleaseDownloadWorker::CapcStateLegends[ SpotReleaseDownloadWorker::eTotalStates ] =
    {
      "Initial",
      "DownloadRequested",
      "DownloadingFromCloud",
      "CheckingFile",
      "DecompressingFile",
      "DownloadingToSpot",
      "DownloadOK",
    };


    SpotReleaseDownloadWorker::SpotReleaseDownloadWorker( UccSession & uccSession, SpotSession & spotSession, std::string const & releasePath, std::string const & spotIp )
      : AbsOptCloudClientWorker("SpotReleaseDownloadWorker"),
        m_pUccSession(&uccSession), m_pSpotSession(&spotSession),
        m_blInProgress(false), m_eState(eInitial), m_iFileSize(0),
        m_spotIp(spotIp), m_sDefaultReleasePath(releasePath),
        m_timerInProgress(0, CInProgressInterval),
        m_timerDownloading(CInDownloadInterval, CInDownloadInterval)
    {
      start();

      LOG_Notice(worker, "Starting OPT Cloud Client Worker...");
    }

    SpotReleaseDownloadWorker::~SpotReleaseDownloadWorker()
    {
      stop();

      LOG_Notice(worker, "Stopping OPT Cloud Client Worker...");
    }

    void SpotReleaseDownloadWorker::sessionEvent( IUccSessionListener::BufferType const & buffer, IUccSessionListener::TypeId const & typeId, IUccSessionListener::KeyType const & keyType)
    {
      LOG_Trace(worker, "Enqueue a new message notification...");

      enqueueNotification ( typeId, buffer );
    }

    void SpotReleaseDownloadWorker::sessionEvent( IUccSessionListener::StatusType status )
    {
      LOG_Trace(worker, "Enqueue a new status notification...");

      enqueueNotification ( status );
    }

    void SpotReleaseDownloadWorker::doSubscriptions( )
    {
      // Here we need to subscribe to a specific event file download that will received from the uccsession.
      LOG_Debug(worker, "Subscribe to file download command and event messages...");

      m_pUccSession->subscribe(*this, UccMessageCloudFileDownload::GetTypeId());
      m_pUccSession->subscribe(*this, UccMessageFileDownloadResponse::GetTypeId());
      m_pUccSession->subscribe(*this, UccMessageCloudFileDownloadStatusEventResponse::GetTypeId());
      m_pUccSession->subscribe(*this, UccMessageFileDownloadStatusEvent::GetTypeId());
    }

    void SpotReleaseDownloadWorker::doUnsubscriptions( )
    {
      // Here we need to unsubscribe to all events.
      LOG_Trace(worker, "Un-subscribe to all type messages...");

      m_pUccSession->unsubscribe(*this);
    }

    void SpotReleaseDownloadWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Trace(worker, "Process event message type notification ...");

      IUccSessionListener::TypeId typeId(tId);

      // Here we need to verify if the notification correspond to UccMessageCloudFileDownloadRequest.
      if( typeId == UccMessageCloudFileDownload::GetTypeId() )
      {
        doFileRequest(data);
      }

      // Here we need to verify if the notification correspond to UccMessageFileDownloadResponse.
      if( typeId == UccMessageFileDownloadResponse::GetTypeId())
      {
        doStartFileDownload(data);
      }

      // Here we need to verify if the notification correspond to UccMessageFileDownloadStatusEvent.
      if( typeId == UccMessageFileDownloadStatusEvent::GetTypeId() )
      {
        doHandleFileDownload(data);
      }

      // Here we need to verify if the notification correspond to UccMessageCloudFileDownloadStatusEventResponse.
      if( typeId == UccMessageCloudFileDownloadStatusEventResponse::GetTypeId())
      {
        processCloudStatusEventResponse(data);
      }
    }

    void SpotReleaseDownloadWorker::processNotification( IUccSessionListener::StatusType status )
    {
      LOG_VTrace(worker, "New session status event has been received from UccSession [%s].\n", m_pUccSession->getStatusDescription().c_str());
    }

    void SpotReleaseDownloadWorker::doFileRequest ( const AbsOptCloudClientWorker::BufferType& data )
    {
      UccMessageCloudFileDownload uccCloudFileDownload;
      ScopeLock    lock( m_mutex ); // exclusive use of member variables and status.

      if( m_eState != eInitial &&  m_eState != eDownloadOK ) // remove eDownloadOK when integrated with install worker.
      {
        LOG_VError( worker, "Unexpected message [CloudFileDownload] while in status [%s]. Discarded!",
                    CapcStateLegends[ m_eState ] );

        // at least try to reply with the same Ids, if they can be obtained
        sendFileDownloadResponse( UccMessageCloudFileDownloadResponse::CCannotExecuteAtThisTimeCode,
                                  UccMessageCloudFileDownloadResponse::CCannotExecuteAtThisTimeMessage,
                                  uccCloudFileDownload.getMessageId() );
        return;
      }

      if( uccCloudFileDownload.parse(data) )
      {
        std::string jsonMessage;
        {
          uccCloudFileDownload.getJson().serialize(jsonMessage);
          LOG_VInfo(worker, "Received a file download from the UCC [\n%s\n].\n", jsonMessage.c_str());
        }

        {
          std::string fileType = uccCloudFileDownload.getPayload().get(UccMessageCloudFileDownload::CFileTypeName).asString();
          if (fileType.compare(CSwBundle) != 0)
          {
            LOG_VError( worker, "Invalid file type requested to be downloaded [%s]. Discarded!",
                        fileType.c_str() );

            sendFileDownloadResponse( UccMessageCloudFileDownloadResponse::CInvalidFileTypeCode,
                                      UccMessageCloudFileDownloadResponse::CInvalidFileTypeMessage,
                                      uccCloudFileDownload.getMessageId());
            return;
          }
        }

        {
          int fileSize = uccCloudFileDownload.getPayload().get(UccMessageCloudFileDownload::CFileSizeName).asInteger();
          unsigned long int diskSizeRequired = ((fileSize>0)?(fileSize*2):(0));
          unsigned long int diskSizeAvailable = getAvailableSpace(m_sDefaultReleasePath.c_str());

          if (diskSizeRequired < fileSize)
          {
            LOG_VError( worker, "Disk size requirements computation failed [%lu]. Should be two times the file size [%lu]. Discarded!",
                        diskSizeRequired, fileSize );

            sendFileDownloadResponse( UccMessageCloudFileDownloadResponse::CNotEnoughSpaceToPreformOperationCode,
                                      UccMessageCloudFileDownloadResponse::CNotEnoughSpaceToPreformOperationMessage,
                                      uccCloudFileDownload.getMessageId());
            return;
          }

          if ((diskSizeAvailable < 1) || (diskSizeAvailable < diskSizeRequired))
          {
            LOG_VError( worker, "Insufficient disk space for the download process [%lu]. At least required [%lu]. Discarded!",
                        diskSizeAvailable, diskSizeRequired );

            sendFileDownloadResponse( UccMessageCloudFileDownloadResponse::CNotEnoughSpaceToPreformOperationCode,
                                      UccMessageCloudFileDownloadResponse::CNotEnoughSpaceToPreformOperationMessage,
                                      uccCloudFileDownload.getMessageId());
            return;
          }
        }

        {
          // Save the current request data for further download steps
          m_sMessageId = uccCloudFileDownload.getMessageId();
          m_sDeviceId = uccCloudFileDownload.getDeviceId();
          m_sTrackingNumber = uccCloudFileDownload.getPayload().get(UccMessageCloudFileDownload::CTrackingNumberName).asCString();
          m_sUrl = uccCloudFileDownload.getPayload().get(UccMessageCloudFileDownload::CUrlName).asCString();
          m_iFileSize = uccCloudFileDownload.getPayload().get(UccMessageCloudFileDownload::CFileSizeName).asInteger();
          m_sChecksumType = uccCloudFileDownload.getPayload().get(UccMessageCloudFileDownload::CChecksumTypeName).asCString();
          m_sChecksum = uccCloudFileDownload.getPayload().get(UccMessageCloudFileDownload::CChecksumName).asCString();
        }

        {
          // Save the specified release folder path.
          std::string fileName;
          std::string::size_type pos = m_sUrl.find_last_of("/");
          if (pos >= 0)
          {
            fileName = m_sUrl.substr(pos + 1);
          }

          m_sPath  = m_sDefaultReleasePath;
          m_sPath += Poco::Path::separator();
          m_sPath += fileName;
        }

        if( startSodaDownload() ) // try to send receiveFromSODA command to ucc
        {
          processFlowEvent( eDownloadRequest ); // Change to "Download Requested" state.
        }
        else // could not send internal Download Request to UCC due session error.
        {
          clearReplicatedFieldsFromRequest();
        }

      }
      else // file download request from cloud has parse errors
      {
        // at least try to reply with the same Ids, if they can be obtained
        LOG_VError(worker, "UccMessageCloudFileDownload Error [%s].\n", "Message parsing failed for file download command failed.");
        sendFileDownloadResponse( UccMessageCloudFileDownloadResponse::CInvalidPayloadCode,
                                  UccMessageCloudFileDownloadResponse::CInvalidPayloadMessage,
                                  uccCloudFileDownload.getMessageId());
      }
    }

    void SpotReleaseDownloadWorker::doStartFileDownload(const AbsOptCloudClientWorker::BufferType& data)
    {
      bool blOk = true;
      ScopeLock    lock( m_mutex ); // exclusive use of member variables and status.

      if( m_eState != eDownloadRequested ) // ignore events out of sequence.
      {
        LOG_VError( worker, "Unexpected message [ReceiveFileFromSODA reply] while in status [%s]. Discarded!",
                    CapcStateLegends[ m_eState ] );
        return;
      }

      LOG_Notice(worker, "doStartFileDownload.");

      UccMessageFileDownloadResponse   uccFileDownloadRs; // the request file answer from UCC

      while( blOk ) // only one cycle, no loop
      {
        if( ! uccFileDownloadRs.parse(data) )
        {
          m_statusMessage = "invalid payload.";
          LOG_VWarn( worker,
                     "Could not parse receiveFileFromCloud reply from UCC [%s] \n",
                     m_statusMessage.c_str() );

          blOk = false;
          break; // UCC reply parse error. Back to initial state.
        }

        if( uccFileDownloadRs.getDeviceId() != m_sDeviceId )
        {
          std::string  sDeviceId = uccFileDownloadRs.getDeviceId();
          m_statusMessage = "UCC Receive File from cloud Reply has unexpected device id ";
          const char * pcOriginalOne = ". Original one was ";

          LOG_VWarn( worker, "%s[%s]%s[%s] \n", m_statusMessage.c_str(),
                     sDeviceId.c_str(), pcOriginalOne, m_sDeviceId.c_str() );

          m_statusMessage += sDeviceId;
          m_statusMessage += pcOriginalOne;
          m_statusMessage += m_sDeviceId;
          blOk = false;
          break; // device ID from UCC reply is wrong. Abort! . Back to initial state.
        }

        if( ! uccFileDownloadRs.getPayload().get(UccMessageFileDownloadResponse::CSuccessName).asBoolean() )
        {
          int nError = uccFileDownloadRs.getPayload().get(UccMessageFileDownloadResponse::CCodeName).asInteger();
          std::string sError = uccFileDownloadRs.getPayload().get(UccMessageFileDownloadResponse::CMessageName).asCString();
          m_statusMessage = "UCC Receive File from cloud Response received  with error ";
          LOG_VWarn(worker, "%s[%d] %s \n", m_statusMessage.c_str(), nError, sError.c_str());

          m_statusMessage += nError;
          m_statusMessage += ", \"";
          m_statusMessage += sError;
          m_statusMessage += "\" ";
          blOk = false; // UCC refused start downloading file. Back to initial state.
        }

        break; // no loop, only one cycle
      }

      if( blOk ) // receiveFileFromSODA reply ( internal FileDownload response from UCC ) is OK
      {
        InProgressCallback callback( *this, & SpotReleaseDownloadWorker::verifyDownloadActivity );

        sendFileDownloadResponse(); // response to cloud, file download was accepted.
        processFlowEvent( eRcvFileFromCloudReplyOk ); // Change to "Downloading From Cloud" state.
        LOG_Trace(worker, "doStartFileDownload - starting timer.");
        m_timerDownloading.stop(); // Make sure previous callback is erased.
        m_timerDownloading.start( callback ); // Start the timer for verifying further "in progress" messages.
        LOG_Trace(worker, "doStartFileDownload - started.");
      }
      else
      {
        sendFailureStatusEvent();
        clearReplicatedFieldsFromRequest();
        processFlowEvent( eOtherFatalError ); // Fatal error. Back to initial state.
      }
    }

    void SpotReleaseDownloadWorker::processCloudStatusEventResponse( const AbsOptCloudClientWorker::BufferType& data)
    {
      bool blParseOk = false;
      std::string sMessageJson;
      UccMessageCloudFileDownloadStatusEventResponse  uccCloudFileDownloadStatusReply;

      blParseOk = uccCloudFileDownloadStatusReply.parse(data);
      uccCloudFileDownloadStatusReply.getJson().serialize(sMessageJson);

      if( blParseOk )
      {
        LOG_VTrace( worker,
                    "Received a status reply from the UCC: \n%s\n\n",
                    sMessageJson.c_str() );
      }
      else
      {
        LOG_VError( worker,
                    "UccMessageCloudFileDownloadStatusEventResponse Error [Message parse error]:\n%s\n\n",
                    sMessageJson.c_str() );
      }
    }

    void SpotReleaseDownloadWorker::doHandleFileDownload(const AbsOptCloudClientWorker::BufferType& data)
    {
      if( m_eState != eDownloadingFromCloud ) // ignore events out of sequence.
      {
        LOG_VError( worker, "Unexpected message [ReceiveFileFromSODA status] while in status [%s]. Discarded!",
                    CapcStateLegends[ m_eState ] );
        return;
      }

      bool         blOk = true;
      bool         blDownloadCompleted = false;
      int          iBytesRcv = 0;    // received bytes so far,
      int          iTotalBytes = 0;  // total bytes to receive,
      std::string  sStatus;  // file download status
      std::string  sError;   // the possible error description.

      // Process receiveFileFromSODA (file download) status notifications from UCC.

      m_mutex.lock(); // exclusive use of member variables and status.

      LOG_Trace(worker, "doHandleFileDownload - reseting timer.");
      m_timerDownloading.restart(CInDownloadInterval); // restart the watchdog timer for another period
      LOG_Trace(worker, "doHandleFileDownload - timer reset.");

      UccMessageFileDownloadStatusEvent   uccFileDownloadStatusEvent; // The file request status event from UCC

      while( blOk ) // only one cycle, no loop
      {
        if( ! uccFileDownloadStatusEvent.parse(data) )
        {
          LOG_Warn( worker, "Could not parse receiveFileFromCloud status from UCC.");

          break; // This situation is very weird and is ignored.
        }

        if( uccFileDownloadStatusEvent.getDeviceId() != m_sDeviceId )
        {
          std::string  sDeviceId = uccFileDownloadStatusEvent.getDeviceId();
          m_statusMessage = "UCC Receive File from cloud Reply has unexpected device id ";
          const char * pcOriginalOne = ". Original one was ";

          LOG_VWarn( worker, "%s[%s]%s[%s] \n", m_statusMessage.c_str(),
                     sDeviceId.c_str(), pcOriginalOne, m_sDeviceId.c_str() );

          m_statusMessage += sDeviceId;
          m_statusMessage += pcOriginalOne;
          m_statusMessage += m_sDeviceId;
          blOk = false;
          break; // device ID from UCC status is wrong. Abort! . Back to initial state.
        }

        std::string trackingnum( uccFileDownloadStatusEvent.getPayload().get(UccMessageFileDownloadStatusEvent::CTrackingNumberName).asCString() );

        if( trackingnum.compare(m_sTrackingNumber) != 0 )
        {
          LOG_VWarn( worker,
                    "UCC Receive File from cloud Reply has an unexpected tracking number [%s]. Original one was [%s]\n",
                    trackingnum.c_str(), m_sTrackingNumber.c_str() );

          // tracking number from UCC status is wrong. Just logging, no aborting.
        }

        sStatus = uccFileDownloadStatusEvent.getPayload().get(UccMessageFileDownloadStatusEvent::CStatusName).asCString();

        if( sStatus == UccMessageCloudFileDownloadStatusEvent::getStatusLegend(UccMessageCloudFileDownloadStatusEvent::eComplete) )
        {
          iBytesRcv = uccFileDownloadStatusEvent.getPayload().get(UccMessageFileDownloadStatusEvent::CCurrentSizeName).asInteger();
          iTotalBytes = uccFileDownloadStatusEvent.getPayload().get(UccMessageFileDownloadStatusEvent::CFileSizeName).asInteger();

          if( iBytesRcv != iTotalBytes )
          {
            LOG_VWarn( worker,
                       "Download is reported \"%s\", but the received bytes are %d of %d originally requested.\n",
                       sStatus.c_str(), iBytesRcv, iTotalBytes);
          }

          blDownloadCompleted = true;
        }
        else if( sStatus == UccMessageCloudFileDownloadStatusEvent::getStatusLegend(UccMessageCloudFileDownloadStatusEvent::eTransferFailure) )
        {
          int nError = uccFileDownloadStatusEvent.getPayload().get(UccMessageFileDownloadStatusEvent::CCodeName).asInteger();
          std::string sError = uccFileDownloadStatusEvent.getPayload().get(UccMessageFileDownloadStatusEvent::CMessageName).asCString();
          m_statusMessage = "File Download finished  with error ";
          LOG_VError( worker, "%s[%d] %s \n", m_statusMessage.c_str(), nError, sError.c_str() );

          m_statusMessage += nError;
          m_statusMessage += ", \"";
          m_statusMessage += sError;
          m_statusMessage += "\" ";
          blOk = false; // UCC reported error downloading file. Back to initial state.
        }
        else if( sStatus == UccMessageCloudFileDownloadStatusEvent::getStatusLegend(UccMessageCloudFileDownloadStatusEvent::eStart) ||
                 sStatus == UccMessageCloudFileDownloadStatusEvent::getStatusLegend(UccMessageCloudFileDownloadStatusEvent::eInProgress)
               )
        {
          if( sStatus == UccMessageCloudFileDownloadStatusEvent::getStatusLegend(UccMessageCloudFileDownloadStatusEvent::eStart) )
          {
            const char * pcMsg = "Start downloading software bundle.";
            m_statusMessage = pcMsg;
            LOG_Info( worker, pcMsg );
            sendStartStatus();
          }
          else // in-progress notification
          {
            if( ! m_blInProgress ) enterInProgressStatus( "Downloading software bundle." );
          }

          iBytesRcv = uccFileDownloadStatusEvent.getPayload().get(UccMessageFileDownloadStatusEvent::CCurrentSizeName).asInteger();
          iTotalBytes = uccFileDownloadStatusEvent.getPayload().get(UccMessageFileDownloadStatusEvent::CFileSizeName).asInteger();
          if( iTotalBytes != m_iFileSize /* && sStatus == "start" */)
          {
            LOG_VWarn( worker,
                       "receiveFileFromCloud status says file size = %d, but cloud requested %d bytes !!\n",
                       iTotalBytes, m_iFileSize );
          }

          LOG_VNotice( worker,
                      "Download status %s. Bytes received %d of %d \n", sStatus.c_str(), iBytesRcv, iTotalBytes);
        }
        else // unknown field status
        {
          LOG_VWarn( worker, "Unknown receiveFileFromCloud status from UCC. status = \"%s\"\n", sStatus.c_str() );
        }

        break; // no loop, only one cycle
      }

      m_mutex.unlock();

      if( blOk )
      {
      }
      else // Some aborting sequence error has arisen.
      {
        m_mutex.lock();
        leaveInProgressStatus(); // error message already set above
        sendFailureStatusEvent();
        clearReplicatedFieldsFromRequest();
        m_timerDownloading.stop();
        processFlowEvent( eOtherFatalError ); // Fatal error. Back to initial state.
        m_mutex.unlock();
      }

      if( blDownloadCompleted )
      {
        const char * pcmsg = "Software bundle download is completed.";
        m_mutex.lock();
        setInProgressMessage( pcmsg, true);
        processFlowEvent(eRcvFileFromCloudStatusComplete); // Change to "Checking File" state.
        m_timerDownloading.stop();
        LOG_Info( worker, pcmsg );
        m_mutex.unlock();

        std::string       sRootBundleFolder;
        SPOTFileDownload  downloadFlow( *m_pSpotSession );

        blOk = processDownloadedFile(downloadFlow, sRootBundleFolder);

        if( blOk ) downloadBundleToSPOT(downloadFlow, sRootBundleFolder);
      }
    }

    bool SpotReleaseDownloadWorker::processDownloadedFile( SPOTFileDownload & downloadFlow,
                                                           std::string & sFolder )
    {
      bool blOk = true;
      bool blChecksumError = false;
      std::string sError; // the possible error description.

      while( blOk ) // not actually a loop, it is executed once
      {
        LOG_Trace(worker, "Checking software bundle integrity.");

        {
          std::string checksumType;
          if (this->m_sChecksumType.compare(CSupportedChecksumTypeBySODA) == 0)
          {
            checksumType = CSHA256;
          }

          if ( validateChecksum(this->m_sPath, checksumType, this->m_sChecksum, sError) == true )
          {
            LOG_VTrace(worker, "Checksum validation for '%s' was successful.", this->m_sPath.c_str());
          }
          else
          {
            blOk = false;
            blChecksumError = true;
            sError.insert(0, "Checksum validation failed - ");
            LOG_VError(worker, "Checksum validation for '%s' failed - [%s].", this->m_sPath.c_str(), sError.c_str());
            break;
          }
        }

        m_mutex.lock();
        setInProgressMessage( "Decompressing bundle." );
        LOG_Trace(worker, "Decompressing bundle.");
        processFlowEvent( eChecksumOK ); // Change to "Decompressing File" state.
        m_mutex.unlock();

        blOk = downloadFlow.decompressRelease(this->m_sPath,m_sDefaultReleasePath);

        // Archive release bundle removal. It is not needed anymore and takes room from file system.
        Poco::File bundleFile(m_sPath);
        bundleFile.remove(false);

        if(!blOk)
        {
          sError = "Error decompressing bundle.";
          LOG_VError(worker, "%s\n", sError.c_str());
          break;
        }

        blOk = downloadFlow.checkAndGetResourceFolder(sFolder);
        if(!blOk)
        {
          sError  = CpcPHError;
          sError += "Decompressed resource folder [ ";
          sError += sFolder;
          sError += " ] not found.";
          LOG_VError(worker, "%s\n", sError.c_str());
        }

        break; // just only once iteration
      }

      if( blOk )
      {
        processFlowEvent( eDecompressOk ); // Change to "Downloading to SPOT" state.
        return true;
      }

      m_mutex.lock();
      leaveInProgressStatus( sError.c_str() );
      sendFailureStatusEvent();
      clearReplicatedFieldsFromRequest();
      m_mutex.unlock();

      if( blChecksumError )
      {
        processFlowEvent( eChecksumError ); // Checksum error. Back to initial state.
      }
      else // decompression error
      {
        processFlowEvent( eDecompressError ); // Decompression error. Back to initial state.
      }

      return false;
    }

    bool SpotReleaseDownloadWorker::downloadBundleToSPOT( SPOTFileDownload & downloadFlow,
                                                          const std::string & sFolder )
    {
      bool blOk = true;
      std::string sError; // the possible error description.

      while(blOk) // not actually a loop, it is executed once
      {
    	const char* pcMsg = "Downloading the new software bundle into SPOT.";
        m_mutex.lock();
        setInProgressMessage( pcMsg, true);
        LOG_Info(worker, pcMsg);
        m_mutex.unlock();

        LOG_Notice(worker, "Downloading debians to SPOT.");
        blOk = downloadFlow.downloadDebians();
        if(!blOk)
        {
          sError  = CpcPHError;
          sError += "Error downloading debian packages.";
          break;
        }

        LOG_Notice(worker, "Downloading ROMfs to SPOT.");
        blOk = downloadFlow.downloadSpotPackages();
        if(!blOk)
        {
          sError  = CpcPHError;
          sError += "Error downloading ROMfs packages.";
          break;
        }

        LOG_Trace(worker, "Removing temporary directory.");
        downloadFlow.removeTemporaryFolder();
        break; // just only once iteration
      }

      LOG_Debug(worker, "SPOT release download finished OK.");

      m_mutex.lock();
      // Set the legend for presumed OK flow. If not, it will be overridden later.
      leaveInProgressStatus( "File download completed successfully." );
      m_mutex.unlock();

      if( blOk ) // Every stage went OK. No errors have arisen.
      {
        LOG_VInfo(worker, "Release %s downloaded OK! \n", m_sPath.data());
        sendSuccessStatusEvent();
        clearReplicatedFieldsFromRequest();
        processFlowEvent( eSPOTDownloadOk ); // Change to "Download OK" state.
      }
      else // errors have arisen.
      {
        if (m_pSpotSession->getError() != SpotSession::SpotSessionError::CNoError)
        {
          m_statusMessage = "Some message or information block could not be sent to SPOT.";
          LOG_VError(worker, "SpotSession Error [%s].\n", m_pSpotSession->getErrorDescription().c_str());
        }
        else
        {
          m_statusMessage  = "Error downloading software into SPOT - ";
          m_statusMessage += sError;
          LOG_VError(worker, "Release Download or Process Error ***  Flow error =  [%04X h].\n", downloadFlow.getLastOperationError());
        }

        sendFailureStatusEvent();
        processFlowEvent( eSPOTDownloadError ); // Download error. Back to initial state.
      }

      clearReplicatedFieldsFromRequest(); // clear data for the next download
      return blOk;
    }

    bool SpotReleaseDownloadWorker::startSodaDownload()
    {
      bool       blRet  = false;
      ErrorType  bError = 0;
      LOG_Trace(worker, "startSodaDownload.");

      UccMessageFileDownload fileRcvFromSODA;
      fileRcvFromSODA.generateMessageId();
      fileRcvFromSODA.setDeviceId(this->m_sDeviceId);
      gvr::utils::Json jsonSnd = fileRcvFromSODA.getJson();
      jsonSnd.set(UccMessageFileDownload::CPayloadName).set(UccMessageFileDownload::CUrlName, m_sUrl);
      jsonSnd.set(UccMessageFileDownload::CPayloadName).set(UccMessageFileDownload::CPathName, m_sPath);
      jsonSnd.set(UccMessageFileDownload::CPayloadName).set(UccMessageFileDownload::CFileSizeName, m_iFileSize);
      jsonSnd.set(UccMessageFileDownload::CPayloadName).set(UccMessageFileDownload::CTrackingNumberName, m_sTrackingNumber);
      jsonSnd.set(UccMessageFileDownload::CPayloadName).set(UccMessageFileDownload::CChecksumTypeName, m_sChecksumType);
      jsonSnd.set(UccMessageFileDownload::CPayloadName).set(UccMessageFileDownload::CChecksumName, m_sChecksum);
      fileRcvFromSODA.setJson(jsonSnd);
      if( (bError = m_pUccSession->send(fileRcvFromSODA) ) == UccSession::UccSessionError::CNoError )
      {
        blRet = true;
        std::string jsonMessage;

        fileRcvFromSODA.getJson().serialize(jsonMessage);
        LOG_VDebug(worker, "File download request from soda has been sent [\n%s\n].\n", jsonMessage.c_str());
      }
      else
      {
        LOG_VCritical( worker, "Could not send File Download request to UCC. Session error: %u\n",
                       static_cast<unsigned int>(bError) );
      }

      return blRet;
    }

    bool SpotReleaseDownloadWorker::sendFileDownloadResponse( int iError /* = 0 */,
                                                              const char * pcError /* = NULL */,
                                                              std::string const & messageIdAux )
    {
      bool blRet = false;
      LOG_Trace(worker, "Try to send file download response.");

      {
        std::string sJsonMessage;

        //Send a file download response
        {
          UccMessageCloudFileDownloadResponse  uccCloudFileDownloadResponse;
          uccCloudFileDownloadResponse.setMessageId(((messageIdAux.empty())?(m_sMessageId):(messageIdAux)));
          uccCloudFileDownloadResponse.setSuccess( iError == 0 ? true : false );
          gvr::utils::Json jsonSnd = uccCloudFileDownloadResponse.getJson();

          if( iError == 0 ) // success reply was ordered to be sent.
          {
            jsonSnd.set(UccMessageCloudFileDownloadResponse::CPayloadName).set(UccMessageCloudFileDownloadResponse::CLocalUrlName, m_spotIp + m_sPath);
          }
          else // some error reply was ordered to be sent.
          {
            if( pcError == NULL )
            {
              pcError = "Unable to execute command"; // this is the explanation by default.
            }

            jsonSnd.set(UccMessageCloudFileDownloadResponse::CCodeName, iError);
            jsonSnd.set(UccMessageCloudFileDownloadResponse::CMessageName, pcError);
          }

          uccCloudFileDownloadResponse.setJson(jsonSnd);
          {
            uccCloudFileDownloadResponse.getJson().serialize(sJsonMessage);
            LOG_VTrace(worker, "Sending a file download response to the cloud [\n%s\n].\n", sJsonMessage.c_str());
          }

          // Send a file download cmd response.
          ErrorType error = m_pUccSession->send(uccCloudFileDownloadResponse);
          if( error == 0 )
          {
            blRet = true;
            LOG_Trace(worker, "File download reply was successfully sent to the cloud.");
          }
          else
          {
            LOG_VCritical( worker, "File download reply could not be sent!! Session error %d\n",
                           static_cast<int>(error) );
          }
        }
      }

      return blRet;
    }

    void SpotReleaseDownloadWorker::clearReplicatedFieldsFromRequest()
    {
      // Clear the current request data for another download cycle.
      m_iFileSize = 0;
      m_sMessageId.clear();
      m_sDeviceId.clear();
      m_sTrackingNumber.clear();
      m_sUrl.clear();
      m_sChecksumType.clear();
      m_sChecksum.clear();
    }

    void SpotReleaseDownloadWorker::setInProgressMessage( const char* pcLegend, bool blSend /* = false */)
    {
      if( pcLegend == NULL )
      {
        m_statusMessage.clear();
      }
      else
      {
        m_statusMessage = pcLegend;
      }

      if( ! blSend ) return; // work done

      m_timerInProgress.restart( CInProgressInterval );
      sendStatusEvent( UccMessageCloudFileDownloadStatusEvent::eInProgress );
    }

    void SpotReleaseDownloadWorker::notifyProgressToCloud(Poco::Timer &)
    {
      ScopeLock lock( m_mutex );

      if( ! m_blInProgress )
      {
        LOG_Trace(worker, "Nothing to do because \"In Progress\" status finished.");
        return;
      }

      sendInProgressStatusEvent();
    }

    void SpotReleaseDownloadWorker::verifyDownloadActivity(Poco::Timer &)
    {
      ScopeLock lock( m_mutex );

      if( m_eState == eDownloadingFromCloud )
      {
        leaveInProgressStatus("timeout expired");
        sendTransferFailureStatusEvent();
        LOG_VError( worker, "%s - Download Aborted.!!\n", m_statusMessage.c_str() );
        clearReplicatedFieldsFromRequest();
        processFlowEvent( eRcvFileFromCloudStatusError ); // Receive file timeout. Back to initial state.
        m_timerDownloading.restart(0); // valid self-kill timer
        LOG_VTrace( worker, "%s - Timer stopped, back in Initial state.\n", m_statusMessage.c_str() );
      }
    }

    void SpotReleaseDownloadWorker::sendStartStatus()
    {
      sendStatusEvent( UccMessageCloudFileDownloadStatusEvent::eStart );
    }

    void SpotReleaseDownloadWorker::sendInProgressStatusEvent()
    {
      sendStatusEvent( UccMessageCloudFileDownloadStatusEvent::eInProgress );
    }

    void SpotReleaseDownloadWorker::sendSuccessStatusEvent()
    {
      sendStatusEvent( UccMessageCloudFileDownloadStatusEvent::eComplete );
    }

    void SpotReleaseDownloadWorker::sendTransferFailureStatusEvent()
    {
      sendStatusEvent( UccMessageCloudFileDownloadStatusEvent::eTransferFailure );
    }

    void SpotReleaseDownloadWorker::sendFailureStatusEvent()
    {
      sendStatusEvent( UccMessageCloudFileDownloadStatusEvent::eFailure );
    }

    void SpotReleaseDownloadWorker::sendStatusEvent(UccMessageCloudFileDownloadStatusEvent::EDownloadEventStatus e)
    {
      UccMessageCloudFileDownloadStatusEvent  statusEvent;
      statusEvent.generateMessageId();
      statusEvent.setDeviceId( m_sDeviceId );

      gvr::utils::Json jsonAll = statusEvent.getJson();
      gvr::utils::Json jsonPld = jsonAll.set(UccMessageCloudFileDownloadStatusEvent::CPayloadName);

      jsonPld.set( UccMessageCloudFileDownloadStatusEvent::CFileTypeName, CSwBundle );
      jsonPld.set( UccMessageCloudFileDownloadStatusEvent::CTrackingNumberName, m_sTrackingNumber );
      jsonPld.set( UccMessageCloudFileDownloadStatusEvent::CStatusName,
                   UccMessageCloudFileDownloadStatusEvent::getStatusLegend(e) );
      jsonPld.set( UccMessageCloudFileDownloadStatusEvent::CMessageName, m_statusMessage );

      jsonAll.set(UccMessageCloudFileDownloadStatusEvent::CPayloadName, jsonPld);
      statusEvent.setJson( jsonAll );
      m_pUccSession->send( statusEvent );
    }

    void SpotReleaseDownloadWorker::enterInProgressStatus( const char * pcStatusMessage /* = NULL */ )
    {
      if( m_blInProgress )
      {
        LOG_Warn(worker, "Attempt to enter in \"In Progress\" status AGAIN!");
        return;
      }

      if( pcStatusMessage != NULL )
      {
        m_statusMessage = pcStatusMessage;
      }

      m_blInProgress = true; // Enter "in progress" status

      sendStatusEvent( UccMessageCloudFileDownloadStatusEvent::eInProgress ); // Send the first "in progress" message.

      InProgressCallback callback( *this, & SpotReleaseDownloadWorker::notifyProgressToCloud );
      m_timerInProgress.setPeriodicInterval(CInProgressInterval);
      m_timerInProgress.setStartInterval(0);
      m_timerInProgress.start( callback ); // Start the timer for sending further "in progress" messages.
    }

    void SpotReleaseDownloadWorker::leaveInProgressStatus( const char * pcStatusMessage /* = NULL */)
    {
      if( ! m_blInProgress )
      {
        LOG_Warn(worker, "Attempt to leave in \"In Progress\" status AGAIN!");
        return;
      }

      m_blInProgress = false; // Leave "in progress" status

      if( pcStatusMessage != NULL ) // exit "in progress" future message field
      {
        m_statusMessage = pcStatusMessage;
      }

      LOG_Trace(worker, "leaveInProgressStatus - stopping cloud notificator timer.");
      m_timerInProgress.stop();
      LOG_Trace(worker, "leaveInProgressStatus - cloud notificator timer stopped.");
    }

    bool SpotReleaseDownloadWorker::processFlowEvent ( SpotReleaseDownloadWorker::EFlowEvent evt )
    {
      bool blRet = false;

      switch( m_eState )
      {
        case eInitial:
          switch( evt )
          {
            case eDownloadRequest:
              performStateChange( eDownloadRequest, eDownloadRequested );
              blRet = true;
            break;

            default:
              logUnexpectedFlowEvent( evt, eInitial );
            break;
          }
        break;

        case eDownloadRequested:
          switch( evt )
          {
            case eRcvFileFromCloudReplyOk:
              performStateChange( eRcvFileFromCloudReplyOk, eDownloadingFromCloud );
              blRet = true;
            break;

            case eRcvFileFromCloudReplyError:
            case eOtherFatalError:
              performStateChange( evt, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eDownloadRequested );
            break;
          }
        break;

        case eDownloadingFromCloud:
          switch( evt )
          {
            case eRcvFileFromCloudStatusComplete:
              performStateChange( eRcvFileFromCloudStatusComplete, eCheckingFile );
              blRet = true;
            break;

            case eRcvFileFromCloudStatusError:
            case eOtherFatalError:
              performStateChange( evt, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eDownloadingFromCloud );
            break;
          }
          break;

        case eCheckingFile:
          switch( evt )
          {
            case eChecksumOK:
              performStateChange( eChecksumOK, eDecompressingFile );
              blRet = true;
            break;

            case eChecksumError:
              performStateChange( eChecksumError, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eCheckingFile );
            break;
          }
          break;

        case eDecompressingFile:
          switch( evt )
          {
            case eDecompressOk:
              performStateChange( eDecompressOk, eDownloadingToSpot );
              blRet = true;
            break;

            case eDecompressError:
              performStateChange( eDecompressError, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eDecompressingFile );
            break;
          }
          break;

        case eDownloadingToSpot:
          switch( evt )
          {
            case eSPOTDownloadOk:
              performStateChange( eSPOTDownloadOk, eDownloadOK );
              blRet = true;
            break;

            case eSPOTDownloadError:
            case eOtherFatalError:
              performStateChange( evt, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eDownloadingToSpot );
            break;
          }
          break;

        case eDownloadOK:
          switch( evt )
          {
            case eDownloadRequest:
              // This will not be allowed once this worker collaborates with Activation (Install) worker.
              // Then, an external event like eInstallationOK will be expected to reset this state machine
              // to the initial state eInitial, and then, continue to accept new downloads.
              performStateChange( eDownloadRequest, eDownloadRequested );
              blRet = true;
            break;

            case eOtherFatalError:
              // This will be the case of "Install Error" in the future
              performStateChange( evt, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eDownloadOK );
            break;
          }
          break;
      }

      return blRet;
    }

    SpotReleaseDownloadWorker::EFlowState
    SpotReleaseDownloadWorker::performStateChange ( SpotReleaseDownloadWorker::EFlowEvent ev,
                                                    SpotReleaseDownloadWorker::EFlowState estat )
    {
      const char * pcChangePrefix = "";

      if( m_eState != estat )
      {
        pcChangePrefix = "Flow state CHANGED. ";
      }

      LOG_VTrace(worker, "%sEvent: %s\n  [%s] ==> [%s]\n",
                 pcChangePrefix, CapcEventLegends[ ev ],
                 CapcStateLegends[ m_eState ], CapcStateLegends[ estat ] );

      m_eState = estat;
      return estat;
    }

    void SpotReleaseDownloadWorker::logUnexpectedFlowEvent( SpotReleaseDownloadWorker::EFlowEvent ev,
                                                            SpotReleaseDownloadWorker::EFlowState estat )
    {
      bool blChange = ( m_eState != estat );
      char acWhatHappened[80];

      sprintf( acWhatHappened, "Unexpected event [%s] while in status [%s].",
               CapcEventLegends[ ev ], CapcStateLegends[ m_eState ] );

      if( blChange )
      {
        LOG_VError( worker, "%s Flow state is reset to [%s]\n",
                    acWhatHappened, CapcStateLegends[ estat ] );
      }
      else // no state change due unexpected event.
      {
        LOG_VWarn( worker, "%s Ignored!\n", acWhatHappened );
      }
    }

    void SpotReleaseDownloadWorker::traceNoStateChanged( EFlowEvent ev )
    {
      LOG_VTrace( worker, "Received event [%s] in state [%s] . No state changes.\n",
                  CapcEventLegends[ ev ], CapcStateLegends[ m_eState ] );
    }

    bool SpotReleaseDownloadWorker::validateChecksum (std::string path, std::string checksumType, std::string checksum, std::string & errorMessage)
    {
      bool result = false;
      if ((checksumType.empty() == false) &&
          ((checksumType.compare(CMD5) == 0) ||
           (checksumType.compare(CSHA1) == 0) ||
           (checksumType.compare(CSHA256) == 0) ||
           (checksumType.compare(CSHA512) == 0)))
      {
        if (path.empty() == false)
        {
          Poco::Crypto::initializeCrypto();
          try
          {
            Poco::Crypto::DigestEngine engine(checksumType);
            std::ifstream file(path.c_str(), std::ios::binary);
            if (file.is_open())
            {
              static int const bufferSize = 8192;
              static char      bufferMemory [bufferSize];
              file.seekg (0,std::ios::beg);
              while (file.eof() == false)
              {
                file.read(bufferMemory,bufferSize);
                engine.update(bufferMemory,file.gcount());
              }
              file.close();

              {
                std::string checksumComputed = Poco::Crypto::DigestEngine::digestToHex(engine.digest());
                if (checksumComputed == checksum)
                {
                  errorMessage.clear();
                  result = true;
                }
                else
                {
                  std::ostringstream oss;
                  oss << "Couldn't validate downloaded file '" << path << "' checksum." << std::endl;
                  oss << "\tComputed checksum [" << checksumComputed << "]." << std::endl;
                  oss << "\tExpected checksum [" << checksum << "]." << std::endl;
                  errorMessage = oss.str();
                }
              }
            }
            else
            {
              std::ostringstream oss;
              oss << "Couldn't open downloaded file '" << path << "' for checksum validation." << std::endl;
              errorMessage = oss.str();
            }
          }
          catch (Poco::NotFoundException & e)
          {
            std::ostringstream oss;
            oss << "Couldn't initialize crypto engine while performing '" << path << "' checksum validation." << std::endl;
            errorMessage = oss.str();

          }
          Poco::Crypto::uninitializeCrypto();
        }
        else
        {
          std::ostringstream oss;
          oss << "Filename for checksum validation is empty." << std::endl;
          errorMessage = oss.str();
        }
      }
      else
      {
        std::ostringstream oss;
        oss << "Checksum type '" << checksumType << "' unsupported." << std::endl << " Supported types are: [ " << CMD5 << ", " << CSHA1 << ", " << CSHA256 << ", " << CSHA512 << " ]" << std::endl;
        errorMessage = oss.str();
      }
      return result;
    }
  }
}
