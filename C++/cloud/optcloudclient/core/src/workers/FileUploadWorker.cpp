/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileUploadWorker.cpp
 * @author gparis
 * @date Monday, June 08, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#include <sstream>
#include <fstream>

//#include <Poco/LocalDateTime.h>
#include <Poco/DateTimeParser.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Crypto/DigestEngine.h>
#include <Poco/Path.h>
#include <Poco/File.h>

#include <gvr/communication/ucc/message/UccMessageCloudFileUpload.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileUploadResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadResponse.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileUploadStatusEventResponse.h>
#include <gvr/communication/ucc/message/UccMessageSendToCloud.h>
#include <gvr/communication/ucc/message/UccMessageSendToCloudResponse.h>
#include <gvr/communication/ucc/message/UccMessageSendToCloudStatusEvent.h>

#include <gvrlog/log.h>
#include <workers/FileUploadWorker.h>

DECLARE_LOG_MODULE(worker);

/// @brief Gilbarco Domain
namespace gvr
{
  /// @brief Communication Domain
  namespace cloud
  {


    using namespace gvr::communication::ucc;
    using namespace gvr::communication::spot;

    typedef Poco::TimerCallback<FileUploadWorker> InProgressCallback;
    typedef Poco::Timer               Timer;
    typedef Poco::FastMutex           Mutex;
    typedef Poco::ScopedLock<Mutex>   ScopeLock; ///< @brief Automatic scope life mutex.

    const char* FileUploadWorker::CapcFileCategories[ eTotalFileCategories ] =
    {
      "log", "configuration"
    };

    char const * FileUploadWorker::CSODA_SHA256 = "SHA 256";
    char const * FileUploadWorker::CSHA256 = "SHA256";
    char const * FileUploadWorker::CSupportedChecksumTypeBySODA = FileUploadWorker::CSODA_SHA256;

    const char * FileUploadWorker::CpcDateFormat = "%Y-%m-%d";
    const char * FileUploadWorker::CpcTimestampInArchiveName = "_%Y%m%d_%H%M%S";
    const char * FileUploadWorker::CpcDefaultZipExtension = ".zip";
    const char * FileUploadWorker::CpcDefaultCompressionType = "zip";
    const char * FileUploadWorker::CpcPHError = "Internal PH error. ";
    const char * FileUploadWorker::CpcUnableToExecuteCmd = "Unable to execute command";
    const char * FileUploadWorker::CpcInvalidFileType = "Unsupported file type: ";


    const char* FileUploadWorker::CapcEventLegends[ FileUploadWorker::eTotalEvents ] =
    {
      "eUploadRequest",
      "eUploadStarted",
      "eSPOTUploadOk",
      "eSPOTUploadError",
      "eCompressOk",
      "eCompressError",
      "eChecksumError",
      "eSendFileToCloudRequested",
      "eSendFileToCloudReplyOk",
      "eSendFileToCloudReplyError",
      "eSendFileToCloudStatusComplete",
      "eSendFileToCloudStatusError",
      "eOtherFatalError"
    };

    const char* FileUploadWorker::CapcStateLegends[ FileUploadWorker::eTotalStates ] =
    {
      "Initial",
      "UploadRequested",
      "UploadingFromSpot",
      "CompressingArchive",
      "GeneratingChecksum",
      "SendFileRequested",
      "UploadingToCloud"
    };


    FileUploadWorker::FileUploadWorker( UccSession & uccSession, SpotSession & spotSession,
                                        std::string const & uploadDir, std::string const & spotIp,
                                        std::string const & uploadableListFileName )
      : AbsOptCloudClientWorker("FileUploadWorker"),
        m_pUccSession(&uccSession), m_pSpotSession(&spotSession),
        m_blInProgress(false), m_eState(eInitial), m_iFileSize(0),
        m_sUploadFolder(uploadDir),
        m_sUloadableListFile(uploadableListFileName),
        m_sChecksumType(CSupportedChecksumTypeBySODA),
        m_sCompressionType(CpcDefaultCompressionType),
        m_sFileID(1,'0'),
        m_tFromDate(0), m_tToDate(0),
        m_timerInProgress(0, CInProgressInterval),
        m_timerUploading(CInUploadInterval, CInUploadInterval)
    {
      start();

      LOG_Notice(worker, "Starting File Upload Worker...");
    }

    FileUploadWorker::~FileUploadWorker()
    {
      stop();

      LOG_Notice(worker, "Stopping File Upload Worker...");
    }

    void FileUploadWorker::sessionEvent( IUccSessionListener::BufferType const & buffer, IUccSessionListener::TypeId const & typeId, IUccSessionListener::KeyType const & keyType)
    {
      LOG_Trace(worker, "Enqueue a new message notification...");

      enqueueNotification ( typeId, buffer );
    }

    void FileUploadWorker::sessionEvent( IUccSessionListener::StatusType status )
    {
      LOG_Trace(worker, "Enqueue a new status notification...");

      enqueueNotification ( status );
    }

    void FileUploadWorker::doSubscriptions( )
    {
      // Here we need to subscribe to a specific event file upload that will sendd from the uccsession.
      LOG_Debug(worker, "Subscribe to file upload command and event messages...");

      m_pUccSession->subscribe(*this, UccMessageCloudFileUpload::GetTypeId());
      m_pUccSession->subscribe(*this, UccMessageSendToCloudResponse::GetTypeId());
      m_pUccSession->subscribe(*this, UccMessageCloudFileUploadStatusEventResponse::GetTypeId());
      m_pUccSession->subscribe(*this, UccMessageSendToCloudStatusEvent::GetTypeId());
    }

    void FileUploadWorker::doUnsubscriptions( )
    {
      // Here we need to unsubscribe to all events.
      LOG_Trace(worker, "Un-subscribe to all type messages...");

      m_pUccSession->unsubscribe(*this);
    }

    void FileUploadWorker::processNotification( AbsOptCloudClientWorker::TypeId tId, const AbsOptCloudClientWorker::BufferType& data )
    {
      LOG_Trace(worker, "Process event message type notification ...");

      IUccSessionListener::TypeId typeId(tId);

      // Here we need to verify if the notification correspond to UccMessageCloudFileUploadRequest.
      if( typeId == UccMessageCloudFileUpload::GetTypeId() )
      {
        processUploadRequest(data);
      }

      // Here we need to verify if the notification correspond to UccMessageSendToCloudResponse.
      if( typeId == UccMessageSendToCloudResponse::GetTypeId())
      {
        doStartFileUpload(data);
      }

      // Here we need to verify if the notification correspond to UccMessageSendToCloudStatusEvent.
      if( typeId == UccMessageSendToCloudStatusEvent::GetTypeId() )
      {
        doHandleFileUpload(data);
      }

      // Here we need to verify if the notification correspond to UccMessageCloudFileUploadStatusEventResponse.
      if( typeId == UccMessageCloudFileUploadStatusEventResponse::GetTypeId())
      {
        processCloudStatusEventResponse(data);
      }
    }

    void FileUploadWorker::processNotification( IUccSessionListener::StatusType status )
    {
      LOG_VTrace(worker, "New session status event has been received from UccSession [%s].\n", m_pUccSession->getStatusDescription().c_str());
    }

    void FileUploadWorker::processUploadRequest ( const AbsOptCloudClientWorker::BufferType& data )
    {
      m_mutex.lock();

      if( m_eState != eInitial )
      {
        LOG_VError( worker, "Unexpected message [CloudFileUpload] while in status [%s]. Discarded!",
                    CapcStateLegends[ m_eState ] );

        m_mutex.unlock();
        return;
      }

      bool                       blOk = true;
                                               // predetermined error is "invalid payload"
      int                        iError =  UccMessageCloudFileDownloadResponse::CInvalidPayloadCode;
      const char*                pcError = UccMessageCloudFileDownloadResponse::CInvalidPayloadMessage;
      std::string                sError, sLogMessage;

      UccMessageCloudFileUpload  uccCloudFileUpload;
      SPOTFileUpload             uploadFlow( *m_pSpotSession );
      UploadableFileList         supportedUploadableFileList( m_sUloadableListFile,
                                                              m_pSpotSession->getDevicePlatformAcronym() );

      while( blOk ) // #1 only one cycle to check errors, no loop
      {
        if( ! uccCloudFileUpload.parse(data) )
        {
          // at least try to reply with the same Ids, if they can be obtained
          m_sMessageId = uccCloudFileUpload.getMessageId();
          m_sDeviceId = uccCloudFileUpload.getDeviceId();

          sLogMessage = "Cloud File Upload request message parse failed";
          blOk = false; // file upload request from cloud has parse errors
          break;
        }

        int itz = 0; // requeried to retrieve value, but not used
        const std::string    sDateFmt(CpcDateFormat);
        std::string          sJsonMessage, sFrom, sTo;
        Poco::DateTimeParser dtParser;

        {
          uccCloudFileUpload.getJson().serialize(sJsonMessage);
          LOG_VInfo(worker, "Received a file upload from the cloud [\n%s\n].\n", sJsonMessage.c_str());
        }

        // Save the current request data for further upload steps
        m_sMessageId = uccCloudFileUpload.getMessageId();
        m_sDeviceId = uccCloudFileUpload.getDeviceId();
        m_sTrackingNumber = uccCloudFileUpload.getPayload().get(UccMessageCloudFileUpload::CTrackingNumberName).asCString();
        m_sUrl = uccCloudFileUpload.getPayload().get(UccMessageCloudFileUpload::CUrlName).asCString();
        m_sFileCategory = uccCloudFileUpload.getPayload().get(UccMessageCloudFileUpload::CFileCategoryName).asCString();
        m_sFileType = uccCloudFileUpload.getPayload().get(UccMessageCloudFileUpload::CFileTypeName).asCString();
        sFrom = uccCloudFileUpload.getPayload().get(UccMessageCloudFileUpload::CDateRangeName).get(UccMessageCloudFileUpload::CDateRangeFromDateName).asCString();
        sTo   = uccCloudFileUpload.getPayload().get(UccMessageCloudFileUpload::CDateRangeName).get(UccMessageCloudFileUpload::CDateRangeToDateName).asCString();

        int i = 0, iMax = eTotalFileCategories;
        while( i < iMax && m_sFileCategory.compare(CapcFileCategories[i++]) != 0 );
        if( i == iMax ) // no match found
        {
          sLogMessage  = "Unknown file category: ";
          sLogMessage += m_sFileCategory;
          blOk = false;
          break;
        }

        i = 0, iMax = eTotalFileTypes;
        if( ! supportedUploadableFileList.isSupported(m_sFileType) )
        {
          sLogMessage  = CpcInvalidFileType;
          sLogMessage += m_sFileType;
          iError = CErrorCodeInvalidFileType;
          pcError = sLogMessage.c_str();
          blOk = false;
          break;
        }

        try
        {
          if( ! sFrom.empty() ) m_tFromDate = dtParser.parse(sDateFmt, sFrom, itz).timestamp();
        }
        catch ( Poco::SyntaxException & e )
        {
          sLogMessage  = "Bad format of field \"";
          sLogMessage += UccMessageCloudFileUpload::CDateRangeFromDateName;
          sLogMessage += '"';
          blOk = false;
          break;
        }

        try
        {
          if( ! sTo.empty() ) m_tToDate = dtParser.parse(sDateFmt, sTo, itz).timestamp();
        }
        catch ( Poco::SyntaxException & e )
        {
          sLogMessage  = "Bad format of field \"";
          sLogMessage += UccMessageCloudFileUpload::CDateRangeToDateName;
          sLogMessage += '"';
          blOk = false;
          break;
        }

        if( m_tFromDate > m_tToDate )
        {
          sLogMessage = "Date interval is incorrect";
          blOk = false;
          break;
        }

        if( ! uploadFlow.checkAndSetFolder(m_sUploadFolder) )
        {
          sError  = CpcPHError;
          sError += "Root collector folder [ ";
          sError += m_sUploadFolder;
          sError += " ] not found.";
          LOG_VCritical(worker, "%s\n", sError.c_str());
          iError = 3;
          pcError = CpcUnableToExecuteCmd;
          blOk = false;
          break;
        }

        if( sendFileUploadResponse() ) // try to send OK command reply to cloud
        {
          processFlowEvent( eUploadRequest ); // Change to "Upload Requested" state.
        }
        else // could not send the OK reply to cloud due session error.
        {
          iError = 3;
          pcError = CpcUnableToExecuteCmd;
          sLogMessage = "Could not send reply to the cloud.";
          blOk = false;
        }

        break; // only one cycle in the while()
      } // end of while #1

      if( !blOk ) // This common error handler sends the error reply to the cloud and returns.
      {
        LOG_VError( worker, "UccMessageCloudFileUpload ERROR [%s].\n", sLogMessage.c_str() );
        sendFileUploadResponse( iError, pcError ); // sends an error reply to cloud
        clearReplicatedFieldsFromRequest();
        m_mutex.unlock();
        return;
      }

      while( blOk ) // #2 only one cycle to check more errors, no loop
      {
        sendStartStatus(); // send "start" event to cloud

        const char cSep = '_'; // underscore character
        Poco::DateTimeFormatter dtf;
        m_sArchiveName = m_sDeviceId + cSep + m_sFileType; // + "_YYMMDD_HHMMSSS" + CpcDefaultZipExtension;
        dtf.append(m_sArchiveName, Poco::LocalDateTime(), CpcTimestampInArchiveName);
        m_sArchiveName += CpcDefaultZipExtension;
        uploadFlow.setArchiveName(m_sArchiveName);
        m_sPath = m_sUploadFolder;
        m_sPath += Poco::Path::separator();
        m_sPath += m_sArchiveName;
        processFlowEvent( eUploadStarted ); // Change to "Uploading From SPOT" state.
        enterInProgressStatus("ready to retrieve files from SPOT"); // set the "in-progress" message.

        m_mutex.unlock(); // leave the notification thread to periodically keep informed the cloud.
        blOk = uploadFiles(uploadFlow, sError, supportedUploadableFileList);
        m_mutex.lock();

        if( blOk )
        {
          processFlowEvent( eSPOTUploadOk ); // Change to "Compressing Archive" state.
          setInProgressMessage( "compressing files to archive", true );
        }
        else
        {
          m_statusMessage = "Error uploading files from SPOT. " + sError;
          LOG_Error( worker, m_statusMessage.c_str() );
          blOk = false;
          break;
        }

        m_mutex.unlock(); // leave the notification thread to periodically keep informed the cloud.
        blOk = generateArchive(uploadFlow, sError);
        m_mutex.lock();

        if( ! blOk )
        {
          m_statusMessage = "Error generating compressed archive. " + sError;
          LOG_Error( worker, m_statusMessage.c_str() );
          blOk = false;
          break;
        }

        if( startSodaUpload() ) // try to send sendToSODA command to ucc
        {
          processFlowEvent( eSendFileToCloudRequested ); // Change to "Send File Requested" state.
          setInProgressMessage( "upload to cloud requested", true );
        }
        else // could not send internal Upload Request to UCC due session error.
        {
          iError = 3;
          pcError = CpcUnableToExecuteCmd;
          sLogMessage = "Could not delegate request to UCC";
          blOk = false;
        }

        break; // only one cycle, no loop
      } // end of while #2

      if( ! blOk )// Some aborting sequence error has arisen.
      {
        leaveInProgressStatus(); // error message already set all above
        sendFailureStatusEvent();
        clearReplicatedFieldsFromRequest();
        processFlowEvent( eOtherFatalError ); // Fatal error. Back to initial state.
      }

      m_mutex.unlock();
    }

    void FileUploadWorker::doStartFileUpload(const AbsOptCloudClientWorker::BufferType& data)
    {
      bool blOk = true;
      ScopeLock   lock( m_mutex ); // exclusive use of member variables and status.

      if( m_eState != eSendFileRequested ) // ignore events out of sequence.
      {
        LOG_VError( worker, "Unexpected message [SendFileToSODA reply] while in status [%s]. Discarded!",
                    CapcStateLegends[ m_eState ] );
        return;
      }

      LOG_Notice(worker, "doStartFileUpload.");

      UccMessageSendToCloudResponse   uccFileUploadRs; // the request file answer from UCC

      while( blOk ) // only one cycle, no loop
      {
        if( ! uccFileUploadRs.parse(data) )
        {
          m_statusMessage = "invalid payload.";
          LOG_VWarn( worker,
                     "Could not parse sendFileToCloud reply from UCC [%s] \n",
                     m_statusMessage.c_str() );

          blOk = false;
          break; // UCC reply parse error. Back to initial state.
        }

        const char * pcOriginalOne = ". Original one was ";

        if( uccFileUploadRs.getDeviceId() != m_sDeviceId )
        {
          std::string  sDeviceId = uccFileUploadRs.getDeviceId();
          m_statusMessage = "UCC Send File from cloud Reply has unexpected device id ";

          LOG_VWarn( worker, "%s[%s]%s[%s] \n", m_statusMessage.c_str(),
                     sDeviceId.c_str(), pcOriginalOne, m_sDeviceId.c_str() );

          m_statusMessage += sDeviceId;
          m_statusMessage += pcOriginalOne;
          m_statusMessage += m_sDeviceId;
          blOk = false;
          break; // device ID from UCC reply is wrong. Abort! . Back to initial state.
        }

        if( ! uccFileUploadRs.getPayload().get(UccMessageSendToCloudResponse::CSuccessName).asBoolean() )
        {
          int nError = uccFileUploadRs.getPayload().get(UccMessageSendToCloudResponse::CCodeName).asInteger();
          std::string sError = uccFileUploadRs.getPayload().get(UccMessageSendToCloudResponse::CMessageName).asCString();
          m_statusMessage = "UCC Send File to cloud Response received  with error ";
          LOG_VWarn(worker, "%s[%d] %s \n", m_statusMessage.c_str(), nError, sError.c_str());

          m_statusMessage += nError;
          m_statusMessage += ", \"";
          m_statusMessage += sError;
          m_statusMessage += "\" ";
          blOk = false; // UCC refused start uploading file. Back to initial state.
        }

        break; // no loop, only one cycle
      }

      if( blOk ) // sendFileToSODA reply ( internal FileUpload response from UCC ) is OK
      {
        InProgressCallback callback( *this, & FileUploadWorker::verifyUploadActivity );

        processFlowEvent( eSendFileToCloudReplyOk ); // Change to "Uploading to Cloud" state.
        LOG_Trace(worker, "doStartFileUpload - starting timer.");
        m_timerUploading.stop(); // Make sure previous callback is erased.
        m_timerUploading.start( callback ); // Start the timer for verifying further "in progress" messages.
        LOG_Trace(worker, "doStartFileUpload - started.");
      }
      else
      {
        sendFailureStatusEvent();
        clearReplicatedFieldsFromRequest();
        processFlowEvent( eSendFileToCloudReplyError ); // Fatal error. Back to initial state.
      }
    }

    void FileUploadWorker::processCloudStatusEventResponse( const AbsOptCloudClientWorker::BufferType& data)
    {
      bool blParseOk = false;
      std::string sMessageJson;
      UccMessageCloudFileUploadStatusEventResponse  uccCloudFileUploadStatusReply;

      blParseOk = uccCloudFileUploadStatusReply.parse(data);
      uccCloudFileUploadStatusReply.getJson().serialize(sMessageJson);

      if( blParseOk )
      {
        LOG_VTrace( worker,
                    "Received a status reply from the cloud: \n%s\n\n",
                    sMessageJson.c_str() );
      }
      else
      {
        LOG_VError( worker,
                    "UccMessageCloudFileUploadStatusEventResponse  [Message parse error]:\n%s\n\n",
                    sMessageJson.c_str() );
      }
    }

    void FileUploadWorker::doHandleFileUpload(const AbsOptCloudClientWorker::BufferType& data)
    {
      ScopeLock    lock( m_mutex ); // exclusive use of member variables and status.

      if( m_eState != eUploadingToCloud ) // ignore events out of sequence.
      {
        LOG_VError( worker, "Unexpected message [SendFileToSODA status] while in status [%s]. Discarded!",
                    CapcStateLegends[ m_eState ] );
        return;
      }

      bool         blOk = true;
      bool         blUploadCompleted = false;
      int          iBytesSent = 0;    // sent bytes so far,
      int          iTotalBytes = 0;  // total bytes to send,
      std::string  sStatus;  // file upload status
      std::string  sError;   // the possible error description.

      // Process sendFileToSODA (file upload) status notifications from UCC.

      LOG_Trace(worker, "doHandleFileUpload - reseting timer.");
      m_timerUploading.restart(CInUploadInterval); // restart the watchdog timer for another period
      LOG_Trace(worker, "doHandleFileUpload - timer reset.");

      UccMessageSendToCloudStatusEvent  uccFileUploadStatusEvent; // The file request status event from UCC

      while( blOk ) // only one cycle, no loop
      {
        if( ! uccFileUploadStatusEvent.parse(data) )
        {
          LOG_Warn( worker, "Could not parse sendFileToCloud status from UCC.");

          break; // This situation is very weird and is ignored.
        }

        if( uccFileUploadStatusEvent.getDeviceId() != m_sDeviceId )
        {
          std::string  sDeviceId = uccFileUploadStatusEvent.getDeviceId();
          m_statusMessage = "UCC Send Archive to cloud status has unexpected device id ";
          const char * pcOriginalOne = ". Original one was ";

          LOG_VError( worker, "%s[%s]%s[%s] \n", m_statusMessage.c_str(),
                     sDeviceId.c_str(), pcOriginalOne, m_sDeviceId.c_str() );

          m_statusMessage += sDeviceId;
          m_statusMessage += pcOriginalOne;
          m_statusMessage += m_sDeviceId;
          blOk = false;
          break; // device ID from UCC status is wrong. Abort! . Back to initial state.
        }

        std::string sTrackingnum( uccFileUploadStatusEvent.getPayload().get(UccMessageSendToCloudStatusEvent::CTrackingNumberName).asCString() );

        if( sTrackingnum.compare(m_sTrackingNumber) != 0 )
        {
          m_statusMessage = "UCC Send Archive to cloud status has unexpected tracking number ";
          const char * pcOriginalOne = ". Original one was ";

          LOG_VWarn( worker, "%s[%s]%s[%s]\n",
                     m_statusMessage.c_str(), sTrackingnum.c_str(),
                     pcOriginalOne, m_sTrackingNumber.c_str() );

          m_statusMessage += sTrackingnum;
          m_statusMessage += pcOriginalOne;
          m_statusMessage += m_sDeviceId;
          blOk = false;
          break; // tracking number from UCC status is wrong. Abort! . Back to initial state.
        }

        sStatus = uccFileUploadStatusEvent.getPayload().get(UccMessageSendToCloudStatusEvent::CStatusName).asCString();
        if( sStatus == "transfer-failure" )
        {
          int nError = uccFileUploadStatusEvent.getPayload().get(UccMessageSendToCloudStatusEvent::CCodeName).asInteger();
          std::string sError = uccFileUploadStatusEvent.getPayload().get(UccMessageSendToCloudStatusEvent::CMessageName).asCString();
          m_statusMessage = "File Upload finished  with error ";
          LOG_VError( worker, "%s[%d] %s \n", m_statusMessage.c_str(), nError, sError.c_str() );

          m_statusMessage += nError;
          m_statusMessage += ", \"";
          m_statusMessage += sError;
          m_statusMessage += "\" ";
          blOk = false; // UCC reported error uploading file. Back to initial state.
        }
        else // all the other three statuses
        {
          if( sStatus == "start" )
          {
            const char * pcMsg = "Start uploading archive.";
            m_statusMessage = pcMsg;
            LOG_Info( worker, pcMsg );
          }

          std::string sFileId = uccFileUploadStatusEvent.getPayload().get(UccMessageSendToCloudStatusEvent::CFileIdName).asString();
          if( !sFileId.empty() )
          {
            m_sFileID = sFileId;
          }

          iBytesSent = uccFileUploadStatusEvent.getPayload().get(UccMessageSendToCloudStatusEvent::CCurrentSizeName).asInteger();
          iTotalBytes = uccFileUploadStatusEvent.getPayload().get(UccMessageSendToCloudStatusEvent::CFileSizeName).asInteger();

          if( sStatus == "complete" )
          {
            if( iBytesSent != iTotalBytes )
            {
              LOG_VWarn( worker,
                         "Upload is reported \"%s\", but the send bytes are %d of %d originally requested.\n",
                         sStatus.c_str(), iBytesSent, iTotalBytes);
            }

            blUploadCompleted = true;
          }
          else if( iTotalBytes != m_iFileSize ) // check done in "start" or "in-progress"
          {
            LOG_VWarn( worker,
                       "sendFileToCloud status says file size = %d, but the archive size is %d bytes !!\n",
                       iTotalBytes, m_iFileSize );
          }

          LOG_VNotice( worker,
                      "Upload status %s. Bytes sent %d of %d \n", sStatus.c_str(), iBytesSent, iTotalBytes);
        }

        break; // no loop, only one cycle
      }

      if( ! blOk ) // Some aborting sequence error has arisen.
      {
        leaveInProgressStatus(); // error message already set above
        sendFailureStatusEvent();
        processFlowEvent( eSendFileToCloudStatusError ); // Fatal error. Back to initial state.
      }
      else if( blUploadCompleted )
      {
        const char * pcmsg = "Archive upload successfully completed.";

        leaveInProgressStatus( pcmsg );
        sendSuccessStatusEvent();
        processFlowEvent( eSendFileToCloudStatusComplete ); // Change to initial state.
        LOG_Info( worker, pcmsg );
      }
      else return; // Ok but not finished, it will enter this routine again.

      m_timerUploading.stop();

      // Archive and log files removal.
      Poco::File uplFile(m_sPath);
      uplFile.remove(false);
      SPOTFileUpload::removeTemporaryFiles( m_sUploadFolder + Poco::Path::separator() + SPOTFileUpload::CLogsFoldername );
      clearReplicatedFieldsFromRequest();
    }

    bool FileUploadWorker::uploadFiles( SPOTFileUpload & uploadFlow, std::string & sError,
                                        UploadableFileList & rUploadableList )

    {
      bool blOk = true;
      EFileCategories eCategory = eFileCatLog;

      if( m_sFileCategory == CapcFileCategories[ eFileCatConfiguration ] )
      {
        eCategory = eFileCatConfiguration;
      }

      std::string sMsg = "Uploading "; //"Uploading log/configuration files from SPOT.";
      sMsg += CapcFileCategories[eCategory];
      sMsg += " files from SPOT.";
      m_mutex.lock();
      setInProgressMessage( sMsg.c_str(), true);
      m_mutex.unlock();
      LOG_Info(worker, sMsg.c_str());

      if( eCategory == eFileCatLog )
      {
        int idx = -1;
        std::vector<UploadableFileList::FileNameAndAttrib> vlogs;

        rUploadableList.getRequeriedFileList(m_sFileType, UploadableFileList::eNoSpecified, -1, -1, vlogs);
        idx = uploadFlow.uploadLogs( vlogs );
        if( idx < vlogs.size() )
        {
          sError  = CpcPHError;
          sError += "Error uploading ";
          sError += vlogs[idx].m_sFilename;
          sError += " log. Flow error code ";
          sError += uploadFlow.getLastOperationError();
          sError += "dec.";
          blOk = false;
        }
      }
      else // configuration files
      {
        blOk = uploadFlow.uploadConfigurationFiles();

        if( !blOk )
        {
          sError  = CpcPHError;
          sError += "Error uploading resources.";
        }
      }

      if( blOk ) // Every stage went OK. No errors have arisen.
      {
        LOG_VDebug(worker, "Folder %s successfully populated from SPOT. \n", m_sUploadFolder.c_str());
      }
      else // errors have arisen.
      {
        if (m_pSpotSession->getError() != SpotSession::SpotSessionError::CNoError)
        {
          sError = "Some message or information block could not be received from SPOT.";
          LOG_VError( worker, "%s SpotSession Error [%s].\n",
                      sError.c_str(), m_pSpotSession->getErrorDescription().c_str() );
        }
        else // errors not related with the SPOT session
        {
          int iErrorCode = uploadFlow.getLastOperationError();
          LOG_VError(worker, "File Upload or Process Error ***  Flow error =  [%04X].\n", iErrorCode);
        }
      }

      return blOk;
    }

    bool FileUploadWorker::generateArchive( SPOTFileUpload & uploadFlow, std::string & sError )
    {
      bool blOk = true;
      bool blChecksumError = false;


      while( blOk ) // not actually a loop, it is executed once
      {
        LOG_Trace(worker, "Compressing collected files...");

        int iSize = uploadFlow.compressFiles();
        if( iSize <= 0 )
        {
          sError = "Error compressing archive.";
          LOG_Error(worker, sError.c_str());
          break;
        }

        m_iFileSize = iSize;
        LOG_Trace(worker, "Generating check sum...");
        m_mutex.lock();
        processFlowEvent( eCompressOk ); // Change to "Generating Checksum" state.
        setInProgressMessage( "generating checksum", true );
        m_mutex.unlock();

        {
          std::string checksumType;
          if (m_sChecksumType.compare(CSupportedChecksumTypeBySODA) == 0)
          {
            checksumType = CSHA256;
          }

          if ( generateChecksum(m_sPath, checksumType, m_sChecksum, sError) == true )
          {
            LOG_VTrace(worker, "Checksum generation for '%s' was successful.", m_sPath.c_str());
          }
          else
          {
            blOk = false;
            blChecksumError = true;
            sError.insert(0, "Checksum generation failed - ");
            LOG_VError(worker, "Checksum generation for '%s' failed - [%s].", m_sPath.c_str(), sError.c_str());
            break;
          }
        }

        break; // just only once iteration
      }

      if( blOk )
      {
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
        processFlowEvent( eCompressError ); // Decompression error. Back to initial state.
      }

      return false;
    }

    bool FileUploadWorker::startSodaUpload()
    {
      bool       blRet  = false;
      ErrorType  bError = 0;
      LOG_Trace(worker, "startSodaUpload.");

      UccMessageSendToCloud fileSendToSODA;
      fileSendToSODA.generateMessageId();
      fileSendToSODA.setDeviceId(m_sDeviceId);

      gvr::utils::Json jsonSnd = fileSendToSODA.getJson();
      jsonSnd.set(UccMessageSendToCloud::CPayloadName).set(UccMessageSendToCloud::CUrlName, m_sUrl);
      jsonSnd.set(UccMessageSendToCloud::CPayloadName).set(UccMessageSendToCloud::CPathName, m_sPath);
      jsonSnd.set(UccMessageSendToCloud::CPayloadName).set(UccMessageSendToCloud::CFileSizeName, m_iFileSize);
      jsonSnd.set(UccMessageSendToCloud::CPayloadName).set(UccMessageSendToCloud::CTrackingNumberName, m_sTrackingNumber);
      jsonSnd.set(UccMessageSendToCloud::CPayloadName).set(UccMessageSendToCloud::CChecksumName, m_sChecksum);
      jsonSnd.set(UccMessageSendToCloud::CPayloadName).set(UccMessageSendToCloud::CFileTypeName, m_sFileType);
      jsonSnd.set(UccMessageSendToCloud::CPayloadName).set(UccMessageSendToCloud::CCompressionTypeName, m_sCompressionType);
      fileSendToSODA.setJson(jsonSnd);
      if( (bError = m_pUccSession->send(fileSendToSODA) ) == UccSession::UccSessionError::CNoError )
      {
        blRet = true;
        std::string jsonMessage;

        fileSendToSODA.getJson().serialize(jsonMessage);
        LOG_VDebug(worker, "Send Send File to SODA has been sent [\n%s\n].\n", jsonMessage.c_str());
      }
      else
      {
        LOG_VCritical( worker, "Could not send Send File to SODA request to UCC. Session error: %u\n",
                       static_cast<unsigned int>(bError) );
      }

      return blRet;
    }

    bool FileUploadWorker::sendFileUploadResponse( int iError /* = 0 */,
                                                   const char * pcError /* = NULL */ )
    {
      bool blRet = false;
      LOG_Trace(worker, "Try to send file upload response.");

      {
        std::string sJsonMessage;

        //Send a file upload response
        {
          UccMessageCloudFileUploadResponse  uccCloudFileUploadResponse;
          uccCloudFileUploadResponse.setMessageId(m_sMessageId);
          uccCloudFileUploadResponse.setSuccess( iError == 0 ? true : false );

          if( iError != 0 ) // some error was consigned to be sent.
          {
            gvr::utils::Json jsonSnd = uccCloudFileUploadResponse.getJson();

            if( pcError == NULL )
            {
              pcError = "Unable to execute command"; // this is the explanation by default.
            }

            jsonSnd.set(UccMessageCloudFileUploadResponse::CCodeName, iError);
            jsonSnd.set(UccMessageCloudFileUploadResponse::CMessageName, pcError);
            uccCloudFileUploadResponse.setJson(jsonSnd);
          }

          {
            uccCloudFileUploadResponse.getJson().serialize(sJsonMessage);
            LOG_VTrace(worker, "Sending a file upload response to the cloud [\n%s\n].\n", sJsonMessage.c_str());
          }

          // Send a file upload cmd response.
          ErrorType error = m_pUccSession->send(uccCloudFileUploadResponse);
          if( error == 0 )
          {
            blRet = true;
            LOG_Trace(worker, "File upload reply was successfully sent to the cloud.");
          }
          else
          {
            LOG_VCritical( worker, "File upload reply could not be sent!! Session error %d\n",
                           static_cast<int>(error) );
          }
        }
      }

      return blRet;
    }

    void FileUploadWorker::clearReplicatedFieldsFromRequest()
    {
      // Clear the current request data for another upload cycle.
      m_iFileSize = 0;
      m_tFromDate = m_tToDate = 0;
      m_sMessageId.clear();
      m_sDeviceId.clear();
      m_sFileID = '0';
      m_sFileCategory.clear();
      m_sFileType.clear();
      m_sTrackingNumber.clear();
      m_sUrl.clear();
      m_sChecksumType = CSupportedChecksumTypeBySODA;
      m_sChecksum.clear();
    }

    void FileUploadWorker::setInProgressMessage( const char* pcLegend, bool blSend /* = false */)
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
      sendStatusEvent( UccMessageCloudFileUploadStatusEvent::eInProgress );
    }

    void FileUploadWorker::notifyProgressToCloud(Poco::Timer &)
    {
      ScopeLock lock( m_mutex );

      if( ! m_blInProgress )
      {
        LOG_Trace(worker, "Nothing to do because \"In Progress\" status finished.");
        return;
      }

      sendInProgressStatusEvent();
    }

    void FileUploadWorker::verifyUploadActivity(Poco::Timer &)
    {
      ScopeLock lock( m_mutex );

      if( m_eState == eUploadingToCloud )
      {
        leaveInProgressStatus("timeout expired");
        sendTransferFailureStatusEvent();
        LOG_VError( worker, "%s - Upload Aborted.!!\n", m_statusMessage.c_str() );

        // Archive and log files removal.
        Poco::File uplFile(m_sPath);
        uplFile.remove(false);
        SPOTFileUpload::removeTemporaryFiles( m_sUploadFolder + Poco::Path::separator() + SPOTFileUpload::CLogsFoldername );
        clearReplicatedFieldsFromRequest();

        processFlowEvent( eSendFileToCloudStatusError ); // Send file timeout. Back to initial state.
        m_timerUploading.restart(0); // valid self-kill timer
        LOG_VTrace( worker, "%s - Timer stopped, back in Initial state.\n", m_statusMessage.c_str() );
      }
    }

    void FileUploadWorker::sendStartStatus()
    {
      sendStatusEvent( UccMessageCloudFileUploadStatusEvent::eStart );
    }

    void FileUploadWorker::sendInProgressStatusEvent()
    {
      sendStatusEvent( UccMessageCloudFileUploadStatusEvent::eInProgress );
    }

    void FileUploadWorker::sendSuccessStatusEvent()
    {
      sendStatusEvent( UccMessageCloudFileUploadStatusEvent::eComplete );
    }

    void FileUploadWorker::sendTransferFailureStatusEvent()
    {
      sendStatusEvent( UccMessageCloudFileUploadStatusEvent::eTransferFailure );
    }

    void FileUploadWorker::sendFailureStatusEvent()
    {
      sendStatusEvent( UccMessageCloudFileUploadStatusEvent::eFailure );
    }

    void FileUploadWorker::sendStatusEvent(UccMessageCloudFileUploadStatusEvent::EUploadEventStatus e)
    {
      UccMessageCloudFileUploadStatusEvent  statusEvent;
      statusEvent.generateMessageId();
      statusEvent.setDeviceId( m_sDeviceId );

      gvr::utils::Json jsonAll = statusEvent.getJson();
      gvr::utils::Json jsonPld = jsonAll.set(UccMessageCloudFileUploadStatusEvent::CPayloadName);

      jsonPld.set( UccMessageCloudFileUploadStatusEvent::CFileCategoryName, m_sFileCategory );
      jsonPld.set( UccMessageCloudFileUploadStatusEvent::CFileTypeName, m_sFileType );
      jsonPld.set( UccMessageCloudFileUploadStatusEvent::CFileIdName, m_sFileID );
      jsonPld.set( UccMessageCloudFileUploadStatusEvent::CTrackingNumberName, m_sTrackingNumber );
      jsonPld.set( UccMessageCloudFileUploadStatusEvent::CStatusName,
                   UccMessageCloudFileUploadStatusEvent::getStatusLegend(e) );
      jsonPld.set( UccMessageCloudFileUploadStatusEvent::CFileCompressionName, m_sCompressionType );
      jsonPld.set( UccMessageCloudFileUploadStatusEvent::CMessageName, m_statusMessage );

      jsonAll.set(UccMessageCloudFileUploadStatusEvent::CPayloadName, jsonPld);
      statusEvent.setJson( jsonAll );
      m_pUccSession->send( statusEvent );
    }

    void FileUploadWorker::enterInProgressStatus( const char * pcStatusMessage /* = NULL */ )
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

      sendStatusEvent( UccMessageCloudFileUploadStatusEvent::eInProgress ); // Send the first "in progress" message.

      InProgressCallback callback( *this, & FileUploadWorker::notifyProgressToCloud );
      m_timerInProgress.setPeriodicInterval(CInProgressInterval);
      m_timerInProgress.setStartInterval(CInProgressInterval);
      m_timerInProgress.start( callback ); // Start the timer for sending further "in progress" messages.
    }

    void FileUploadWorker::leaveInProgressStatus( const char * pcStatusMessage /* = NULL */)
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

    bool FileUploadWorker::processFlowEvent ( FileUploadWorker::EFlowEvent evt )
    {
      bool blRet = false;

      switch( m_eState )
      {
        case eInitial:
          switch( evt )
          {
            case eUploadRequest:
              performStateChange( eUploadRequest, eUploadRequested );
              blRet = true;
            break;

            default:
              logUnexpectedFlowEvent( evt, eInitial );
            break;
          }
        break;

        case eUploadRequested:
          switch( evt )
          {
            case eUploadStarted:
              performStateChange( eUploadStarted, eUploadingFromSpot );
              blRet = true;
            break;

            case eOtherFatalError:
              performStateChange( evt, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eUploadRequested );
            break;
          }
        break;

        case eUploadingFromSpot:
          switch( evt )
          {
            case eSPOTUploadOk:
              performStateChange( eSPOTUploadOk, eCompressingArchive );
              blRet = true;
            break;

            case eSPOTUploadError:
            case eOtherFatalError:
              performStateChange( evt, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eUploadingToCloud );
            break;
          }
          break;

        case eCompressingArchive:
          switch( evt )
          {
            case eCompressOk:
              performStateChange( eCompressOk, eGeneratingChecksum );
              blRet = true;
            break;

            case eCompressError:
              performStateChange( eCompressError, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eCompressingArchive );
            break;
          }
          break;

        case eGeneratingChecksum:
          switch( evt )
          {
            case eSendFileToCloudRequested:
              performStateChange( eSendFileToCloudRequested, eSendFileRequested );
              blRet = true;
            break;

            case eChecksumError:
              performStateChange( eChecksumError, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eGeneratingChecksum );
            break;
          }
          break;

        case eSendFileRequested:
          switch( evt )
          {
            case eSendFileToCloudReplyOk:
              performStateChange( eSendFileToCloudReplyOk, eUploadingToCloud );
              blRet = true;
            break;

            case eSendFileToCloudReplyError:
            case eOtherFatalError:
              performStateChange( evt, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eSendFileRequested);
            break;
          }
          break;

        case eUploadingToCloud:
          switch( evt )
          {
            case eSendFileToCloudStatusComplete:
              // This is the successful expected end of this state machine
              // to the initial state eInitial, and then, continue to accept new uploads.
              performStateChange( eSendFileToCloudStatusComplete, eInitial );
              blRet = true;
            break;

            case eSendFileToCloudStatusError:
            case eOtherFatalError:
              // This is the case of "Upload transfer Error" .
              performStateChange( evt, eInitial );
            break;

            default:
              logUnexpectedFlowEvent( evt, eInitial );
            break;
          }
          break;
      }

      return blRet;
    }

    FileUploadWorker::EFlowState
    FileUploadWorker::performStateChange ( FileUploadWorker::EFlowEvent ev,
                                                    FileUploadWorker::EFlowState estat )
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

    void FileUploadWorker::logUnexpectedFlowEvent( FileUploadWorker::EFlowEvent ev,
                                                            FileUploadWorker::EFlowState estat )
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

    void FileUploadWorker::traceNoStateChanged( EFlowEvent ev )
    {
      LOG_VTrace( worker, "Sendd event [%s] in state [%s] . No state changes.\n",
                  CapcEventLegends[ ev ], CapcStateLegends[ m_eState ] );
    }

    // TODO: Update the function code with the proper one to checksum generation, not verification.
    bool FileUploadWorker::generateChecksum ( const std::string & path, const std::string & checksumType,
                                              std::string & checksum, std::string & errorMessage )
    {
      bool blRet = false;

      if ( !checksumType.empty() && checksumType.compare(CSHA256) == 0 )
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

              checksum = Poco::Crypto::DigestEngine::digestToHex(engine.digest());
              errorMessage.clear();
              blRet = true;
            }
            else
            {
              std::ostringstream oss;
              oss << "Couldn't open uploaded file '" << path << "' for checksum generation." << std::endl;
              errorMessage = oss.str();
            }
          }
          catch (Poco::NotFoundException & e)
          {
            std::ostringstream oss;
            oss << "Couldn't initialize crypto engine while performing '" << path << "' checksum generation." << std::endl;
            errorMessage = oss.str();

          }
          Poco::Crypto::uninitializeCrypto();
        }
        else
        {
          std::ostringstream oss;
          oss << "Filename for checksum generation is empty." << std::endl;
          errorMessage = oss.str();
        }
      }
      else
      {
        std::ostringstream oss;
        oss << "Checksum type '" << checksumType << "' unsupported." << std::endl << " Supported types are: [ " << CSHA256 <<  " ]" << std::endl;
        errorMessage = oss.str();
      }
      return blRet;
    }
  }
}




