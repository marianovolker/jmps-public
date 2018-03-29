/**
 * Copyright © 2015 Gilbarco Inc.
 * Confidential and Proprietary.
 *
 * @file SecurityChangeWorker.h
 * @author mvolker
 * @date Monday, June 8, 2015
 * @copyright Copyright © 2015 Gilbarco Inc. Confidential and Proprietary.
 */

#ifndef _SECURITYCHANGEWORKER_H_
#define _SECURITYCHANGEWORKER_H_

# include <workers/AbsOptCloudClientWorker.h>
# include <CloudStatus.h>
# include <gvr/utils/Json.h>

# include <gvr/communication/ucc/session/UccSession.h>
# include <gvr/communication/spot/SpotSession.h>
# include <gvr/communication/ucc/IUccSessionListener.h>

using namespace gvr::communication::ucc;
using namespace gvr::communication::spot;

  /// @brief Gilbarco Domain
  namespace gvr
  {
    /// @brief Cloud Domain
    namespace cloud
    {
      class SecurityChangeWorker : public gvr::cloud::AbsOptCloudClientWorker, gvr::communication::ucc::IUccSessionListener
      {
        public:
          /**
           * @brief default constructor.
           */
          SecurityChangeWorker (UccSession &, SpotSession &, CloudStatus &);

          /**
           * @brief default destructor.
           */
          virtual ~SecurityChangeWorker (void);

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
          virtual void sessionEvent (IUccSessionListener::BufferType const &, IUccSessionListener::TypeId const &, IUccSessionListener::KeyType const &);

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
          virtual void sessionEvent (IUccSessionListener::StatusType);

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
           * @brief This method is where the thread does the process to each status event notification received.
           */
          virtual bool storeNewCertificatesInformation( const gvr::utils::Json& );


          /**
           * @brief UccSession to be used by this worker.
           */
          UccSession & m_uccSession;

          /**
           * @brief SpotSession to be used by this worker.
           */
          SpotSession & m_spotSession;

          /**
           * @brief Cloud status member to be used by this worker.
           */
          CloudStatus & m_cloudStatus;
      };
    }
  }

#endif
