/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file CloudRegistrationWorker.h
 * @author mvolker
 * @date Thursday, April 16, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _CLOUDREGISTRATIONWORKER_H_
#define _CLOUDREGISTRATIONWORKER_H_

# include <workers/AbsCrlWorker.h>
# include <CloudStatus.h>
# include <CloudConfigDeviceChecker.h>

# include <gvr/communication/crl/session/CrlSessionServer.h>
# include <gvr/communication/spot/SpotSession.h>
# include <gvr/communication/ucc/session/UccSession.h>

# include <gvr/communication/ucc/message/UccMessageRegisterResponse.h>
# include <gvr/communication/ucc/message/UccMessageServerUpdate.h>
# include <gvr/communication/crl/message/CrlMessageRegistrationResponse.h>

using namespace gvr::communication::ucc;
using namespace gvr::communication::spot;
using namespace gvr::communication::crl;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class CloudRegistrationWorker : public gvr::cloud::AbsCrlWorker
      {
        public:

          /**
           * @brief default constructor.
           */
          CloudRegistrationWorker( UccSession &, SpotSession &, CrlSessionServer &, CloudStatus &, CloudConfigDeviceChecker & );

          /**
           * @brief default destructor.
           */
          virtual ~CloudRegistrationWorker(void);


        protected:

          /**
           * @brief Callback for the retrieved message by the session to which the listener is subscribed.
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
          virtual void sessionEvent (ICrlSessionListener::BufferType const &, ICrlSessionListener::TypeId const &, ICrlSessionListener::KeyType const &);

          /**
           * @brief Callback for the triggered status change by the session to which the listener is subscribed.
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
          virtual void sessionEvent (ICrlSessionListener::StatusType);

          /**
           * @brief This method is where the thread does the subscribe to events that will received from the UccSession.
           */
          virtual void doSubscriptions ();

          /**
           * @brief This method is where the thread does the unsubscribe to events that was previously subscribed.
           */
          virtual void doUnsubscriptions ();

          /**
           * @brief This method is where the thread does the process an event data notification received.
           */
          virtual void processNotification ( AbsOptCloudClientWorker::TypeId typeId, const AbsOptCloudClientWorker::BufferType& data );

          /**
           * @brief This method is where the thread does the process to each status event notification received.
           */
          virtual void processNotification ( StatusType statusType );

          /**
           * @brief Helper method to collect all Spot information.
           *
           * @param jsonRegInfo is the json object where the collected information is stored.
           */
          bool collectRegistrationInformation( gvr::utils::Json &, const std::string&, const std::string& );

          /**
           * @brief Helper method to do a registration.
           */
          void doRegistration( std::string&, std::string& );

          /**
           * @brief Helper method to fill Crl registration response.
           */
          bool populateAndSendCrlResponse( std::string const &, const UccMessageRegisterResponse&, CrlMessageRegistrationResponse& );

          /**
           * @brief Helper method to fill and send Registration Server Update Message to UCC server.
           */
          bool populateAndSendUccRegistrationServerUpdateMessage( );

          /**
           * @brief Helper method to fill and send Register Request Message to send to UCC server.
           */
          bool populateAndSendUccRegisterRequestMessage( const std::string &, const std::string &, UccMessageRegisterResponse & );

          /**
           * @brief Helper method to fill Crl registration response.
           */
          bool storeUccRegistrationResponseOk( const gvr::utils::Json& );

          /**
           * @brief Helper method to fill and send a Crl generic fail registration response.
           */
          void sendGenericFailResponse( );

          /**
           * @brief UccSession to be used by this worker.
           */
          UccSession & m_uccSession;

          /**
           * @brief SpotSession to be used by this worker.
           */
          SpotSession & m_spotSession;

          /**
           * @brief CrlSession to be used by this worker.
           */
          CrlSessionServer & m_crlSession;

          /**
           * @brief Cloud status member to be used by this worker.
           */
          CloudStatus & m_cloudStatus;

          /**
           * @brief Cloud config device checker member to be used by this worker.
           */
          CloudConfigDeviceChecker & m_configChecker;
      };
    }
  }

#endif
