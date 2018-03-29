/**
 * Copyright © 2014, 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SpotReleaseDownloadWorker.h
 * @author mgrieco
 * @date Tuesday, April 07, 2015
 * @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SPOTRELEASEDOWNLOADWORKER_H_
#define _SPOTRELEASEDOWNLOADWORKER_H_

#include <Poco/Timer.h>

#include <gvr/communication/ucc/IUccSessionListener.h>
#include <gvr/communication/spot/SpotSession.h>
#include <gvr/communication/ucc/session/UccSession.h>
#include <gvr/communication/ucc/message/UccMessageCloudFileDownloadStatusEvent.h>
#include <gvr/communication/spot/SPOTFileDownloadFlow.h>

#include <workers/AbsOptCloudClientWorker.h>


  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {

      class SpotReleaseDownloadWorker : public gvr::cloud::AbsOptCloudClientWorker,
                                        public gvr::communication::ucc::IUccSessionListener
      {
          /**
           * @brief These are some events of the event universe that produce state changes.
           */
          enum EFlowEvent
          {
            eDownloadRequest,                 ///< @brief Received a FileDownload request from the cloud.
            eRcvFileFromCloudReplyOk,         ///< @brief ReceiveFile cmd accepted and started.
            eRcvFileFromCloudReplyError,      ///< @brief ReceiveFile cmd finished with error.
            eRcvFileFromCloudStatusComplete,  ///< @brief ReceiveFile cmd completed OK.
            eRcvFileFromCloudStatusError,
            eChecksumOK,
            eChecksumError,
            eDecompressOk,
            eDecompressError,
            eSPOTDownloadOk,
            eSPOTDownloadError,
            eOtherFatalError,
            eTotalEvents                      ///< @brief Total number of events.
          };

          /**
           * @brief These are the states this worker passes by, following a sequence.
           */
          enum EFlowState
          {
            eInitial,                         ///< @brief Initial idle state.
            eDownloadRequested,               ///< @brief Download request cmd was received from the cloud.

            /**
             * @brief ReceiveFile cmd was accepted and started. This app is now listening status progress
             *        from UCC and also sending "in progress" messages to the cloud.
             */
            eDownloadingFromCloud,

            eCheckingFile,                    ///< @brief File integrity is being checked.
            eDecompressingFile,               ///< @brief Integrity was OK and the file is being decompressed.
            eDownloadingToSpot,               ///< @brief File decompressed OK and components are being downloaded to SPOT.
            eDownloadOK,                      ///< @brief Release downloaded OK and waiting installation OK.
            eTotalStates                      ///< @brief Total number of states.
          };


          static const char* CapcEventLegends[ eTotalEvents ];    ///<@brief Legends for events.
          static const char* CapcStateLegends[ eTotalStates ];    ///<@brief Legends for states.

          static const char* CSwBundle;                    ///< @brief "software-bundle"
          static const int   CInProgressInterval = 10000;// 10s test // 180000 //  3 minutes
          static const int   CInDownloadInterval = 60000;// 60s test // 600000 // 10 minutes


        public:

          /**
           * @brief default constructor.
           */
          SpotReleaseDownloadWorker ( gvr::communication::ucc::UccSession &,
                                      gvr::communication::spot::SpotSession &,
                                      std::string const &, std::string const & );

          /**
           * @brief default destructor.
           */
          virtual ~SpotReleaseDownloadWorker (void);


        protected:

          /**
           * @brief This method is where the worker subscribes itself to events that will received from the UccSession.
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
           *        instead of process event received.
           *
           * @param buffer is the message buffer that occur.
           * @param typeId is the received message class type identification of the message received to be parsed.
           * @param key is the received message key identification, which in many protocols is the sequence number.
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
           *        instead of process event status received.
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
           * @brief This method is where the thread does the process an event data notification received.
           */
          virtual void processNotification ( AbsOptCloudClientWorker::TypeId typeId, const AbsOptCloudClientWorker::BufferType& data );

          /**
           * @brief This method is where the thread does the process to each status event notification received.
           */
          virtual void processNotification ( StatusType statusType );

          /**
           * @brief This method process the command file download ("file-download-cmd"), received from the cloud.
           */
          void doFileRequest ( const AbsOptCloudClientWorker::BufferType& );

          /**
           * @brief This method process the specific "receiveFileFromCloud" response, from UCC.
           * @param The response data received.
           */
          void doStartFileDownload ( const AbsOptCloudClientWorker::BufferType& data );

          /**
           * @brief This method just logs the download status event response from cloud.
           * @param The response data received.
           */
          void processCloudStatusEventResponse( const AbsOptCloudClientWorker::BufferType& data );

          /**
           * @brief This method is where the thread does the process of the specific
           *        "receiveFileFromCloud-status" ( UccMessageFileDownloadStatusEvent )
           *        response, from UCC.
           * @param The response data received.
           */
          void doHandleFileDownload (const AbsOptCloudClientWorker::BufferType& data);

          /**
           * @brief This method is where the downloaded file is checked and decompressed.
           * @param
           */
          bool processDownloadedFile ( gvr::communication::spot::SPOTFileDownload & ,
                                       std::string & sFolder );

          /**
           * @brief This method is where the decompressed files in the tree are downloaded into SPOT.
           * @param
           */
          bool downloadBundleToSPOT ( gvr::communication::spot::SPOTFileDownload & ,
                                      const std::string & sFolder);

          /**
           * @brief This method starts the file download when a receiveFromCloud is sent as an internal cmd to ucc session.
           *
           * @ return @b true if the receiveFromCloud ( UccMessageFileDownload ) message was successfully sent.
           *          @b false if there was a session error and the command message could not be sent.
           */
          bool startSodaDownload();

          /**
           * @brief This method replies to the cloud the previous file download request ( UccMessageCloudFileDownload ).
           * @param @b iError is the optional error code in case of detected errors in the original request.
           * @param @b pcError is the optional pointer to error message in case of errors.
           *
           * @return @b true if the reply to cloud ( UccMessageCloudFileDownloadResponse ) message was successfully sent.
           *         @b false if there was a session error and the command message could not be sent.
           */
          bool sendFileDownloadResponse (int iError = 0, const char * pcError = NULL, std::string const & messageIdAux = "");

          /**
           * @brief Clears those member variables whose values were taken from the original request from the cloud.
           */
          void clearReplicatedFieldsFromRequest (void);


          gvr::communication::ucc::UccSession   * m_pUccSession;  ///< @brief The UCC  session
          gvr::communication::spot::SpotSession * m_pSpotSession; ///< @brief The SPOT session

          int         m_iFileSize;/**@brief the current file size from the starting file-download-cmd request message*/
          std::string m_sMessageId; /**@brief the current message id from the starting file-download-cmd request message*/
          std::string m_sDeviceId; /**@brief the current device id from the starting file-download-cmd request message*/
          std::string m_spotIp; /**@brief the current spot session ip*/
          std::string m_sDefaultReleasePath; /**@brief the default configured release path where the release bundle should be located*/
          std::string m_sTrackingNumber;/**@brief the current tracking number the starting file-download-cmd request message*/
          std::string m_sUrl;/**@brief the current url from the starting file-download-cmd request message*/
          std::string m_sChecksumType;/**@brief the current checksum type from the starting file-download-cmd request message*/
          std::string m_sChecksum;/**@brief the current checksum from the starting file-download-cmd request message*/
          std::string m_sPath;/**@brief the according release bundle path*/


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
           *        This routine sends "InProgress" message to the cloud during eDownloadingFromCloud state.
           * @param Not used.
           *
           */
          void notifyProgressToCloud(Poco::Timer &);

          /**
           * @brief This is the timer callback only executed if during a period of CInDownloadInterval
           *        none of the "in-progress" message from UCC has been received. This is a timeout condition,
           *        that resets the machine state to initial state.
           *        This routine then verifies messaging activity for the state eDownloadingFromCloud.
           *
           * @param Not used.
           *
           */
          void verifyDownloadActivity(Poco::Timer &);

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
          void sendStatusEvent(gvr::communication::ucc::UccMessageCloudFileDownloadStatusEvent::EDownloadEventStatus);

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

          /** @brief Validates file checksum.
           *
           * @param path is the location for the file to validate.
           * @param checksumType is the type of checksum to use for validation, available types are strings "MD5", "SHA1", "SHA256" and "SHA512".
           * @param checksum is the hex string value to validate.
           * @param errorMessage is where an error message is stored in case of error.
           *
           * @return true if validated. False otherwise.
           */
          static bool validateChecksum (std::string path, std::string checksumType, std::string checksum, std::string & errorMessage);

          static char const * CSODA_MD5;
          static char const * CSODA_SHA1;
          static char const * CSODA_SHA2;
          static char const * CSODA_SHA256;
          static char const * CSODA_SHA512;
          static char const * CSupportedChecksumTypeBySODA;

          static char const * CMD5;
          static char const * CSHA1;
          static char const * CSHA256;
          static char const * CSHA512;

          static const char * CpcPHError;


          bool         m_blInProgress;  ///<@brief Flag signaling the "in progress" sub-status of eDownload.
          EFlowState   m_eState;        ///<@brief State of the flow: initial, download, receiveFromCloud.
          std::string  m_statusMessage; ///<@brief The message to include in download status events. */

          Poco::FastMutex m_mutex;           ///<@brief Mutex to protect access shared members with timer routine */
          Poco::Timer     m_timerInProgress; ///<@brief Timer to send to cloud the "in progress" event at regular interval. */
          Poco::Timer     m_timerDownloading;///<@brief Watchdog to supervise the "downloading" stage. */
      };


    }
  }

#endif
