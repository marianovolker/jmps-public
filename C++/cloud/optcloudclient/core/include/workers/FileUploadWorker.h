/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file FileUploadWorker.h
 * @author gparis
 * @date Monday, June 08, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef FILEUPLOADWORKER_H_
#define FILEUPLOADWORKER_H_

#include <Poco/Timer.h>
#include <Poco/DateTime.h>

#include <gvr/communication/ucc/IUccSessionListener.h>
#include <gvr/communication/spot/SpotSession.h>
#include <gvr/communication/ucc/session/UccSession.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileUploadStatusEvent.h>
#include <gvr/communication/spot/SPOTFileUploadFlow.h>

#include <workers/AbsOptCloudClientWorker.h>


  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {

      class FileUploadWorker : public gvr::cloud::AbsOptCloudClientWorker,
                               public gvr::communication::ucc::IUccSessionListener
      {
          /**
           * @brief These are some events of the event universe that produce state changes.
           */
          enum EFlowEvent
          {
            eUploadRequest,                 ///< @brief Received a FileUpload request from the cloud.
            eUploadStarted,                 ///< @brief Upload to cloud, start event has just been sent
            eSPOTUploadOk,
            eSPOTUploadError,
            eCompressOk,
            eCompressError,
            eChecksumError,
            eSendFileToCloudRequested,
            eSendFileToCloudReplyOk,        ///< @brief SendFile cmd accepted and started.
            eSendFileToCloudReplyError,     ///< @brief SendFile cmd finished with error.
            eSendFileToCloudStatusComplete, ///< @brief SendFile cmd completed OK.
            eSendFileToCloudStatusError,
            eOtherFatalError,
            eTotalEvents                    ///< @brief Total number of events.
          };

          /**
           * @brief These are the states this worker passes by, following a sequence.
           */
          enum EFlowState
          {
            eInitial,                       ///< @brief Initial idle state.
            eUploadRequested,               ///< @brief Upload request cmd was received from the cloud.
            eUploadingFromSpot,             ///< @brief Files are being collected from SPOT.
            eCompressingArchive,            ///< @brief File recollection was OK and the archive is being compressed.
            eGeneratingChecksum,            ///< @brief Archive compression was OK and the checksum is being generated.
            eSendFileRequested,             ///< @brief Send file to Cloud Request has sent to UCC.

            /**
             * @brief SendFile cmd was accepted and started. This app is now listening status progress
             *        from UCC and also sending "in progress" messages to the cloud.
             */
            eUploadingToCloud,
            eTotalStates                    ///< @brief Total number of states.
          };

          enum EFileCategories
          {
            eFileCatLog, eFileCatConfiguration, eTotalFileCategories
          };

          enum EFileTypes
          {
            eFullLogs, ePinpad, ePinpadHist, eSysmanServ, eSysmanServHist, eEMV, eEMVHist, eOPT, OPTHist, // to be extended
            eTotalFileTypes
          };

          static const char* CapcFileCategories[ eTotalFileCategories ];///<@brief Legends for events.
          static const char* CapcFileTypes[ eTotalFileTypes ];          ///<@brief Legends for events.
          static const char* CapcEventLegends[ eTotalEvents ];          ///<@brief Legends for events.
          static const char* CapcStateLegends[ eTotalStates ];          ///<@brief Legends for states.

          static const int   CInProgressInterval = 10000;// 10s test // 180000 //  3 minutes
          static const int   CInUploadInterval = 60000;// 60s test // 600000 // 10 minutes
          static const int   CErrorCodeUnableToExec = 3;
          static const int   CErrorCodeInvalidFileType = 7;
          static const char* CpcDateFormat; ///< @brief Date format for filenames and dates from payloads.
          static const char* CpcTimestampInArchiveName; ///< @brief Date/time format inside log archive name.
          static const char* CpcDefaultZipExtension;
          static const char* CpcDefaultCompressionType;
          static const char* CpcUnableToExecuteCmd; ///< @brief "Unable to execute command"
          static const char* CpcInvalidFileType; ///< @brief "Unsupported file type: "


        public:

          /**
           * @brief default constructor.
           */
          FileUploadWorker ( gvr::communication::ucc::UccSession &,
                             gvr::communication::spot::SpotSession &,
                             std::string const & uploadDir, std::string const & spotIp,
                             std::string const & uploadableListFileName );

          /**
           * @brief default destructor.
           */
          virtual ~FileUploadWorker (void);


        protected:

          /**
           * @brief This method is where the worker subscribes itself to events that will sendd from the UccSession.
           */
          virtual void doSubscriptions ();

          /**
           * @brief This method is where the worker unsubscribes itself to events that was previously subscribed.
           */
          virtual void doUnsubscriptions ();

          /**
           * @brief Callback for the retrieved Message by the session to which the listener is subscribed.
           *
           * @note  This method is called from the thread inside the session, so in this we need to enqueue something
           *        instead of process event sendd.
           *
           * @param buffer is the message buffer that occur.
           * @param typeId is the sendd message class type identification of the message sendd to be parsed.
           * @param key is the sendd message key identification, which in many protocols is the sequence number.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISessionListener.BufferType
           * @see gvr.communication.ISessionListener.TypeId
           * @see gvr.communication.ISessionListener.KeyType
           *
           * @since 0.0.0
           */
          virtual void sessionEvent ( gvr::communication::ucc::IUccSessionListener::BufferType const &,
                                      gvr::communication::ucc::IUccSessionListener::TypeId const &,
                                      gvr::communication::ucc::IUccSessionListener::KeyType const & );

          /**
           * @brief Callback for the triggered session Status change to which the listener is subscribed.
           *
           * @note  This method is called from the thread inside the session, so in this we need to enqueue something
           *        instead of process event status sendd.
           *
           * @param status is the current session status. Session state changes and errors are reported through this callback.
           *
           * @exception Throws no exception.
           *
           * @see gvr.communication.ISessionListener.StatusType
           *
           * @since 0.0.0
           */
          virtual void sessionEvent ( gvr::communication::ucc::IUccSessionListener::StatusType );

          /**
           * @brief This method is where the thread does the process an event data notification sendd.
           */
          virtual void processNotification ( AbsOptCloudClientWorker::TypeId typeId, const AbsOptCloudClientWorker::BufferType& data );

          /**
           * @brief This method is where the thread does the process to each status event notification sendd.
           */
          virtual void processNotification ( StatusType statusType );

          /**
           * @brief This method process the command file upload ("file-upload-cmd"), sendd from the cloud.
           */
          void processUploadRequest ( const AbsOptCloudClientWorker::BufferType& );

          /**
           * @brief This method process the specific "sendFileToCloud" response, from UCC.
           * @param The response data sendd.
           */
          void doStartFileUpload ( const AbsOptCloudClientWorker::BufferType& data );

          /**
           * @brief This method just logs the upload status event response from cloud.
           * @param The response data sendd.
           */
          void processCloudStatusEventResponse( const AbsOptCloudClientWorker::BufferType& data );

          /**
           * @brief This method is where the thread does the process of the specific
           *        "sendFileToCloud-status" ( UccMessageFileUploadStatusEvent )
           *        response, from UCC.
           * @param The response data sendd.
           */
          void doHandleFileUpload (const AbsOptCloudClientWorker::BufferType& data);

          /**
           * @brief This method is where the upload directory is populated with files uploaded from SPOT.
           * @param @b rUplFlow is a reference to an already constructed File Upload Flow.
           * @param @b [Out] sError is where the flow error message is populated if errors arise.
           * @param @b rUploadableList is a reference to an already constructed list for all the file
           *           type names that this device support to upload.
           */
          bool uploadFiles ( gvr::communication::spot::SPOTFileUpload & rUplFlow, std::string & sError,
                             gvr::communication::spot::UploadableFileList & rUploadableList );

          /**
           * @brief This method is where the retrieved files are compressed.
           * @param
           */
          bool generateArchive ( gvr::communication::spot::SPOTFileUpload & ,
                                 std::string & sError);

          /**
           * @brief This method starts the file upload when a sendToCloud is sent as an internal cmd to ucc session.
           *
           * @ return @b true if the sendToCloud ( UccMessageFileUpload ) message was successfully sent.
           *          @b false if there was a session error and the command message could not be sent.
           */
          bool startSodaUpload();

          /**
           * @brief This method replies to the cloud the previous file upload request ( UccMessageCloudFileUpload ).
           * @param @b iError is the optional error code in case of detected errors in the original request.
           * @param @b pcError is the optional pointer to error message in case of errors.
           *
           * @return @b true if the reply to cloud ( UccMessageCloudFileUploadResponse ) message was successfully sent.
           *         @b false if there was a session error and the command message could not be sent.
           */
          bool sendFileUploadResponse (int iError = 0, const char * pcError = NULL);

          /**
           * @brief Clears those member variables whose values were taken from the original request from the cloud.
           */
          void clearReplicatedFieldsFromRequest (void);


          gvr::communication::ucc::UccSession   *      m_pUccSession;  ///< @brief The UCC  session
          gvr::communication::spot::SpotSession *      m_pSpotSession; ///< @brief The SPOT session

          int         m_iFileSize;///< @brief the current file size from the starting file-upload-cmd request message.

          std::string m_sMessageId;///< @brief the current message id from the starting file-upload-cmd request message.
          std::string m_sDeviceId;///< @brief the current device id from the starting file-upload-cmd request message.
          std::string m_sFileID;  ///< @brief the current file ID.
          std::string m_sFileCategory;///< @brief The category for file selection.
          std::string m_sFileType;///< @brief The file type for selection.
          std::string m_sTrackingNumber;///< @brief the current tracking number the starting file-upload-cmd request message.
          std::string m_sUrl;///< @brief the current url from the starting file-upload-cmd request message.
          std::string m_sPath;///< @brief The complete full pathname to the archive being sent.
          std::string m_sChecksum;///< @brief the current checksum from the starting file-upload-cmd request message.
          std::string m_sChecksumType;///< @brief the current checksum type from the starting file-upload-cmd request message.
          std::string m_sCompressionType;///< @brief The current file compression type.

          std::string m_sUploadFolder;///< @brief The path to the folder having archive(s) for uploading.
          std::string m_sArchiveName;///< @brief The filename of the archive being sent.
          std::string m_sUloadableListFile;///< @brief The pathname to the file having the uploadable file list.

          Poco::Timestamp m_tFromDate;///< @brief The starting date for file selection by interval.
          Poco::Timestamp m_tToDate;  ///< @brief The ending date for file selection by interval.


        private:

          /**
           * @brief Sets the message legend for the status event to be notified by the timer callback routine.
           * @note Optionally sends the message instead of delegating this in the timer callback routine.
           *
           * @param @b pcLegend is the char string to appear in "message" field into the payload.
           *        If this parameter is NULL, then this field is cleared.
           *
           * @param @b blSend If @true then also sends the "in progress" event and reset the elapsed time
           *           for the timer routine, avoiding this way having two possibly "adjacent" events to be sent.
           */
          void setInProgressMessage( const char* pcLegend, bool blSend = false );

          /**
           * @brief This is the timer callback to be executed every period of CInProgressInterval.
           *        This routine sends "InProgress" message to the cloud during eUploadingToCloud state.
           * @param Not used.
           *
           */
          void notifyProgressToCloud(Poco::Timer &);

          /**
           * @brief This is the timer callback only executed if during a period of CInUploadInterval
           *        none of the "in-progress" message from UCC has been sendd. This is a timeout condition,
           *        that resets the machine state to initial state.
           *        This routine then verifies messaging activity for the state eUploadingToCloud.
           *
           * @param Not used.
           *
           */
          void verifyUploadActivity(Poco::Timer &);

          /** @brief Sends a starting event status */
          void sendStartStatus();

          /** @brief Sends an "in progress" event status */
          void sendInProgressStatusEvent();

          /** @brief Sends a success event status */
          void sendSuccessStatusEvent();

          /** @brief Sends a transfer failure event status */
          void sendTransferFailureStatusEvent();

          /** @brief Sends a failure event status */
          void sendFailureStatusEvent();

          /** @brief Sends a status event with field settings according the status. */
          void sendStatusEvent(gvr::communication::ucc::UccMessageCloudFileUploadStatusEvent::EUploadEventStatus);

          /**
           * @brief Enter into the "in progress" status.
           * @note  In this status a pregress notification will be sent to the cloud every CInProgressInterval
           *
           * @param @b pcStatusMessage An optional legend for message field in payload, to start with.
           */
          void enterInProgressStatus ( const char * pcStatusMessage = NULL );

          /** @brief Leave from the "in progress" status.
           *
           * @param @b pcStatusMessage An optional legend for message field in payload, for further events.
           */
          void leaveInProgressStatus ( const char * pcStatusMessage = NULL );

          /**
           * @brief Process the flow event to change current state to a new one, or deny the change.
           * @note This method is the only entry point to the internal state machine.
           * @param @b ev is the flow event.
           * @return @b true if a state transition happened, @b false if no transition occurred.
           */
          bool processFlowEvent ( EFlowEvent ev );

          /**
           * @brief Change current state to a new one and logs the transition.
           * @param @b ev is the flow event.
           * @param @b estat is the new flow state after the transition.
           * @return The new state as passed in last parameter.
           */
          EFlowState performStateChange ( EFlowEvent ev, EFlowState estat );

          /**
           * @brief Helper function to log the arrival of an unexpected flow event in the sequence.
           */
          void logUnexpectedFlowEvent( EFlowEvent ev, EFlowState estat );

          /**
           * @brief Helper function to log the arrival of an inoffensive (no error) flow event in the sequence,
           *        that do not produce a state change.
           */
          void traceNoStateChanged( EFlowEvent ev );

          /** @brief Generates file checksum.
           *
           * @param @b path is the file location for checksum generation.
           * @param @b checksumType is the type of checksum to generate. Available types are strings "MD5", "SHA1", "SHA256" and "SHA512".
           * @param @b [Out] checksum is the generated hex string to set in send message.
           * @param @b [Out] errorMessage is where an error message is stored in case of error.
           *
           * @return true if validated. False otherwise.
           */
          static bool generateChecksum ( const std::string & path, const std::string & checksumType,
                                         std::string & checksum, std::string & errorMessage );

          /**
           * @brief Some checksum type identifiers
           */
          static char const * CSODA_SHA256;
          static char const * CSupportedChecksumTypeBySODA;
          static char const * CSHA256;

          static const char * CpcPHError; ///<@brief Payload handler error prefix message


          bool            m_blInProgress;  ///<@brief Flag signaling the "in progress" sub-status of eUpload.
          EFlowState      m_eState;        ///<@brief State of the flow: initial, upload, sendToCloud.
          std::string     m_statusMessage; ///<@brief The message to include in upload status events. */

          Poco::FastMutex m_mutex;           ///<@brief Mutex to protect access shared members with timer routine */
          Poco::Timer     m_timerInProgress; ///<@brief Timer to send to cloud the "in progress" event at regular interval. */
          Poco::Timer     m_timerUploading;///<@brief Watchdog to supervise the "uploading" stage. */
      };


    }
  }


#endif /* FILEUPLOADWORKER_H_ */


